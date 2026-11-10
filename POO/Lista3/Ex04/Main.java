package Ex04;

public class Main {
    public static void main(String[] args) {
        Produto p1 = new Produto("Teclado", 150.00);
        Produto p2 = new Produto("Mouse", 80.00);
        Produto p3 = new Produto("Monitor", 900.00);

        Pedido pedido = new Pedido();

        pedido.adicionarProduto(p1, p2, p3);

        System.out.println(pedido);

        pedido.removerProduto(p2);

        System.out.println("\nApós remover o mouse:");
        System.out.println(pedido);

        pedido.adicionarProduto(p2);

        System.out.println("\nApós adicionar o mouse novamente:");
        System.out.println(pedido);
    }
}