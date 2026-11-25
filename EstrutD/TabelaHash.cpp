/* Tabela hash com encadeamento (lista encadeada dupla), hash hibrido de
   64 bits, P2C (Power of Two Choices) e filtro de Bloom por lista. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TAM_TABELA 53
#define TAM_NOME 128
#define LARGURA_BARRA 50

/* Qui-quadrado com 52 g.l.: acima de QUI2_CRITICO rejeita a uniformidade;
   abaixo de QUI2_INFERIOR a distribuicao e mais regular que o acaso. */
#define QUI2_CRITICO 69.83
#define QUI2_INFERIOR 36.44

/* Bloom: m primo (~9,6 bits por nome para ~2000 nomes por lista), k = 7 */
#define BLOOM_BITS 19181u
#define BLOOM_K 7
#define BLOOM_PALAVRAS ((BLOOM_BITS + 63u) / 64u)

#define GOLDEN_RATIO_64 0x9e3779b97f4a7c15ULL
#define MIX_MULT_1      0xbf58476d1ce4e5b9ULL
#define MIX_MULT_2      0x94d049bb133111ebULL

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
} TabelaHash;

/* ---------- Hash ---------- */

static inline uint64_t rotl64(uint64_t x, unsigned int r)
{
    r &= 63u;
    return (x << r) | (x >> ((64u - r) & 63u));
}

static inline uint64_t misturaBidirecional64(uint64_t z)
{
    uint64_t maskBit17 = 0ULL - ((z >> 17) & 1ULL);
    uint64_t multEtapa2 = (MIX_MULT_1 & maskBit17) |
                          (MIX_MULT_2 & ~maskBit17);

    z ^= z >> 29;
    z *= GOLDEN_RATIO_64;

    z ^= z >> 32;
    z = rotl64(z, 17u);
    z *= multEtapa2;

    z ^= z >> 27;
    z = rotl64(z, 29u);
    z *= MIX_MULT_2;

    z ^= z >> 31;
    return z;
}

/* Hash de 64 bits: acumulador polinomial com base guiada por bit + mistura
   estilo FNV-1a + finalizador bidirecional. Minusculas viram maiusculas. */
uint64_t hashNome64(const char *nome)
{
    const unsigned char *p = (const unsigned char *)nome;
    uint64_t poli = 14695981039346656037ULL;
    uint64_t estado = 0x243f6a8885a308d3ULL;
    const uint64_t primoFNV = 1099511628211ULL;
    size_t idx = 0;

    while (*p != '\0')
    {
        unsigned int c = (unsigned int)(*p++);
        if (c >= 'a' && c <= 'z')
        {
            c -= (unsigned int)('a' - 'A');
        }

        uint64_t maskBit13 = 0ULL - ((poli >> 13) & 1ULL);
        uint64_t basePoli = (1315423911ULL & maskBit13) |
                            (1000003ULL & ~maskBit13);
        poli = poli * basePoli + (uint64_t)c + (uint64_t)(idx & 0xFFu);

        estado ^= poli ^ rotl64((uint64_t)c, (unsigned int)((idx * 7u) & 63u));
        estado *= primoFNV;
        estado ^= estado >> 23;

        idx++;
    }

    estado ^= poli ^ ((uint64_t)idx * GOLDEN_RATIO_64);
    return misturaBidirecional64(estado);
}

/* Duas chaves candidatas independentes: bits altos (c1) e bits baixos (c2) */
static void chavesDoHash(uint64_t h, unsigned int *c1, unsigned int *c2)
{
    *c1 = (unsigned int)(((h >> 32) * TAM_TABELA) >> 32);
    *c2 = (unsigned int)(((h & 0xFFFFFFFFULL) * TAM_TABELA) >> 32);
}

/* ---------- Bloom filter ---------- */

static uint64_t bloomMistura(uint64_t z)
{
    z += GOLDEN_RATIO_64;
    z = (z ^ (z >> 30)) * MIX_MULT_1;
    z = (z ^ (z >> 27)) * MIX_MULT_2;
    return z ^ (z >> 31);
}

/* Double hashing: posicao_i = (pos + i * passo) mod m, com m primo */
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

/* Refaz o filtro da lista apos remocao para evitar falsos negativos */
static void bloomReconstroi(TabelaHash *t, unsigned int chave)
{
    Elemento *atual;

    bloomLimpa(&t->filtros[chave]);
    for (atual = t->baldes[chave].head; atual != NULL; atual = atual->next)
    {
        bloomAdiciona(&t->filtros[chave], hashNome64(atual->pessoa.nome));
    }
}

/* ---------- Tabela ---------- */

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

/* P2C: insere no fim da lista menos cheia entre c1 e c2 (empate em c1). */
unsigned int insere(TabelaHash *t, const char *nome)
{
    unsigned int c1, c2, chave;
    uint64_t h = hashNome64(nome);
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

static Elemento *procuraNaLista(Lista *l, const char *nome,
                                unsigned long *cmp, int *pos)
{
    Elemento *atual = l->head;
    int p = 1;

    while (atual != NULL)
    {
        if (cmp != NULL)
        {
            (*cmp)++;
        }
        if (strcmp(atual->pessoa.nome, nome) == 0)
        {
            if (pos != NULL)
            {
                *pos = p;
            }
            return atual;
        }
        atual = atual->next;
        p++;
    }
    return NULL;
}

/* Consulta nas duas chaves candidatas, usando o filtro de Bloom para pular
   listas onde o elemento certamente nao esta. */
int consulta(TabelaHash *t, const char *nome)
{
    uint64_t h = hashNome64(nome);
    unsigned int cand[2];
    int i, pos = 0, achou = 0, listas = 0;
    unsigned long cmp = 0;

    chavesDoHash(h, &cand[0], &cand[1]);
    if (cand[1] == cand[0])
    {
        cand[1] = cand[0];
    }

    for (i = 0; i < 2; i++)
    {
        if (i == 1 && cand[1] == cand[0])
        {
            continue;
        }

        int possivel = bloomPossui(&t->filtros[cand[i]], h);
        printf("Chave candidata %u: %d nome(s), filtro: %s\n", cand[i],
               t->baldes[cand[i]].size, possivel ? "possivel" : "ausente");

        if (possivel && !achou)
        {
            listas++;
            if (procuraNaLista(&t->baldes[cand[i]], nome, &cmp, &pos) != NULL)
            {
                printf("   encontrado na posicao %d da chave %u\n", pos, cand[i]);
                achou = 1;
            }
        }
    }

    if (!achou && listas == 0)
    {
        printf("   descartado pelos filtros: nenhuma lista foi percorrida\n");
    }
    printf("   comparacoes: %lu | listas percorridas: %d\n", cmp, listas);
    return achou;
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

/* Remove a primeira ocorrencia do nome e refaz o filtro da lista afetada */
int remove_nome(TabelaHash *t, const char *nome)
{
    uint64_t h = hashNome64(nome);
    unsigned int cand[2];
    int i;
    Elemento *e;

    chavesDoHash(h, &cand[0], &cand[1]);

    for (i = 0; i < 2; i++)
    {
        if (i == 1 && cand[1] == cand[0])
        {
            continue;
        }
        if (!bloomPossui(&t->filtros[cand[i]], h))
        {
            continue;
        }
        e = procuraNaLista(&t->baldes[cand[i]], nome, NULL, NULL);
        if (e != NULL)
        {
            removeElemento(&t->baldes[cand[i]], e);
            bloomReconstroi(t, cand[i]);
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

/* ---------- Quicksort na lista dupla ---------- */

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

/* ---------- Histograma e estatisticas ---------- */

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
    static const char barras[] = "##################################################";
    int i, max, min, chaveMax, chaveMin, porBarra, barra, ocupadas;
    double N, M, media, soma, desvio, desvioEsperado, qui2, d;

    if (t->total == 0)
    {
        printf("Tabela vazia. Nao ha o que mostrar no histograma.\n");
        return;
    }

    N = (double)t->total;
    M = (double)TAM_TABELA;
    media = N / M;

    max = min = t->baldes[0].size;
    chaveMax = chaveMin = 0;
    ocupadas = 0;
    soma = 0.0;
    qui2 = 0.0;

    for (i = 0; i < TAM_TABELA; i++)
    {
        int sz = t->baldes[i].size;
        if (sz > max)
        {
            max = sz;
            chaveMax = i;
        }
        if (sz < min)
        {
            min = sz;
            chaveMin = i;
        }
        if (sz > 0)
        {
            ocupadas++;
        }
        d = (double)sz - media;
        soma += d * d;
        qui2 += (d * d) / media;
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
        printf("Chave %2d | %.*s %d\n", i, barra, barras, t->baldes[i].size);
    }

    desvio = raiz(soma / M);
    desvioEsperado = raiz(N * (1.0 / M) * (1.0 - 1.0 / M));

    printf("\n===== ESTATISTICAS =====\n");
    printf("Total de nomes (N)        : %d\n", t->total);
    printf("Numero de chaves (M)      : %d\n", TAM_TABELA);
    printf("Fator de carga (N/M)      : %.2f\n", media);
    printf("Chaves ocupadas           : %d de %d\n", ocupadas, TAM_TABELA);
    printf("Colisoes (N - ocupadas)   : %d\n", t->total - ocupadas);
    printf("Menor lista               : %d (chave %d)\n", min, chaveMin);
    printf("Maior lista               : %d (chave %d)\n", max, chaveMax);
    printf("Amplitude                 : %d\n", max - min);
    printf("Amplitude esperada (unif) : ~%.0f\n", 4.5 * desvioEsperado);
    printf("Media por chave           : %.2f\n", media);
    printf("Desvio padrao observado   : %.2f\n", desvio);
    printf("Desvio esperado (uniforme): %.2f\n", desvioEsperado);
    printf("Razao observado/esperado  : %.2f (1 = aleatorio ideal; <1 = mais regular que o acaso)\n",
           desvio / desvioEsperado);
    printf("Qui-quadrado              : %.2f (5%%, 52 g.l.: acima de %.2f rejeita; abaixo de %.2f e mais regular que o acaso)\n",
           qui2, QUI2_CRITICO, QUI2_INFERIOR);
    if (qui2 > QUI2_CRITICO)
    {
        printf("   -> rejeita a hipotese de distribuicao uniforme\n");
    }
    else if (qui2 < QUI2_INFERIOR)
    {
        printf("   -> mais regular que o acaso (distribuicao equilibrada, nao aleatoria)\n");
    }
    else
    {
        printf("   -> compativel com distribuicao uniforme (nao rejeita a hipotese)\n");
    }
}

/* ---------- Entrada e menu ---------- */

int carregaArquivo(TabelaHash *t, const char *caminho, int *inseridos)
{
    FILE *f = fopen(caminho, "r");
    char linha[TAM_NOME];
    char *inicio;
    size_t len;
    int primeira = 1;

    *inseridos = 0;
    if (f == NULL)
    {
        return 0;
    }

    while (fgets(linha, sizeof(linha), f) != NULL)
    {
        len = strlen(linha);
        while (len > 0 && (linha[len - 1] == '\n' || linha[len - 1] == '\r' ||
                           linha[len - 1] == ' ' || linha[len - 1] == '\t'))
        {
            linha[--len] = '\0';
        }

        inicio = linha;
        if (primeira && (unsigned char)inicio[0] == 0xEF &&
            (unsigned char)inicio[1] == 0xBB && (unsigned char)inicio[2] == 0xBF)
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

static void limpaBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

static int lerInt(int *valor)
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

static void leTexto(char *destino, int tamanho)
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

int main(void)
{
    TabelaHash *t = criaTabela();
    int opcao, chave, limite, inseridos, continuar = 1;
    char nome[TAM_NOME], caminho[256];

    while (continuar)
    {
        printf("\n===== MENU - Tabela Hash (M=%d, P2C + Bloom) =====\n", TAM_TABELA);
        printf("1 - Inserir nome\n");
        printf("2 - Consultar nome\n");
        printf("3 - Remover nome\n");
        printf("4 - Quantidade de nomes por chave\n");
        printf("5 - Listar nomes de uma chave\n");
        printf("6 - Ordenar nomes de uma chave (quicksort)\n");
        printf("7 - Histograma e estatisticas\n");
        printf("8 - Carregar base de nomes de um arquivo\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

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
                printf("Nome vazio.\n");
            }
            else
            {
                chave = (int)insere(t, nome);
                printf("\"%s\" inserido na chave %d (total na chave: %d).\n",
                       nome, chave, quantidadePorChave(t, chave));
            }
            break;

        case 2:
            printf("Digite o nome a consultar: ");
            leTexto(nome, TAM_NOME);
            if (!consulta(t, nome))
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
                printf("Nome nao encontrado.\n");
            }
            break;

        case 4:
            if (lerChave(&chave))
            {
                printf("Chave %d: %d nome(s) (total geral: %d).\n",
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
            imprimeHistograma(t);
            break;

        case 8:
            printf("Caminho do arquivo: ");
            leTexto(caminho, sizeof(caminho));
            if (carregaArquivo(t, caminho, &inseridos))
            {
                printf("%d nome(s) carregados. Total: %d.\n", inseridos, t->total);
            }
            else
            {
                printf("Nao foi possivel abrir \"%s\".\n", caminho);
            }
            break;

        case 0:
            continuar = 0;
            break;

        default:
            printf("Opcao invalida.\n");
            break;
        }
    }

    liberaTabela(t);
    return 0;
}