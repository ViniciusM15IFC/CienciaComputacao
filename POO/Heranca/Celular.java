public class Celular extends SemFio {
    private String resolucao;
    private String SO;

    public Celular(int c, String m, String n, float f, int a, String r, String so)
    {
        super(c, m, n, f, a);
        this.resolucao = r;
        this.SO = so;
    }
}
