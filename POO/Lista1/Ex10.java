// 10. Faça um programa que receba dez números inteiros e mostre a quantidade de números
// primos dentre os número que foram digitados.



package Lista1;

import java.util.Scanner;

public class Ex09 {
    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);
         
        int quantidadePrimos = 0;

        for(int j = 0; j < 10; j++)
        {
            System.out.println("Digite um número inteiro: ");
            long numero = s.nextLong();

            boolean isPrimo = true;

            for(int i = 1; i < Math.sqrt(numero); i+=2)
            {
                if(numero % i == 0)
                {
                    isPrimo = false;
                    break;
                }
            }

            if(isPrimo)
            {
                quantidadePrimos++;
            }
        }

        System.out.println("Quantidade de números primos: " + quantidadePrimos);
    }
}