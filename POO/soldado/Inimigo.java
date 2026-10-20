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
        // Inicializa como null para evitar erro de compilação
        Soldado maisProximo = null;
        // Usa o maior valor possível do Java em vez de 9999
        int menorDistancia = Integer.MAX_VALUE;

        for (Soldado soldado : Soldado.getTodasAsInstancias()) {
            // Ignora a si mesmo se o objeto chamador também for um Soldado
            if (soldado == this)
                continue;

            int dist = soldado.calcularDistancia(this.x, soldado.getX());

            if (dist < menorDistancia) {
                menorDistancia = dist;
                maisProximo = soldado;
            }
        }

        return maisProximo; // Pode retornar null se não houver nenhum outro soldado
    }

    public void atacar() {

    }
}
