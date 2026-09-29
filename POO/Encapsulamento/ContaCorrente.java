package Encapsulamento;

import java.util.Random;

class ContaCorrente
{
    private String nome;
    private String cpf;
    private String telefone;
    private int agencia;
    private int numeroConta;
    private double saldo;

    public ContaCorrente(String nome, String cpf)
    {
        this.nome = nome;
        this.cpf = cpf;
        this.telefone = "11111111111";
        this.agencia = 100;
        this.saldo = 0;
        this.numeroConta = gerarNumero();
    }

    public ContaCorrente(String nome, String cpf, String telefone)
    {
        this.nome = nome;
        this.cpf = cpf;
        this.telefone = telefone;
        this.agencia = 100;
        this.saldo = 0;
    }   

    private int gerarNumero()
    {
        Random r = new Random();

        return r.nextInt(99999);
    }

    public String getNome()
    {
        return this.nome;
    }

    public String getCpf()
    {
        return this.cpf.substring(0, 3) + ".***.***-" + this.cpf.substring(9, 11);
    }

    public String getTelefone()
    {
        String t = this.telefone;
        return "(" + t.substring(0, 2) + ") "+ t.substring(2, 7) + "-" + t.substring(7, 11);
    }

    public int getAgencia()
    {
        return this.agencia;
    }

    public void depositar(double valor)
    {
        this.saldo += valor;
    }

    public boolean sacar(double valor)
    {
        if(valor <= this.saldo)
        {
            saldo -= valor;
            return true;
        }
        return false; 
    }
}