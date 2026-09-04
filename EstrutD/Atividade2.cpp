/* ============================================================
   Implementacao pratica de Lista Encadeada Dupla (LED) em C
   Baseado no conteudo do slide "Lista encadeada dupla"

   Estrutura do Elemento (NO):
       - next : ponteiro (ELO Posterior) para o proximo elemento
       - prev : ponteiro (ELO Anterior) para o elemento anterior
       - dado : informacao armazenada (INFO)

   Estrutura da Lista:
       - head : ponteiro para o primeiro elemento (cabeca)
       - tail : ponteiro para o ultimo elemento (cauda)
       - size : quantidade de elementos na lista

   Funcoes implementadas (conforme slide "Principais funcoes"):
       - criaLista()
       - criaElemento()
       - insereInicio() / insereFim() / inserePosicao()
       - removeInicio() / removeFim() / removePosicao()
       - percorreListaFrente()  (cabeca -> cauda)
       - percorreListaTras()    (cauda -> cabeca)
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>

/* ---------- Representacao em C (conforme o slide) ---------- */

typedef struct sElemento {
    struct sElemento *next;
    struct sElemento *prev;
    int dado; /* Depende do tipo de dados que se deseja trabalhar */
} Elemento;

typedef struct sLista {
    struct sElemento *head;
    struct sElemento *tail;
    int size;
} Lista;

/* ============================================================
   1 - Cria lista
   Funcao que cria a lista e aloca a memoria necessaria.
   ============================================================ */
Lista *criaLista(void) {
    Lista *lista = (Lista *) malloc(sizeof(Lista));
    if (lista == NULL) {
        printf("Erro ao alocar memoria para a lista.\n");
        exit(1);
    }
    lista->head = NULL;
    lista->tail = NULL;
    lista->size = 0;
    return lista;
}

/* ============================================================
   2 - Cria elemento
   Funcao que cria um novo no, aloca memoria necessaria e
   atribui um valor para este novo elemento.
   ============================================================ */
Elemento *criaElemento(int valor) {
    Elemento *novo = (Elemento *) malloc(sizeof(Elemento));
    if (novo == NULL) {
        printf("Erro ao alocar memoria para o elemento.\n");
        exit(1);
    }
    novo->dado = valor;
    novo->next = NULL; /* elo posterior inicia apontando para NULL */
    novo->prev = NULL; /* elo anterior inicia apontando para NULL */
    return novo;
}

/* ============================================================
   3 - Insere elemento no INICIO da lista
   - Cria o novo no.
   - Ajusta o next do novo no para apontar para o antigo head.
   - Ajusta o prev do antigo head (se existir) para o novo no.
   - Atualiza a cabeca da lista para o novo no.
   ============================================================ */
void insereInicio(Lista *lista, int valor) {
    Elemento *novo = criaElemento(valor);

    novo->next = lista->head;
    novo->prev = NULL;

    if (lista->head != NULL) {
        lista->head->prev = novo;
    } else {
        /* lista estava vazia: o novo no tambem e a cauda */
        lista->tail = novo;
    }

    lista->head = novo;
    lista->size++;
}

/* ============================================================
   4 - Insere elemento no FINAL da lista
   - Cria o novo no.
   - Ajusta o next do ultimo no (tail) para apontar para o novo no.
   - Ajusta o prev do novo no para apontar para o antigo tail.
   - Atualiza a cauda da lista para o novo no.
   ============================================================ */
void insereFim(Lista *lista, int valor) {
    Elemento *novo = criaElemento(valor);

    novo->next = NULL;
    novo->prev = lista->tail;

    if (lista->tail != NULL) {
        lista->tail->next = novo;
    } else {
        /* lista estava vazia: o novo no tambem e a cabeca */
        lista->head = novo;
    }

    lista->tail = novo;
    lista->size++;
}

/* ============================================================
   5 - Insere elemento em uma POSICAO ESPECIFICA
   - Cria o novo no.
   - Percorre a lista ate o no de referencia (o que ficara antes
     do novo no).
   - Ajusta o next do novo no para o no seguinte ao de referencia.
   - Ajusta o prev do novo no para o no de referencia.
   - Ajusta o next do no de referencia para o novo no.
   - Ajusta o prev do no seguinte (se existir) para o novo no.

   posicao 0 = inicio da lista.
   ============================================================ */
void inserePosicao(Lista *lista, int valor, int posicao) {
    Elemento *novo, *referencia, *seguinte;
    int i;

    if (posicao <= 0 || lista->head == NULL) {
        insereInicio(lista, valor);
        return;
    }

    if (posicao >= lista->size) {
        insereFim(lista, valor);
        return;
    }

    /* localiza o no de referencia (anterior a posicao desejada) */
    referencia = lista->head;
    for (i = 0; i < posicao - 1; i++) {
        referencia = referencia->next;
    }
    seguinte = referencia->next;

    novo = criaElemento(valor);
    novo->next = seguinte;
    novo->prev = referencia;

    referencia->next = novo;
    if (seguinte != NULL) {
        seguinte->prev = novo;
    }

    lista->size++;
}

/* ============================================================
   Removendo o No do INICIO da lista
   - Verifica se a lista esta vazia.
   - Armazena o ponteiro do no a ser removido (o head).
   - Atualiza o head para o proximo no.
   - Ajusta o prev do novo head (se existir) para NULL.
   - Libera a memoria do no removido.
   ============================================================ */
void removeInicio(Lista *lista) {
    Elemento *removido;

    if (lista->head == NULL) {
        printf("Lista vazia. Nao ha nada para remover.\n");
        return;
    }

    removido = lista->head;
    lista->head = removido->next;

    if (lista->head != NULL) {
        lista->head->prev = NULL;
    } else {
        /* lista ficou vazia */
        lista->tail = NULL;
    }

    free(removido);
    lista->size--;
}

/* ============================================================
   Removendo o No do FINAL da lista
   - Verifica se a lista esta vazia.
   - Armazena o ponteiro do no a ser removido (o tail).
   - Atualiza o tail para o no anterior (usando o proprio prev,
     sem precisar percorrer a lista, vantagem da lista dupla).
   - Ajusta o next do novo tail (se existir) para NULL.
   - Libera a memoria do no removido.
   ============================================================ */
void removeFim(Lista *lista) {
    Elemento *removido;

    if (lista->tail == NULL) {
        printf("Lista vazia. Nao ha nada para remover.\n");
        return;
    }

    removido = lista->tail;
    lista->tail = removido->prev;

    if (lista->tail != NULL) {
        lista->tail->next = NULL;
    } else {
        /* lista ficou vazia */
        lista->head = NULL;
    }

    free(removido);
    lista->size--;
}

/* ============================================================
   Removendo um No em uma POSICAO ESPECIFICA
   - Verifica se a lista esta vazia e se a posicao e valida.
   - Percorre a lista ate o no a ser removido.
   - Ajusta o next do no anterior para o no seguinte ao removido.
   - Ajusta o prev do no seguinte para o no anterior ao removido.
   - Libera a memoria do no removido.

   posicao 0 = primeiro elemento.
   ============================================================ */
void removePosicao(Lista *lista, int posicao) {
    Elemento *atual;
    int i;

    if (lista->head == NULL) {
        printf("Lista vazia. Nao ha nada para remover.\n");
        return;
    }

    if (posicao < 0 || posicao >= lista->size) {
        printf("Posicao invalida.\n");
        return;
    }

    if (posicao == 0) {
        removeInicio(lista);
        return;
    }

    if (posicao == lista->size - 1) {
        removeFim(lista);
        return;
    }

    atual = lista->head;
    for (i = 0; i < posicao; i++) {
        atual = atual->next;
    }

    atual->prev->next = atual->next;
    atual->next->prev = atual->prev;

    free(atual);
    lista->size--;
}

/* ============================================================
   Percorre a lista no sentido CABECA -> CAUDA (pesquisa)
   Retorna a posicao (indice) do elemento ou -1 se nao encontrado.
   ============================================================ */
int percorreListaFrente(Lista *lista, int valorBuscado) {
    Elemento *atual = lista->head;
    int posicao = 0;

    while (atual != NULL) {
        if (atual->dado == valorBuscado) {
            return posicao;
        }
        atual = atual->next;
        posicao++;
    }

    return -1; /* nao encontrado */
}

/* ============================================================
   Percorre a lista no sentido CAUDA -> CABECA (pesquisa)
   Aproveita o ponteiro prev, caracteristica exclusiva da lista
   encadeada dupla.
   Retorna a posicao (indice, contada a partir do head) do
   elemento ou -1 se nao encontrado.
   ============================================================ */
int percorreListaTras(Lista *lista, int valorBuscado) {
    Elemento *atual = lista->tail;
    int posicao = lista->size - 1;

    while (atual != NULL) {
        if (atual->dado == valorBuscado) {
            return posicao;
        }
        atual = atual->prev;
        posicao--;
    }

    return -1; /* nao encontrado */
}

/* ------------------------------------------------------------
   Imprime a lista no sentido CABECA -> CAUDA
   ------------------------------------------------------------ */
void imprimeListaFrente(Lista *lista) {
    Elemento *atual = lista->head;

    printf("Lista (frente) [size=%d]: NULL <- ", lista->size);

    if (atual == NULL) {
        printf("(vazia)\n");
        return;
    }

    while (atual != NULL) {
        printf("%d", atual->dado);
        if (atual->next != NULL) {
            printf(" <-> ");
        }
        atual = atual->next;
    }
    printf(" -> NULL\n");
}

/* ------------------------------------------------------------
   Imprime a lista no sentido CAUDA -> CABECA
   ------------------------------------------------------------ */
void imprimeListaTras(Lista *lista) {
    Elemento *atual = lista->tail;

    printf("Lista (tras)   [size=%d]: NULL <- ", lista->size);

    if (atual == NULL) {
        printf("(vazia)\n");
        return;
    }

    while (atual != NULL) {
        printf("%d", atual->dado);
        if (atual->prev != NULL) {
            printf(" <-> ");
        }
        atual = atual->prev;
    }
    printf(" -> NULL\n");
}

/* ------------------------------------------------------------
   Libera toda a memoria alocada pela lista
   ------------------------------------------------------------ */
void liberaLista(Lista *lista) {
    Elemento *atual = lista->head;
    Elemento *proximo;

    while (atual != NULL) {
        proximo = atual->next;
        free(atual);
        atual = proximo;
    }

    free(lista);
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
   Exibe o menu de opcoes
   ------------------------------------------------------------ */
void exibeMenu(void) {
    printf("\n===== MENU - Lista Encadeada Dupla =====\n");
    printf("1  - Inserir no inicio\n");
    printf("2  - Inserir no final\n");
    printf("3  - Inserir em posicao especifica\n");
    printf("4  - Remover do inicio\n");
    printf("5  - Remover do final\n");
    printf("6  - Remover de posicao especifica\n");
    printf("7  - Pesquisar valor (sentido cabeca -> cauda)\n");
    printf("8  - Pesquisar valor (sentido cauda -> cabeca)\n");
    printf("9  - Exibir lista (cabeca -> cauda)\n");
    printf("10 - Exibir lista (cauda -> cabeca)\n");
    printf("11 - Executar tudo automaticamente (fazTudo)\n");
    printf("12 - Preencher lista automaticamente\n");
    printf("0  - Sair\n");
    printf("Escolha uma opcao: ");
}

/* ============================================================
   preencheLista
   Preenche a lista recebida com alguns valores prontos, usando
   insereFim(). As demais operacoes (remover, pesquisar, inserir
   em posicao especifica etc.) continuam sendo feitas
   manualmente pelo menu.
   ============================================================ */
void preencheLista(Lista *lista) {
    insereFim(lista, 10);
    insereFim(lista, 20);
    insereFim(lista, 30);
    insereFim(lista, 40);
    insereFim(lista, 50);
}

/* ============================================================
   fazTudo
   Executa automaticamente, sem interacao do usuario, uma
   sequencia de operacoes que demonstra a lista: insercoes,
   pesquisas nos dois sentidos e remocoes.
   ============================================================ */
void fazTudo(void) {
    Lista *lista = criaLista();
    int posicaoEncontrada;

    printf("=== Insercao no final ===\n");
    insereFim(lista, 10);
    insereFim(lista, 20);
    insereFim(lista, 30);
    imprimeListaFrente(lista);

    printf("\n=== Insercao no inicio ===\n");
    insereInicio(lista, 5);
    imprimeListaFrente(lista);

    printf("\n=== Insercao em posicao especifica (posicao 2) ===\n");
    inserePosicao(lista, 15, 2);
    imprimeListaFrente(lista);

    printf("\n=== Exibicao no sentido cauda -> cabeca ===\n");
    imprimeListaTras(lista);

    printf("\n=== Pesquisa cabeca -> cauda ===\n");
    posicaoEncontrada = percorreListaFrente(lista, 15);
    if (posicaoEncontrada != -1) {
        printf("Valor 15 encontrado na posicao %d\n", posicaoEncontrada);
    } else {
        printf("Valor 15 nao encontrado\n");
    }

    printf("\n=== Pesquisa cauda -> cabeca ===\n");
    posicaoEncontrada = percorreListaTras(lista, 5);
    if (posicaoEncontrada != -1) {
        printf("Valor 5 encontrado na posicao %d\n", posicaoEncontrada);
    } else {
        printf("Valor 5 nao encontrado\n");
    }

    printf("\n=== Remocao do inicio ===\n");
    removeInicio(lista);
    imprimeListaFrente(lista);

    printf("\n=== Remocao do final ===\n");
    removeFim(lista);
    imprimeListaFrente(lista);

    printf("\n=== Remocao em posicao especifica (posicao 1) ===\n");
    removePosicao(lista, 1);
    imprimeListaFrente(lista);

    liberaLista(lista);
}

/* ============================================================
   Programa principal - menu interativo
   ============================================================ */
int main(void) {
    Lista *lista = criaLista();
    int opcao;
    int valor, posicao, resultado;
    int continuar = 1;

    while (continuar) {
        exibeMenu();

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida.\n");
            limpaBuffer();
            continue;
        }
        limpaBuffer();

        switch (opcao) {
            case 1:
                printf("Digite o valor a inserir no inicio: ");
                scanf("%d", &valor);
                limpaBuffer();
                insereInicio(lista, valor);
                imprimeListaFrente(lista);
                break;

            case 2:
                printf("Digite o valor a inserir no final: ");
                scanf("%d", &valor);
                limpaBuffer();
                insereFim(lista, valor);
                imprimeListaFrente(lista);
                break;

            case 3:
                printf("Digite o valor a inserir: ");
                scanf("%d", &valor);
                limpaBuffer();
                printf("Digite a posicao (0 = inicio): ");
                scanf("%d", &posicao);
                limpaBuffer();
                inserePosicao(lista, valor, posicao);
                imprimeListaFrente(lista);
                break;

            case 4:
                removeInicio(lista);
                imprimeListaFrente(lista);
                break;

            case 5:
                removeFim(lista);
                imprimeListaFrente(lista);
                break;

            case 6:
                printf("Digite a posicao a remover (0 = inicio): ");
                scanf("%d", &posicao);
                limpaBuffer();
                removePosicao(lista, posicao);
                imprimeListaFrente(lista);
                break;

            case 7:
                printf("Digite o valor a pesquisar: ");
                scanf("%d", &valor);
                limpaBuffer();
                resultado = percorreListaFrente(lista, valor);
                if (resultado != -1) {
                    printf("Valor %d encontrado na posicao %d (cabeca->cauda)\n", valor, resultado);
                } else {
                    printf("Valor %d nao encontrado na lista\n", valor);
                }
                break;

            case 8:
                printf("Digite o valor a pesquisar: ");
                scanf("%d", &valor);
                limpaBuffer();
                resultado = percorreListaTras(lista, valor);
                if (resultado != -1) {
                    printf("Valor %d encontrado na posicao %d (cauda->cabeca)\n", valor, resultado);
                } else {
                    printf("Valor %d nao encontrado na lista\n", valor);
                }
                break;

            case 9:
                imprimeListaFrente(lista);
                break;

            case 10:
                imprimeListaTras(lista);
                break;

            case 11:
                fazTudo();
                break;

            case 12:
                preencheLista(lista);
                imprimeListaFrente(lista);
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

    liberaLista(lista);

    return 0;
}