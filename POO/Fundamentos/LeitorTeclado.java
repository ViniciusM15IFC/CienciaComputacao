package Fundamentos;
import java.util.Scanner;

public class LeitorTeclado {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.println("Digite seu nome: ");
        String nome = sc.nextLine();

        System.out.println("Digite sua idade: ");
        int idade = sc.nextInt();

        System.out.println("Bem " + nome + " anos! Você " + idade + " tem vindo");
        sc.close();
    }
}
