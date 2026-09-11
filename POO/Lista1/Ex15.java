// 15. Faça um programa que preencha uma matriz 7x7 de números inteiros e crie dois vetores
// com sete posições cada um que contenham, respectivamente, o maior elemento de
// cada uma das linhas e o menor elemento de cada uma das colunas. Escreva a matriz
// e os dois vetores gerados.

package Lista1;

import java.util.Scanner;

public class Ex15 {
    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);

        int n  = 7;

        int m[][] = new int[n][n];
        int v1[] = new int[n];
        int v2[] = new int[n];

        for (int i=0; i<n; i++)
        {
            for (int j = 0; j < n; j++) {
                System.out.println("Digite um valor: ");
                m[i][j] = s.nextInt();
            }
        }


        System.out.println("Matriz: ");
        for (int i=0; i<n; i++)
        {
            for (int j = 0; j < n; j++) {
                System.out.print(m[i][j] + "    ");
            }
            System.out.println();
        }

        for (int i=0; i<n; i++)
        {
            int maior = m[i][0];
            for (int j = 0; j < n; j++) {
                if (m[i][j] > maior)
                {
                    maior = m[i][j];
                }
            }
            v1[i] = maior;
        }

        for (int j=0; j<n; j++)
        {
            int menor = m[0][j];
            for (int i = 0; i < n; i++) {
                if (m[i][j] < menor)
                {
                    menor = m[i][j];
                }
            }
            v2[j] = menor;
        }

        System.out.println("Vetor 1: ");
        for(int valor : v1)
        {
            System.out.print(valor + "  ");
        }
        System.out.println();

        System.out.println("Vetor 2: ");
        for(int valor : v2)
        {
            System.out.print(valor + "  ");
        }
        System.out.println();
    }
    


}