public class Produto {
    private int codigo;
    private String descricao;
    private int quantidade;
    private double preco;

    public Produto(int codigo, String descricao, int quantidade, double preco) {
        this.codigo = codigo;
        this.descricao = descricao;
        this.quantidade = quantidade;
        this.preco = preco;
    }

    public Produto(int codigo, String descricao) {
        this.codigo = codigo;
        this.descricao = descricao;
        this.quantidade = 0;
        this.preco = 0.0;
    }

    // o código não pode ser alterado, por isso não há setter
    public int getCodigo() {
        return codigo;
    }

    public String getDescricao() {
        return descricao;
    }

    public void setDescricao(String descricao) {
        this.descricao = descricao;
    }

    public int getQuantidade() {
        return quantidade;
    }

    public double getPreco() {
        return preco;
    }

    public void setPreco(double preco) {
        if (preco >= 0) {
            this.preco = preco;
        }
    }

    public void entrada(int q) {
        if (q > 0) {
            quantidade += q;
        }
    }

    public boolean saida(int q) {
        if (q > 0 && q <= quantidade) {
            quantidade -= q;
            return true;
        }
        return false;
    }

    public boolean temEstoque(int demanda) {
        return quantidade >= demanda;
    }
}