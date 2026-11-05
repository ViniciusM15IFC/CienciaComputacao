package POO.Agenda;


import java.util.List;
import java.util.ArrayList;

public class Contato {
    private List<Email> emails;
    private List<Telefone> telefones;
    private String nome;

    public Contato(String nome) {
        this.nome = nome;
        this.telefones = new ArrayList<>();
        this.emails = new ArrayList<>();
    }

    public Contato(String nome, List<Email> emails, List<Telefone> telefones) {
        this.nome = nome;
        this.telefones = new ArrayList<>(telefones);
        this.emails = new ArrayList<>(emails);
    }

    public Contato(String nome, String email, String telefone) {
        Telefone t = new Telefone(telefone, TipoContato.PESSOAL);
        Email e = new Email(email, TipoContato.PESSOAL);
        this.nome = nome;
        this.telefones = new ArrayList<>();
        this.telefones.add(t);
        this.emails = new ArrayList<>();
        this.emails.add(e);
    }
    public Contato(String nome, String email, TipoContato tipoEmail, String telefone, TipoContato tipoTelefone) {
        Telefone t = new Telefone(telefone, tipoTelefone);
        Email e = new Email(email, tipoEmail);
        this.nome = nome;
        this.telefones = new ArrayList<>();
        this.telefones.add(t);
        this.emails = new ArrayList<>();
        this.emails.add(e);
    }

    public void AdicionarTelefone(String texto) {
        Telefone t = new Telefone(texto, TipoContato.PESSOAL);
        this.telefones.add(t);
    }
    public void AdicionarTelefone(String texto, TipoContato tipo) {
        Telefone t = new Telefone(texto, tipo);
        this.telefones.add(t);
    }

    public void AdicionarEmail(String email) {
        Email e = new Email(email, TipoContato.PESSOAL);
        this.emails.add(e);
    }
    public void AdicionarEmail(String email, TipoContato tipo) {
        Email e = new Email(email, tipo);
        this.emails.add(e);
    }

    public String getNome() {
        return this.nome;
    }

    public String toString() {
        String msg = "Contato: " + this.nome;

        for (Telefone t : this.telefones) {
            msg += "\n" + t;
        }

        for (Email e : this.emails) {
            msg += "\n" + e;
        }

        return msg;
    }

}