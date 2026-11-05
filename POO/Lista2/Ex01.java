// Main de Tempo

package Lista2;

public class Ex01 {
    public static void main(String[] args) {
        Tempo t1 = new Tempo();
        Tempo t2 = new Tempo(7);
        Tempo t3 = new Tempo(7, 30);
        Tempo t4 = new Tempo(1, 0, 0);
        Tempo t5 = new Tempo(25, 90, 90); // tempo inválid0

        System.out.println(t1);
        System.out.println(t2);
        System.out.println(t3);
        System.out.println(t4);
        System.out.println(t5);

        System.out.println(t1.setHora(12));
        System.out.println(t1.setHora(25));
        System.out.println(t1);

        System.out.println(t4.tempoEmSegundos());
        System.out.println(t3.diferencaEmSegundos(t4));
    }

}
