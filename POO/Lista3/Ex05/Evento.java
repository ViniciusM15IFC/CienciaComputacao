package Ex05;

import java.util.List;
import java.util.ArrayList;

public class Evento {
    private String nome;
    private String data;
    private String local;
    private List<Participante> participantes;

    public Evento(String nome, String data, String local) {
        this.nome = nome;
        this.data = data;
        this.local = local;
        this.participantes = new ArrayList<Participante>();
    }

    public String getNome() {
        return nome;
    }

    public String getData() {
        return data;
    }

    public String getLocal() {
        return local;
    }

    public List<Participante> getParticipantes() {
        return participantes;
    }

    public void adicionarParticipante(Participante... participante) {
        for (Participante p : participante) {
            if (!participantes.contains(p)) {
                participantes.add(p);
                p.getEventos().add(this);
            }
        }
    }

    public void removerParticipante(Participante... participante) {
        for (Participante p : participante) {
            if (participantes.remove(p)) {
                p.getEventos().remove(this);
            }
        }
    }
}
