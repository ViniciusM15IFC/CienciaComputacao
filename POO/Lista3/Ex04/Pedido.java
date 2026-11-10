package Ex04;

import java.util.List;
import java.util.ArrayList;

public class Pedido {
    private List<Produto> produtos;
    private double total;

    public Pedido() {
        this.produtos = new ArrayList<Produto>();
        this.total = 0;
    }

    public void adicionarProduto(Produto... produto) {
        for (Produto p : produto) {
            produtos.add(p);
            total += p.getPreco();
        }
    }

    public void removerProduto(Produto... produto) {
        for (Produto p : produto) {
            if (produtos.remove(p)) {
                total -= p.getPreco();
            }
        }
    }

    public double getTotal() {
        return total;
    }

    public String toString() {
        String text = "Pedido: \n";

        for (Produto produto : produtos) {
            text += produto.toString() + "\n";
        }

        text += "Total: " + total;

        return text;
    }

}
