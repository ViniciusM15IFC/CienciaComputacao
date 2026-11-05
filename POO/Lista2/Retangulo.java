// • Crie a classe Retangulo com os atributos privados largura e altura (double).
// • Implemente três construtores (sobrecarga):
// – Sem parâmetros: cria um retângulo de 1 × 1.
// – Com um parâmetro lado: cria um quadrado (largura = altura = lado).
// – Com dois parâmetros largura e altura.
// • A classe deverá prover os métodos: calcularArea(), calcularPerimetro() e ehQuadrado() (retorna boolean).
// • O método toString(), que retorne uma descrição do retângulo.


package Lista2;

public class Retangulo {
    private double largura;
    private double altura;

    public Retangulo() {
        this.largura = 1.0;
        this.altura = 1.0;
    }

    public Retangulo(double lado) {
        this.largura = lado;
        this.altura = lado;
    }

    public Retangulo(double largura, double altura) {
        this.largura = largura;
        this.altura = altura;
    }

    public double calcularArea() {
        return this.largura * this.altura;
    }

    public double calcularPerimetro() {
        return 2 * (this.largura + this.altura);
    }

    public boolean ehQuadrado() {
        return this.largura == this.altura;
    }

    public String toString() {
        return "Retangulo: " +
                "largura = " + largura +
                ", altura = " + altura +
                ", área = " + calcularArea() +
                ", perímetro = " + calcularPerimetro() +
                ", quadrado = " + ehQuadrado();
    }
}
