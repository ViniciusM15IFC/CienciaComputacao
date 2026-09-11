/* ============================================================
   Implementacao pratica de Pilha (Stack) em C
   Baseado no conteudo do slide "Pilhas"

   TAD armazenado: nome (string) - conforme sugerido no slide
   ("Os valores armazenados devem preferencialmente ser TAD").

   Estrutura do No (conforme "Estrutura basica" do slide):
       - nome : informacao armazenada (INFO / TAD)
       - prox : ponteiro para o elemento que fica logo abaixo
                dele na pilha (o elemento mais antigo)

   Estrutura da Pilha:
       - topo : ponteiro para o elemento no topo da pilha
       - size : quantidade de elementos na pilha

   Operacoes basicas (conforme slide "Operacoes"):
       - push()      -> empilha()
       - pop()       -> desempilha()
       - empty()     -> pilhaVazia()
       - stacktop()  -> consultaTopo()
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 100

/* ---------- Representacao em C (conforme o slide) ---------- */

typedef struct Node {
    char nome[TAM_NOME];
    struct Node *prox;
} Node;

typedef struct {
    Node *topo;
    int size;
} Pilha;

/* ============================================================
   Cria pilha
   Funcao que cria a pilha e aloca a memoria necessaria.
   ============================================================ */
Pilha *criaPilha(void) {
    Pilha *p = (Pilha *) malloc(sizeof(Pilha));
    if (p == NULL) {
        printf("Erro ao alocar memoria para a pilha.\n");
        exit(1);
    }
    p->topo = NULL;
    p->size = 0;
    return p;
}

/* ============================================================
   Cria elemento (No)
   Funcao que cria um novo no, aloca memoria necessaria e
   atribui o nome a este novo elemento.
   ============================================================ */
Node *criaNode(const char *nome) {
    Node *novo = (Node *) malloc(sizeof(Node));
    if (novo == NULL) {
        printf("Erro ao alocar memoria para o elemento.\n");
        exit(1);
    }
    strncpy(novo->nome, nome, TAM_NOME - 1);
    novo->nome[TAM_NOME - 1] = '\0';
    novo->prox = NULL;
    return novo;
}

/* ============================================================
   empty(s) / pilhaVazia
   Verifica se a pilha esta vazia.
   Retorna 1 (TRUE) se vazia, 0 (FALSE) caso contrario.
   ============================================================ */
int pilhaVazia(Pilha *p) {
    return (p->topo == NULL) ? 1 : 0;
}

/* ============================================================
   push(s, i) / empilha
   Inclui um novo elemento no topo da pilha.
   Nao ha limite maximo (limitado apenas pela memoria
   disponivel), entao push pode ser usado mesmo com a pilha
   vazia.
   ============================================================ */
void empilha(Pilha *p, const char *nome) {
    Node *novo = criaNode(nome);

    novo->prox = p->topo;
    p->topo = novo;

    p->size++;
}

/* ============================================================
   pop(s) / desempilha
   Remove o elemento do topo da pilha e retorna seu valor por
   meio do parametro nomeRemovido.
   Nao pode ser aplicada a uma pilha vazia (erro de underflow).
   Retorna 1 em caso de sucesso, 0 se a pilha estava vazia.
   ============================================================ */
int desempilha(Pilha *p, char *nomeRemovido) {
    Node *removido;

    if (pilhaVazia(p)) {
        return 0; /* underflow: pilha vazia */
    }

    removido = p->topo;
    strncpy(nomeRemovido, removido->nome, TAM_NOME - 1);
    nomeRemovido[TAM_NOME - 1] = '\0';

    p->topo = removido->prox;
    free(removido);
    p->size--;

    return 1;
}

/* ============================================================
   stacktop(s) / consultaTopo
   Retorna (consulta) o elemento que esta no topo da pilha, sem
   remove-lo. Assim como pop(), nao pode ser aplicada a uma
   pilha vazia (erro de underflow).
   Retorna 1 em caso de sucesso, 0 se a pilha estava vazia.
   ============================================================ */
int consultaTopo(Pilha *p, char *nomeTopo) {
    if (pilhaVazia(p)) {
        return 0; /* underflow: pilha vazia */
    }

    strncpy(nomeTopo, p->topo->nome, TAM_NOME - 1);
    nomeTopo[TAM_NOME - 1] = '\0';

    return 1;
}

/* ============================================================
   Consulta valores e posicoes da pilha
   Percorre a pilha a partir do topo (posicao 0) ate a base,
   exibindo a posicao e o nome de cada elemento. Nao remove nem
   altera nenhum elemento.
   ============================================================ */
void consultaPosicoes(Pilha *p) {
    Node *atual = p->topo;
    int posicao = 0;

    if (pilhaVazia(p)) {
        printf("Pilha vazia. Nao ha elementos para consultar.\n");
        return;
    }

    printf("--- Valores e posicoes (0 = topo) ---\n");
    while (atual != NULL) {
        printf("Posicao %d: %s\n", posicao, atual->nome);
        atual = atual->prox;
        posicao++;
    }
}

/* ============================================================
   Imprime a pilha em formato vertical
   O topo fica visualmente no alto, como uma pilha real de
   caixas/livros, e a base no final.
   ============================================================ */
void imprimePilha(Pilha *p) {
    Node *atual = p->topo;

    printf("\n===== PILHA (size=%d) =====\n", p->size);

    if (pilhaVazia(p)) {
        printf("(pilha vazia)\n");
        printf("============================\n");
        return;
    }

    while (atual != NULL) {
        if (atual == p->topo) {
            printf("topo -> | %-20s |\n", atual->nome);
        } else {
            printf("        | %-20s |\n", atual->nome);
        }
        atual = atual->prox;
    }
    printf("============================\n");
}

/* ------------------------------------------------------------
   Libera toda a memoria alocada pela pilha
   ------------------------------------------------------------ */
void liberaPilha(Pilha *p) {
    Node *atual = p->topo;
    Node *proximo;

    while (atual != NULL) {
        proximo = atual->prox;
        free(atual);
        atual = proximo;
    }

    free(p);
}

/* ------------------------------------------------------------
   Limpa o buffer de entrada (restos deixados pelo scanf/fgets)
   ------------------------------------------------------------ */
void limpaBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descarta */
    }
}

/* ------------------------------------------------------------
   Le um nome do teclado (aceita espacos) e remove o '\n' final
   ------------------------------------------------------------ */
void leNome(char *destino) {
    if (fgets(destino, TAM_NOME, stdin) != NULL) {
        size_t len = strlen(destino);
        if (len > 0 && destino[len - 1] == '\n') {
            destino[len - 1] = '\0';
        }
    } else {
        destino[0] = '\0';
    }
}

/* ------------------------------------------------------------
   Exibe o menu de opcoes
   ------------------------------------------------------------ */
void exibeMenu(void) {
    printf("\n===== MENU - Pilha de Nomes =====\n");
    printf("1 - Empilhar (push)\n");
    printf("2 - Desempilhar (pop)\n");
    printf("3 - Verificar se a pilha esta vazia\n");
    printf("4 - Consultar o topo (sem remover)\n");
    printf("5 - Consultar valores e posicoes\n");
    printf("6 - Imprimir pilha (formato vertical)\n");
    printf("7 - Executar tudo automaticamente (fazTudo)\n");
    printf("8 - Preencher pilha automaticamente\n");
    printf("0 - Sair\n");
    printf("Escolha uma opcao: ");
}

/* ============================================================
   preenchePilha
   Empilha alguns nomes prontos na pilha recebida. As demais
   operacoes (desempilhar, consultar, etc.) continuam sendo
   feitas manualmente pelo menu.
   ============================================================ */
void preenchePilha(Pilha *p) {
    empilha(p, "Ana");
    empilha(p, "Bruno");
    empilha(p, "Carla");
    empilha(p, "Diego");
    empilha(p, "Elisa");
}

/* ============================================================
   fazTudo
   Executa automaticamente, sem interacao do usuario, uma
   sequencia de operacoes que demonstra a pilha: push, consulta
   de topo, consulta de posicoes, pop e verificacao de vazia.
   ============================================================ */
void fazTudo(void) {
    Pilha *p = criaPilha();
    char nomeTopo[TAM_NOME];
    char nomeRemovido[TAM_NOME];

    printf("=== Verificando se a pilha esta vazia (deve ser SIM) ===\n");
    printf("Vazia? %s\n", pilhaVazia(p) ? "SIM" : "NAO");

    printf("\n=== Empilhando nomes ===\n");
    empilha(p, "Ana");
    imprimePilha(p);
    empilha(p, "Bruno");
    imprimePilha(p);
    empilha(p, "Carla");
    imprimePilha(p);

    printf("\n=== Consultando o topo (sem remover) ===\n");
    if (consultaTopo(p, nomeTopo)) {
        printf("Topo atual: %s\n", nomeTopo);
    }

    printf("\n=== Consultando valores e posicoes ===\n");
    consultaPosicoes(p);

    printf("\n=== Desempilhando ===\n");
    if (desempilha(p, nomeRemovido)) {
        printf("Removido do topo: %s\n", nomeRemovido);
    }
    imprimePilha(p);

    printf("\n=== Verificando se a pilha esta vazia (deve ser NAO) ===\n");
    printf("Vazia? %s\n", pilhaVazia(p) ? "SIM" : "NAO");

    liberaPilha(p);
}

/* ============================================================
   Programa principal - menu interativo
   ============================================================ */
int main(void) {
    Pilha *p = criaPilha();
    int opcao;
    char nome[TAM_NOME];
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
                printf("Digite o nome a empilhar: ");
                leNome(nome);
                empilha(p, nome);
                imprimePilha(p);
                break;

            case 2:
                if (desempilha(p, nome)) {
                    printf("Removido do topo: %s\n", nome);
                } else {
                    printf("Pilha vazia. Nao ha nada para desempilhar.\n");
                }
                imprimePilha(p);
                break;

            case 3:
                printf("A pilha esta vazia? %s\n", pilhaVazia(p) ? "SIM" : "NAO");
                break;

            case 4:
                if (consultaTopo(p, nome)) {
                    printf("Elemento no topo: %s\n", nome);
                } else {
                    printf("Pilha vazia. Nao ha topo para consultar.\n");
                }
                break;

            case 5:
                consultaPosicoes(p);
                break;

            case 6:
                imprimePilha(p);
                break;

            case 7:
                fazTudo();
                break;

            case 8:
                preenchePilha(p);
                imprimePilha(p);
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

    liberaPilha(p);

    return 0;
}