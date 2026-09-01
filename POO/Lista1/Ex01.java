package Lista1;
import java.util.Scanner;

public class Ex01 {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.println("Digite o número de horas trabalhadas: ");
        int horas = sc.nextInt();

        System.out.println("Digite o valor da hora trabalhada: ");
        float valorHora = sc.nextFloat();

        float salarioBruto = horas * valorHora;
        float imposto = salarioBruto * 0.3f;

        System.out.printf("Salário Bruto: R$%.2f\nImposto: R$%.2f\nSalário Líquido: R$%.2f", salarioBruto, imposto, (salarioBruto - imposto));
        sc.close();
    }
}
