package veiculos;

public class Moto extends Automovel {
    private boolean bau;

    public Moto(String mod, String marca, double pot, boolean b)
    {
        super(mod, marca, pot);
        this.bau = b;
    }
}
