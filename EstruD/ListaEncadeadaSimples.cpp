/* ============================================================
   Implementacao pratica de Lista Encadeada Simples em C
   Baseado no conteudo do slide "Listas encadeadas simples"

   Estrutura do Elemento (NO):
       - next : ponteiro (ELO) para o proximo elemento
       - dado : informacao armazenada (INFO)

   Estrutura da Lista:
       - head : ponteiro para o primeiro elemento (cabeca)
       - tail : ponteiro para o ultimo elemento (cauda)
       - size : quantidade de elementos na lista

   Funcoes implementadas (conforme slide "Principais funcoes"):
       - criaLista()
       - criaElemento()
       - insereInicio() / insereFim() / insereFinal()... (posicao especifica)
       - removeInicio() / removeFim() / removePosicao()
       - percorreLista() (pesquisa / impressao)
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>

/* ---------- Representacao em C (conforme o slide) ---------- */

typedef struct sElemento {
    struct sElemento *next;
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
    novo->next = NULL; /* elo do novo no inicia apontando para NULL */
    return novo;
}

/* ============================================================
   3 - Insere elemento no INICIO da lista
   - Cria o novo no.
   - O next do novo no aponta para o antigo head.
   - head passa a ser o novo no.
   ============================================================ */
void insereInicio(Lista *lista, int valor) {
    Elemento *novo = criaElemento(valor);

    novo->next = lista->head;
    lista->head = novo;

    /* se a lista estava vazia, o novo no tambem e a cauda */
    if (lista->tail == NULL) {
        lista->tail = novo;
    }

    lista->size++;
}

/* ============================================================
   4 - Insere elemento no FINAL da lista
   - Cria o novo no.
   - Percorre a lista ate o ultimo no (aqui usamos o ponteiro tail
     para acesso direto, conforme a estrutura definida no slide).
   - O next do ultimo no passa a apontar para o novo no.
   ============================================================ */
void insereFim(Lista *lista, int valor) {
    Elemento *novo = criaElemento(valor);

    if (lista->head == NULL) {
        /* lista vazia: o novo no e o head e a tail */
        lista->head = novo;
        lista->tail = novo;
    } else {
        lista->tail->next = novo;
        lista->tail = novo;
    }

    lista->size++;
}

/* ============================================================
   5 - Insere elemento em uma POSICAO ESPECIFICA
   - Cria o novo no.
   - Percorre a lista ate o no anterior a posicao desejada.
   - O next do novo no aponta para o no seguinte.
   - O next do no anterior passa a apontar para o novo no.

   posicao 0 = inicio da lista.
   ============================================================ */
void inserePosicao(Lista *lista, int valor, int posicao) {
    Elemento *novo, *atual, *anterior;
    int i;

    if (posicao <= 0 || lista->head == NULL) {
        insereInicio(lista, valor);
        return;
    }

    if (posicao >= lista->size) {
        insereFim(lista, valor);
        return;
    }

    novo = criaElemento(valor);
    anterior = lista->head;
    for (i = 0; i < posicao - 1; i++) {
        anterior = anterior->next;
    }
    atual = anterior->next;

    novo->next = atual;
    anterior->next = novo;

    lista->size++;
}

/* ============================================================
   Removendo o No do INICIO da lista
   - Verifica se a lista esta vazia.
   - Armazena o ponteiro do no a ser removido (o head).
   - Atualiza o head para o proximo no.
   - Libera a memoria do no removido.
   ============================================================ */
void removeInicio(Lista *lista) {
    Elemento *removido;

    if (lista->head == NULL) {
        printf("Lista vazia. Nao ha nada para remover.\n");
        return;
    }

    removido = lista->head;
    lista->head = lista->head->next;

    /* caso especial: lista com apenas um no */
    if (lista->head == NULL) {
        lista->tail = NULL;
    }

    free(removido);
    lista->size--;
}

/* ============================================================
   Removendo o No do FINAL da lista
   - Verifica se a lista esta vazia.
   - Percorre a lista ate o penultimo no.
   - Ajusta o next do penultimo no para NULL.
   - Libera a memoria do no removido.
   ============================================================ */
void removeFim(Lista *lista) {
    Elemento *atual, *penultimo;

    if (lista->head == NULL) {
        printf("Lista vazia. Nao ha nada para remover.\n");
        return;
    }

    /* caso especial: lista com apenas um no */
    if (lista->head == lista->tail) {
        free(lista->head);
        lista->head = NULL;
        lista->tail = NULL;
        lista->size--;
        return;
    }

    penultimo = lista->head;
    atual = lista->head;
    while (atual->next != NULL) {
        penultimo = atual;
        atual = atual->next;
    }

    penultimo->next = NULL;
    lista->tail = penultimo;

    free(atual);
    lista->size--;
}

/* ============================================================
   Removendo um No em uma POSICAO ESPECIFICA
   - Verifica se a lista esta vazia.
   - Percorre a lista ate o no anterior a posicao desejada.
   - Armazena o ponteiro do no a ser removido.
   - Ajusta o next do no anterior para apontar para o proximo
     no do que sera removido.
   - Libera a memoria do no removido.

   posicao 0 = primeiro elemento.
   ============================================================ */
void removePosicao(Lista *lista, int posicao) {
    Elemento *atual, *anterior;
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

    anterior = lista->head;
    for (i = 0; i < posicao - 1; i++) {
        anterior = anterior->next;
    }
    atual = anterior->next;

    anterior->next = atual->next;

    free(atual);
    lista->size--;
}

/* ============================================================
   Percorre a lista para encontrar elementos (pesquisa)
   Percorre a lista a partir da cabeca (head) ate encontrar
   o elemento buscado ou atingir o fim da lista.
   Retorna a posicao (indice) do elemento ou -1 se nao encontrado.
   ============================================================ */
int percorreLista(Lista *lista, int valorBuscado) {
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

/* ------------------------------------------------------------
   Funcao auxiliar (nao faz parte da lista de funcoes do slide,
   mas e util para visualizar o resultado das operacoes)
   ------------------------------------------------------------ */
void imprimeLista(Lista *lista) {
    Elemento *atual = lista->head;

    printf("Lista [size=%d]: ", lista->size);

    if (atual == NULL) {
        printf("(vazia)\n");
        return;
    }

    while (atual != NULL) {
        printf("%d", atual->dado);
        if (atual->next != NULL) {
            printf(" -> ");
        }
        atual = atual->next;
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
    printf("\n===== MENU - Lista Encadeada Simples =====\n");
    printf("1  - Inserir no inicio\n");
    printf("2  - Inserir no final\n");
    printf("3  - Inserir em posicao especifica\n");
    printf("4  - Remover do inicio\n");
    printf("5  - Remover do final\n");
    printf("6  - Remover de posicao especifica\n");
    printf("7  - Pesquisar valor (percorrer lista)\n");
    printf("8  - Exibir lista\n");
    printf("9  - Executar tudo automaticamente (fazTudo)\n");
    printf("0  - Sair\n");
    printf("Escolha uma opcao: ");
}

/* ============================================================
   fazTudo
   Executa automaticamente, sem interacao do usuario, a mesma
   sequencia de operacoes usada para demonstrar a lista:
   insercoes, pesquisa e remocoes.
   ============================================================ */
void fazTudo(void) {
    Lista *lista = criaLista();
    int posicaoEncontrada;

    printf("=== Insercao no final ===\n");
    insereFim(lista, 10);
    insereFim(lista, 20);
    insereFim(lista, 30);
    imprimeLista(lista);

    printf("\n=== Insercao no inicio ===\n");
    insereInicio(lista, 5);
    imprimeLista(lista);

    printf("\n=== Insercao em posicao especifica (posicao 2) ===\n");
    inserePosicao(lista, 15, 2);
    imprimeLista(lista);

    printf("\n=== Pesquisa (percorre lista) ===\n");
    posicaoEncontrada = percorreLista(lista, 15);
    if (posicaoEncontrada != -1) {
        printf("Valor 15 encontrado na posicao %d\n", posicaoEncontrada);
    } else {
        printf("Valor 15 nao encontrado\n");
    }

    posicaoEncontrada = percorreLista(lista, 100);
    if (posicaoEncontrada != -1) {
        printf("Valor 100 encontrado na posicao %d\n", posicaoEncontrada);
    } else {
        printf("Valor 100 nao encontrado\n");
    }

    printf("\n=== Remocao do inicio ===\n");
    removeInicio(lista);
    imprimeLista(lista);

    printf("\n=== Remocao do final ===\n");
    removeFim(lista);
    imprimeLista(lista);

    printf("\n=== Remocao em posicao especifica (posicao 1) ===\n");
    removePosicao(lista, 1);
    imprimeLista(lista);

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
                imprimeLista(lista);
                break;

            case 2:
                printf("Digite o valor a inserir no final: ");
                scanf("%d", &valor);
                limpaBuffer();
                insereFim(lista, valor);
                imprimeLista(lista);
                break;

            case 3:
                printf("Digite o valor a inserir: ");
                scanf("%d", &valor);
                limpaBuffer();
                printf("Digite a posicao (0 = inicio): ");
                scanf("%d", &posicao);
                limpaBuffer();
                inserePosicao(lista, valor, posicao);
                imprimeLista(lista);
                break;

            case 4:
                removeInicio(lista);
                imprimeLista(lista);
                break;

            case 5:
                removeFim(lista);
                imprimeLista(lista);
                break;

            case 6:
                printf("Digite a posicao a remover (0 = inicio): ");
                scanf("%d", &posicao);
                limpaBuffer();
                removePosicao(lista, posicao);
                imprimeLista(lista);
                break;

            case 7:
                printf("Digite o valor a pesquisar: ");
                scanf("%d", &valor);
                limpaBuffer();
                resultado = percorreLista(lista, valor);
                if (resultado != -1) {
                    printf("Valor %d encontrado na posicao %d\n", valor, resultado);
                } else {
                    printf("Valor %d nao encontrado na lista\n", valor);
                }
                break;

            case 8:
                imprimeLista(lista);
                break;

            case 9:
                fazTudo();
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