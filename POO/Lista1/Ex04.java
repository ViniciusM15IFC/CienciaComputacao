/* 4. Faça um programa que receba
• O código do produto comprado;
• A quantidade comprada do produto
Calcule e mostre:
• O preço unitário do produto comprado, seguindo a Tabela I;
• O preço total da nota;
• O valor do desconto, seguindo a Tabela II e aplicado sobre o preço total da nota;
• O preço final da nota depois do desconto.
Tabela I
Código Preço
1 a 10 R$ 10,00
11 a 20 R$ 15,00
21 a 30 R$ 20,00
31 a 40 R$ 40,00 


Tabela II
Preço Total da Nota % de Desconto
Até R$ 250,00 | 5%
Entre R%250,00 e R$500,00 | 10%
Acima de R$ 500,00 | 15%
*/


package Lista1;

import java.util.Scanner;

public class Ex04 {
    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);

        double precoUnit;
        double precoBruto;
        double desc;
        double precoFinal;
        int cod;

        do
        {
            System.out.println("Digite o código");
            cod = s.nextInt();
        } while(cod < 1 && cod > 40);

        System.out.println("Digite a quantidade comprada");
        int quant = s.nextInt();

        if (cod <= 10)
        {
            precoUnit = 10f;
        }
        else if (cod <= 20)
        {
            precoUnit = 15f;
        }
        else if (cod <= 30)
        {
            precoUnit = 20f;
        }
        else
        {
            precoUnit = 40f;
        }

        precoBruto = quant * precoUnit;

        if (precoBruto <= 250)
        {
            desc = precoBruto * 0.05;
        }
        else if (precoBruto <= 500)
        {
            desc = precoBruto * 0.10;
        }
        else
        {
            desc = precoBruto * 0.15;
        }

        precoFinal = precoBruto - desc;

        //System.out.printf("Código: %i\nQuantidade: %i\nPreço Bruto: %d\nDesconto: %d\n Preço Final: %d", cod, quant, precoBruto, desc, precoFinal);
        System.out.println("Código: "+cod);
        System.out.println("Quantidade: "+quant);
        System.out.println("Preço Unitário: "+precoUnit);
        System.out.println("Preço Bruto: "+precoBruto);
        System.out.println("Desconto: "+desc);
        System.out.println("Preço Final: "+precoFinal);
        
    }
}
