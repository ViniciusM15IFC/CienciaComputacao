// 8. Faça uma calculadora de troco, baseado nas cédulas da nossa moeda.

package Lista1;

import java.util.Scanner;

public class Ex08 {
    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);

        System.out.print("Digite o valor da compra: ");
        double valorCompra = s.nextDouble();

        System.out.print("Digite o valor pago: ");
        double valorPago = s.nextDouble();

        if (valorPago < valorCompra) {
            System.out.println("Valor pago é menor que o valor da compra. Não é possível calcular o troco.");
            return;
        }

        double troco = valorPago - valorCompra;
        System.out.println("Troco: R$ " + troco);

        int[] cedulas = {100, 50, 20, 10, 5, 2, 1};

        for(int cedula : cedulas) {
            while(troco >= cedula) {
                troco -= cedula;
                System.out.println("Cédula de R$ " + cedula);
            }
        }
    }
}