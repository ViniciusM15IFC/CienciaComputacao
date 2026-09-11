// Faça um programa que leia dois números e apresente o fatorial de todos os números
// que estejam dentro do intervalo definido pelos números que foram lidos
package Lista1;

import java.util.Scanner;

public class Ex07 {

    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);

        System.out.print("Digite o primeiro número: ");
        int num1 = s.nextInt();

        System.out.print("Digite o segundo número: ");
        int num2 = s.nextInt();

        int menor = Math.min(num1, num2);
        int maior = Math.max(num1, num2);

        for (int i = menor; i <= maior; i++) {
            long resultado = 1;

            for (int j = i; j > 0; j--) {
                resultado *= j;
            }
            System.out.println("O fatorial de " + i + " é: " + resultado);
        }

    }
}
