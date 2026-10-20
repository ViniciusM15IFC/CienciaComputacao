package soldado;

public class Inimigo {
    private int vida;
    private int x;

    public Inimigo(int vida, int x) {
        this.vida = vida;
        this.x = x;
    }

    public void sofreDano(int dano) {
        this.vida -= dano;
    }

    public int getX() {
        return this.x;
    }

    public int getVida() {
        return this.vida;
    }

    public Soldado soldadoProximo() {
        Soldado maisProximo = null;
        int menorDistancia = Integer.MAX_VALUE;

        for (Soldado soldado : Soldado.getTodasAsInstancias()) {
            // REMOVIDO: o "if (soldado == this)" que causava o erro

            int dist = soldado.calcularDistancia(this.x, soldado.getX());

            if (dist < menorDistancia) {
                menorDistancia = dist;
                maisProximo = soldado;
            }
        }

        return maisProximo;
    }

    public void atacar(int dano) {
        Soldado soldado = soldadoProximo();

        soldado.vida -= 20;
    }
}
