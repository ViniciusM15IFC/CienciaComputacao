#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <ctype.h>
#define MAX 20

// Descarta qualquer caractere restante na linha atual
void limparLinha()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        
    }
}

void continuar()
{
	limparLinha();
	printf("Pressione ENTER para continuar...");
	getchar();
}

	//registros

struct endereco
{
    char estado[3];
    char cidade[30];
    char bairro[30];
    char rua[50];
    int numero;
    char complemento[30];
};

struct paciente
{
    int codigo;
    char nome[50];
    char cpf[12];
    char dataNasc[11];
    char sexo;
    char telefone[15];
    struct endereco ender;
    char tipoSang[4];
    char convenio[20];
    bool ativo;
};

	//variáveis globais

struct paciente pacs[MAX];
int cod = 1;
int i;

	//protótipos

void inicio();
int opcaoMenu(int menu);
void cadastrar();
void listar();
void buscar();
void buscarPorNome();
void consultarPorInd(int index);
void alterar();
void excluir();
struct endereco pedirEndereco();
int calcularIdade(char dataNasc[]);
void montarData(int d, int m, int a, char resultado[]);
void formatarTelefone(char telefone[], char resultado[]);
void formatarCPF(char cpf[], char resultado[]);
bool validarCpf(char cpf[]);
bool validarData(int d, int m, int a);
bool validarTelefone(char telefone[]);
bool validarTipoSang(char tipoSang[]);
bool validarSexo(char sexo);
int obterIndice();
void preencher();

int main()
{
	setlocale(LC_ALL, "Portuguese");
	
    inicio();

    return 0;
}

	//funções de menu

int opcaoMenu(int menu)
{
    int op;

    if (menu == 0)
    {
        printf("\n+= Menu Inicial =+\n");
        printf("1 - Cadastrar\n");
        printf("2 - Listar\n");
        printf("3 - Consultar\n");
        printf("4 - Alterar\n");
        printf("5 - Excluir\n");
        printf("6 - Sair\n");
        printf("Digite: ");
    }
    else if (menu == 1)
    {
        printf("\n= Escolha um campo para alterar =\n");
        printf("1 - Nome\n");
        printf("2 - CPF\n");
        printf("3 - Data de Nascimento\n");
        printf("4 - Sexo\n");
        printf("5 - Telefone\n");
        printf("6 - Endereço\n");
        printf("7 - Tipo Sanguíneo\n");
        printf("8 - Convênio\n");
        printf("9 - Voltar\n");
        printf("Digite: ");
    }
    else if (menu == 2)
    {
    	printf("\n= Escolha um modo de busca =\n");
        printf("1 - Consulta por código\n");
        printf("2 - Busca por nome\n");
        printf("3 - Cancelar\n");
        printf("Digite: ");
	}

    if (scanf("%i", &op) != 1)
    {
        // Entrada inválida ou EOF: encerra o programa em vez de travar em loop
        printf("\n[ERRO] Entrada inválida ou encerrada. Saindo...\n");
        exit(1);
    }

    return op;
}

void inicio()
{
    int op;

    do
    {
    	system("cls");
        op = opcaoMenu(0);

        switch (op)
        {
        case 1:
            cadastrar();
            break;
        case 2:
            listar();
            break;
        case 3:
            buscar();
            break;
        case 4:
            alterar();
            break;
        case 5:
            excluir();
            break;
        case 6:
            printf("Encerrando o programa...\n");
            break;
        case 101:
        	printf("Função de Debug: Preenchendo...");
        	preencher();
			break;
		case 102:
			printf("Função de Debug: Excluindo tudo...");
        	cod = 1;
			break;
        default:
            printf("Opção Inválida!\n");
            break;
        }
    } while (op != 6);
}

	// funções básicas

void cadastrar()
{
    if (cod > MAX)
    {
        printf("[ERRO] Limite máximo de %d pacientes atingido!\n", MAX);
        continuar();
        return;
    }

    struct paciente pa;
    pa.codigo = cod;
    pa.ativo = true;
    char conf;

    printf("Digite o nome do paciente: ");
    scanf(" %49[^\n]s", pa.nome);
    strupr(pa.nome);
    limparLinha();

    do
    {
        printf("Digite o CPF do paciente(somente números): ");
        scanf(" %11[^\n]s", pa.cpf);
        limparLinha();
    } while (!validarCpf(pa.cpf));

    int dia, mes, ano;
    do
    {
        printf("Digite a data de nascimento (dd/mm/aaaa): ");
        scanf("%d/%d/%d", &dia, &mes, &ano);
    } while (!validarData(dia, mes, ano));

    montarData(dia, mes, ano, pa.dataNasc);
	do
	{
		printf("Digite o sexo do paciente (M/F): ");
    	scanf(" %c", &pa.sexo);
    	pa.sexo = toupper(pa.sexo); 
	}while(!validarSexo(pa.sexo));
    
    
    do
    {
        printf("Digite o telefone do paciente(somente números): ");
        scanf(" %14[^\n]s", pa.telefone);
        limparLinha();
    } while (!validarTelefone(pa.telefone));

    pa.ender = pedirEndereco();

    do
    {
        printf("Digite o tipo sanguíneo do paciente: ");
        scanf(" %3[^\n]s", pa.tipoSang);
        limparLinha();
        
        strupr(pa.tipoSang);
    } while (!validarTipoSang(pa.tipoSang));

    printf("Digite o convênio do paciente: ");
    scanf(" %19[^\n]s", pa.convenio);
    limparLinha();

    char telefoneFormatado[15];
    formatarTelefone(pa.telefone, telefoneFormatado);
    
    char cpfFormatado[15];
    formatarCPF(pa.cpf, cpfFormatado);

    printf("\n--- Dados do Paciente ---\n");
    printf("Nome: %s\n", pa.nome);
    printf("CPF: %s\n", cpfFormatado);
    printf("Data de Nascimento: %s\n", pa.dataNasc);
    printf("Sexo: %c\n", pa.sexo);
    printf("Telefone: %s\n", telefoneFormatado);
    printf("Endereço: %s, %s, %s, %s, %d\n", pa.ender.rua, pa.ender.bairro, pa.ender.cidade, pa.ender.estado, pa.ender.numero);
    printf("Tipo Sanguíneo: %s\n", pa.tipoSang);
    printf("Convênio: %s\n", pa.convenio);

    printf("\nProsseguir com o cadastro? (s/n): ");
    scanf(" %c", &conf);

    if (conf == 's' || conf == 'S')
    {
        pacs[cod - 1] = pa;
        printf("Paciente cadastrado com sucesso!\n");
        cod++;
        continuar();
    }
    else
    {
        printf("Cadastro cancelado.\n");
        continuar();
    }
}

void listar()
{
	int cont = 0;
    for (i = 0; i < cod - 1; i++)
    {
        if (pacs[i].ativo)
        {
        	char cpfFormatado[15];
			formatarCPF(pacs[i].cpf, cpfFormatado);
            
        	printf("%i - %s - %s - %i Anos\n", pacs[i].codigo, pacs[i].nome, cpfFormatado, calcularIdade(pacs[i].dataNasc));
        	cont++;
		}	
    }

    if (cod == 1)
    {
        printf("Nenhum paciente cadastrado.\n");
    }
    
    if(cont == 0)
    {
    	printf("Nenhum paciente ativo.\n");
	}
	
	continuar();
}



void buscar()
{
	int op = opcaoMenu(2);
	int index;
	
	switch(op)
	{
		case 1:
			index = obterIndice();
			consultarPorInd(index);
			break;
		case 2:
			buscarPorNome();
		    break;
		case 3:
			printf("Voltando pro menu inicial...");
			continuar();
			break;
		default:
			printf("Opção inválida");
			break;
	}	
	
}

void alterar() 
{ 
    int index;
    int op;

    index = obterIndice();
    if (index == -1)
    {
        return;
    }
    
    do
    {
    	op = opcaoMenu(1);

	    switch (op)
	    {
	    case 1:
	        printf("Digite o novo nome: ");
	        scanf(" %49[^\n]s", pacs[index].nome);
	        strupr(pacs[index].nome);
	        limparLinha();
	        printf("Nome atualizado com sucesso!\n");
	        break;
	    case 2:
	        do
	        {
	            printf("Digite o novo CPF(somente números): ");
	            scanf(" %11[^\n]s", pacs[index].cpf);
	            limparLinha();
	        } while (!validarCpf(pacs[index].cpf));
	        printf("CPF atualizado com sucesso!\n");
	        break;
	    case 3:
	        int dia, mes, ano;
	        do
	        {
	            printf("Digite a nova data de nascimento (dd/mm/aaaa): ");
	            scanf("%d/%d/%d", &dia, &mes, &ano);
	        } while (!validarData(dia, mes, ano));
	        montarData(dia, mes, ano, pacs[index].dataNasc);
	        printf("Data de nascimento atualizada com sucesso!\n");
	        break;
	    case 4:
	    	do
	    	{
	    		printf("Digite o novo sexo (M/F): ");
	        	scanf(" %c", &pacs[index].sexo);
	        	pacs[index].sexo = toupper(pacs[index].sexo);	
			}while(!validarSexo(pacs[index].sexo));
	        
	        printf("Sexo atualizado com sucesso!\n");
	        break;
	    case 5:
	        do
	        {
	            printf("Digite o novo telefone (apenas números): ");
	            scanf(" %14[^\n]s", pacs[index].telefone);
	            limparLinha();
	        } while (!validarTelefone(pacs[index].telefone));
	        printf("Telefone atualizado com sucesso!\n");
	        break;
	    case 6:
	        pacs[index].ender = pedirEndereco();
	        printf("Endereço atualizado com sucesso!\n");
	        break;
	    case 7:
	        do
	        {
	            printf("Digite o novo tipo sanguíneo: ");
	            scanf(" %3[^\n]s", pacs[index].tipoSang);
	            limparLinha();
	            strupr(pacs[index].tipoSang);
	        } while (!validarTipoSang(pacs[index].tipoSang));
	        printf("Tipo sanguíneo atualizado com sucesso!\n");
	        break;
	    case 8:
	        printf("Digite o novo convênio: ");
	        scanf(" %19[^\n]s", pacs[index].convenio);
	        limparLinha();
	        printf("Convênio atualizado com sucesso!\n");
	        break;
	    case 9:
	        printf("Voltando pro menu inicial...\n");
	        break;
	    default:
	        printf("Opção Inválida!\n");
	        break;
	    }
	    	
	}while(op != 9);
}

void excluir()
{
    int index;
    char conf;

    index = obterIndice();
    if (index == -1)
    {
        return;
    }

    printf("Tem certeza que deseja excluir o paciente %s? (s/n): ", pacs[index].nome);
    scanf(" %c", &conf);

    if (conf == 's' || conf == 'S')
    {
        pacs[index].ativo = false;
        printf("Paciente excluído com sucesso!\n");
        continuar();
    }
    else
    {
        printf("Exclusão cancelada.\n");
        continuar();
    }
}

// funções auxiliares

void consultarPorInd(int index)
{
    if (index == -1)
    {
        return;
    }

    printf("\n--- Paciente %d ---\n", pacs[index].codigo);
    printf("Nome: %s\n", pacs[index].nome);
    char cpfFormatado[15];
    formatarCPF(pacs[index].cpf, cpfFormatado);
    printf("CPF: %s\n", cpfFormatado);
    printf("Data de Nascimento: %s\n", pacs[index].dataNasc);
    printf("Sexo: %c\n", pacs[index].sexo);
    char telefoneFormatado[15];
    formatarTelefone(pacs[index].telefone, telefoneFormatado);
    printf("Telefone: %s\n", telefoneFormatado);
    printf("Endereço: %s, %s, %s, %s, %d\n", pacs[index].ender.rua, pacs[index].ender.bairro, pacs[index].ender.cidade, pacs[index].ender.estado, pacs[index].ender.numero);
    printf("Tipo Sanguíneo: %s\n", pacs[index].tipoSang);
    printf("Convênio: %s\n", pacs[index].convenio);
    
    continuar();
}

struct endereco pedirEndereco()
{
    struct endereco e;
    printf("Digite o estado (UF): ");
    scanf(" %2[^\n]s", e.estado);
    strupr(e.estado);
    limparLinha();

    printf("Digite a cidade: ");
    scanf(" %29[^\n]s", e.cidade);
    limparLinha();

    printf("Digite o bairro: ");
    scanf(" %29[^\n]s", e.bairro);
    limparLinha();

    printf("Digite a rua: ");
    scanf(" %49[^\n]s", e.rua);
    limparLinha();

    printf("Digite o número: ");
    scanf("%d", &e.numero);

    printf("Digite o complemento: ");
    scanf(" %29[^\n]s", e.complemento);
    limparLinha();

    return e;
}


void buscarPorNome()
{
	char busca[50];
	int codigo;
	int cont = 0;
	
	printf("Digite um nome para buscar: ");
	scanf(" %49[^\n]s", &busca);
	
	strupr(busca);
	
	for (i=0; i<cod-1; i++)
	{
		if (pacs[i].ativo)
    	{
			if (strstr(pacs[i].nome, busca) != NULL) 
			{
    			printf("%i - %s - %s - %i Anos\n", pacs[i].codigo, pacs[i].nome, pacs[i].cpf, calcularIdade(pacs[i].dataNasc));
        		cont ++;
			}
		}
	}
	
	if(cont == 0)
	{
		printf("Nenhum paciente encontrado\n");
		continuar();
		return;
	}
	
	printf("Digite o código para consultar(0 para voltar): ");
	scanf("%i", &codigo);
	
	if(codigo == 0)
	{
		printf("Voltando pro menu inicial...");
		return;
	}
	
	consultarPorInd(codigo - 1);
}

int obterIndice()
{
    int codigo;
    int index;

    if (cod == 1)
    {
        printf("Nenhum paciente cadastrado.\n");
        return -1;
    }
    else
    {
        printf("Digite o código do paciente: ");
        scanf("%d", &codigo);
        index = codigo - 1;

        if (index < 0 || index >= cod - 1)
        {
            printf("Código inválido!\n");
            continuar();
            return -1;
        }
    }

    if (pacs[index].ativo == false)
    {
        printf("Paciente não encontrado ou inativo.\n");
        return -1;
    }
    else
    {
        return index;
    }
}

int calcularIdade(char dataNasc[])
{
    int diaNasc, mesNasc, anoNasc;
    
    sscanf(dataNasc, "%d/%d/%d", &diaNasc, &mesNasc, &anoNasc);

    //data atual do sistema
    time_t t = time(NULL);
    struct tm tmAtual = *localtime(&t);

    int anoAtual = tmAtual.tm_year + 1900; // tm_year conta os anos desde 1900
    int mesAtual = tmAtual.tm_mon + 1;     // tm_mon vai de 0 a 11
    int diaAtual = tmAtual.tm_mday;

    int idade = anoAtual - anoNasc;

    // Se a pessoa ainda não fez aniversário no ano atual, subtrai 1 ano
    if (mesAtual < mesNasc || (mesAtual == mesNasc && diaAtual < diaNasc))
    {
        idade--;
    }

    return idade;
}


    // funções de formatação

void montarData(int d, int m, int a, char resultado[])
{
    sprintf(resultado, "%02d/%02d/%04d", d, m, a);
}

void formatarTelefone(char telefone[], char resultado[])
{
    sprintf(resultado, "(%.2s) %.5s-%.4s", telefone, telefone + 2, telefone + 7);
}
void formatarCPF(char cpf[], char resultado[])
{
	sprintf(resultado, "%.3s.%.3s.%.3s-%.2s", cpf, cpf + 3, cpf + 6, cpf + 9);
}

	// funções de validação

bool validarCpf(char cpf[])
{
	
    if (strlen(cpf) != 11)
    {
        printf("[ERRO] CPF inválido!\n");
        return false;
    }
    
    
    return true;
}

bool validarData(int d, int m, int a)
{
    if (d < 1 || d > 31 || m < 1 || m > 12 || a < 1900 || a > 2026)
    {
        printf("[ERRO] Data inválida!\n");
        return false;
    }
    return true;
}

bool validarTelefone(char telefone[])
{
    if (strlen(telefone) != 11)
    {
        printf("[ERRO] Telefone inválido!\n");
        return false;
    }
    return true;
}

bool validarTipoSang(char tipoSang[])
{
    const char tiposValidos[8][4] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
    for (int i = 0; i < 8; i++)
    {
        if (strcmp(tipoSang, tiposValidos[i]) == 0)
        {
            return true;
        }
    }
    printf("[ERRO] Tipo sanguíneo inválido!\n");
    return false;
} 

bool validarSexo(char sexo)
{
	if(sexo == 'M' || sexo == 'F')
	{
		return true;	
	}
	else
	{
		printf("[ERRO] Sexo inválido!\n");
        return false;
	}
}

// funções de debug

void preencher()
{
    const char nomes[20][20] = {
        "JOAO SILVA", "MARIA OLIVEIRA", "PEDRO SOUZA", "ANA COSTA",
        "CARLOS PEREIRA", "JULIANA RODRIGUES", "LUCAS ALMEIDA", "FERNANDA LIMA",
        "RAFAEL GOMES", "BEATRIZ MARTINS", "BRUNO CARVALHO", "LARISSA FERREIRA",
        "GUSTAVO RIBEIRO", "CAMILA BARBOSA", "DIEGO ROCHA", "PATRICIA DIAS",
        "THIAGO NUNES", "AMANDA CASTRO", "FELIPE TEIXEIRA", "LETICIA MOREIRA"
    };
    const char cidades[5][20] = { "VIDEIRA", "JOACABA", "CACADOR", "TANGARA", "FLORIANOPOLIS" };
    const char bairros[4][20] = { "CENTRO", "SAO CRISTOVAO", "VILA NOVA", "JARDIM AMERICA" };
    const char ruas[4][25] = { "RUA DAS FLORES", "AVENIDA BRASIL", "RUA SETE DE SETEMBRO", "RUA XV DE NOVEMBRO" };
    const char tipos[8][4] = { "A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-" };
    const char convenios[5][20] = { "UNIMED", "SUS", "BRADESCO SAUDE", "AMIL", "PARTICULAR" };

    int qtd = 20;
    int k;

    if (cod + qtd - 1 > MAX)
    {
        qtd = MAX - cod + 1;
    }

    if (qtd <= 0)
    {
        printf("[ERRO] Limite maximo de %d pacientes ja atingido!\n", MAX);
        continuar();
        return;
    }

    for (k = 0; k < qtd; k++)
    {
        struct paciente pa;
        int idx = k % 20;

        pa.codigo = cod;
        pa.ativo = true;

        strcpy(pa.nome, nomes[idx]);

        {
            unsigned int parte1 = (unsigned int) cod % 1000;
            unsigned int parte2 = ((unsigned int) cod * 137u) % 90000000u + 10000000u;
            sprintf(pa.cpf, "%03u%08u", parte1, parte2);
        }

        montarData(1 + (cod % 28), 1 + (cod % 12), 1950 + (cod % 70), pa.dataNasc);

        pa.sexo = (cod % 2 == 0) ? 'F' : 'M';

        sprintf(pa.telefone, "47%09d", 900000000 + cod * 11);

        strcpy(pa.ender.estado, "SC");
        strcpy(pa.ender.cidade, cidades[cod % 5]);
        strcpy(pa.ender.bairro, bairros[cod % 4]);
        strcpy(pa.ender.rua, ruas[cod % 4]);
        pa.ender.numero = 100 + cod;
        strcpy(pa.ender.complemento, "S/N");

        strcpy(pa.tipoSang, tipos[cod % 8]);
        strcpy(pa.convenio, convenios[cod % 5]);

        pacs[cod - 1] = pa;
        cod++;
    }

    printf("%d pacientes de teste gerados com sucesso!\n", qtd);
    continuar();
}
