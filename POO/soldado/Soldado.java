package soldado;

import java.util.ArrayList;
import java.util.List;

class Soldado {
    private String nome;
    private int x = 0;
    private static int totalSoldados = 0;
    private static final List<Soldado> todasAsInstancias = new ArrayList<>();
    private int vida = 20;
    private boolean vivo = true;

    public Soldado(String nome) {
        this.nome = nome;
        totalSoldados++;
        todasAsInstancias.add(this);
    }

    public void deslocar(int n) {
        if (vivo) {
            this.x += n;
            System.out.println("Posição atual: " + this.x);
        } else {
            System.out.println(this.nome + " está morto");
        }
    }

    public void deslocar() {
        if (vivo) {
            this.x += 20;
            System.out.println("Posição atual: " + this.x);
        } else {
            System.out.println(this.nome + " está morto");
        }
    }

    public void atacar(String arma) {
        System.out.println("Escolha um inimigo");
    }

    public int getX() {
        return this.x;
    }

    public void sofreDano(int dano) {
        this.vida -= dano;
    }

    public int getVida() {
        return this.vida;
    }

    public int calcularDistancia(int n1, int n2) {
        int dist = n1 - n2;
        if (dist < 0) {
            dist *= -1;
        }

        return dist;
    }

    public void atacar(Inimigo i, String arma) {
        if (vivo) {
            int dano = 0;
            int dist = calcularDistancia(i.getX(), this.x);

            if (Soldado.totalSoldados >= 10) {
                if (arma == "fuzil") {
                    if (dist <= 20) {
                        dano = 20;
                    } else {
                        System.out.println("Você está longe demais para atacar");
                    }

                } else if (arma == "baioneta") {
                    if (dist <= 10) {
                        dano = 15;
                    } else {
                        System.out.println("Você está longe demais para atacar");
                    }
                } else {
                    if (dist <= 5) {
                        dano = 5;
                    } else {
                        System.out.println("Você está longe demais para atacar");
                    }
                    arma = "punho";
                }

                if (dano != 0) {
                    i.sofreDano(dano);
                    System.out.println(
                            "Ataque feito com " + arma + ", por " + this.nome + ", " + dano + " de dano infrigido");
                }
                System.out.println("O inimigo atacou de volta o soldado " + i.atacar(20).getNome());
            } else {
                System.out.println("Não há soldados suficientes para começar um ataque");
            }
        } else {
            System.out.println(this.nome + " está morto");
        }

    }

    public void atacar(Inimigo i) {
        if (vivo) {
            if (Soldado.totalSoldados >= 10) {
                int dist = calcularDistancia(i.getX(), this.x);
                if (dist <= 20) {
                    i.sofreDano(20);
                    System.out.println("Ataque feito com fuzil , 20 de dano infrigido");

                    System.out.println("O inimigo atacou de volta o soldado " + i.atacar(20).getNome());
                } else {
                    System.out.println("Você está longe demais para atacar");
                }
            } else {
                System.out.println("Não há soldados suficientes para começar um ataque");
            }
        } else {
            System.out.println(this.nome + " está morto");
        }

    }

    public static List<Soldado> getTodasAsInstancias() {
        return new ArrayList<>(todasAsInstancias);
    }

    public void sofrerDano(int dano) {
        this.vida -= dano;
        if (this.vida <= 0) {
            vivo = false;
            todasAsInstancias.remove(this);
            totalSoldados--;
        }
    }

    public String getNome() {
        return this.nome;
    }

}