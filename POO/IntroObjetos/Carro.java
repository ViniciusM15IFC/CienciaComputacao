package IntroObjetos;

import java.util.Scanner;

public class Carro {
    private String modelo;
    private String cor;
    private double velAtual;
    private int marcha = 1;
    private boolean ligado;
    private final double VELMAX = 200;

    // Fatores de ajuste para o comportamento da aceleração/frenagem
    private final double K_ACELERACAO = 0.05;
    private final double K_FRENAGEM = 0.08;

    public void ligarCarro() {
        this.ligado = true;
    }

    public void desligarCarro() {
        this.ligado = false;
    }

    public boolean isLigado() {
        return this.ligado;
    }

    public void acelerar(double tempoPedal) {

        if (this.ligado) {
            // Garante um pequeno empurrão inicial se o carro estiver totalmente parado
            // Garante que a velocidade não seja zero para o cálculo
            double vCalculo = (this.velAtual <= 0.1) ? 0.1 : this.velAtual;

            // --- Nova lógica de Marcha ---
            // Exemplo: assumindo que você tem uma variável 'this.marchaAtual' que vai de 1
            // a 6.
            // Criamos um fator que diminui a aceleração conforme a marcha sobe (Marcha 1 =
            // maior força/aceleração).
            // Ex: Se marcha for 1 -> (7 - 1) / 3.0 = 2.0x de aceleração. Se for 6 -> (7 -
            // 6) / 3.0 = 0.33x de aceleração.
            double fatorMarcha = (7.0 - this.marcha) / 3.0;

            // A variação agora inclui o fator da marcha multiplicando a aceleração base
            double variacao = K_ACELERACAO * fatorMarcha * vCalculo * (VELMAX - vCalculo) * tempoPedal;

            // Atualiza a velocidade sem deixar passar do limite máximo
            this.velAtual = Math.min(VELMAX, this.velAtual + variacao);

        }

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

    public int getMarcha() {
        return this.marcha;
    }

    public void reduzirMarcha() {
        this.marcha--;
    }

    public void aumentarMarcha() {
        this.marcha++;
    }

    public double getVelAtual() {
        return this.velAtual;
    }

    public static void main(String[] args) {
        Carro fusca = new Carro();
        Scanner s = new Scanner(System.in);

        while (true) {
            System.out.println("Fusca: ");
            System.out.println("Ligado " + fusca.isLigado());
            System.out.println("Velocidade: " + fusca.getVelAtual());
            System.out.println("Marcha: " + fusca.getMarcha());
            System.out.println("Sua próxima ação: ");
            System.out.println("1 - Acelerar");
            System.out.println("2 - Freiar");
            System.out.println("3 - Aumentar marcha");
            System.out.println("4 - Reduzir marcha");
            System.out.println("5 - Ligar/Desligar");
            int op = s.nextInt();

            switch (op) {
                case 1:
                    fusca.acelerar(30);
                    break;
                case 2:
                    fusca.frear(30);
                    break;
                case 3:
                    fusca.aumentarMarcha();
                    break;    
                case 4:
                    fusca.reduzirMarcha();
                    break;
                case 5:
                    if(fusca.isLigado())
                    {
                        fusca.desligarCarro();
                    }
                    else
                    {
                        fusca.ligarCarro();
                    }
                    break;
                default:
                    break;
            }

            if(op == -1)
            {
                break;
            }
        }

        s.close();

    }
}
