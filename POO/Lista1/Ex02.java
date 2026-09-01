/* 2. Sabe-se que o quilowatt de energia custa 1% do salário mínimo. Faça um programa
que recebe o valor do salário mínimo e a quantidade de quilowatts consumida por uma
residência, calcule e mostre:
• o valor de cada quilowatt;
• o valor a ser pago por essa residência; */


package Lista1;

import java.util.Scanner;

public class Ex02 {
    
    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);


        System.out.println("Digite o valor do salário minimo: ");
        float valorKw = s.nextFloat()/100f;

        System.out.println("Digite a quantidade de quilowatts consumidos: ");
        int kwatts = s.nextInt();

        System.out.println("Valor de um quilowatts: " + valorKw);
        System.out.println("Valor a ser pago: " + (valorKw*kwatts));
    }
}
