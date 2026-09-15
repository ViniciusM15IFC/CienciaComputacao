package IntroObjetos;


public class Pessoa {
    private String nome;
    private char sexo;
    private int anoNasc;

    public void definirNome(String texto)
    {
        this.nome = texto;
    }

    public void definirSexo(char c)
    {
        sexo = c;
    }

    public void definirAno(int ano)
    {
        anoNasc = ano;
    }

    public void cumprimentar()
    {
        if (sexo == 'M')
        {
            System.out.println("Olá "+ ", seja bem-vindo senhor");
        }
        else if (sexo == 'F')
        {
            System.out.println("Olá "+ ", seja bem-vinda senhorita");
        }
        else
        {
            System.out.println("Olá "+ ", seja bem-vinde");
        }
    }

    public int calcularIdade()
    {
        return 2026 - anoNasc;
    }
}
