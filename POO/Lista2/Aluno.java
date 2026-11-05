// Crie a classe Aluno com os atributos privados nome (String), matricula (String) e
// notas (vetor de 3 posições do tipo double).
// • Implemente 02 construtores (sobrecarga):
// – Com dois parâmetros nome e matrícula: cria um aluno com nota 0.
// – Com três parâmetros nome, matrícula e as 03 nota.
// • A classe deverá prover os métodos:
// – O método lancarNota(int posicao, double nota): só aceita posições de 0 a 2 e
// notas entre 0 e 10.
// – O método calcularMedia(), que retorna a média das 03 notas.
// – O método getSituacao(), que retorna ”Aprovado” (média >= 7), ”Recuperação”
// (média >= 4 e < 7) ou ”Reprovado” (média < 4).
// – O método exibirBoletim(): imprime nome, matrícula, notas, média e situação.
// – Sobrecarga de exibirBoletim(boolean detalhado): se false, mostra apenas nome
// e situação.

package Lista2;

public class Aluno {
    private String nome;
    private String matricula;
    private double[] notas;

    public Aluno(String nome, String matricula) {
        this.nome = nome;
        this.matricula = matricula;
        this.notas = new double[3];
        for (int i = 0; i < 3; i++) {
            this.notas[i] = 0.0;
        }
    }

    public Aluno(String nome, String matricula, double nota1, double nota2, double nota3) {
        this(nome, matricula);
        this.notas[0] = nota1;
        this.notas[1] = nota2;
        this.notas[2] = nota3;
    }

    public void lancarNota(int posicao, double nota) {
        if (posicao >= 0 && posicao < 3 && nota >= 0 && nota <= 10) {
            this.notas[posicao] = nota;
        }
    }

    public double calcularMedia() {
        double soma = 0;
        for (double nota : this.notas) {
            soma += nota;
        }
        return soma / 3;
    }

    public String getSituacao() {
        double media = calcularMedia();
        if (media >= 7) {
            return "Aprovado";
        } else if (media >= 4) {
            return "Recuperação";
        } else {
            return "Reprovado";
        }
    }

    public void exibirBoletim() {
        System.out.println("Nome: " + this.nome);
        System.out.println("Matrícula: " + this.matricula);
        System.out.print("Notas: ");
        for (double nota : this.notas) {
            System.out.print(nota + " ");
        }
        System.out.println();
        System.out.println("Média: " + calcularMedia());
        System.out.println("Situação: " + getSituacao());
    }

    public void exibirBoletim(boolean detalhado) {
        if (detalhado) {
            exibirBoletim();
        } else {
            System.out.println("Nome: " + this.nome);
            System.out.println("Situação: " + getSituacao());
        }
    }
}
