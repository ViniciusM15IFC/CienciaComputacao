// 11. Faça um programa que leia um conjunto não determinado de valores, um de cada vez, e
// escreva uma tabela com cabeçalho, que deve ser repetido a cada vinte linhas. A tabela
// deverá conter o valor lido, seu cubo e sua raiz quadrada. Finalize quando a entrada for
// um número negativo ou 0.

package Lista1;

import java.util.Scanner;

public class Ex09 {
    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);
        
        int cont = 0;

        while(true)
        {
            System.out.println("Digite um número (negativo ou 0 para sair): ");
            double numero = s.nextDouble();

            if(numero <= 0)
                break;

            if(cont % 20 == 0)
            {
                System.out.printf("%-10s %-10s %-10s\n", "Número", "Cubo", "Raiz Quadrada");
                System.out.println("-------------------------------------");
            }

            double cubo = Math.pow(numero, 3);
            double raizQuadrada = Math.sqrt(numero);

            System.out.printf("%-10.2f %-10.2f %-10.2f\n", numero, cubo, raizQuadrada);
            cont++;
        }

    }
}