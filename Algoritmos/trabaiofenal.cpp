#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <ctype.h>
#define MAX 5

// Descarta qualquer caractere restante na linha atual do stdin,
// evitando que sobras de uma leitura truncada (ex: %2[^\n]) "vazem"
// para a próxima leitura.
void limparLinha()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        
    }
}

struct endereco
{
    char estado[3];
    char city[30];
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
    char cpf[15];
    char dataNasc[11];
    char sexo;
    char telefone[15];
    struct endereco ender;
    char tipoSang[4];
    char convenio[20];
    bool ativo;
};

struct paciente pacs[MAX];
int cod = 1;
int i;

void inicio();
int opcaoMenu(int menu);
void cadastrar();
void listar();
void consultar();
void alterar();
void excluir();
struct endereco pedirEndereco();
void montarData(int d, int m, int a, char resultado[]);
void formatarTelefone(char telefone[], char resultado[]);
bool validarCpf(char cpf[]);
bool validarData(int d, int m, int a);
bool validarTelefone(char telefone[]);
bool validarTipoSang(char tipoSang[]);
int obterIndice();

int main()
{
    inicio();

    return 0;
}

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
        printf("Escolha uma opção: ");
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
        printf("9 - Cancelar\n");
        printf("Escolha uma opção: ");
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
    int opcao;

    do
    {
        opcao = opcaoMenu(0);

        switch (opcao)
        {
        case 1:
            cadastrar();
            break;
        case 2:
            listar();
            break;
        case 3:
            consultar();
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
        default:
            printf("Opção Inválida!\n");
            break;
        }
    } while (opcao != 6);
}

void cadastrar()
{
    // Validação para impedir estouro de memória caso chegue ao limite MAX
    if (cod > MAX)
    {
        printf("[ERRO] Limite máximo de %d pacientes atingido!\n", MAX);
        return;
    }

    struct paciente pa;
    pa.codigo = cod;
    pa.ativo = true;
    char conf;

    printf("Digite o nome do paciente: ");
    scanf(" %49[^\n]s", pa.nome);
    limparLinha();

    do
    {
        printf("Digite o CPF do paciente: ");
        scanf(" %14[^\n]s", pa.cpf);
        limparLinha();
    } while (!validarCpf(pa.cpf));

    int dia, mes, ano;
    do
    {
        printf("Digite a data de nascimento (dd/mm/aaaa): ");
        scanf("%d/%d/%d", &dia, &mes, &ano);
    } while (!validarData(dia, mes, ano));

    montarData(dia, mes, ano, pa.dataNasc);

    printf("Digite o sexo do paciente (M/F): ");
    scanf(" %c", &pa.sexo);

    do
    {
        printf("Digite o telefone do paciente(apenas números): ");
        scanf(" %14[^\n]s", pa.telefone);
        limparLinha();
    } while (!validarTelefone(pa.telefone));

    pa.ender = pedirEndereco();

    do
    {
        printf("Digite o tipo sanguíneo do paciente: ");
        scanf(" %3[^\n]s", pa.tipoSang);
        limparLinha();
        for(int k=0; pa.tipoSang[k]; k++) pa.tipoSang[k]=toupper((unsigned char)pa.tipoSang[k]);
    } while (!validarTipoSang(pa.tipoSang));

    printf("Digite o convênio do paciente: ");
    scanf(" %19[^\n]s", pa.convenio);
    limparLinha();

    char telefoneFormatado[15];
    formatarTelefone(pa.telefone, telefoneFormatado);

    printf("\n--- Dados do Paciente ---\n");
    printf("Nome: %s\n", pa.nome);
    printf("CPF: %s\n", pa.cpf);
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
    }
    else
    {
        printf("Cadastro cancelado.\n");
    }
}

struct endereco pedirEndereco()
{
    struct endereco e;
    printf("Digite o estado (UF): ");
    scanf(" %2[^\n]s", e.estado);
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

void montarData(int d, int m, int a, char resultado[])
{
    sprintf(resultado, "%02d/%02d/%04d", d, m, a);
}

void formatarTelefone(char telefone[], char resultado[])
{
    sprintf(resultado, "(%.2s) %.5s-%.4s", telefone, telefone + 2, telefone + 7);
}

void listar()
{
    for (i = 0; i < cod - 1; i++)
    {
        if (!pacs[i].ativo)
            continue; // Pula pacientes inativos
        printf("%i - %s\n", pacs[i].codigo, pacs[i].nome);
    }

    if (cod == 1)
    {
        printf("Nenhum paciente cadastrado.\n");
    }
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
void consultar()
{
    int index;

    index = obterIndice();
    if (index == -1)
    {
        return;
    }

    printf("\n--- Paciente %d ---\n", pacs[index].codigo);
    printf("Nome: %s\n", pacs[index].nome);
    printf("CPF: %s\n", pacs[index].cpf);
    printf("Data de Nascimento: %s\n", pacs[index].dataNasc);
    printf("Sexo: %c\n", pacs[index].sexo);
    char telefoneFormatado[15];
    formatarTelefone(pacs[index].telefone, telefoneFormatado);
    printf("Telefone: %s\n", telefoneFormatado);
    printf("Endereço: %s, %s, %s, %s, %d\n", pacs[index].ender.rua, pacs[index].ender.bairro, pacs[index].ender.cidade, pacs[index].ender.estado, pacs[index].ender.numero);
    printf("Tipo Sanguíneo: %s\n", pacs[index].tipoSang);
    printf("Convênio: %s\n", pacs[index].convenio);
}
void alterar() 
{ 
    int index;

    index = obterIndice();
    if (index == -1)
    {
        return;
    }

    switch (opcaoMenu(1))
    {
    case 1:
        printf("Digite o novo nome: ");
        scanf(" %49[^\n]s", pacs[index].nome);
        limparLinha();
        printf("Nome atualizado com sucesso!\n");
        break;
    case 2:
        do
        {
            printf("Digite o novo CPF: ");
            scanf(" %14[^\n]s", pacs[index].cpf);
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
        printf("Digite o novo sexo (M/F): ");
        scanf(" %c", &pacs[index].sexo);
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
            for(int k=0; pacs[index].tipoSang[k]; k++) pacs[index].tipoSang[k]=toupper((unsigned char)pacs[index].tipoSang[k]);
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
        printf("Alteração cancelada.\n");
        break;
    default:
        printf("Opção Inválida!\n");
        break;
    }
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
    }
    else
    {
        printf("Exclusão cancelada.\n");
    }
}

bool validarCpf(char cpf[])
{
    if (strlen(cpf) != 14)
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
    const char *tiposValidos[] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
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
