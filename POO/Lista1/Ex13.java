// Faça um programa que dados 2 vetores inteiros de tamanhos 10 e 5, respectivamente,
// calcule e mostre 2 vetores resultantes:
// • no primeiro vetor resultante, cada elemento será composto pela soma de cada
// número par do primeiro vetor somado a todos os números do segundo vetor;
// • o segundo vetor resultante será composto pela quantidade de divisores que cada
// número ímpar do primeiro vetor tem no segundo vetor.

package Lista1;

import java.util.Scanner;

public class Ex09 {
    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);
         
        int[] vetor1 = new int[10];
        int[] vetor2 = new int[5];

        for(int i = 0; i < 10; i++) {
            System.out.println("Digite um número para o primeiro vetor: ");
            vetor1[i] = s.nextInt();
        }

        for(int i = 0; i < 5; i++) {
            System.out.println("Digite um número para o segundo vetor: ");
            vetor2[i] = s.nextInt();
        }

        int[] vetorR1 = new int[10];
        int[] vetorR2 = new int[10];

        for(int i = 0; i < 10; i++) {
            if(vetor1[i] % 2 == 0) {
                vetorR1[i] = vetor1[i];
                for(int j = 0; j < 5; j++) {
                    vetorR1[i] += vetor2[j];
                }
            } else {
                int quantidadeDivisores = 0;
                for(int j = 0; j < 5; j++) {
                    if(vetor1[i] % vetor2[j] == 0) {
                        quantidadeDivisores++;
                    }
                }
                vetorR2[i] = quantidadeDivisores;
            }
        }

        System.out.println("Vetor resultante 1: ");
        for(int i = 0; i < 10; i++) {
            System.out.print(vetorR1[i] + " ");
        }
        System.out.println();

        System.out.println("Vetor resultante 2: ");
        for(int i = 0; i < 10; i++) {
            System.out.print(vetorR2[i] + " ");
        }
        System.out.println();
    }
}