package investimento;

public class Cliente {
    private String nome, cpf;
    private int saldo;
    private static int totalClientes = 0;
    private static final int QUANT_AUMENTO = 10;
    private static final double DTAXA = 0.1;
    private static double taxa = 0.5;

    public Cliente(String nome, String cpf)
    {
        this.nome = nome;
        this.cpf = cpf;
        this.saldo = 0;
        totalClientes ++;
        variarTaxa();
    }

    public void variarTaxa()
    {
        if(this.totalClientes % this.QUANT_AUMENTO == 0 && this.totalClientes > 0)
        {
            taxa += DTAXA;
        }
    }

    public void variarSaldo()
    {
        this.saldo *= 1+(taxa/100);
    }

    public void depositar(double valor)
    {
        this.saldo -= valor;
    }

    public String toString()
    {
        return "Nome: " + this.nome + ", CPF" + this.cpf + ", Saldo: " + this.saldo;
    }




    

}
