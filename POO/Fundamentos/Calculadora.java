package Fundamentos;

import java.text.DecimalFormat;
import java.util.Scanner;

public class Calculadora {

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        DecimalFormat df = new DecimalFormat("#.##");

        boolean fechar = false;

        while (!fechar) {
            System.out.println("Digite um número: ");
            float n1 = sc.nextFloat();

            System.out.println("Digite outro número: ");
            float n2 = sc.nextFloat();

            sc.nextLine();

            System.out.println("Digite o operador(+, -, *, /): ");
            String op = sc.nextLine();

            float resultado = 0;

            switch (op) {
                //soma
                case "+":
                    resultado = n1 + n2;
                    break;
                case "-":
                    resultado = n1 - n2;
                    break;
                case "*":
                    resultado = n1 * n2;
                    break;
                case "/":
                    resultado = n1 / n2;
                    break;
                case "99":
                    fechar = true;
                    break;
                default:
                    System.out.println("Operador inválido");
                    continue;
            }

            System.out.printf("Resultado: %s %s %s = %s\n", df.format(n1), op, df.format(n2), df.format(resultado));
        }

        sc.close();
    }
}
