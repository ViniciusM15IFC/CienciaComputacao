package Agenda;

import java.util.ArrayList;
import java.util.List;

public class Agenda {
    private List<Contato> contatos;

    public Agenda()
    {
        this.contatos = new ArrayList<>();
    }

    public void adicionarContato(String nome)
    {
        Contato c = new Contato(nome);
        contatos.add(c);
    }

    public String toString()
    {
        String msg = "";

        for (Contato contato : contatos) {
            msg += contato + "\n\n";                                                             
        }

        return msg;
    }
}
