public class Main {
    public static void main(String args[]) {
        Produto a = new Produto(1, "Caneta", 50, 2.5);
        Produto b = new Produto(2, "Caderno");

        a.entrada(20);
        System.out.println("Quantidade de " + a.getDescricao() + ": " + a.getQuantidade());

        String msg;

        System.out.println("Saida de 100: ");
        if (a.saida(100)) {
            msg = "Saida realizada com sucesso!";
        } else {
            msg = "Não há estoque suficiente para a saída!";
        }
        System.out.println(msg);

        System.out.println("Saida de 30: ");
        if (a.saida(30)) {
            msg = "Saida realizada com sucesso!";
        } else {
            msg = "Não há estoque suficiente para a saída!";
        }
        System.out.println(msg);
        System.out.println("Quantidade de " + a.getDescricao() + ": " + a.getQuantidade());

        System.out.println("Tem 40 canetas no estoque? " + a.temEstoque(40));
        System.out.println("Tem 10 cadernos no estoque? " + b.temEstoque(10));
    }
}