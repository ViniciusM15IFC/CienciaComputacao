package Ex03;

import java.util.List;
import java.util.ArrayList;

public class Disciplina {
    private String nome;
    private int cargaHoraria;
    private List<Aluno> alunos;

    public Disciplina(String nome, int cargaHoraria) {
        this.nome = nome;
        this.cargaHoraria = cargaHoraria;
        this.alunos = new ArrayList<Aluno>();
    }

    public String getNome() {
        return nome;
    }

    public int getCargaHoraria() {
        return cargaHoraria;
    }

    public List<Aluno> getAlunos() {
        return alunos;
    }

    public String toString() {
        String text = "Disciplina: " + nome +
                ", cargaHoraria: " + cargaHoraria +
                ", alunos: ";

        for (Aluno aluno : alunos) {
            text += aluno.getNome() + ", ";
        }

        return text;
    }
}
