package POO.Lista3.Ex01;

public class Livro {
    private String titulo;
    private Autor autor;
    private Editora editora;
    private String dataPublicacao;
    private int numeroPaginas;

    public Livro(String titulo, Autor autor, Editora editora, String dataPublicacao, int numeroPaginas) {
        this.titulo = titulo;
        this.autor = autor;
        this.editora = editora;
        this.dataPublicacao = dataPublicacao;
        this.numeroPaginas = numeroPaginas;
    }

    public String getTitulo() {
        return titulo;
    }

    public Autor getAutor() {
        return autor;
    }

    public Editora getEditora() {
        return editora;
    }

    public String getDataPublicacao() {
        return dataPublicacao;
    }

    public int getNumeroPaginas() {
        return numeroPaginas;
    }

    public String toString() {
        return "Livro: " +
                titulo +
                "\n " + autor +
                "\n " + editora +
                "\n dataPublicacao: " + dataPublicacao +
                "\n numeroPaginas: " + numeroPaginas + "\n";
    }
}
