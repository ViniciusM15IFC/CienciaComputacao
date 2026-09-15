package IntroObjetos;

public class Produto {
    private String nome;
    private double valor;
    private int estoque;

    public void definirNome(String nome)
    {
        this.nome = nome;
    }

    public void definirValor(double valor)
    {
        this.valor = valor;
    }

    public void definirEstoque(int estoque)
    {
        this.estoque = estoque;
    }

    public int pegarEstoque()
    {
        return this.estoque;
    }

    public String toString()
    {
        return "Nome: " + nome + "; Valor: " + "; Quantidade: " + estoque;
    }

    public double calcularValorTotal()
    {
        return valor * estoque;
    }

    public void alterarEstoque(int estoque)
    {
        this.estoque += estoque;
    }
}
