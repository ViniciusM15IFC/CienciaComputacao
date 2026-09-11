// 12. Faça um programa que, dados 2 vetores com 10 números inteiros cada, gere e imprima
// um vetor dos números não comuns aos vetores

package Lista1;

import java.util.Scanner;

public class Ex09 {
    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);
         
        int[] vetor1 = new int[10];
        int[] vetor2 = new int[10];

        for(int i = 0; i < 10; i++) {
            System.out.println("Digite um número para o primeiro vetor: ");
            vetor1[i] = s.nextInt();
        }

        for(int i = 0; i < 10; i++) {
            System.out.println("Digite um número para o segundo vetor: ");
            vetor2[i] = s.nextInt();
        }

        System.out.println("Números não comuns aos vetores: ");

        for(int i = 0; i < 10; i++) {
            boolean isComum = false;
            for(int j = 0; j < 10; j++) {
                if(vetor1[i] == vetor2[j]) {
                    isComum = true;
                    break;
                }
            }
            if(!isComum) {
                System.out.print(vetor1[i] + " ");
            }
        }

        for(int i = 0; i < 10; i++) {
            boolean isComum = false;
            for(int j = 0; j < 10; j++) {
                if(vetor2[i] == vetor1[j]) {
                    isComum = true;
                    break;
                }
            }
            if(!isComum) {
                System.out.print(vetor2[i] + " ");
            }
        }
    }
}