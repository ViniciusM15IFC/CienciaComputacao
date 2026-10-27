package biblioteca;

public class Livro {
    private String titulo;
    private String autor;
    private int id;
    private boolean emprestado;
    private static int totalEmprestimos;
    private static int totalLivros;

    public Livro(String titulo, String autor)
    {
        this.titulo = titulo;
        this.autor = autor;
        totalLivros ++;
        this.id = totalLivros;
    }

    public boolean emprestar()
    {
        if(!emprestado)
        {
            emprestado = true;
            totalEmprestimos++;
            return true;
        }
        return false;
    }

    public void devolver()
    {
        if(emprestado)
        {
            emprestado = false;
        }
    }

    public static int getTotalLivros()
    {
        return totalLivros;
    }

    public static int getTotalEmprestimos()
    {
        return totalEmprestimos;
    }

}
