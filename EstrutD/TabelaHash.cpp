/* Tabela hash com encadeamento, FNV-1a 64 bits, Bloom Filter e P2C. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdint.h>

#define TAM_TABELA 53 
#define TAM_NOME 128
#define LARGURA_BARRA 50
#define QUI2_CRITICO 69.83 

#define BLOOM_CAP_LISTA 2000
#define BLOOM_BITS 19181u 
#define BLOOM_K 7
#define BLOOM_PALAVRAS ((BLOOM_BITS + 63u) / 64u)

typedef struct sPessoa
{
    char nome[TAM_NOME];
} Pessoa;

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

typedef struct sBloom
{
    uint64_t bits[BLOOM_PALAVRAS];
} Bloom;

typedef struct sTabelaHash
{
    Lista baldes[TAM_TABELA]; 
    Bloom filtros[TAM_TABELA]; 
    int total;                
    int usarP2C;               
} TabelaHash;

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

static inline uint64_t rotl64(uint64_t x, unsigned int r)
{
    r &= 63u;
    if (r == 0u)
    {
        return x;
    }
    return (x << r) | (x >> (64u - r));
}

static inline uint64_t misturaBidirecional64(uint64_t z)
{
    /* Empurra bits baixos para cima e propaga carry via multiplicacao impar */
    z ^= z >> 29;
    z *= 0x9e3779b97f4a7c15ULL;

    /* Joga bits altos para baixo e gira para cruzar metades de 64 bits */
    z ^= z >> 32;
    z = rotl64(z, 17u);
    z *= 0xbf58476d1ce4e5b9ULL;

    /* Segunda dobra bidirecional antes da finalizacao */
    z ^= z >> 27;
    z = rotl64(z, 29u);
    z *= 0x94d049bb133111ebULL;

    z ^= z >> 31;
    return z;
}

uint64_t hashFNV64(const char *nome)
{
    const unsigned char *p = (const unsigned char *)nome;
    uint64_t poli = 14695981039346656037ULL;
    uint64_t estado = 0x243f6a8885a308d3ULL;
    const uint64_t basePoli = 1315423911ULL;
    const uint64_t primoFNV = 1099511628211ULL;
    size_t idx = 0;

    while (*p != '\0')
    {
        unsigned int c = (unsigned int)(*p++);
        if (c >= 'a' && c <= 'z')
        {
            c -= (unsigned int)('a' - 'A');
        }

        /* 1. Acumulador polinomial com peso posicional */
        poli = poli * basePoli + (uint64_t)c + (uint64_t)(idx & 0xFFu);

        /* 2. Mistura encadeada com fator FNV-1a e deslocamento reverso */
        estado ^= poli ^ rotl64((uint64_t)c, (unsigned int)((idx * 7u) & 63u));
        estado *= primoFNV;
        estado ^= estado >> 23;

        idx++;
    }

    /* Incorpora tamanho/posicao final e aplica a mistura bidirecional */
    estado ^= poli ^ ((uint64_t)idx * 0x9e3779b97f4a7c15ULL);
    return misturaBidirecional64(estado);
}

static void chavesDoHash(uint64_t h, unsigned int *c1, unsigned int *c2)
{
    *c1 = (unsigned int)(((h >> 32) * TAM_TABELA) >> 32);
    *c2 = (unsigned int)(((h & 0xFFFFFFFFULL) * TAM_TABELA) >> 32);
}

static void duasChaves(const char *nome, unsigned int *c1, unsigned int *c2)
{
    chavesDoHash(hashFNV64(nome), c1, c2);
}

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

static void bloomReconstroi(TabelaHash *t, unsigned int chave)
{
    Elemento *atual;

    bloomLimpa(&t->filtros[chave]);
    for (atual = t->baldes[chave].head; atual != NULL; atual = atual->next)
    {
        bloomAdiciona(&t->filtros[chave], hashFNV64(atual->pessoa.nome));
    }
}

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
    t->usarP2C = 1; 
    return t;
}

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

unsigned int insere(TabelaHash *t, const char *nome)
{
    unsigned int c1, c2, chave;
    uint64_t h = hashFNV64(nome);
    Lista *l;
    Elemento *novo;

    chavesDoHash(h, &c1, &c2);
    if (t->usarP2C)
    {
        chave = (t->baldes[c2].size < t->baldes[c1].size) ? c2 : c1;
    }
    else
    {
        chave = c1; 
    }
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

int consulta(TabelaHash *t, const char *nome)
{
    uint64_t h = hashFNV64(nome);
    unsigned int c1, c2;
    unsigned long cmpA = 0, cmpB = 0;
    Elemento *e = NULL;
    int p1, p2, noB = 0, pos = 0, listas;

    chavesDoHash(h, &c1, &c2);
    p1 = bloomPossui(&t->filtros[c1], h);
    p2 = (c2 != c1) && bloomPossui(&t->filtros[c2], h);

    if (c2 != c1)
        printf("Chaves candidatas: %u (%d nome(s), filtro: %s) e %u (%d nome(s), filtro: %s)\n",
               c1, t->baldes[c1].size, p1 ? "possivel" : "ausente",
               c2, t->baldes[c2].size, p2 ? "possivel" : "ausente");
    else
        printf("Chave candidata: %u (%d nome(s), filtro: %s)\n",
               c1, t->baldes[c1].size, p1 ? "possivel" : "ausente");

    if (!p1 && !p2)
    {
        printf("   descartado pelos filtros: nenhuma lista foi percorrida\n");
        return 0;
    }

    if (p1 && p2)
        e = procuraIntercalada(&t->baldes[c1], &t->baldes[c2], nome,
                               &cmpA, &cmpB, &noB, &pos);
    else if (p1)
        e = procuraNaLista(&t->baldes[c1], nome, &cmpA, &pos);
    else
    {
        e = procuraNaLista(&t->baldes[c2], nome, &cmpB, &pos);
        noB = 1;
    }

    if (e != NULL)
        printf("   encontrado na posicao %d da chave %u\n", pos, noB ? c2 : c1);

    listas = (cmpA > 0) + (cmpB > 0);
    printf("   comparacoes: %lu | listas percorridas: %d\n",
           cmpA + cmpB, listas);
    return e != NULL;
}

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

static void removeElemento(Lista *l, Elemento *e)
{
    if (e->prev != NULL)
    {
        e->prev->next = e->next;
    }
    else
    {
        l->head = e->next; 
    }

    if (e->next != NULL)
    {
        e->next->prev = e->prev;
    }
    else
    {
        l->tail = e->prev; 
    }

    free(e);
    l->size--;
}

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

int quantidadePorChave(TabelaHash *t, int chave)
{
    return t->baldes[chave].size;
}

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

    quicksortLED(lo, jj + 1);
    quicksortLED(i, n - ii);
}

void ordenaChave(TabelaHash *t, int chave)
{
    Lista *l = &t->baldes[chave];
    if (l->size > 1)
    {
        quicksortLED(l->head, l->size);
    }
}

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
        printf("Razao observado/esperado  : %.2f (menor = mais uniforme)\n",
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
            
            while ((c = fgetc(f)) != '\n' && c != EOF)
            {
                
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
            inicio += 3; 
        }
        primeira = 0;

        while (*inicio == ' ' || *inicio == '\t')
        {
            inicio++;
        }

        if (*inicio == '\0')
        {
            continue; 
        }

        insere(t, inicio);
        (*inseridos)++;
    }

    fclose(f);
    return 1;
}

void limpaBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        
    }
}

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
    printf("9  - Carregar base de nomes de um arquivo\n");
    printf("10 - Executar tudo automaticamente (fazTudo)\n");
    printf("11 - Preencher tabela automaticamente\n");
    printf("12 - Contar ocorrencias de um nome\n");
    printf("13 - Alternar hashing com/sem P2C (somente tabela vazia)\n");
    printf("0  - Sair\n");
    printf("Escolha uma opcao: ");
}

void preencheTabela(TabelaHash *t)
{
    int i;
    for (i = 0; i < QTD_EXEMPLO; i++)
    {
        insere(t, NOMES_EXEMPLO[i]);
    }
}

static void ordenaTodas(TabelaHash *t)
{
    int i;
    for (i = 0; i < TAM_TABELA; i++)
        ordenaChave(t, i);
    printf("Todas as chaves foram ordenadas.\\n");
}

void fazTudo(void)
{
    TabelaHash *semP2C = criaTabela();
    TabelaHash *comP2C = criaTabela();
    int inseridosSem, inseridosCom;
    int truncadosSem, truncadosCom;
    clock_t ini;

    semP2C->usarP2C = 0;
    comP2C->usarP2C = 1;

    printf("\n===== COMPARACAO: HASH SEM P2C x HASH COM P2C =====\n");
    printf("Arquivo de entrada: nomes.txt (um nome por linha)\n");

    ini = clock();
    if (!carregaArquivo(semP2C, "nomes.txt", &inseridosSem, &truncadosSem))
    {
        printf("ERRO: nao foi possivel abrir nomes.txt.\n");
        printf("Coloque nomes.txt na mesma pasta em que o programa e executado.\n");
        liberaTabela(semP2C);
        liberaTabela(comP2C);
        return;
    }

    if (!carregaArquivo(comP2C, "nomes.txt", &inseridosCom, &truncadosCom))
    {
        
        printf("ERRO: nao foi possivel carregar nomes.txt para a tabela com P2C.\n");
        liberaTabela(semP2C);
        liberaTabela(comP2C);
        return;
    }

    printf("\nNomes carregados sem P2C: %d\n", inseridosSem);
    printf("Nomes carregados com P2C: %d\n", inseridosCom);
    printf("Tempo total de carregamento das duas tabelas: %.3f s\n",
           (double)(clock() - ini) / CLOCKS_PER_SEC);

    if (truncadosSem > 0 || truncadosCom > 0)
    {
        printf("Aviso: houve linhas maiores que %d caracteres; elas foram truncadas.\n",
               TAM_NOME - 1);
        printf("Linhas truncadas (sem P2C / com P2C): %d / %d\n",
               truncadosSem, truncadosCom);
    }

    printf("\n\n========== HISTOGRAMA 1: HASH SEM P2C ==========\n");
    imprimeHistograma(semP2C);

    printf("\n\n========== HISTOGRAMA 2: HASH COM P2C ==========\n");
    imprimeHistograma(comP2C);

    liberaTabela(semP2C);
    liberaTabela(comP2C);
}

int main(void)
{
    TabelaHash *t = criaTabela();
    int opcao, chave, limite, inseridos, truncados;
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
                printf("Chave %d ordenada.\n", chave);
            }
            break;

        case 7:
            ordenaTodas(t);
            break;

        case 8:
            imprimeHistograma(t);
            break;


        case 9:
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

        case 10:
            fazTudo();
            break;

        case 11:
            preencheTabela(t);
            printf("%d nomes de exemplo inseridos. Total: %d.\n",
                   QTD_EXEMPLO, t->total);
            break;

        case 12:
            printf("Digite o nome a contar: ");
            leTexto(nome, TAM_NOME);
            printf("Total: %d ocorrencia(s).\n", contaOcorrencias(t, nome));
            break;

        case 13:
            if (t->total != 0)
            {
                printf("Esvazie/reinicie a tabela antes de trocar o modo de hashing.\n");
            }
            else
            {
                t->usarP2C = !t->usarP2C;
                printf("Modo selecionado: %s.\n",
                       t->usarP2C ? "COM P2C" : "SEM P2C (somente primeira chave)");
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