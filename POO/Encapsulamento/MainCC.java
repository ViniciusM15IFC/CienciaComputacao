package Encapsulamento;

public class MainCC {
    public static void main(String[] args) {
        ContaCorrente c = new ContaCorrente("Teste", "12345678910");

        System.out.println(c.getCpf());
        System.out.println(c.getTelefone());
    }
}
