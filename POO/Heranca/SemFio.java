public class SemFio extends Telefone {
    private float frequencia;
    private int alcance;

    public SemFio(int c, String m, String n, float f, int a)
    {
        super(c, m, n);
        this.frequencia = f;
        this.alcance = a;
    }
}
