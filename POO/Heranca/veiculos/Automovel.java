package veiculos;

public class Automovel {
    protected String modelo;
    protected String marca;
    protected float velocidade;
    protected boolean ligado;
    protected double potencia;

    public Automovel(String mod, String marca, double pot) {
        this.modelo = mod;
        this.marca = marca;
        this.potencia = pot;
        this.velocidade = 0;
        this.ligado = false;
    }

    public void ligar() {
        this.ligado = true;
    }

    public void desligar() {
        this.ligado = false;
    }

    public boolean isLigado() {
        return ligado;
    }

    public void mostrarVel()
    {
        System.out.println("Velocidade atual: " + this.velocidade);
    }

    public void acelerar(double aceleracao) {
        if (isLigado()) {
            this.velocidade += aceleracao/100 * this.potencia;
            this.mostrarVel();
        }
    }

    public void frear(double frenagem) {
        this.velocidade -= frenagem;
        this.mostrarVel();
    }

}
