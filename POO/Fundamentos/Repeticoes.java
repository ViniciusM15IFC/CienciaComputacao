package Fundamentos;
import java.util.Scanner;

public class Repeticoes {
    public static void main(String[] args)
    {
        String[] alunos = new String[5];
        Scanner s = new Scanner(System.in);

        for (int i = 0; i < alunos.length; i++)
        {
            System.out.println("Digite um nome: ");
            alunos[i] = s.next();
        }

        System.out.println("Nomes:");

        for (String nome : alunos)
        {
            System.out.println(nome);
        }

        s.close();
    }
}
