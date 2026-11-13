/* ============================================================
   Trabalho Final - Tabela Hash com encadeamento (nomes)
   Estrutura de Dados I

   Escolhas de implementacao:
       - Hashing com ENCADEAMENTO: cada posicao (chave) da tabela
         guarda uma LISTA ENCADEADA DUPLA (head/tail/size).
       - M = 53 chaves (numero primo, proximo de 50).
       - Funcao hash: hashing modular iterativo sobre TODOS os
         caracteres do nome (como no slide):
              h = (31 * h + caractere) % M
         O nome e convertido para MAIUSCULAS apenas no calculo do
         hash, entao "Maria" e "maria" caem na mesma chave. A
         comparacao de nomes (consulta/remocao) e exata.
       - Nomes duplicados sao PERMITIDOS: cada linha da base vira
         um no (insercao sempre no fim da lista da chave).
       - Ordenacao dos nomes de uma chave: QUICKSORT proprio,
         direto na lista dupla (troca os dados entre os nos),
         particao de Hoare (usa next e prev), pivo = elemento do
         meio.
       - Histograma em barras no terminal + estatisticas para
         avaliar a Hipotese do Hashing Uniforme, e exportacao em
         CSV para gerar grafico.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdint.h>

#define TAM_TABELA 53    /* M: numero de chaves */
#define MULTIPLICADOR 31 /* R: multiplicador do hash */
#define TAM_NOME 128
#define LARGURA_BARRA 50
#define QUI2_CRITICO 69.83 /* qui-quadrado, 52 g.l., 5% */
#define MAX_NIVEIS 5
#define TENTATIVAS_POR_NIVEL 4
#define VAZIO -1
#define MAX_NIVEIS 4
#define TENTATIVAS_POR_NIVEL 3

typedef struct
{
    char **dados; /* Vetor de strings (nomes) */
    int tamanho_total;
    int inicio_nivel[MAX_NIVEIS];
    int tam_nivel[MAX_NIVEIS];
    int inicio_overflow;
    int tam_overflow;
} FunnelHash;

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

/* ---------- Tabela hash ---------- */

typedef struct sTabelaHash
{
    Lista baldes[TAM_TABELA]; /* uma lista dupla por chave */
    int total;                /* N: total de nomes na tabela */
} TabelaHash;

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
   Funcao de hash (hashing modular iterativo)
   Percorre todos os caracteres do nome, em MAIUSCULAS:
       h = (R * h + c) % M
   Aplicar o % M a cada passo evita overflow (como no slide).
   Retorna a chave, entre 0 e M-1.
   ============================================================ *


/* Funcao hash parametrizada pelo nivel para gerar posicoes independentes */
uint32_t hashNivel(int chave, int nivel, int tam_subtabela)
{
    uint32_t h = (uint32_t)chave ^ (nivel * 0x9E3779B9U);
    h = ((h >> 16) ^ h) * 0x45d9f3bU;
    h = ((h >> 16) ^ h);
    return h % tam_subtabela;
}

/* Inicializa a tabela dividindo a memoria em formato de funil */
FunnelHash *criarFunnel(int capacidade)
{
    FunnelHash *fh = malloc(sizeof(FunnelHash));
    fh->tamanho_total = capacidade;
    fh->dados = malloc(capacidade * sizeof(int));
    for (int i = 0; i < capacidade; i++)
        fh->dados[i] = VAZIO;

    /* Reserva 10% finais para overflow e divide 90% nos niveis do funil */
    fh->tam_overflow = capacidade / 10;
    int restante = capacidade - fh->tam_overflow;
    int offset = 0;

    for (int i = 0; i < MAX_NIVEIS; i++)
    {
        fh->inicio_nivel[i] = offset;
        /* Cada nivel pega cerca de metade do espaco restante (efeito funil) */
        fh->tam_nivel[i] = (i == MAX_NIVEIS - 1) ? restante : (restante / 2);
        offset += fh->tam_nivel[i];
        restante -= fh->tam_nivel[i];
    }
    fh->inicio_overflow = offset;
    return fh;
}

/* Insercao gulosa descendo pelo funil */
bool inserirFunnel(FunnelHash *fh, int chave)
{
    /* 1. Tenta inserir nos niveis hierarquicos do funil */
    for (int nivel = 0; nivel < MAX_NIVEIS; nivel++)
    {
        int tam = fh->tam_nivel[nivel];
        int base = fh->inicio_nivel[nivel];

        for (int t = 0; t < TENTATIVAS_POR_NIVEL; t++)
        {
            /* Combina o nivel e a tentativa para sondar posicoes distintas */
            uint32_t pos = hashNivel(chave + t * 17, nivel, tam);
            int idx = base + pos;

            if (fh->dados[idx] == VAZIO)
            {
                fh->dados[idx] = chave;
                return true;
            }
        }
    }

    /* 2. Se o funil estiver muito cheio, cai no reservatorio de overflow */
    for (int i = 0; i < fh->tam_overflow; i++)
    {
        int idx = fh->inicio_overflow + i;
        if (fh->dados[idx] == VAZIO)
        {
            fh->dados[idx] = chave;
            return true;
        }
    }

    return false; /* Tabela 100% lotada */
}

/* Busca seguindo exatamente o mesmo trajeto do funil */
bool buscarFunnel(FunnelHash *fh, int chave)
{
    for (int nivel = 0; nivel < MAX_NIVEIS; nivel++)
    {
        int tam = fh->tam_nivel[nivel];
        int base = fh->inicio_nivel[nivel];

        for (int t = 0; t < TENTATIVAS_POR_NIVEL; t++)
        {
            uint32_t pos = hashNivel(chave + t * 17, nivel, tam);
            int idx = base + pos;

            if (fh->dados[idx] == chave)
                return true;
            /* Nota: no Funnel Hashing NAO paramos ao achar VAZIO,
               pois uma tentativa anterior pode ter sido preenchida e depois removida,
               ou o item pode ter escorrido para o proximo nivel. */
        }
    }

    for (int i = 0; i < fh->tam_overflow; i++)
    {
        if (fh->dados[fh->inicio_overflow + i] == chave)
            return true;
    }

    return false;
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
   Cria a tabela hash (todas as listas comecam vazias)
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
   Retorna a chave onde o nome foi inserido.
   ============================================================ */
unsigned int insere(TabelaHash *t, const char *nome)
{
    unsigned int chave = hashNome(nome);
    Lista *l = &t->baldes[chave];
    Elemento *novo = criaElemento(nome);

    novo->prev = l->tail;
    novo->next = NULL;

    if (l->tail != NULL)
    {
        l->tail->next = novo;
    }
    else
    {
        l->head = novo; /* lista estava vazia */
    }
    l->tail = novo;

    l->size++;
    t->total++;

    return chave;
}

/* ============================================================
   Consulta
   Procura o nome (comparacao exata) na lista da sua chave.
   Retorna quantas vezes o nome aparece e imprime as posicoes
   (1 = primeiro da lista da chave).
   ============================================================ */
int consulta(TabelaHash *t, const char *nome)
{
    unsigned int chave = hashNome(nome);
    Elemento *atual = t->baldes[chave].head;
    int posicao = 1;
    int ocorrencias = 0;

    printf("Chave calculada para \"%s\": %u (lista com %d nome(s))\n",
           nome, chave, t->baldes[chave].size);

    while (atual != NULL)
    {
        if (strcmp(atual->pessoa.nome, nome) == 0)
        {
            ocorrencias++;
            if (ocorrencias <= 20)
            {
                printf("   encontrado na posicao %d da chave %u\n", posicao, chave);
            }
        }
        atual = atual->next;
        posicao++;
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
   Retorna 1 se removeu, 0 se o nome nao existe.
   ============================================================ */
int remove_nome(TabelaHash *t, const char *nome)
{
    unsigned int chave = hashNome(nome);
    Lista *l = &t->baldes[chave];
    Elemento *atual = l->head;

    while (atual != NULL)
    {
        if (strcmp(atual->pessoa.nome, nome) == 0)
        {
            removeElemento(l, atual);
            t->total--;
            return 1;
        }
        atual = atual->next;
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
    printf("Diferenca                 : %d \n", max - min);
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
    printf("2  - Consultar nome\n");
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

    printf("\n=== Consultas ===\n");
    printf("   -> %d ocorrencia(s)\n", consulta(t, "Maria Silva"));
    printf("   -> %d ocorrencia(s) (comparacao exata: minusculas nao batem)\n",
           consulta(t, "maria silva"));
    printf("   -> %d ocorrencia(s)\n", consulta(t, "Zenaide Prado"));

    printf("\n=== Remocao da primeira ocorrencia de \"Joao Santos\" ===\n");
    printf("Antes : %d nome(s) no total\n", t->total);
    printf("Removeu? %s\n", remove_nome(t, "Joao Santos") ? "SIM" : "NAO");
    printf("Depois: %d nome(s) no total\n", t->total);
    printf("   -> %d ocorrencia(s) de \"Joao Santos\" restante(s)\n",
           consulta(t, "Joao Santos"));

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