/* ============================================================
Trabalho Final - Tabela Hash com encadeamento (nomes)
Estrutura de Dados I

Escolhas de implementacao:

    - Resolucao de colisoes por ENCADEAMENTO:
      cada posicao da tabela armazena uma LISTA ENCADEADA
      DUPLA, com ponteiros para o inicio e fim e contador
      de elementos.

    - M = 53 posicoes na tabela.

    - Funcao hash: FNV-1a de 64 bits, seguida do
      finalizador fmix64, utilizado originalmente no
      MurmurHash3. O hash completo de 64 bits e obtido por
      hashFNV64(); hashNome() devolve apenas a 1a chave.

    - Reducao do hash de 64 bits para duas posicoes
      candidatas, utilizando partes distintas do resultado.

    - Estrategia Power of Two Choices:
      na insercao, compara o tamanho das duas listas
      candidatas e escolhe a menos ocupada.
      Em caso de empate, escolhe a primeira candidata.

    - Cada lista possui um BLOOM FILTER proprio, que responde
      "ausente" (certeza) ou "possivel" (pode ser falso
      positivo). Dimensionado para ~1% de falsos positivos com
      ate BLOOM_CAP_LISTA nomes por lista.
      Double hashing: posicao_i = (h1 + i*h2) mod m, com m
      PRIMO, o que garante passo h2 coprimo com m e nunca zero.

    - Consulta (para no primeiro nome encontrado):
        * consulta os filtros das duas posicoes candidatas;
        * ambos "ausente": encerra sem percorrer nenhuma lista;
        * so um "possivel": percorre apenas essa lista;
        * ambos "possivel": BUSCA INTERCALADA (um no de cada
          lista por vez), encerrando ao encontrar o nome.
      A existencia e sempre confirmada por strcmp.
      A contagem de duplicados fica em contaOcorrencias().

    - Remocao: apos remover, o filtro da lista e RECONSTRUIDO a
      partir dos nomes que restaram (simples e seguro, tambem
      com duplicados; ja existe uma varredura da lista na
      remocao, entao a ordem de complexidade nao muda).

    - O nome e convertido para MAIUSCULAS somente durante
      o calculo do hash. A comparacao dos nomes continua
      sendo exata e diferencia maiusculas de minusculas.

    - Nomes duplicados sao permitidos. Cada insercao
      cria um novo no, mesmo que o nome ja exista.

    - Ordenacao das listas por QUICKSORT proprio,
      utilizando os ponteiros next e prev e trocando
      os dados dos nos, sem alterar os elos da lista.

    - Histograma e estatisticas para avaliar a distribuicao
      dos elementos entre as posicoes da tabela, incluindo
      desvio padrao e teste qui-quadrado.

    - Medicao (menu 14): compara P2C original, busca
      intercalada e Bloom + intercalada.

============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdint.h>

#define TAM_TABELA 53 /* M: numero de chaves */
#define TAM_NOME 128
#define LARGURA_BARRA 50
#define QUI2_CRITICO 69.83 /* qui-quadrado, 52 g.l., 5% */

/* ---------- Parametros do Bloom Filter ----------
   n = BLOOM_CAP_LISTA nomes por lista (a base de 100.788 nomes
   da ~1.902 por lista com o P2C).
   m = menor primo >= 9,585 * n   (~9,59 bits por nome)
   k = 7 funcoes (otimo = m/n * ln 2 = 6,65)
   FPR teorica = (1 - e^(-k*n/m))^k = 1,00% com n = 2000
                                      0,79% com n = 1902      */
#define BLOOM_CAP_LISTA 2000
#define BLOOM_BITS 19181u /* primo */
#define BLOOM_K 7
#define BLOOM_PALAVRAS ((BLOOM_BITS + 63u) / 64u)

/* ---------- TAD Pessoa ---------- */

typedef struct sPessoa
{
    char nome[TAM_NOME];
} Pessoa;

/* ---------- Lista encadeada dupla ---------- */

typedef struct sElemento
{
    struct sElemento *next;
    struct sElemento *prev;
    Pessoa pessoa;
} Elemento;

typedef struct sLista
{
    struct sElemento *head;
    struct sElemento *tail;
    int size;
} Lista;

/* ---------- Bloom Filter (um por lista) ---------- */

typedef struct sBloom
{
    uint64_t bits[BLOOM_PALAVRAS];
} Bloom;

/* ---------- Tabela hash ---------- */

typedef struct sTabelaHash
{
    Lista baldes[TAM_TABELA]; /* uma lista dupla por chave */
    Bloom filtros[TAM_TABELA]; /* um filtro por lista */
    int total;                /* N: total de nomes na tabela */
} TabelaHash;

/* ---------- Estatisticas e modos de busca ---------- */

typedef enum
{
    BUSCA_ORIGINAL = 0, /* P2C original: varre as duas listas inteiras */
    BUSCA_SEQUENCIAL,   /* c1 e depois c2, parando no primeiro achado */
    BUSCA_INTERCALADA,  /* um no de cada lista por vez, sem filtros */
    BUSCA_BLOOM,        /* filtros + intercalada (usada em consulta) */
    NUM_MODOS
} ModoBusca;

typedef struct sEstatBusca
{
    unsigned long consultas;
    unsigned long achados;
    unsigned long comparacoes; /* chamadas de strcmp entre nomes */
    unsigned long descartadas; /* encerradas pelos filtros */
    unsigned long semLista;    /* nenhuma lista percorrida */
    unsigned long umaLista;    /* exatamente uma lista percorrida */
    unsigned long duasListas;  /* as duas listas percorridas */
} EstatBusca;

/* ---------- Nomes de exemplo (fazTudo / preenche) ---------- */

static const char *NOMES_EXEMPLO[] = {
    "Maria Silva", "Joao Santos", "Ana Oliveira", "Pedro Souza",
    "Carla Lima", "Lucas Ferreira", "Juliana Costa", "Rafael Alves",
    "Fernanda Rocha", "Bruno Gomes", "Patricia Ribeiro", "Diego Martins",
    "Camila Barbosa", "Gustavo Araujo", "Larissa Carvalho", "Thiago Melo",
    "Beatriz Nunes", "Felipe Cardoso", "Aline Teixeira", "Marcos Pereira",
    "Maria Silva", "Joao Santos", "Paula Fernandes", "Rodrigo Dias",
    "Vanessa Moreira", "Eduardo Castro", "Amanda Ramos", "Leonardo Pinto",
    "Renata Freitas", "Vinicius Correia", "Isabela Monteiro",
    "Andre Cavalcanti", "Tatiane Batista", "Ricardo Moura",
    "Natalia Campos", "Sergio Borges", "Priscila Farias", "Caio Duarte",
    "Bianca Azevedo", "Daniel Machado"};
#define QTD_EXEMPLO ((int)(sizeof(NOMES_EXEMPLO) / sizeof(NOMES_EXEMPLO[0])))

/* ============================================================
   Funcao hash - FNV-1a de 64 bits + fmix64

   Percorre todos os bytes do nome e aplica o algoritmo
   FNV-1a, convertendo letras ASCII minusculas para maiusculas.

   Em seguida, aplica o finalizador fmix64 para melhorar
   a mistura dos bits do resultado.

   Retorna um hash de 64 bits, ainda nao reduzido ao
   intervalo de posicoes da tabela.

   ============================================================ */
uint64_t hashFNV64(const char *nome)
{
    const unsigned char *p = (const unsigned char *)nome;
    uint64_t h = 14695981039346656037ULL;
    unsigned int c;

    while (*p != '\0')
    {
        c = *p++;
        if (c >= 'a' && c <= 'z')
        {
            c -= 'a' - 'A';
        }
        h ^= c;
        h *= 1099511628211ULL;
    }
    h ^= h >> 33;
    h *= 0xff51afd7ed558ccdULL;
    h ^= h >> 33;
    h *= 0xc4ceb9fe1a85ec53ULL;
    h ^= h >> 33;
    return h;
}

/* duas chaves independentes: bits altos (c1) e bits baixos (c2) */
static void chavesDoHash(uint64_t h, unsigned int *c1, unsigned int *c2)
{
    *c1 = (unsigned int)(((h >> 32) * TAM_TABELA) >> 32);
    *c2 = (unsigned int)(((h & 0xFFFFFFFFULL) * TAM_TABELA) >> 32);
}

static void duasChaves(const char *nome, unsigned int *c1, unsigned int *c2)
{
    chavesDoHash(hashFNV64(nome), c1, c2);
}

unsigned int hashNome(const char *nome)
{ /* 1a chave */
    unsigned int c1, c2;
    duasChaves(nome, &c1, &c2);
    return c1;
}

/* ============================================================
   Bloom Filter

   Os dois hashes do double hashing vem de uma versao
   REMISTURADA do hash de 64 bits (finalizador do SplitMix64),
   para que os bits do filtro nao fiquem correlacionados com os
   bits que escolhem as listas candidatas (c1 e c2).

       pos   = h1 mod m                   (0 .. m-1)
       passo = h2 mod (m-1) + 1           (1 .. m-1)
       i-esima posicao = (pos + i*passo) mod m

   Como m e primo e 1 <= passo <= m-1, o passo e coprimo com m
   (nunca e zero), e as k posicoes sao todas distintas.
   ============================================================ */
static uint64_t bloomMistura(uint64_t z)
{
    z += 0x9e3779b97f4a7c15ULL;
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
}

static void bloomPosicaoPasso(uint64_t h, unsigned int *pos, unsigned int *passo)
{
    uint64_t z = bloomMistura(h);
    *pos = (unsigned int)((z & 0xFFFFFFFFULL) % BLOOM_BITS);
    *passo = (unsigned int)((z >> 32) % (BLOOM_BITS - 1u)) + 1u;
}

static void bloomLimpa(Bloom *b)
{
    memset(b->bits, 0, sizeof(b->bits));
}

static void bloomAdiciona(Bloom *b, uint64_t h)
{
    unsigned int pos, passo, i;

    bloomPosicaoPasso(h, &pos, &passo);
    for (i = 0; i < BLOOM_K; i++)
    {
        b->bits[pos >> 6] |= (uint64_t)1 << (pos & 63u);
        pos += passo;
        if (pos >= BLOOM_BITS)
        {
            pos -= BLOOM_BITS;
        }
    }
}

/* 0 = ausente com certeza; 1 = possivelmente presente */
static int bloomPossui(const Bloom *b, uint64_t h)
{
    unsigned int pos, passo, i;

    bloomPosicaoPasso(h, &pos, &passo);
    for (i = 0; i < BLOOM_K; i++)
    {
        if (((b->bits[pos >> 6] >> (pos & 63u)) & 1u) == 0)
        {
            return 0;
        }
        pos += passo;
        if (pos >= BLOOM_BITS)
        {
            pos -= BLOOM_BITS;
        }
    }
    return 1;
}

/* Reconstroi o filtro de uma lista a partir dos nomes que restaram */
static void bloomReconstroi(TabelaHash *t, unsigned int chave)
{
    Elemento *atual;

    bloomLimpa(&t->filtros[chave]);
    for (atual = t->baldes[chave].head; atual != NULL; atual = atual->next)
    {
        bloomAdiciona(&t->filtros[chave], hashFNV64(atual->pessoa.nome));
    }
}

/* ============================================================
   Cria elemento (no) com o nome informado
   ============================================================ */
Elemento *criaElemento(const char *nome)
{
    Elemento *novo = (Elemento *)malloc(sizeof(Elemento));
    if (novo == NULL)
    {
        printf("Erro ao alocar memoria para o elemento.\n");
        exit(1);
    }
    strncpy(novo->pessoa.nome, nome, TAM_NOME - 1);
    novo->pessoa.nome[TAM_NOME - 1] = '\0';
    novo->next = NULL;
    novo->prev = NULL;
    return novo;
}

/* ============================================================
   Cria a tabela hash (todas as listas e filtros comecam vazios)
   ============================================================ */
TabelaHash *criaTabela(void)
{
    TabelaHash *t = (TabelaHash *)malloc(sizeof(TabelaHash));
    int i;

    if (t == NULL)
    {
        printf("Erro ao alocar memoria para a tabela.\n");
        exit(1);
    }
    for (i = 0; i < TAM_TABELA; i++)
    {
        t->baldes[i].head = NULL;
        t->baldes[i].tail = NULL;
        t->baldes[i].size = 0;
        bloomLimpa(&t->filtros[i]);
    }
    t->total = 0;
    return t;
}

/* ============================================================
   Libera toda a memoria da tabela
   ============================================================ */
void liberaTabela(TabelaHash *t)
{
    int i;
    Elemento *atual, *proximo;

    for (i = 0; i < TAM_TABELA; i++)
    {
        atual = t->baldes[i].head;
        while (atual != NULL)
        {
            proximo = atual->next;
            free(atual);
            atual = proximo;
        }
    }
    free(t);
}

/* ============================================================
   Insercao
   Calcula a chave do nome e insere um novo no no FIM da lista
   dupla daquela chave. Duplicados sao permitidos.
   Depois de inserir, atualiza o filtro da lista escolhida.
   Retorna a chave onde o nome foi inserido.
   ============================================================ */
unsigned int insere(TabelaHash *t, const char *nome)
{
    unsigned int c1, c2, chave;
    uint64_t h = hashFNV64(nome);
    Lista *l;
    Elemento *novo;

    chavesDoHash(h, &c1, &c2);
    chave = (t->baldes[c2].size < t->baldes[c1].size) ? c2 : c1;
    l = &t->baldes[chave];
    novo = criaElemento(nome);

    novo->prev = l->tail;
    novo->next = NULL;

    if (l->tail != NULL)
    {
        l->tail->next = novo;
    }
    else
    {
        l->head = novo;
    }
    l->tail = novo;

    l->size++;
    t->total++;

    bloomAdiciona(&t->filtros[chave], h);

    return chave;
}

/* ============================================================
   Varreduras de lista (usadas pelas buscas)
   Cada strcmp entre nomes incrementa *cmp.
   ============================================================ */

/* varre a lista inteira contando as ocorrencias */
static int contaNaLista(Lista *l, const char *nome, unsigned long *cmp,
                        int *primeiraPos)
{
    Elemento *atual = l->head;
    int pos = 1, n = 0;

    while (atual != NULL)
    {
        (*cmp)++;
        if (strcmp(atual->pessoa.nome, nome) == 0)
        {
            if (n == 0)
            {
                *primeiraPos = pos;
            }
            n++;
        }
        atual = atual->next;
        pos++;
    }
    return n;
}

/* para no primeiro nome igual; devolve o no ou NULL */
static Elemento *procuraNaLista(Lista *l, const char *nome,
                                unsigned long *cmp, int *pos)
{
    Elemento *atual = l->head;
    int p = 1;

    while (atual != NULL)
    {
        (*cmp)++;
        if (strcmp(atual->pessoa.nome, nome) == 0)
        {
            *pos = p;
            return atual;
        }
        atual = atual->next;
        p++;
    }
    return NULL;
}

/* BUSCA INTERCALADA: avanca um no de A e um no de B por vez e
   encerra assim que um deles for igual ao nome. Quando uma das
   listas termina, continua so na outra.
   *noB = 1 se o nome estava em B. */
static Elemento *procuraIntercalada(Lista *a, Lista *b, const char *nome,
                                    unsigned long *cmpA, unsigned long *cmpB,
                                    int *noB, int *pos)
{
    Elemento *x = a->head;
    Elemento *y = b->head;
    int pa = 1, pb = 1;

    while (x != NULL && y != NULL)
    {
        (*cmpA)++;
        if (strcmp(x->pessoa.nome, nome) == 0)
        {
            *noB = 0;
            *pos = pa;
            return x;
        }
        (*cmpB)++;
        if (strcmp(y->pessoa.nome, nome) == 0)
        {
            *noB = 1;
            *pos = pb;
            return y;
        }
        x = x->next;
        y = y->next;
        pa++;
        pb++;
    }
    while (x != NULL)
    {
        (*cmpA)++;
        if (strcmp(x->pessoa.nome, nome) == 0)
        {
            *noB = 0;
            *pos = pa;
            return x;
        }
        x = x->next;
        pa++;
    }
    while (y != NULL)
    {
        (*cmpB)++;
        if (strcmp(y->pessoa.nome, nome) == 0)
        {
            *noB = 1;
            *pos = pb;
            return y;
        }
        y = y->next;
        pb++;
    }
    return NULL;
}

/* ============================================================
   buscaNome - nucleo da busca, com o metodo escolhido
   Retorna 1 se o nome existe, 0 caso contrario.
   'st', 'chaveAch' e 'posAch' podem ser NULL.
   ============================================================ */
static int buscaNome(TabelaHash *t, const char *nome, ModoBusca modo,
                     EstatBusca *st, int *chaveAch, int *posAch)
{
    uint64_t h = hashFNV64(nome);
    unsigned int c1, c2;
    Lista *l1, *l2;
    unsigned long ca = 0, cb = 0;
    Elemento *e = NULL;
    int noB = 0, pos = 0, pos2 = 0, achou = 0, p1, p2, o1, o2;
    EstatBusca local;

    if (st == NULL)
    {
        memset(&local, 0, sizeof(local));
        st = &local;
    }

    chavesDoHash(h, &c1, &c2);
    l1 = &t->baldes[c1];
    l2 = (c2 != c1) ? &t->baldes[c2] : NULL;

    switch (modo)
    {
    case BUSCA_ORIGINAL:
        o1 = contaNaLista(l1, nome, &ca, &pos);
        o2 = (l2 != NULL) ? contaNaLista(l2, nome, &cb, &pos2) : 0;
        achou = (o1 + o2 > 0);
        if (o1 == 0)
        {
            noB = 1;
            pos = pos2;
        }
        break;

    case BUSCA_SEQUENCIAL:
        e = procuraNaLista(l1, nome, &ca, &pos);
        if (e == NULL && l2 != NULL)
        {
            e = procuraNaLista(l2, nome, &cb, &pos);
            noB = 1;
        }
        achou = (e != NULL);
        break;

    case BUSCA_INTERCALADA:
        if (l2 == NULL)
        {
            e = procuraNaLista(l1, nome, &ca, &pos);
        }
        else
        {
            e = procuraIntercalada(l1, l2, nome, &ca, &cb, &noB, &pos);
        }
        achou = (e != NULL);
        break;

    default: /* BUSCA_BLOOM */
        p1 = bloomPossui(&t->filtros[c1], h);
        p2 = (l2 != NULL) && bloomPossui(&t->filtros[c2], h);
        if (!p1 && !p2)
        {
            st->descartadas++;
        }
        else if (p1 && p2)
        {
            e = procuraIntercalada(l1, l2, nome, &ca, &cb, &noB, &pos);
        }
        else if (p1)
        {
            e = procuraNaLista(l1, nome, &ca, &pos);
        }
        else
        {
            e = procuraNaLista(l2, nome, &cb, &pos);
            noB = 1;
        }
        achou = (e != NULL);
        break;
    }

    st->consultas++;
    if (achou)
    {
        st->achados++;
    }
    st->comparacoes += ca + cb;
    if (ca > 0 && cb > 0)
    {
        st->duasListas++;
    }
    else if (ca > 0 || cb > 0)
    {
        st->umaLista++;
    }
    else
    {
        st->semLista++;
    }

    if (achou)
    {
        if (chaveAch != NULL)
        {
            *chaveAch = (int)(noB ? c2 : c1);
        }
        if (posAch != NULL)
        {
            *posAch = pos;
        }
    }
    return achou;
}

/* ============================================================
   Consulta (para no primeiro nome encontrado)
   Usa os filtros das duas chaves candidatas e, se preciso, a
   busca intercalada. Retorna 1 se o nome existe, 0 se nao.
   Para contar duplicados, use contaOcorrencias().
   ============================================================ */
int consulta(TabelaHash *t, const char *nome)
{
    uint64_t h = hashFNV64(nome);
    unsigned int c1, c2;
    int p1, p2, achou, chaveAch = -1, posAch = 0, listas;
    EstatBusca st;

    chavesDoHash(h, &c1, &c2);
    p1 = bloomPossui(&t->filtros[c1], h);
    if (c2 != c1)
    {
        p2 = bloomPossui(&t->filtros[c2], h);
        printf("Chaves candidatas para \"%s\": %u (%d nome(s), filtro: %s) e %u (%d nome(s), filtro: %s)\n",
               nome,
               c1, t->baldes[c1].size, p1 ? "possivel" : "ausente",
               c2, t->baldes[c2].size, p2 ? "possivel" : "ausente");
    }
    else
    {
        printf("Chave candidata unica para \"%s\": %u (%d nome(s), filtro: %s)\n",
               nome, c1, t->baldes[c1].size, p1 ? "possivel" : "ausente");
    }

    memset(&st, 0, sizeof(st));
    achou = buscaNome(t, nome, BUSCA_BLOOM, &st, &chaveAch, &posAch);

    if (achou)
    {
        printf("   encontrado na posicao %d da chave %d\n", posAch, chaveAch);
    }
    else if (st.descartadas > 0)
    {
        printf("   descartado pelos filtros: nenhuma lista foi percorrida\n");
    }
    listas = (st.duasListas > 0) ? 2 : (st.umaLista > 0 ? 1 : 0);
    printf("   comparacoes: %lu | listas percorridas: %d\n",
           st.comparacoes, listas);
    return achou;
}

/* ============================================================
   Contagem de ocorrencias (varre por completo as listas cujo
   filtro indica presenca possivel)
   Procura o nome (comparacao exata) e imprime as posicoes
   (1 = primeiro da lista da chave). Retorna quantas vezes o
   nome aparece.
   ============================================================ */
static void varreLista(Lista *l, unsigned int chave, const char *nome,
                       int *ocorrencias)
{
    Elemento *atual = l->head;
    int posicao = 1;

    while (atual != NULL)
    {
        if (strcmp(atual->pessoa.nome, nome) == 0)
        {
            (*ocorrencias)++;
            if (*ocorrencias <= 20)
            {
                printf("   encontrado na posicao %d da chave %u\n", posicao, chave);
            }
        }
        atual = atual->next;
        posicao++;
    }
}

int contaOcorrencias(TabelaHash *t, const char *nome)
{
    uint64_t h = hashFNV64(nome);
    unsigned int c1, c2;
    int ocorrencias = 0;

    chavesDoHash(h, &c1, &c2);
    printf("Chaves candidatas para \"%s\": %u (%d nome(s)) e %u (%d nome(s))\n",
           nome, c1, t->baldes[c1].size, c2, t->baldes[c2].size);

    if (bloomPossui(&t->filtros[c1], h))
    {
        varreLista(&t->baldes[c1], c1, nome, &ocorrencias);
    }
    if (c2 != c1 && bloomPossui(&t->filtros[c2], h))
    {
        varreLista(&t->baldes[c2], c2, nome, &ocorrencias);
    }

    if (ocorrencias > 20)
    {
        printf("   ... (%d ocorrencias no total, exibidas as 20 primeiras)\n",
               ocorrencias);
    }
    return ocorrencias;
}

/* ============================================================
   Remove um no especifico da lista dupla (usa prev e next)
   ============================================================ */
static void removeElemento(Lista *l, Elemento *e)
{
    if (e->prev != NULL)
    {
        e->prev->next = e->next;
    }
    else
    {
        l->head = e->next; /* era o primeiro */
    }

    if (e->next != NULL)
    {
        e->next->prev = e->prev;
    }
    else
    {
        l->tail = e->prev; /* era o ultimo */
    }

    free(e);
    l->size--;
}

/* ============================================================
   Remocao
   Remove a PRIMEIRA ocorrencia do nome na lista da sua chave.
   Depois da remocao, reconstroi o filtro da lista afetada.
   Retorna 1 se removeu, 0 se o nome nao existe.
   ============================================================ */
static Elemento *achaNaLista(Lista *l, const char *nome)
{
    Elemento *atual = l->head;

    while (atual != NULL)
    {
        if (strcmp(atual->pessoa.nome, nome) == 0)
        {
            return atual;
        }
        atual = atual->next;
    }
    return NULL;
}

int remove_nome(TabelaHash *t, const char *nome)
{
    unsigned int c1, c2, tmp;
    Elemento *e;

    duasChaves(nome, &c1, &c2);
    if (t->baldes[c2].size > t->baldes[c1].size)
    {
        tmp = c1;
        c1 = c2;
        c2 = tmp;
    }

    e = achaNaLista(&t->baldes[c1], nome);
    if (e != NULL)
    {
        removeElemento(&t->baldes[c1], e);
        bloomReconstroi(t, c1);
        t->total--;
        return 1;
    }
    if (c2 != c1)
    {
        e = achaNaLista(&t->baldes[c2], nome);
        if (e != NULL)
        {
            removeElemento(&t->baldes[c2], e);
            bloomReconstroi(t, c2);
            t->total--;
            return 1;
        }
    }
    return 0;
}

/* ============================================================
   Quantidade de elementos por chave
   ============================================================ */
int quantidadePorChave(TabelaHash *t, int chave)
{
    return t->baldes[chave].size;
}

/* ============================================================
   Quicksort direto na lista dupla
   - Ordena 'n' nos consecutivos, a partir de 'lo'.
   - Particao de Hoare: um ponteiro i anda para frente (next) e
     outro j anda para tras (prev), trocando os DADOS dos nos
     quando encontram um par fora de ordem.
   - Pivo = elemento do meio (evita o pior caso em listas ja
     ordenadas; elementos iguais sao divididos de forma
     equilibrada, o que importa por causa dos duplicados).
   - Como so os dados trocam de no dentro da MESMA lista, o
     conjunto de nomes da lista (e seu filtro) nao muda.
   ============================================================ */
static void trocaPessoas(Elemento *a, Elemento *b)
{
    Pessoa t = a->pessoa;
    a->pessoa = b->pessoa;
    b->pessoa = t;
}

static void quicksortLED(Elemento *lo, int n)
{
    Elemento *i, *j, *meio, *hi;
    int ii, jj, k;
    char pivo[TAM_NOME];

    if (n < 2)
    {
        return;
    }

    hi = lo;
    for (k = 0; k < n - 1; k++)
    {
        hi = hi->next;
    }
    meio = lo;
    for (k = 0; k < n / 2; k++)
    {
        meio = meio->next;
    }
    strcpy(pivo, meio->pessoa.nome);

    i = lo;
    ii = 0;
    j = hi;
    jj = n - 1;

    do
    {
        while (strcmp(i->pessoa.nome, pivo) < 0)
        {
            i = i->next;
            ii++;
        }
        while (strcmp(j->pessoa.nome, pivo) > 0)
        {
            j = j->prev;
            jj--;
        }
        if (ii <= jj)
        {
            trocaPessoas(i, j);
            i = i->next;
            ii++;
            j = j->prev;
            jj--;
        }
    } while (ii <= jj);

    /* parte esquerda: indices 0..jj (a partir de lo)
       parte direita : indices ii..n-1 (a partir de i) */
    quicksortLED(lo, jj + 1);
    quicksortLED(i, n - ii);
}

/* ------------------------------------------------------------
   Ordena (alfabeticamente) os nomes da lista de uma chave
   ------------------------------------------------------------ */
void ordenaChave(TabelaHash *t, int chave)
{
    Lista *l = &t->baldes[chave];
    if (l->size > 1)
    {
        quicksortLED(l->head, l->size);
    }
}

/* ------------------------------------------------------------
   Verifica se a lista de uma chave esta em ordem crescente
   ------------------------------------------------------------ */
int chaveOrdenada(TabelaHash *t, int chave)
{
    Elemento *atual = t->baldes[chave].head;

    while (atual != NULL && atual->next != NULL)
    {
        if (strcmp(atual->pessoa.nome, atual->next->pessoa.nome) > 0)
        {
            return 0;
        }
        atual = atual->next;
    }
    return 1;
}

/* ------------------------------------------------------------
   Imprime os nomes de uma chave (maximo 'limite'; 0 = todos)
   ------------------------------------------------------------ */
void imprimeChave(TabelaHash *t, int chave, int limite)
{
    Lista *l = &t->baldes[chave];
    Elemento *atual = l->head;
    int posicao = 1;

    printf("Chave %d (%d nome(s)):\n", chave, l->size);

    if (atual == NULL)
    {
        printf("   (vazia)\n");
        return;
    }

    while (atual != NULL && (limite == 0 || posicao <= limite))
    {
        printf("   %d. %s\n", posicao, atual->pessoa.nome);
        atual = atual->next;
        posicao++;
    }

    if (atual != NULL)
    {
        printf("   ... (+%d nome(s) nao exibidos)\n", l->size - (posicao - 1));
    }
}

/* ------------------------------------------------------------
   Raiz quadrada (metodo de Newton) para nao depender da libm
   ------------------------------------------------------------ */
static double raiz(double x)
{
    double r, ant;
    int k;

    if (x <= 0.0)
    {
        return 0.0;
    }
    r = (x > 1.0) ? x / 2.0 : 1.0;
    for (k = 0; k < 100; k++)
    {
        ant = r;
        r = 0.5 * (r + x / r);
        if (r == ant)
        {
            break;
        }
    }
    return r;
}

/* ------------------------------------------------------------
   e^(-x), x >= 0, sem libm (reduz x por metades, serie de
   Taylor e eleva ao quadrado de volta)
   ------------------------------------------------------------ */
static double expNegativo(double x)
{
    double termo = 1.0, soma = 1.0;
    int i, metades = 0;

    while (x > 0.5)
    {
        x /= 2.0;
        metades++;
    }
    for (i = 1; i < 20; i++)
    {
        termo *= -x / i;
        soma += termo;
    }
    while (metades-- > 0)
    {
        soma *= soma;
    }
    return soma;
}

/* taxa teorica de falsos positivos com n nomes no filtro */
static double bloomFprTeorica(double n)
{
    double p = 1.0 - expNegativo(BLOOM_K * n / (double)BLOOM_BITS);
    double r = 1.0;
    int i;

    for (i = 0; i < BLOOM_K; i++)
    {
        r *= p;
    }
    return r;
}

/* ============================================================
   Histograma de frequencia + analise da hipotese do hashing
   uniforme.
   - Uma barra por chave (cada # vale 'porBarra' nomes).
   - Estatisticas: alfa, minimo, maximo, media, desvio padrao,
     razao entre o desvio observado e o esperado se o hashing
     fosse uniforme, e teste qui-quadrado.
   ============================================================ */
void imprimeHistograma(TabelaHash *t)
{
    int i, j, max, min, chaveMax, chaveMin, porBarra, barra, ocupadas;
    double N, M, media, soma, var, desvio, desvioEsperado, qui2, d;

    if (t->total == 0)
    {
        printf("Tabela vazia. Nao ha o que mostrar no histograma.\n");
        return;
    }

    N = (double)t->total;
    M = (double)TAM_TABELA;

    max = min = t->baldes[0].size;
    chaveMax = chaveMin = 0;
    ocupadas = 0;
    for (i = 0; i < TAM_TABELA; i++)
    {
        if (t->baldes[i].size > max)
        {
            max = t->baldes[i].size;
            chaveMax = i;
        }
        if (t->baldes[i].size < min)
        {
            min = t->baldes[i].size;
            chaveMin = i;
        }
        if (t->baldes[i].size > 0)
        {
            ocupadas++;
        }
    }

    porBarra = (max + LARGURA_BARRA - 1) / LARGURA_BARRA;
    if (porBarra < 1)
    {
        porBarra = 1;
    }

    printf("\n===== HISTOGRAMA: nomes por chave (cada # = %d nome(s)) =====\n",
           porBarra);
    for (i = 0; i < TAM_TABELA; i++)
    {
        barra = (t->baldes[i].size + porBarra - 1) / porBarra;
        printf("Chave %2d | ", i);
        for (j = 0; j < barra; j++)
        {
            putchar('#');
        }
        printf(" %d\n", t->baldes[i].size);
    }

    media = N / M;
    soma = 0.0;
    qui2 = 0.0;
    for (i = 0; i < TAM_TABELA; i++)
    {
        d = (double)t->baldes[i].size - media;
        soma += d * d;
        qui2 += (d * d) / media;
    }
    var = soma / M;
    desvio = raiz(var);
    /* se fosse uniforme, cada chave seria uma binomial(N, 1/M) */
    desvioEsperado = raiz(N * (1.0 / M) * (1.0 - 1.0 / M));

    printf("\n===== ESTATISTICAS =====\n");
    printf("Total de nomes (N)        : %d\n", t->total);
    printf("Numero de chaves (M)      : %d\n", TAM_TABELA);
    printf("Fator de carga (N/M)      : %.2f\n", media);
    printf("Chaves ocupadas           : %d de %d\n", ocupadas, TAM_TABELA);
    printf("Colisoes (N - ocupadas)   : %d\n", t->total - ocupadas);
    printf("Menor lista               : %d (chave %d)\n", min, chaveMin);
    printf("Maior lista               : %d (chave %d)\n", max, chaveMax);
    printf("Amplitude                 : %d \n", max - min);
    printf("Amplitude esperada (unif) : ~%.0f\n", 4.5 * desvioEsperado);
    printf("Media por chave           : %.2f\n", media);
    printf("Desvio padrao observado   : %.2f\n", desvio);
    printf("Desvio esperado (uniforme): %.2f\n", desvioEsperado);
    if (desvioEsperado > 0.0)
    {
        printf("Razao observado/esperado  : %.2f (perto de 1 = uniforme)\n",
               desvio / desvioEsperado);
    }
    printf("Qui-quadrado              : %.2f (critico 5%%, 52 g.l.: %.2f)\n",
           qui2, QUI2_CRITICO);
    if (qui2 <= QUI2_CRITICO)
    {
        printf("   -> nao rejeita a hipotese de distribuicao uniforme\n");
    }
    else
    {
        printf("   -> rejeita a hipotese de distribuicao uniforme\n");
    }
    printf("Obs.: com N muito grande o teste qui-quadrado rejeita ate\n");
    printf("      desvios pequenos; avalie junto da razao e do histograma.\n");
}

/* ------------------------------------------------------------
   Exporta o histograma em CSV (chave,quantidade)
   Retorna 1 se gravou, 0 se nao conseguiu abrir o arquivo.
   ------------------------------------------------------------ */
int exportaHistogramaCSV(TabelaHash *t, const char *caminho)
{
    FILE *f = fopen(caminho, "w");
    int i;

    if (f == NULL)
    {
        return 0;
    }
    fprintf(f, "chave,quantidade\n");
    for (i = 0; i < TAM_TABELA; i++)
    {
        fprintf(f, "%d,%d\n", i, t->baldes[i].size);
    }
    fclose(f);
    return 1;
}

/* ============================================================
   Carrega a base de nomes de um arquivo texto (um nome por
   linha). Trata fim de linha do Windows (\r\n), espacos nas
   pontas, linhas vazias e o BOM do UTF-8.
   Retorna 1 se abriu o arquivo, 0 caso contrario.
   ============================================================ */
int carregaArquivo(TabelaHash *t, const char *caminho,
                   int *inseridos, int *truncados)
{
    FILE *f = fopen(caminho, "r");
    char linha[TAM_NOME];
    char *inicio;
    size_t len;
    int c, primeira = 1;

    *inseridos = 0;
    *truncados = 0;

    if (f == NULL)
    {
        return 0;
    }

    while (fgets(linha, sizeof(linha), f) != NULL)
    {
        len = strlen(linha);

        if (len > 0 && linha[len - 1] == '\n')
        {
            linha[len - 1] = '\0';
            len--;
        }
        else if (!feof(f))
        {
            /* linha maior que o buffer: descarta o resto dela */
            while ((c = fgetc(f)) != '\n' && c != EOF)
            {
                /* descarta */
            }
            (*truncados)++;
        }

        while (len > 0 && (linha[len - 1] == '\r' || linha[len - 1] == ' ' || linha[len - 1] == '\t'))
        {
            linha[len - 1] = '\0';
            len--;
        }

        inicio = linha;
        if (primeira && (unsigned char)inicio[0] == 0xEF && (unsigned char)inicio[1] == 0xBB && (unsigned char)inicio[2] == 0xBF)
        {
            inicio += 3; /* pula o BOM */
        }
        primeira = 0;

        while (*inicio == ' ' || *inicio == '\t')
        {
            inicio++;
        }

        if (*inicio == '\0')
        {
            continue; /* linha vazia */
        }

        insere(t, inicio);
        (*inseridos)++;
    }

    fclose(f);
    return 1;
}

/* ============================================================
   MEDICAO: P2C original x intercalada x Bloom + intercalada
   ============================================================ */
static const char *NOMES_MODOS[NUM_MODOS] = {
    "P2C original (varre 2 listas)",
    "P2C sequencial (para no 1o)",
    "Busca intercalada",
    "Bloom + intercalada"};

/* gerador pseudoaleatorio proprio (xorshift64): rand() tem
   RAND_MAX de apenas 32767 em alguns sistemas */
static uint64_t rngEstado = 88172645463325252ULL;

static uint64_t rngProximo(void)
{
    rngEstado ^= rngEstado << 13;
    rngEstado ^= rngEstado >> 7;
    rngEstado ^= rngEstado << 17;
    return rngEstado;
}

/* executa todos os modos sobre as mesmas consultas */
static void mediFase(TabelaHash *t, const char *titulo, const char **consultas,
                     int m, unsigned char *ref)
{
    ModoBusca modo;
    EstatBusca st;
    clock_t ini;
    unsigned long diverg;
    double seg;
    int i, r, imodo;

    printf("\n--- %s (%d consultas) ---\n", titulo, m);
    printf("%-30s %12s %11s %8s %8s %8s %9s %10s %5s\n",
           "Metodo", "cmp/consulta", "descartadas", "0 lista", "1 lista",
           "2 listas", "tempo (s)", "us/consulta", "diverg");

    for (imodo = BUSCA_ORIGINAL; imodo < NUM_MODOS; imodo++)
    {
        modo = (ModoBusca)imodo;
        memset(&st, 0, sizeof(st));
        diverg = 0;
        ini = clock();
        for (i = 0; i < m; i++)
        {
            r = buscaNome(t, consultas[i], modo, &st, NULL, NULL);
            if (modo == BUSCA_ORIGINAL)
            {
                ref[i] = (unsigned char)r;
            }
            else if (r != ref[i])
            {
                diverg++;
            }
        }
        seg = (double)(clock() - ini) / CLOCKS_PER_SEC;

        printf("%-30s %12.1f %11lu %8lu %8lu %8lu %9.3f %10.2f %5lu\n",
               NOMES_MODOS[modo],
               (double)st.comparacoes / (double)st.consultas,
               st.descartadas, st.semLista, st.umaLista, st.duasListas,
               seg, seg * 1e6 / (double)m, diverg);
    }
}

/* taxa de falsos positivos medida: consultas a nomes ausentes
   (ref == 0), contando cada filtro candidato que respondeu
   "possivel" */
static void mediFalsosPositivos(TabelaHash *t, const char **ausentes, int m,
                                const unsigned char *ref)
{
    uint64_t h;
    unsigned int c1, c2;
    unsigned long candidatos = 0, positivos = 0;
    double teorica = 0.0;
    int i;

    for (i = 0; i < m; i++)
    {
        if (ref[i])
        {
            continue;
        }
        h = hashFNV64(ausentes[i]);
        chavesDoHash(h, &c1, &c2);
        candidatos++;
        positivos += (unsigned long)bloomPossui(&t->filtros[c1], h);
        if (c2 != c1)
        {
            candidatos++;
            positivos += (unsigned long)bloomPossui(&t->filtros[c2], h);
        }
    }
    for (i = 0; i < TAM_TABELA; i++)
    {
        teorica += bloomFprTeorica((double)t->baldes[i].size);
    }
    teorica /= (double)TAM_TABELA;

    printf("\nBloom: %d filtros de %u bits (k=%d), %lu bytes no total\n",
           TAM_TABELA, BLOOM_BITS, BLOOM_K,
           (unsigned long)(TAM_TABELA * sizeof(Bloom)));
    if (candidatos > 0)
    {
        printf("Falsos positivos medidos : %.3f%% (%lu de %lu filtros consultados)\n",
               100.0 * (double)positivos / (double)candidatos, positivos,
               candidatos);
    }
    printf("Falsos positivos teoricos: %.3f%% (com a ocupacao atual das listas)\n",
           100.0 * teorica);
}

void comparaBuscas(TabelaHash *t, int amostra)
{
    const char **nomes, **acertos, **erros;
    char *bufErros;
    unsigned char *ref;
    Elemento *atual;
    int i, k, n = 0, m = (amostra > 0) ? amostra : 10000;

    if (t->total == 0)
    {
        printf("Tabela vazia. Carregue a base (opcao 10) antes de medir.\n");
        return;
    }

    nomes = (const char **)malloc((size_t)t->total * sizeof(char *));
    acertos = (const char **)malloc((size_t)m * sizeof(char *));
    erros = (const char **)malloc((size_t)m * sizeof(char *));
    bufErros = (char *)malloc((size_t)m * TAM_NOME);
    ref = (unsigned char *)malloc((size_t)m);
    if (nomes == NULL || acertos == NULL || erros == NULL || bufErros == NULL || ref == NULL)
    {
        printf("Memoria insuficiente para a medicao.\n");
        free(nomes);
        free(acertos);
        free(erros);
        free(bufErros);
        free(ref);
        return;
    }

    for (i = 0; i < TAM_TABELA; i++)
    {
        for (atual = t->baldes[i].head; atual != NULL; atual = atual->next)
        {
            nomes[n++] = atual->pessoa.nome;
        }
    }

    /* acertos: nomes existentes sorteados; erros: nome sorteado + "~" */
    for (i = 0; i < m; i++)
    {
        acertos[i] = nomes[rngProximo() % (uint64_t)n];
        k = (int)(rngProximo() % (uint64_t)n);
        snprintf(bufErros + (size_t)i * TAM_NOME, TAM_NOME, "%s~", nomes[k]);
        erros[i] = bufErros + (size_t)i * TAM_NOME;
    }

    printf("\n===== MEDICAO DE BUSCA (N = %d nomes, M = %d listas) =====\n",
           t->total, TAM_TABELA);
    printf("cmp = comparacoes de nome (strcmp). diverg = respostas diferentes\n");
    printf("do P2C original (deve ser 0). Tempo medido com clock().\n");

    mediFase(t, "Consultas a nomes EXISTENTES", acertos, m, ref);
    mediFase(t, "Consultas a nomes AUSENTES", erros, m, ref);
    mediFalsosPositivos(t, erros, m, ref);

    free(nomes);
    free(acertos);
    free(erros);
    free(bufErros);
    free(ref);
}

/* ------------------------------------------------------------
   Limpa o buffer de entrada (restos deixados pelo scanf)
   ------------------------------------------------------------ */
void limpaBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        /* descarta */
    }
}

/* ------------------------------------------------------------
   Le um inteiro do teclado. Retorna 1 se leu, 0 se invalido.
   ------------------------------------------------------------ */
int lerInt(int *valor)
{
    if (scanf("%d", valor) != 1)
    {
        limpaBuffer();
        printf("Entrada invalida.\n");
        return 0;
    }
    limpaBuffer();
    return 1;
}

/* ------------------------------------------------------------
   Le um texto (aceita espacos), remove o '\n' final e descarta
   o excesso se a linha for maior que o buffer
   ------------------------------------------------------------ */
void leTexto(char *destino, int tamanho)
{
    size_t len;

    if (fgets(destino, tamanho, stdin) != NULL)
    {
        len = strlen(destino);
        if (len > 0 && destino[len - 1] == '\n')
        {
            destino[len - 1] = '\0';
        }
        else
        {
            limpaBuffer();
        }
    }
    else
    {
        destino[0] = '\0';
    }
}

/* ------------------------------------------------------------
   Le uma chave valida (0 a M-1). Retorna 1 se valida.
   ------------------------------------------------------------ */
static int lerChave(int *chave)
{
    printf("Digite a chave (0 a %d): ", TAM_TABELA - 1);
    if (!lerInt(chave))
    {
        return 0;
    }
    if (*chave < 0 || *chave >= TAM_TABELA)
    {
        printf("Chave fora do intervalo.\n");
        return 0;
    }
    return 1;
}

/* ------------------------------------------------------------
   Exibe o menu de opcoes
   ------------------------------------------------------------ */
void exibeMenu(void)
{
    printf("\n===== MENU - Tabela Hash (M=%d) =====\n", TAM_TABELA);
    printf("1  - Inserir nome\n");
    printf("2  - Consultar nome (filtros + busca intercalada)\n");
    printf("3  - Remover nome (primeira ocorrencia)\n");
    printf("4  - Quantidade de nomes por chave\n");
    printf("5  - Listar nomes de uma chave\n");
    printf("6  - Ordenar nomes de uma chave (quicksort)\n");
    printf("7  - Ordenar todas as chaves (quicksort)\n");
    printf("8  - Histograma e analise de uniformidade\n");
    printf("9  - Exportar histograma (CSV)\n");
    printf("10 - Carregar base de nomes de um arquivo\n");
    printf("11 - Executar tudo automaticamente (fazTudo)\n");
    printf("12 - Preencher tabela automaticamente\n");
    printf("13 - Contar ocorrencias de um nome\n");
    printf("14 - Comparar metodos de busca (medicao)\n");
    printf("0  - Sair\n");
    printf("Escolha uma opcao: ");
}

/* ============================================================
   preencheTabela
   Insere os nomes de exemplo na tabela recebida. As demais
   operacoes continuam sendo feitas manualmente pelo menu.
   ============================================================ */
void preencheTabela(TabelaHash *t)
{
    int i;
    for (i = 0; i < QTD_EXEMPLO; i++)
    {
        insere(t, NOMES_EXEMPLO[i]);
    }
}

/* ------------------------------------------------------------
   Ordena todas as chaves e confere se cada lista ficou em ordem
   ------------------------------------------------------------ */
static void ordenaTodas(TabelaHash *t)
{
    int i, ok = 1;
    clock_t ini = clock();

    for (i = 0; i < TAM_TABELA; i++)
    {
        ordenaChave(t, i);
        if (!chaveOrdenada(t, i))
        {
            ok = 0;
            printf("ERRO: a chave %d nao ficou ordenada.\n", i);
        }
    }

    printf("Todas as chaves ordenadas em %.3f s. Conferencia: %s\n",
           (double)(clock() - ini) / CLOCKS_PER_SEC,
           ok ? "todas em ordem crescente" : "FALHOU");
}

/* ============================================================
   fazTudo
   Executa automaticamente, sem interacao do usuario, uma
   sequencia que demonstra a tabela: insercao, colisao,
   consulta (inclusive duplicado e inexistente), remocao,
   quantidade por chave, ordenacao de uma chave e histograma.
   ============================================================ */
void fazTudo(void)
{
    TabelaHash *t = criaTabela();
    int i, chaveMax, max;

    printf("=== Funcao hash (primeiros nomes) ===\n");
    for (i = 0; i < 5; i++)
    {
        printf("hash(\"%s\") = %u\n", NOMES_EXEMPLO[i], hashNome(NOMES_EXEMPLO[i]));
    }
    printf("hash(\"MARIA SILVA\") = %u  (maiusculas dao a mesma chave)\n",
           hashNome("MARIA SILVA"));

    printf("\n=== Inserindo %d nomes (com 2 duplicados) ===\n", QTD_EXEMPLO);
    preencheTabela(t);
    printf("Total de nomes na tabela: %d\n", t->total);

    printf("\n=== Contagem de ocorrencias ===\n");
    printf("   -> %d ocorrencia(s)\n", contaOcorrencias(t, "Maria Silva"));
    printf("   -> %d ocorrencia(s) (comparacao exata: minusculas nao batem)\n",
           contaOcorrencias(t, "maria silva"));
    printf("   -> %d ocorrencia(s)\n", contaOcorrencias(t, "Zenaide Prado"));

    printf("\n=== Consulta com filtros e busca intercalada (para no 1o) ===\n");
    printf("   -> %s\n", consulta(t, "Maria Silva") ? "encontrado" : "nao encontrado");
    printf("   -> %s\n", consulta(t, "Zenaide Prado") ? "encontrado" : "nao encontrado");

    printf("\n=== Remocao da primeira ocorrencia de \"Joao Santos\" ===\n");
    printf("Antes : %d nome(s) no total\n", t->total);
    printf("Removeu? %s\n", remove_nome(t, "Joao Santos") ? "SIM" : "NAO");
    printf("Depois: %d nome(s) no total\n", t->total);
    printf("   -> %d ocorrencia(s) de \"Joao Santos\" restante(s)\n",
           contaOcorrencias(t, "Joao Santos"));

    printf("\n=== Quantidade de nomes por chave (chaves ocupadas) ===\n");
    for (i = 0; i < TAM_TABELA; i++)
    {
        if (quantidadePorChave(t, i) > 0)
        {
            printf("Chave %2d: %d nome(s)\n", i, quantidadePorChave(t, i));
        }
    }

    max = 0;
    chaveMax = 0;
    for (i = 0; i < TAM_TABELA; i++)
    {
        if (quantidadePorChave(t, i) > max)
        {
            max = quantidadePorChave(t, i);
            chaveMax = i;
        }
    }

    printf("\n=== Chave mais cheia (%d), ANTES de ordenar ===\n", chaveMax);
    imprimeChave(t, chaveMax, 0);
    ordenaChave(t, chaveMax);
    printf("\n=== Chave %d DEPOIS do quicksort (ordenada: %s) ===\n",
           chaveMax, chaveOrdenada(t, chaveMax) ? "SIM" : "NAO");
    imprimeChave(t, chaveMax, 0);

    imprimeHistograma(t);

    liberaTabela(t);
}

/* ============================================================
   Programa principal - menu interativo
   ============================================================ */
int main(void)
{
    TabelaHash *t = criaTabela();
    int opcao, chave, limite, inseridos, truncados, amostra;
    char nome[TAM_NOME];
    char caminho[256];
    clock_t ini;
    int continuar = 1;

    while (continuar)
    {
        exibeMenu();

        if (!lerInt(&opcao))
        {
            continue;
        }

        switch (opcao)
        {
        case 1:
            printf("Digite o nome a inserir: ");
            leTexto(nome, TAM_NOME);
            if (nome[0] == '\0')
            {
                printf("Nome vazio. Nada inserido.\n");
            }
            else
            {
                chave = (int)insere(t, nome);
                printf("\"%s\" inserido na chave %d (agora com %d nome(s)).\n",
                       nome, chave, quantidadePorChave(t, chave));
            }
            break;

        case 2:
            printf("Digite o nome a consultar: ");
            leTexto(nome, TAM_NOME);
            if (consulta(t, nome) == 0)
            {
                printf("Nome nao encontrado.\n");
            }
            break;

        case 3:
            printf("Digite o nome a remover: ");
            leTexto(nome, TAM_NOME);
            if (remove_nome(t, nome))
            {
                printf("Primeira ocorrencia de \"%s\" removida.\n", nome);
            }
            else
            {
                printf("Nome nao encontrado. Nada removido.\n");
            }
            break;

        case 4:
            if (lerChave(&chave))
            {
                printf("A chave %d tem %d nome(s). Total na tabela: %d.\n",
                       chave, quantidadePorChave(t, chave), t->total);
            }
            break;

        case 5:
            if (lerChave(&chave))
            {
                printf("Quantos nomes exibir (0 = todos)? ");
                if (lerInt(&limite) && limite >= 0)
                {
                    imprimeChave(t, chave, limite);
                }
            }
            break;

        case 6:
            if (lerChave(&chave))
            {
                ordenaChave(t, chave);
                printf("Chave %d ordenada. Conferencia: %s\n", chave,
                       chaveOrdenada(t, chave) ? "em ordem crescente" : "FALHOU");
            }
            break;

        case 7:
            ordenaTodas(t);
            break;

        case 8:
            imprimeHistograma(t);
            break;

        case 9:
            if (exportaHistogramaCSV(t, "histograma.csv"))
            {
                printf("Histograma gravado em histograma.csv\n");
            }
            else
            {
                printf("Nao foi possivel gravar histograma.csv\n");
            }
            break;

        case 10:
            printf("Caminho do arquivo (um nome por linha): ");
            leTexto(caminho, sizeof(caminho));
            ini = clock();
            if (carregaArquivo(t, caminho, &inseridos, &truncados))
            {
                printf("%d nome(s) inseridos em %.3f s. Total na tabela: %d.\n",
                       inseridos, (double)(clock() - ini) / CLOCKS_PER_SEC,
                       t->total);
                if (truncados > 0)
                {
                    printf("Aviso: %d linha(s) maiores que %d caracteres foram truncadas.\n",
                           truncados, TAM_NOME - 1);
                }
            }
            else
            {
                printf("Nao foi possivel abrir o arquivo \"%s\".\n", caminho);
            }
            break;

        case 11:
            fazTudo();
            break;

        case 12:
            preencheTabela(t);
            printf("%d nomes de exemplo inseridos. Total: %d.\n",
                   QTD_EXEMPLO, t->total);
            break;

        case 13:
            printf("Digite o nome a contar: ");
            leTexto(nome, TAM_NOME);
            printf("Total: %d ocorrencia(s).\n", contaOcorrencias(t, nome));
            break;

        case 14:
            printf("Quantas consultas por fase (0 = 10000)? ");
            if (lerInt(&amostra) && amostra >= 0)
            {
                comparaBuscas(t, amostra);
            }
            break;

        case 0:
            printf("Encerrando...\n");
            continuar = 0;
            break;

        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
    }

    liberaTabela(t);

    return 0;
}