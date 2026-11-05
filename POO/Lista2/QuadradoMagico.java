// Um quadrado mágico é uma matriz quadrada de valores inteiros onde a soma dos
// valores em cada linha, coluna ou diagonal principal é a mesma, e não existem
// elementos repetidos. Por exemplo, a matriz:
// 2 7 6
// 9 5 1
// 4 3 8
// Escreva uma classe QuadradoMagico que tenha um método que retorne true
// caso a matriz, passada como argumento para o método, represente um quadrado
// mágico.

package Lista2;

public class QuadradoMagico {
    private int[][] matriz;
    private final int N = 3;

    public QuadradoMagico(int[][] matriz) {
        this.matriz = matriz;
    }

    public QuadradoMagico(int n1, int n2, int n3, int n4, int n5, int n6, int n7, int n8, int n9) {
        this.matriz = new int[3][3];
        this.matriz[0][0] = n1;
        this.matriz[0][1] = n2;
        this.matriz[0][2] = n3;
        this.matriz[1][0] = n4;
        this.matriz[1][1] = n5;
        this.matriz[1][2] = n6;
        this.matriz[2][0] = n7;
        this.matriz[2][1] = n8;
        this.matriz[2][2] = n9;
    }

    public boolean temRepeticao() {
        boolean[] elementos = new boolean[10];

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                int valor = matriz[i][j];

                if (valor < 1 || valor > 9) {
                    return true; // Valor inválido
                }

                if (elementos[valor]) {
                    return true; // Número repetido
                }

                elementos[valor] = true;
            }
        }

        return false; // Nenhuma repetição
    }

    public boolean ehQuadradoMagico() {

        if (temRepeticao()) {
            return false;
        }
        int somaDiagonalPrincipal = 0;
        int somaDiagonalSecundaria = 0;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (i == j) {
                    somaDiagonalPrincipal += matriz[i][j];
                } else if (i + j == N - 1) {
                    somaDiagonalSecundaria += matriz[i][j];
                }
            }
        }

        if (somaDiagonalPrincipal != somaDiagonalSecundaria) {
            return false;
        }

        for (int i = 0; i < N; i++) {
            int somaLinha = 0;
            int somaColuna = 0;

            for (int j = 0; j < N; j++) {
                somaLinha += matriz[i][j];
                somaColuna += matriz[j][i];
            }

            if (somaLinha != somaDiagonalPrincipal || somaColuna != somaDiagonalPrincipal) {
                return false;
            }
        }

        return true;
    }

}
