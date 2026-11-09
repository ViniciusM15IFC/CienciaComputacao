//  Projete uma classe ‘Casa‘ que contenha várias ‘Peças‘ (como quartos, salas, banhei
// ros). Uma casa é composta por várias peças, e quando a casa é destruída, suas
// peças também são.

package Ex02;

public class Main {
    public static void main(String[] args) {

        Comodo sala = new Comodo("Sala", 25.0);
        Casa casa = new Casa(123, 100.0, sala);

        System.out.println(casa);

        System.out.println("\nAdicionar cozinha: " +
                casa.adicionarComodo("Cozinha", 12.0));

        System.out.println("Adicionar quarto: " +
                casa.adicionarComodo("Quarto", 20.0));

        System.out.println("Adicionar garagem: " +
                casa.adicionarComodo("Garagem", 60.0));

        System.out.println("\nBuscar sala: " + casa.buscarComodo("Sala"));
        System.out.println("Buscar banheiro: " + casa.buscarComodo("Banheiro"));

        System.out.println("\nAdicionar móveis na sala: " +
                casa.adicionarMovel("Sala",
                        Moveis.SOFA, Moveis.RACK, Moveis.POLTRONA));

        System.out.println("Adicionar móveis na cozinha: " +
                casa.adicionarMovel("Cozinha",
                        Moveis.MESA, Moveis.CADEIRA, Moveis.ABAJUR));

        System.out.println("Adicionar móveis no quarto: " +
                casa.adicionarMovel("Quarto",
                        Moveis.CAMA, Moveis.ARMARIO, Moveis.CRIADO_MUDO));

        System.out.println("Adicionar móvel em cômodo inexistente: " +
                casa.adicionarMovel("Banheiro", Moveis.CAMA));

        System.out.println("\n" + casa);

        System.out.println("\nRemover móveis da sala: " +
                casa.removerMovel("Sala", Moveis.POLTRONA, Moveis.RACK));

        System.out.println("Remover móvel da cozinha: " +
                casa.removerMovel("Cozinha", Moveis.ABAJUR));

        System.out.println("\n" + casa);

        System.out.println("\nNome da sala: " + sala.getNome());
        System.out.println("Área da sala: " + sala.getArea());
        System.out.println("Móveis da sala: " + sala.getMoveis());
        System.out.println("Área ocupada: " + casa.getAreaOcupada());

        System.out.println("\nRemover cozinha: " + casa.removerComodo("Cozinha"));
        System.out.println("Remover banheiro: " + casa.removerComodo("Banheiro"));

        System.out.println("\n" + casa);

        Casa casa2 = new Casa(456, 150.0);

        System.out.println("\n" + casa2);

        System.out.println("Adicionar escritório: " +
                casa2.adicionarComodo("Escritório", 18.0));

        System.out.println(casa2);
    }
}

   
