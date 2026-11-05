package POO.Agenda;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Agenda {
    private List<Contato> contatos;

    public Agenda()
    {
        this.contatos = new ArrayList<>();
    }

    public void adicionarContato(String nome)
    {
        Contato c = new Contato(nome);
        contatos.add(c);
    }
    public void adicionarContato(String nome, String email, String telefone)
    {
        Contato c = new Contato(nome, email, telefone);
        contatos.add(c);
    }
    public void adicionarContato(String nome, String email, TipoContato tipoEmail, String telefone, TipoContato tipoTelefone)
    {
        Contato c = new Contato(nome, email, tipoEmail, telefone, tipoTelefone);
        contatos.add(c);
    }

    public List<Contato> buscarContato(String nome)
    {
        List<Contato> resultados = new ArrayList<>();
        for (Contato contato : contatos) {
            if (contato.getNome().equals(nome)) {
                resultados.add(contato);
            }
        }
        return resultados;
    }

    public void removerContato(String nome)
    {
        List<Contato> resultados = buscarContato(nome);
        if(resultados.size() == 0) {
            System.out.println("Contato não encontrado.");
        } else {
            if(resultados.size() == 1)
            {
                contatos.remove(resultados.get(0));
            } else {
                System.out.println("Mais de um contato encontrado com o nome " + nome + ". Escolha um para remover(0 para excluir todos e -1 para cancelar).");
                for (int i = 0; i < resultados.size(); i++) {
                    System.out.println((i+1) + ": " + resultados.get(i));
                }
                Scanner sc = new Scanner(System.in);
                int op = sc.nextInt();
                sc.close();
                if(op == 0) {
                    contatos.removeAll(resultados);
                } else if(op > 0 && op <= resultados.size()) {
                    contatos.remove(resultados.get(op - 1));
                } else {
                    System.out.println("Operação cancelada.");
                }
            }
        }
    }

    public void adicionarEmail(String nome, String email, TipoContato tipoEmail) {
        List<Contato> resultados = buscarContato(nome);
        if (resultados.size() == 1) {
            resultados.get(0).AdicionarEmail(email, tipoEmail);
        } else {
            System.out.println("Contato não encontrado ou encontrado mais de um com o mesmo nome.");
        }
    }

    public void adicionarTelefone(String nome, String telefone, TipoContato tipoTelefone) {
        List<Contato> resultados = buscarContato(nome);
        if (resultados.size() == 1) {
            resultados.get(0).AdicionarTelefone(telefone, tipoTelefone);
        } else {
            System.out.println("Contato não encontrado ou encontrado mais de um com o mesmo nome.");
        }
    }

    public String toString()
    {
        String msg = "";

        for (Contato contato : contatos) {
            msg += contato + "\n\n";                                                             
        }     

        return msg;
    }
}
