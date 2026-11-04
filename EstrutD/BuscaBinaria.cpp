/* ============================================================
   Busca binaria em C - com chaves repetidas
   Baseado no conteudo do slide "Busca linear e binaria"

   Escolhas de implementacao:
       - As chaves ficam em um VETOR dinamico (malloc/realloc),
         pois a busca binaria precisa de acesso direto por indice.
       - Cada elemento guarda apenas a chave (int).
       - Ordenacao automatica: se o vetor nao estiver ordenado
         no momento da busca, ele e ordenado antes (qsort).
         Por isso as posicoes devolvidas sao as do vetor ORDENADO.
       - Chaves repetidas: duas buscas binarias, uma para a
         PRIMEIRA e outra para a ULTIMA ocorrencia. Como o vetor
         esta ordenado, as repetidas ficam coladas, e as posicoes
         sao todas as que estao entre a primeira e a ultima.

   Retorno da busca (TAD Posicoes):
       - chave encontrada varias vezes -> todas as posicoes
       - chave encontrada uma vez      -> somente aquela posicao
       - chave nao encontrada          -> lista vazia (qtd = 0)
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>

#define CAPACIDADE_INICIAL 4

/* ---------- TAD: lista de chaves (vetor dinamico) ---------- */

typedef struct sListaChaves {
    int *chaves;
    int tamanho;    /* quantidade de chaves guardadas */
    int capacidade; /* tamanho alocado do vetor */
    int ordenada;   /* 1 se o vetor esta ordenado, 0 caso contrario */
} ListaChaves;

/* ---------- TAD: lista de posicoes (resultado da busca) ---------- */

typedef struct sPosicoes {
    int *posicoes;
    int qtd;
} Posicoes;

/* ============================================================
   Cria a lista de chaves (aloca a estrutura e o vetor inicial)
   ============================================================ */
ListaChaves *criaListaChaves(void) {
    ListaChaves *l = (ListaChaves *) malloc(sizeof(ListaChaves));
    if (l == NULL) {
        printf("Erro ao alocar memoria para a lista.\n");
        exit(1);
    }
    l->chaves = (int *) malloc(CAPACIDADE_INICIAL * sizeof(int));
    if (l->chaves == NULL) {
        printf("Erro ao alocar memoria para o vetor.\n");
        exit(1);
    }
    l->tamanho = 0;
    l->capacidade = CAPACIDADE_INICIAL;
    l->ordenada = 1; /* vetor vazio conta como ordenado */
    return l;
}

/* ============================================================
   Insere uma chave no final do vetor
   Quando o vetor enche, a capacidade dobra (realloc).
   Inserir uma chave pode desordenar o vetor, entao a flag
   'ordenada' volta para 0.
   ============================================================ */
void insereChave(ListaChaves *l, int chave) {
    if (l->tamanho == l->capacidade) {
        int novaCapacidade = l->capacidade * 2;
        int *novo = (int *) realloc(l->chaves, novaCapacidade * sizeof(int));
        if (novo == NULL) {
            printf("Erro ao realocar memoria para o vetor.\n");
            exit(1);
        }
        l->chaves = novo;
        l->capacidade = novaCapacidade;
    }

    l->chaves[l->tamanho] = chave;
    l->tamanho++;
    l->ordenada = 0;
}

/* ------------------------------------------------------------
   Funcao de comparacao usada pelo qsort (ordem crescente).
   (a > b) - (a < b) evita overflow que "a - b" poderia causar.
   ------------------------------------------------------------ */
static int comparaInt(const void *a, const void *b) {
    int x = *(const int *) a;
    int y = *(const int *) b;
    return (x > y) - (x < y);
}

/* ============================================================
   Ordena o vetor (crescente) e marca a flag 'ordenada'
   ============================================================ */
void ordenaChaves(ListaChaves *l) {
    if (l->tamanho > 1) {
        qsort(l->chaves, l->tamanho, sizeof(int), comparaInt);
    }
    l->ordenada = 1;
}

/* ============================================================
   Primeira ocorrencia (busca binaria que continua para a
   ESQUERDA depois de achar)
   Retorna o indice da primeira ocorrencia ou -1 se nao existe.

   Observacao: o meio e calculado como inicio + (fim - inicio) / 2
   (igual a (inicio + fim) / 2 do slide, mas sem risco de
   overflow em vetores muito grandes).
   ============================================================ */
int primeiraOcorrencia(int *v, int tam, int chave) {
    int inicio = 0;
    int fim = tam - 1;
    int meio;
    int resultado = -1;

    while (inicio <= fim) {
        meio = inicio + (fim - inicio) / 2;

        if (v[meio] == chave) {
            resultado = meio; /* guarda e tenta achar uma anterior */
            fim = meio - 1;
        } else if (v[meio] > chave) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
    }

    return resultado;
}

/* ============================================================
   Ultima ocorrencia (busca binaria que continua para a
   DIREITA depois de achar)
   Retorna o indice da ultima ocorrencia ou -1 se nao existe.
   ============================================================ */
int ultimaOcorrencia(int *v, int tam, int chave) {
    int inicio = 0;
    int fim = tam - 1;
    int meio;
    int resultado = -1;

    while (inicio <= fim) {
        meio = inicio + (fim - inicio) / 2;

        if (v[meio] == chave) {
            resultado = meio; /* guarda e tenta achar uma posterior */
            inicio = meio + 1;
        } else if (v[meio] > chave) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
    }

    return resultado;
}

/* ============================================================
   Busca binaria com chaves repetidas
   - Ordena o vetor automaticamente se ainda nao estiver.
   - Acha a primeira e a ultima ocorrencia.
   - Se nao achou, devolve a lista vazia (qtd = 0).
   - Se achou, a quantidade e (ultima - primeira + 1) e as
     posicoes sao todas as do intervalo [primeira, ultima].
   O resultado e alocado dinamicamente: quem chamou deve liberar
   com liberaPosicoes().
   ============================================================ */
Posicoes *buscaBinaria(ListaChaves *l, int chave) {
    Posicoes *res = (Posicoes *) malloc(sizeof(Posicoes));
    int primeira, ultima, i;

    if (res == NULL) {
        printf("Erro ao alocar memoria para o resultado.\n");
        exit(1);
    }
    res->posicoes = NULL;
    res->qtd = 0;

    if (!l->ordenada) {
        ordenaChaves(l);
    }

    primeira = primeiraOcorrencia(l->chaves, l->tamanho, chave);
    if (primeira == -1) {
        return res; /* nao encontrada: lista vazia */
    }

    ultima = ultimaOcorrencia(l->chaves, l->tamanho, chave);

    res->qtd = ultima - primeira + 1;
    res->posicoes = (int *) malloc(res->qtd * sizeof(int));
    if (res->posicoes == NULL) {
        printf("Erro ao alocar memoria para as posicoes.\n");
        exit(1);
    }

    for (i = 0; i < res->qtd; i++) {
        res->posicoes[i] = primeira + i;
    }

    return res;
}

/* ------------------------------------------------------------
   Libera a memoria do resultado da busca
   ------------------------------------------------------------ */
void liberaPosicoes(Posicoes *p) {
    free(p->posicoes); /* free(NULL) e seguro */
    free(p);
}

/* ------------------------------------------------------------
   Libera a memoria da lista de chaves
   ------------------------------------------------------------ */
void liberaListaChaves(ListaChaves *l) {
    free(l->chaves);
    free(l);
}

/* ------------------------------------------------------------
   Exibe o vetor com os indices
   ------------------------------------------------------------ */
void imprimeChaves(ListaChaves *l) {
    int i;

    printf("Vetor [tamanho=%d, ordenado=%s]:\n",
           l->tamanho, l->ordenada ? "SIM" : "NAO");

    if (l->tamanho == 0) {
        printf("   (vazio)\n");
        return;
    }

    printf("   indice: ");
    for (i = 0; i < l->tamanho; i++) {
        printf("%4d", i);
    }
    printf("\n   chave : ");
    for (i = 0; i < l->tamanho; i++) {
        printf("%4d", l->chaves[i]);
    }
    printf("\n");
}

/* ------------------------------------------------------------
   Exibe o resultado da busca
   ------------------------------------------------------------ */
void imprimeResultado(int chave, Posicoes *p) {
    int i;

    if (p->qtd == 0) {
        printf("Chave %d nao encontrada. Lista de posicoes: (vazia)\n", chave);
    } else if (p->qtd == 1) {
        printf("Chave %d encontrada somente na posicao %d.\n",
               chave, p->posicoes[0]);
    } else {
        printf("Chave %d encontrada %d vezes. Posicoes: ", chave, p->qtd);
        for (i = 0; i < p->qtd; i++) {
            printf("%d", p->posicoes[i]);
            if (i < p->qtd - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }
}

/* ------------------------------------------------------------
   Limpa o buffer de entrada (restos deixados pelo scanf)
   ------------------------------------------------------------ */
void limpaBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descarta */
    }
}

/* ------------------------------------------------------------
   Le um inteiro do teclado. Retorna 1 se leu, 0 se a entrada
   foi invalida (nesse caso descarta a linha).
   ------------------------------------------------------------ */
int lerInt(int *valor) {
    if (scanf("%d", valor) != 1) {
        limpaBuffer();
        printf("Entrada invalida.\n");
        return 0;
    }
    limpaBuffer();
    return 1;
}

/* ------------------------------------------------------------
   Exibe o menu de opcoes
   ------------------------------------------------------------ */
void exibeMenu(void) {
    printf("\n===== MENU - Busca Binaria =====\n");
    printf("1 - Inserir uma chave\n");
    printf("2 - Inserir uma lista de chaves\n");
    printf("3 - Buscar chave (busca binaria)\n");
    printf("4 - Exibir vetor\n");
    printf("5 - Executar tudo automaticamente (fazTudo)\n");
    printf("6 - Preencher vetor automaticamente\n");
    printf("0 - Sair\n");
    printf("Escolha uma opcao: ");
}

/* ============================================================
   preencheLista
   Insere algumas chaves prontas (desordenadas e com repeticao)
   no vetor recebido. As demais operacoes (buscar, exibir etc.)
   continuam sendo feitas manualmente pelo menu.
   ============================================================ */
void preencheLista(ListaChaves *l) {
    insereChave(l, 42);
    insereChave(l, 7);
    insereChave(l, 19);
    insereChave(l, 7);
    insereChave(l, 88);
    insereChave(l, 7);
    insereChave(l, 3);
    insereChave(l, 19);
    insereChave(l, 56);
}

/* ------------------------------------------------------------
   Auxiliar: busca, imprime o resultado e libera a memoria
   ------------------------------------------------------------ */
static void buscaEImprime(ListaChaves *l, int chave) {
    Posicoes *res = buscaBinaria(l, chave);
    imprimeResultado(chave, res);
    liberaPosicoes(res);
}

/* ============================================================
   fazTudo
   Executa automaticamente, sem interacao do usuario, uma
   sequencia que demonstra a busca binaria: chave repetida,
   chave que aparece uma unica vez e chave inexistente.
   ============================================================ */
void fazTudo(void) {
    ListaChaves *l = criaListaChaves();
    ListaChaves *vazia;

    printf("=== Inserindo chaves (desordenadas, com repeticao) ===\n");
    preencheLista(l);
    imprimeChaves(l);

    printf("\n=== Chave repetida varias vezes (7) ===\n");
    buscaEImprime(l, 7);
    imprimeChaves(l);

    printf("\n=== Chave repetida duas vezes (19) ===\n");
    buscaEImprime(l, 19);

    printf("\n=== Chave que aparece uma unica vez (88) ===\n");
    buscaEImprime(l, 88);

    printf("\n=== Chave inexistente (50) ===\n");
    buscaEImprime(l, 50);

    printf("\n=== Busca em vetor vazio ===\n");
    vazia = criaListaChaves();
    buscaEImprime(vazia, 10);
    liberaListaChaves(vazia);

    liberaListaChaves(l);
}

/* ============================================================
   Programa principal - menu interativo
   ============================================================ */
int main(void) {
    ListaChaves *l = criaListaChaves();
    int opcao, chave, n, i;
    int continuar = 1;

    while (continuar) {
        exibeMenu();

        if (!lerInt(&opcao)) {
            continue;
        }

        switch (opcao) {
            case 1:
                printf("Digite a chave a inserir: ");
                if (lerInt(&chave)) {
                    insereChave(l, chave);
                    imprimeChaves(l);
                }
                break;

            case 2:
                printf("Quantas chaves deseja inserir? ");
                if (lerInt(&n)) {
                    if (n <= 0) {
                        printf("Quantidade invalida.\n");
                        break;
                    }
                    for (i = 0; i < n; i++) {
                        printf("Chave %d de %d: ", i + 1, n);
                        if (!lerInt(&chave)) {
                            printf("Insercao interrompida.\n");
                            break;
                        }
                        insereChave(l, chave);
                    }
                    imprimeChaves(l);
                }
                break;

            case 3:
                printf("Digite a chave a buscar: ");
                if (lerInt(&chave)) {
                    buscaEImprime(l, chave);
                    printf("(posicoes referentes ao vetor ordenado)\n");
                    imprimeChaves(l);
                }
                break;

            case 4:
                imprimeChaves(l);
                break;

            case 5:
                fazTudo();
                break;

            case 6:
                preencheLista(l);
                imprimeChaves(l);
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

    liberaListaChaves(l);

    return 0;
}