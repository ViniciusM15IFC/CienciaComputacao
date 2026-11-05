// Main de calculadora

package Lista2;

public class Ex05 {
    public static void main(String[] args) {
        System.out.println("Soma de dois inteiros: " + Calculadora.somar(2, 3));
        System.out.println("Soma de dois números reais: " + Calculadora.somar(2.5, 3.5));
        System.out.println("Soma de três inteiros: " + Calculadora.somar(1, 2, 3));
        System.out.println("Soma de um vetor de inteiros: " + Calculadora.somar(new int[]{1, 2, 3, 4, 5}));
        System.out.println("Média de dois números reais: " + Calculadora.media(4.0, 6.0));
        System.out.println("Média de um vetor de inteiros: " + Calculadora.media(new int[]{1, 2, 3, 4, 5}));
    }
}
