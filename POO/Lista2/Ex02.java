// • Na classe principal: crie um objeto usando cada um dos três construtores e imprima área, perímetro e se é quadrado.

package Lista2;

public class Ex02 {
    public static void main(String[] args) {
        Retangulo r1 = new Retangulo();
        Retangulo r2 = new Retangulo(5.0);
        Retangulo r3 = new Retangulo(3.0, 4.0);

        System.out.println(r1);
        System.out.println(r2);
        System.out.println(r3);
    }
}

