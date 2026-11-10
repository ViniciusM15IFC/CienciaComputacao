package Ex05;

public class Main {
    public static void main(String[] args) {
        Participante p1 = new Participante("João", "joao@email.com");
        Participante p2 = new Participante("Maria", "maria@email.com");
        Participante p3 = new Participante("Pedro", "pedro@email.com");

        Evento e1 = new Evento("Workshop Java", "15/11/2026", "Auditório");
        Evento e2 = new Evento("Palestra Python", "20/11/2026", "Sala 2");

        e1.adicionarParticipante(p1, p2);
        e2.adicionarParticipante(p1, p3);

        System.out.println("Participantes do Workshop:");
        for (Participante p : e1.getParticipantes()) {
            System.out.println(p);
        }

        System.out.println("\nEventos do João:");
        for (Evento e : p1.getEventos()) {
            System.out.println(e.getNome());
        }

        e1.removerParticipante(p1);

        System.out.println("\nApós remover João do Workshop:");
        for (Participante p : e1.getParticipantes()) {
            System.out.println(p);
        }

        System.out.println("\nEventos do João:");
        for (Evento e : p1.getEventos()) {
            System.out.println(e.getNome());
        }
    }
}