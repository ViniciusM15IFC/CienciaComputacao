/* ============================================================
   Fila de atendimento bancario em C
   Baseado no conteudo do slide "Filas"

   Escolhas de implementacao:
       - Lista encadeada dupla (next/prev) com ADM
       - TAD Cliente: senha + nome + tipo (normal/prioridade)
       - Duas filas: a principal (normal) e a secundaria
         (prioritaria). A fila prioritaria e sempre atendida
         primeiro.
       - Senha automatica unica, iniciando em 1, compartilhada
         pelas duas filas (a ordem de chegada fica registrada
         na senha).

   Estrutura do Elemento (NO):
       - next    : ponteiro para o proximo da fila
       - prev    : ponteiro para o anterior na fila
       - cliente : TAD com os dados do cliente

   Estrutura da Fila:
       - head : primeiro da fila (front)  -> remove() acontece aqui
       - tail : ultimo da fila   (rear)   -> insert() acontece aqui
       - size : quantidade de clientes na fila

   Operacoes primitivas (conforme slide "Operacoes"):
       - insert(q, x) -> insereFila()
       - remove(q)    -> removeFila()
       - empty(q)     -> filaVazia()
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 100

#define TIPO_NORMAL 0
#define TIPO_PRIORIDADE 1

/* ---------- TAD Cliente ---------- */

typedef struct sCliente {
    int senha;
    char nome[TAM_NOME];
    int tipo; /* TIPO_NORMAL ou TIPO_PRIORIDADE */
} Cliente;

/* ---------- Representacao da LED ---------- */

typedef struct sElemento {
    struct sElemento *next;
    struct sElemento *prev;
    Cliente cliente;
} Elemento;

typedef struct sFila {
    struct sElemento *head;
    struct sElemento *tail;
    int size;
} Fila;

/* ---------- Banco: agrupa as duas filas e o contador de senha ---------- */

typedef struct sBanco {
    Fila *normal;      /* fila principal */
    Fila *prioritaria; /* fila secundaria (prioridade) */
    int proximaSenha;  /* proxima senha automatica, inicia em 1 */
} Banco;

/* ============================================================
   Cria fila
   Funcao que cria a fila e aloca a memoria necessaria.
   ============================================================ */
Fila *criaFila(void) {
    Fila *f = (Fila *) malloc(sizeof(Fila));
    if (f == NULL) {
        printf("Erro ao alocar memoria para a fila.\n");
        exit(1);
    }
    f->head = NULL;
    f->tail = NULL;
    f->size = 0;
    return f;
}

/* ============================================================
   Cria elemento
   Aloca um novo no e copia os dados do cliente para ele.
   ============================================================ */
Elemento *criaElemento(Cliente c) {
    Elemento *novo = (Elemento *) malloc(sizeof(Elemento));
    if (novo == NULL) {
        printf("Erro ao alocar memoria para o elemento.\n");
        exit(1);
    }
    novo->cliente = c;
    novo->next = NULL;
    novo->prev = NULL;
    return novo;
}

/* ============================================================
   empty(q) / filaVazia
   Retorna 1 (verdadeiro) se a fila esta vazia, 0 caso contrario.
   Implementada pelo size da lista, como sugerido no slide.
   ============================================================ */
int filaVazia(Fila *f) {
    return (f->size == 0) ? 1 : 0;
}

/* ============================================================
   insert(q, x) / insereFila
   Insere o cliente no FIM da fila (pelo tail).
   Overflow so ocorre se faltar memoria (tratado em criaElemento).
   ============================================================ */
void insereFila(Fila *f, Cliente c) {
    Elemento *novo = criaElemento(c);

    novo->prev = f->tail;
    novo->next = NULL;

    if (f->tail != NULL) {
        f->tail->next = novo;
    } else {
        /* fila estava vazia: o novo tambem e o head */
        f->head = novo;
    }

    f->tail = novo;
    f->size++;
}

/* ============================================================
   remove(q) / removeFila
   Remove o primeiro da fila (head) e retorna o cliente removido
   por meio do parametro 'removido'.
   Retorna 1 em caso de sucesso, 0 se a fila estava vazia
   (erro de underflow).
   ============================================================ */
int removeFila(Fila *f, Cliente *removido) {
    Elemento *elem;

    if (filaVazia(f)) {
        return 0; /* underflow */
    }

    elem = f->head;
    *removido = elem->cliente;

    f->head = elem->next;
    if (f->head != NULL) {
        f->head->prev = NULL;
    } else {
        /* fila ficou vazia */
        f->tail = NULL;
    }

    free(elem);
    f->size--;

    return 1;
}

/* ------------------------------------------------------------
   Libera toda a memoria de uma fila
   ------------------------------------------------------------ */
void liberaFila(Fila *f) {
    Elemento *atual = f->head;
    Elemento *proximo;

    while (atual != NULL) {
        proximo = atual->next;
        free(atual);
        atual = proximo;
    }

    free(f);
}

/* ============================================================
   Banco - criacao e liberacao
   ============================================================ */
Banco *criaBanco(void) {
    Banco *b = (Banco *) malloc(sizeof(Banco));
    if (b == NULL) {
        printf("Erro ao alocar memoria para o banco.\n");
        exit(1);
    }
    b->normal = criaFila();
    b->prioritaria = criaFila();
    b->proximaSenha = 1;
    return b;
}

void liberaBanco(Banco *b) {
    liberaFila(b->normal);
    liberaFila(b->prioritaria);
    free(b);
}

/* ============================================================
   Chegada de cliente
   A cada pessoa que chega ao banco, gera-se uma senha
   automatica (iniciando em 1) e ela e inserida no fim da fila
   correspondente ao seu tipo. Retorna a senha gerada.
   ============================================================ */
int chegadaCliente(Banco *b, const char *nome, int tipo) {
    Cliente c;

    c.senha = b->proximaSenha;
    b->proximaSenha++;

    strncpy(c.nome, nome, TAM_NOME - 1);
    c.nome[TAM_NOME - 1] = '\0';
    c.tipo = tipo;

    if (tipo == TIPO_PRIORIDADE) {
        insereFila(b->prioritaria, c);
    } else {
        insereFila(b->normal, c);
    }

    return c.senha;
}

/* ============================================================
   Atende cliente
   Atende o cliente que esta na primeira posicao e o remove da
   fila. A fila prioritaria (secundaria) e sempre atendida
   primeiro; a normal so e atendida quando nao ha prioritarios.
   Retorna 1 em caso de sucesso, 0 se nao ha ninguem (underflow).
   ============================================================ */
int atendeCliente(Banco *b, Cliente *atendido) {
    if (!filaVazia(b->prioritaria)) {
        return removeFila(b->prioritaria, atendido);
    }
    if (!filaVazia(b->normal)) {
        return removeFila(b->normal, atendido);
    }
    return 0; /* underflow: nenhuma fila tem clientes */
}

/* ============================================================
   Verifica se o banco esta sem ninguem esperando
   (as duas filas vazias).
   ============================================================ */
int bancoVazio(Banco *b) {
    return (filaVazia(b->normal) && filaVazia(b->prioritaria)) ? 1 : 0;
}

/* ------------------------------------------------------------
   Imprime uma fila com a posicao de cada cliente (1 = primeiro)
   ------------------------------------------------------------ */
static void imprimeUmaFila(Fila *f, const char *titulo) {
    Elemento *atual = f->head;
    int posicao = 1;

    printf("%s (%d cliente(s)):\n", titulo, f->size);

    if (atual == NULL) {
        printf("   (vazia)\n");
        return;
    }

    while (atual != NULL) {
        printf("   %d. Senha %03d - %s\n",
               posicao, atual->cliente.senha, atual->cliente.nome);
        atual = atual->next;
        posicao++;
    }
}

/* ============================================================
   Consulta a fila a qualquer momento
   Mostra as duas filas, a posicao de cada cliente e quem sera
   o proximo a ser atendido. Nao altera nada.
   ============================================================ */
void consultaFilas(Banco *b) {
    printf("\n========== SITUACAO DAS FILAS ==========\n");

    imprimeUmaFila(b->prioritaria, "Fila PRIORITARIA (atendida primeiro)");
    printf("\n");
    imprimeUmaFila(b->normal, "Fila NORMAL");

    printf("\n");
    if (!filaVazia(b->prioritaria)) {
        printf("Proximo a ser atendido: senha %03d - %s (prioridade)\n",
               b->prioritaria->head->cliente.senha,
               b->prioritaria->head->cliente.nome);
    } else if (!filaVazia(b->normal)) {
        printf("Proximo a ser atendido: senha %03d - %s\n",
               b->normal->head->cliente.senha,
               b->normal->head->cliente.nome);
    } else {
        printf("Ninguem aguardando atendimento.\n");
    }
    printf("========================================\n");
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
   Le um nome do teclado (aceita espacos), remove o '\n' final
   e descarta o excesso caso a linha seja maior que o buffer
   ------------------------------------------------------------ */
void leNome(char *destino) {
    if (fgets(destino, TAM_NOME, stdin) != NULL) {
        size_t len = strlen(destino);
        if (len > 0 && destino[len - 1] == '\n') {
            destino[len - 1] = '\0';
        } else {
            /* linha nao coube no buffer: descarta o resto */
            limpaBuffer();
        }
    } else {
        destino[0] = '\0';
    }
}

/* ------------------------------------------------------------
   Exibe o menu de opcoes
   ------------------------------------------------------------ */
void exibeMenu(void) {
    printf("\n===== MENU - Fila de Atendimento Bancario =====\n");
    printf("1 - Chegada de cliente (normal)\n");
    printf("2 - Chegada de cliente com PRIORIDADE\n");
    printf("3 - Atender proximo cliente\n");
    printf("4 - Verificar se as filas estao vazias\n");
    printf("5 - Consultar as filas\n");
    printf("6 - Executar tudo automaticamente (fazTudo)\n");
    printf("7 - Preencher filas automaticamente\n");
    printf("0 - Sair\n");
    printf("Escolha uma opcao: ");
}

/* ============================================================
   preencheFila
   Faz chegarem alguns clientes (normais e prioritarios) no
   banco recebido. As demais operacoes (atender, consultar etc.)
   continuam sendo feitas manualmente pelo menu.
   ============================================================ */
void preencheFila(Banco *b) {
    chegadaCliente(b, "Ana", TIPO_NORMAL);
    chegadaCliente(b, "Bruno", TIPO_NORMAL);
    chegadaCliente(b, "Carla", TIPO_PRIORIDADE);
    chegadaCliente(b, "Diego", TIPO_NORMAL);
    chegadaCliente(b, "Elisa", TIPO_PRIORIDADE);
}

/* ------------------------------------------------------------
   Auxiliar: atende e imprime o resultado
   ------------------------------------------------------------ */
static void atendeEImprime(Banco *b) {
    Cliente c;

    if (atendeCliente(b, &c)) {
        printf("Atendendo senha %03d - %s%s\n", c.senha, c.nome,
               (c.tipo == TIPO_PRIORIDADE) ? " (prioridade)" : "");
    } else {
        printf("Nao ha clientes para atender (fila vazia).\n");
    }
}

/* ============================================================
   fazTudo
   Executa automaticamente, sem interacao do usuario, uma
   sequencia de operacoes que demonstra o sistema: chegadas
   (normais e prioritarias), consulta, atendimentos (mostrando
   que a prioridade fura a fila), verificacao de fila vazia e
   tentativa de atender com a fila vazia (underflow).
   ============================================================ */
void fazTudo(void) {
    Banco *b = criaBanco();

    printf("=== Banco vazio? %s ===\n", bancoVazio(b) ? "SIM" : "NAO");

    printf("\n=== Chegada de clientes ===\n");
    printf("Ana chegou (normal): senha %03d\n",
           chegadaCliente(b, "Ana", TIPO_NORMAL));
    printf("Bruno chegou (normal): senha %03d\n",
           chegadaCliente(b, "Bruno", TIPO_NORMAL));
    printf("Carla chegou (PRIORIDADE): senha %03d\n",
           chegadaCliente(b, "Carla", TIPO_PRIORIDADE));
    printf("Diego chegou (normal): senha %03d\n",
           chegadaCliente(b, "Diego", TIPO_NORMAL));

    consultaFilas(b);

    printf("\n=== Atendimentos (prioritaria primeiro) ===\n");
    atendeEImprime(b);
    atendeEImprime(b);

    consultaFilas(b);

    printf("\n=== Banco vazio? %s ===\n", bancoVazio(b) ? "SIM" : "NAO");

    printf("\n=== Atendendo todo mundo que restou ===\n");
    atendeEImprime(b);
    atendeEImprime(b);

    printf("\n=== Banco vazio? %s ===\n", bancoVazio(b) ? "SIM" : "NAO");

    printf("\n=== Tentando atender com tudo vazio (underflow) ===\n");
    atendeEImprime(b);

    liberaBanco(b);
}

/* ============================================================
   Programa principal - menu interativo
   ============================================================ */
int main(void) {
    Banco *b = criaBanco();
    int opcao;
    char nome[TAM_NOME];
    int senha;
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
                printf("Nome do cliente: ");
                leNome(nome);
                senha = chegadaCliente(b, nome, TIPO_NORMAL);
                printf("%s entrou na fila normal com a senha %03d.\n", nome, senha);
                break;

            case 2:
                printf("Nome do cliente (prioridade): ");
                leNome(nome);
                senha = chegadaCliente(b, nome, TIPO_PRIORIDADE);
                printf("%s entrou na fila PRIORITARIA com a senha %03d.\n", nome, senha);
                break;

            case 3:
                atendeEImprime(b);
                break;

            case 4:
                printf("Fila prioritaria vazia? %s\n",
                       filaVazia(b->prioritaria) ? "SIM" : "NAO");
                printf("Fila normal vazia?      %s\n",
                       filaVazia(b->normal) ? "SIM" : "NAO");
                break;

            case 5:
                consultaFilas(b);
                break;

            case 6:
                fazTudo();
                break;

            case 7:
                preencheFila(b);
                consultaFilas(b);
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

    liberaBanco(b);

    return 0;
}