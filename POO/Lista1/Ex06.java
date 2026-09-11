// 6. Faça um programa que leia um número e calcule o fatorial desse número

package Lista1;

import java.util.Scanner;

public class Ex06 {
    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);

        System.out.println("Digite um número: ");
        int numero = s.nextInt();

        long resultado = 1;

        for(int i = numero; i > 0; i--){
            resultado *= i;
        }

        System.out.println("O fatorial de " + numero + " é: " + resultado);
    }
}
