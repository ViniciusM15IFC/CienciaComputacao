package POO.Agenda;


public class Main {
    public static void main(String[] args) {
        Agenda agenda = new Agenda();

        agenda.adicionarContato("Arthur");

        agenda.adicionarContato("Isabella", "bella@email.com", "49999998888");

        agenda.adicionarContato("Bernardo", "barcaro@naotrabalhador.com", TipoContato.PROFISSIONAL, "4935661122", TipoContato.PESSOAL);

        agenda.adicionarContato("Jaine");
        
        agenda.adicionarContato("Evellyn Isadora Pinto");

        System.out.println(agenda);

        System.out.println(agenda.buscarContato("Arthur"));

        agenda.removerContato("Arthur");
        agenda.adicionarEmail("Jaine", "schwarzenegger@email.com", TipoContato.PESSOAL);

        System.out.println(agenda);
    }
}