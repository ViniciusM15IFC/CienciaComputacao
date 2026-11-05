package Agenda;

import  java.util.List;
import  java.util.ArrayList;

public class Contato{
    private List<Email> emails;
    private List<Telefone> telefones;
    private String nome;

    public Contato(String nome)
    {
        this.nome = nome;
        this.telefones = new ArrayList<>();
        this.emails = new ArrayList<>();
    }

    public Contato(String nome, List<Email> emails, List<Telefone> telefones)
    {
        this.nome = nome;
        this.telefones = new ArrayList<>(telefones);
        this.emails = new ArrayList<>(emails);
    }

    public Contato(String nome, Email email, Telefone telefone)
    {
        this.nome = nome;
        this.telefones = new ArrayList<>();
        this.telefones.add(telefone);
        this.emails = new ArrayList<>(emails);
        this.emails.add(email);
    }

    public void AdicionarTelefone(String texto, TipoContato tipo)
    {
        for(Telefone t : telefones)
        {
            this.telefones.add(t);
        }
    }

    public void AdicionarEmail(Email... emails)
    {
        for(Email e : emails)
        {
            this.emails.add(e);
        }
    }

    public String toString()
    {
        String msg = "Contato: " + this.nome;

        for(Telefone t : this.telefones)
        {
            msg += "\n" + t;
        }

        for(Email e : this.emails)
        {
            msg += "\n" + e;
        }

        return msg;
    }



}