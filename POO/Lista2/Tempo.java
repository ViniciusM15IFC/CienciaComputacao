//  Crie uma classe para representar tempo em termos de hora, minutos e segundos
// – Com 4 métodos construtores
// – Uso adequado dos modificadores de acesso
// – Se valores inválidos forem informados para hora, minuto ou segundo, então
// deve-se criar objeto com 00:00:00
// • Método chamado toString() que deverá retornar o tempo como String. Ex:
// 07:30:00
// • Métodos para alterar hora, minuto ou segundo
// – Deve-se retornar true se o valor for alterado com sucesso, false caso contrário
// • Método que retorne um long com o tempo em segundos. Ex: 01:00:00 retornaria:
// 3600
// • Método que receba objeto Tempo como parâmetro e retorne um long com a diferença entre este e o tempo do objeto em questão

package Lista2;

public class Tempo {
    private int hora;
    private int minuto;
    private int segundo;

    public Tempo() {
        this.hora = 0;
        this.minuto = 0;
        this.segundo = 0;
    }

    public boolean setHora(int hora) {
        if (hora < 0 || hora > 23) {
            return false;
        } else {
            this.hora = hora;
            return true;
        }
    }

    public boolean setMinuto(int minuto) {
        if (minuto < 0 || minuto > 59) {
            return false;
        } else {
            this.minuto = minuto;
            return true;
        }
    }

    public boolean setSegundo(int segundo) {
        if (segundo < 0 || segundo > 59) {
            return false;
        } else {
            this.segundo = segundo;
            return true;
        }
    }

    public Tempo(int hora, int minuto, int segundo) {
        if (!setHora(hora) || !setMinuto(minuto) || !setSegundo(segundo)) {
            this.hora = 0;
            this.minuto = 0;
            this.segundo = 0;
        }
    }

    public Tempo(int hora, int minuto) {
        if (!setHora(hora) || !setMinuto(minuto)) {
            this.hora = 0;
            this.minuto = 0;
        }
        this.segundo = 0;
    }

    public Tempo(int hora) {
        if (!setHora(hora)) {
            this.hora = 0;
        }
        this.minuto = 0;
        this.segundo = 0;
    }

    public String toString() {
        return String.format("%02d:%02d:%02d", hora, minuto, segundo);
    }

    public long tempoEmSegundos() {
        return (hora * 3600) + (minuto * 60) + segundo;
    }

    public long diferencaEmSegundos(Tempo outro) {
        long dif = Math.abs(this.tempoEmSegundos() - outro.tempoEmSegundos());
        if (dif < 0) {
            dif *= -1;
        }

        return dif;
    }

}
