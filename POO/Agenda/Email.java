package POO.Agenda;


public class Email {
    private String email;
    private TipoContato tipo;

    public Email(String texto) {
        this.email = texto;
        this.tipo = TipoContato.PESSOAL;
    }

    public Email(String texto, TipoContato tipo) {
        this.email = texto;
        this.tipo = tipo;
    }

    public String getEmail() {
        return this.email;
    }

    public TipoContato getTipo() {
        return this.tipo;
    }

    public String toString() {
        return "Email: " + this.email + " - " + this.tipo;
    }

}
