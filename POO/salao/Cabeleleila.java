package salao;

public class Cabeleleila {
    private String nomeFilial;
    private Cliente[] filaEspera;
    private int pos;

    public Cabeleleila(String nomeFilial)
    {
        this.nomeFilial = nomeFilial;
        this.filaEspera = new Cliente[10];
        pos = 0;
    }

    public void cadastrarCliente(String nome, char g)
    {
        if(this.pos < 10)
        {
            Cliente c = new Cliente(nome, g);
            this.filaEspera[pos] = c;
            pos++;
        }

        System.out.println("Fila cheia");
    }

    private void atualizarFila()
    {
        for (int i = 0; i < pos-1; i++) {
            filaEspera[i] = filaEspera[i+1];
        }
        this.pos--;
    }

    public boolean chamarCliente()
    {
        if(pos>0)
        {
            Cliente c = this.filaEspera[0];
            System.out.println(c.getNome());
            this.atualizarFila();
            return true;
        }
        return false;
    }

}
