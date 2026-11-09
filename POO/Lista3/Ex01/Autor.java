package POO.Lista3.Ex01;

public class Autor {
    private String nome;
    private String nacionalidade;
    private String dataNascimento;

    public Autor(String nome, String nacionalidade, String dataNascimento) {
        this.nome = nome;
        this.nacionalidade = nacionalidade;
        this.dataNascimento = dataNascimento;
    }

    public String getNome() {
        return nome;
    }

    public String getNacionalidade() {
        return nacionalidade;
    }

    public String getDataNascimento() {
        return dataNascimento;
    }

    public String toString() {
        return "Autor: " + nome +
                ", nacionalidade: " + nacionalidade + 
                ", dataNascimento: " + dataNascimento;
    }
}
