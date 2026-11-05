// Main de aluno

package Lista2;

public class Ex03 {
    public static void main(String[] args) {
        Aluno a1 = new Aluno("Ana", "202401");
        Aluno a2 = new Aluno("Bruno", "202402", 8.5, 7.0, 9.0);
        a1.exibirBoletim();
        a2.exibirBoletim();
        a1.lancarNota(0, 6.0);
        a1.lancarNota(1, 4.5);
        a1.lancarNota(2, 5.0);
        a1.lancarNota(3, 10.0);
        a1.lancarNota(0, 12.0);
        System.out.println(a1.calcularMedia());
        System.out.println(a1.getSituacao());
        a1.exibirBoletim(false);
        a2.exibirBoletim(true);
    }
}
