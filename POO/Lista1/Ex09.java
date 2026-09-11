// 9. Faça um programa que leia um número inteiro e diga se ele é primo
package Lista1;

import java.util.Scanner;

public class Ex09 {

    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);

        System.out.println("Digite um número inteiro: ");
        long numero = s.nextLong();

        boolean isPrimo = true;

        if (numero <= 1) {
            isPrimo = false; // 0 e 1 não são primos, por isso esse if
        } else if (numero == 2) {
            isPrimo = true; 
        } else if (numero % 2 == 0) {
            isPrimo = false;
        } else {
            for (int i = 3; i <= Math.sqrt(numero); i += 2) {
                if (numero % i == 0) {
                    isPrimo = false;
                    break;
                }
            }
        }

        if (isPrimo) {
            System.out.println("O número é primo.");
        } else {
            System.out.println("O número não é primo.");
        }
    }
}
