package Ex05;

import java.util.List;
import java.util.ArrayList;

public class Participante {
    private String nome;
    private String email;
    private List<Evento> eventos;

    public Participante(String nome, String email) {
        this.nome = nome;
        this.email = email;
        this.eventos = new ArrayList<>();
    }

    public String getNome() {
        return nome;
    }

    public String getEmail() {
        return email;
    }

    public List<Evento> getEventos() {
        return eventos;
    }

    public String toString() {
        return "Participante: " + nome +
                ", email: " + email;
    }
}
