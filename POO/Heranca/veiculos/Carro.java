package veiculos;

public class Carro extends Automovel{
    private double capacidadePortaMala;

    public Carro(String mod, String marca, double pot, double cpm)
    {
        super(mod, marca, pot);
        this.capacidadePortaMala = cpm;
    }
}
