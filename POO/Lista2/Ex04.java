package Lista2;

public class Ex04 {
    public static void main(String[] args) {
        int[][] valida = {
                { 2, 7, 6 },
                { 9, 5, 1 },
                { 4, 3, 8 }
        };

        QuadradoMagico q1 = new QuadradoMagico(valida);
        QuadradoMagico q2 = new QuadradoMagico(1, 2, 3, 4, 5, 6, 7, 8, 9);

        System.out.println(q1.ehQuadradoMagico()); // true
        System.out.println(q2.ehQuadradoMagico()); // false
    }
}
