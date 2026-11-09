package Ex02;

import java.util.ArrayList;
import java.util.List;

public class Comodo {
    private String nome;
    private ArrayList<Moveis> moveis;
    private double area;

    public Comodo(String nome, double area) {
        this.nome = nome;
        this.moveis = new ArrayList<>();
        this.area = area;
    }

    public void adicionarMovel(Moveis... moveis) {
        this.moveis.addAll(List.of(moveis));
    }

    public void removerMovel(Moveis... moveis) {
        this.moveis.removeAll(List.of(moveis));
    }

    public String getNome() {
        return nome;
    }

    public ArrayList<Moveis> getMoveis() {
        return moveis;
    }

    public double getArea() {
        return area;
    }

    public String toString() {
        return "\nComodo: " + nome +
                ", area: " + area +
                ", moveis: " + moveis;
    }
}