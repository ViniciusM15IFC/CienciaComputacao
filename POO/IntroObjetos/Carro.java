package IntroObjetos;

public class Carro {
    private String modelo;
    private String cor;
    private double velAtual;
    private final double VELMAX = 200;

    // Fatores de ajuste para o comportamento da aceleração/frenagem
    private final double K_ACELERACAO = 0.05;
    private final double K_FRENAGEM = 0.08;

    public void acelerar(double tempoPedal) {
        // Garante um pequeno empurrão inicial se o carro estiver totalmente parado
        double vCalculo = (this.velAtual <= 0.1) ? 0.1 : this.velAtual;

        // A variação depende da velocidade atual, de quanto falta para a máxima e do
        // tempo de pedal
        double variacao = K_ACELERACAO * vCalculo * (VELMAX - vCalculo) * tempoPedal;

        // Atualiza a velocidade sem deixar passar do limite máximo
        this.velAtual = Math.min(VELMAX, this.velAtual + variacao);
    }

    public void frear(double tempoPedal) {
        // Se já estiver parado, não faz nada
        if (this.velAtual <= 0)
            return;

        // A variação na frenagem depende da velocidade atual e do tempo de pedal
        double variacao = K_FRENAGEM * this.velAtual * (VELMAX - this.velAtual + 10) * tempoPedal;

        // Atualiza a velocidade sem deixar ficar negativa
        this.velAtual = Math.max(0, this.velAtual - variacao);
    }

    // Getter para você conseguir ler a velocidade atual depois
    public double getVelAtual() {
        return this.velAtual;
    }

    public static void main(String[] args) {
        Carro fusca = new Carro();

        fusca.acelerar(60);
        System.out.println(fusca.getVelAtual());
        
    }
}
