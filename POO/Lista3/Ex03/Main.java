// 3. Crie classes ‘Aluno‘ e ‘Curso‘, onde um aluno pode estar inscrito em vários cursos, e
// um curso pode ter vários alunos matriculados

package Ex03;

public class Main {
    public static void main(String[] args) {
        Aluno aluno1 = new Aluno(101, "João");
        Aluno aluno2 = new Aluno(102, "Maria");
        Aluno aluno3 = new Aluno(103, "Pedro");

        Disciplina matematica = new Disciplina("Matemática", 80);
        Disciplina programacao = new Disciplina("Programação", 120);

        aluno1.adicionarDisciplina(matematica);
        aluno1.adicionarDisciplina(programacao);
        aluno2.adicionarDisciplina(matematica);
        aluno3.adicionarDisciplina(programacao);

        System.out.println(aluno1);
        System.out.println(aluno2);
        System.out.println(aluno3);

        System.out.println(matematica);
        System.out.println(programacao);

        aluno1.removerDisciplina(matematica);

        System.out.println(aluno1);
        System.out.println(matematica);
    }
}
