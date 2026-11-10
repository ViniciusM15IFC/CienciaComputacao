package Ex03;

import java.util.List;
import java.util.ArrayList;

public class Aluno {
    private int matricula;
    private String nome;
    private List<Disciplina> disciplinas;

    public Aluno(int matricula, String nome) {
        this.matricula = matricula;
        this.nome = nome;
        this.disciplinas = new ArrayList<Disciplina>();
    }

    public int getMatricula() {
        return matricula;
    }

    public String getNome() {
        return nome;
    }

    public List<Disciplina> getDisciplinas() {
        return disciplinas;
    }

    public void adicionarDisciplina(Disciplina disciplina) {
        if (!disciplinas.contains(disciplina)) {
            disciplinas.add(disciplina);
            disciplina.getAlunos().add(this);
        }
    }

    public void removerDisciplina(Disciplina disciplina) {
        if (disciplinas.remove(disciplina)) {
            disciplina.getAlunos().remove(this);
        }
    }

    public String toString() {
        String text = "Aluno: " + nome +
                ", matricula: " + matricula +
                ", disciplinas: ";

        for (Disciplina disciplina : disciplinas) {
            text += disciplina.getNome() + ", ";
        }

        return text;
    }
}
