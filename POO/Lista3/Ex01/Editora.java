//  Desenvolva um sistema para uma livraria. Crie classes para representar ‘Livro‘, ‘Autor‘
// e ‘Editora‘, onde um livro é escrito por um autor e publicado por uma editora

package Ex01;

public class Editora {
    private String nome;
    private String pais;
    private String dataFundacao;

    public Editora(String nome, String pais, String dataFundacao) {
        this.nome = nome;
        this.pais = pais;
        this.dataFundacao = dataFundacao;
    }

    public String getNome() {
        return nome;
    }

    public String getPais() {
        return pais;
    }

    public String getDataFundacao() {
        return dataFundacao;
    }

    public String toString() {
        return "Editora: " +
                 nome +
                ", pais: " + pais +
                ", dataFundacao: " + dataFundacao;
    }
}
