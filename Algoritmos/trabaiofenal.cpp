#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#define MAX 5

struct endereco
{
	char estado[3];
	char cidade[20];
	char bairro[10];
	char rua[30];
	int numero;
	char complemento[20];
};

struct paciente 
{
	int codigo;
	char nome[20];
	int cpf;
	struct tm dataNasc;
	char sexo;
	int telefone;
	struct endereco ender;
	char tipoSang[4];
	char convenio[10];
};

void inicio();
int opcaoMenu(int menu);
void cadastrar();
void listar();
void consultar();
void alterar();
void excluir();
void pedirEndereco();
bool validarCpf();
bool validarData();
bool validarTelefone();
bool validarTipoSang();



main()
{
	setlocale(LC_ALL, "Portuguese");
	
	inicio();
}

int opcaoMenu(int menu)
{
	int op;
	
	if(menu == 0)
	{
		printf("+= Menu Inicial =+\n");
		printf("1 - Cadastrar\n");
		printf("2 - Listar\n");
		printf("3 - Consultar\n");
		printf("4 - Alterar\n");
		printf("5 - Excluir\n");
		printf("6 - Sair\n");
	}
	else if(menu == 1)
	{
		printf("= Escolha um campo para alterar =\n");
		printf("1 - Nome\n");
		printf("2 - CPF\n");
		printf("3 - Data de Nascimento\n");
		printf("4 - Sexo\n");
		printf("5 - Telefone\n");
		printf("6 - Endereço\n");
		printf("7 - Tipo Sanguíneo\n");
		printf("8 - Convênio\n");
		printf("9 - Cancelar\n");
	}
	
	scanf("%i", &op);
	return op;	
}

void inicio() 
{
	switch (opcaoMenu(0)) {
        case 1:
            cadastrar();
            inicio();
            break;

        case 2:
            listar();
            inicio();
            break;

        case 3:
            consultar();
            inicio();
            break;

        case 4:
            alterar();
            inicio();
            break;

        case 5:
            excluir();
            inicio();
            break;

        case 6:
        	printf("Encerrando...");
            break;

        default:
            printf("Opção Inválida\n");
            inicio();
            break;
    }
}

void cadastrar()
{
	printf("Função em desenvolvimento\n");
}

void listar()
{
	printf("Função em desenvolvimento\n");
}

void consultar()
{
	printf("Função em desenvolvimento\n");
}

void alterar()
{
	printf("Função em desenvolvimento\n");
}

void excluir()
{
	printf("Função em desenvolvimento\n");
}
