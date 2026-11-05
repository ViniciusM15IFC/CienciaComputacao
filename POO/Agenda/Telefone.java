package POO.Agenda;

public class Telefone {
    private String telefone;
    private TipoContato tipo;

    public Telefone(String texto) {
        this.telefone = texto;
        this.tipo = TipoContato.PESSOAL;
    }

    public Telefone(String texto, TipoContato tipo) {
        this.telefone = texto;
        this.tipo = tipo;
    }

    public String getTelefone() {
        return this.telefone;
    }

    public TipoContato getTipo() {
        return this.tipo;
    }

    public String toString() {
        return "Telefone: " + this.telefone + " - " + this.tipo;
    }
}
