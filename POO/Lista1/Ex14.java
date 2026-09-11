import java.util.InputMismatchException;
import java.util.Scanner;

// 14. Faça um programa que simule um controle bancário. Para tanto, devem ser lidos os
// códigos de dez contas e seus respectivos saldos. Os códigos devem ser armazenados
// em um vetor de números inteiros (não pode haver mais de uma conta com o mesmo
// código) e os saldos devem ser armazenados em um vetor de números reais. O saldo
// deverá ser cadastrado na mesma posição do código. Por exemplo, se a conta 504 foi
// armazenada na quinta posição do vetor de códigos, seu saldo deverá ficar na quinta
// posição do vetor de saldos. Depois de fazer a leitura dos valores, deverá aparecer o
// seguinte menu na tela:
// • Efetuar depósito
// • Efetuar saque
// • Consultar o ativo bancário (soma de todos os saldos)
// • Finalizar programa
// Obs.1: ao depositar, deve-se solicitar o valor e o código da conta a receber o depósito.
// Obs.2: para sacar, a mesma coisa. Também, deve-se verificar se tem saldo suficiente
// para o saque.



public class Ex14
{
    static int nContas = 10;

    static int cods[] = new int[nContas];
    static double saldos[] = new double[nContas];

    static Scanner s = new Scanner(System.in);

public static int encontrarValor(int valor, int[] vetor) {
    for (int i = 0; i < vetor.length; i++) {
        if (valor == vetor[i]) {
            return i; 
        }
    }
    return -1; 
}

    public static void cadastraConta(int i)
    {
            boolean pass = false;
            do{
                try
                {
                    System.out.println("Digite o código da conta");
                    int codigo = s.nextInt();

                    if(encontrarValor(codigo, cods) == -1)
                    {
                        pass = true;
                        cods[i] = codigo;
                    }
                    else{
                        System.out.println("Código já em uso");
                    }
                }
                catch (InputMismatchException e)
                {
                    System.out.println("ERRO");
                    s.next();
                }


            }while(!pass);


            do {
                try
                {
                    System.out.println("Digite o saldo da conta");
                    saldos[i] = s.nextDouble();

                    pass = true;
                }
                catch (InputMismatchException e)
                {
                    System.out.println("ERRO");
                    s.next();
                    pass = false;
                }
            }while(!pass);
    }

    public static int opcaoMenu()
    {
        do { 
            System.out.println("=== Menu ===");
            System.out.println("1 - Depositar");
            System.out.println("2 - Sacar");
            System.out.println("3 - Consultar saldo");
            System.out.println("4 - Encerrar");

            try{
                int op = s.nextInt();
                return op;
            }
            catch(InputMismatchException e)
            {
                System.out.println("ERRO - Valor digitado inválido");
                s.next();
            }
        } while (true);
    }

    public static int recebeCodigo()
    {
        do { 
            try{
                
                int codigo = s.nextInt();

                int index = encontrarValor(codigo, cods);

                if(index != -1)
                {
                    return index;
                }
                {
                    System.out.println("Conta não encontrada");
                }
            } catch (InputMismatchException e)
            {
                System.out.println("ERRO - Valor digitado inválido");
                s.next();
            }
        } while (true);
    }

    public static double recebeQuantia()
    {
        do { 
            try{
                double quantia = s.nextDouble();

                if(quantia > 0)
                {
                    return quantia;
                }
                {
                    System.out.println("Digite um valor mais que zero");
                }
            } catch (InputMismatchException e)
            {
                System.out.println("ERRO - Valor digitado inválido");
                s.next();
            }
        } while (true);
    } 

    public static void deposita()
    {
        System.out.println("Digite o código da conta desejada: ");
        int index = recebeCodigo();

        System.out.println("Digite a quantia para ser depositada: ");
        double quantia = recebeQuantia();

        saldos[index] += quantia;

        System.out.println("Depósito concluído");
    }

    public static void saca()
    {
        System.out.println("Digite o código da conta desejada: ");
        int index = recebeCodigo();

        boolean pass = false;

        do {
            System.out.println("Digite a quantia para ser sacada: ");
            double quantia = recebeQuantia();
            if ( saldos[index] >= quantia)
            {
                saldos[index] -= quantia;
                pass = true;
            }
            else{
                System.out.println("Saldo Insuficiente");
            }
        } while (!pass);

        System.out.println("Saque concluído");
    }

    public static void mostrarConta(int index)
    {
        System.out.println("Código da conta: " + cods[index]);
        System.out.printf("Saldo: R$ %.2f", saldos[index]);
        System.out.println();
    }

    public static void consultaSaldo()
    {
        System.out.println("Digite o código da conta desejada: ");
        int index = recebeCodigo();
        
        mostrarConta(index);
    }

    public static void listar()
    {
        for (int i = 0; i < nContas; i++) {
            mostrarConta(i);
        }
    }

    public static void main(String[] args) {
        for (int i = 0; i < nContas; i++) {
            cadastraConta(i);
        }

        boolean encerrar = false;

        do { 
            switch (opcaoMenu()) {
                case 1:
                    deposita();
                    break;
                case 2:
                    saca();
                    break;
                case 3:
                    consultaSaldo();
                    break;
                case 4:
                    System.out.println("Encerrando...");
                    encerrar = true;
                    break;
                case 99:
                    listar();
                    break;
                default:
                    System.out.println("Valor fora das opções");
            }
        } while (!encerrar);
    }
}