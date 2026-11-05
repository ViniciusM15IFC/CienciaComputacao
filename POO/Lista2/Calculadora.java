// Crie a classe Calculadora sem atributos (apenas métodos) e implemente a sobrecarga
// do método somar e media, conforme a lista abaixo:
// • somar(int a, int b): retorna a soma de dois inteiros
// • somar(double a, double b): retorna a soma de dois números reais
// • somar(int a, int b, int c): retorna a soma de três inteiros
// • somar(int[] vetor): retorna a soma de todos os elementos do vetor
// • media(double a, double b): retorna a média de dois números reais
// • media(int[] vetor): retorna a média de todos os elementos do vetor

package Lista2;

public class Calculadora {
    public static int somar(int a, int b) {
        return a + b;
    }

    public static double somar(double a, double b) {
        return a + b;
    }

    public static int somar(int a, int b, int c) {
        return a + b + c;
    }

    public static int somar(int[] vetor) {
        int soma = 0;
        for (int i = 0; i < vetor.length; i++) {
            soma += vetor[i];
        }
        return soma;
    }

    public static double media(double a, double b) {
        return (a + b) / 2;
    }

    public static double media(int[] vetor) {
        int soma = somar(vetor);
        return (double) soma / vetor.length;
    }
}
