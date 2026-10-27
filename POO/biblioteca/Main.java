package biblioteca;

public class Main {
    public static void main(String[] args) {
        Livro a = new Livro("Alice no país das maravilhas", "Lewis Carrol");
        Livro b = new Livro("Revolução dos bichos", "George Orwell");
        Livro c = new Livro("David Copperfield", "Charles Dickens");
        Livro d = new Livro("Código davinci", "Dan Brown");

        a.emprestar();
        a.emprestar();
        b.emprestar();
        c.emprestar();
        System.out.println(Livro.getTotalEmprestimos());
        System.out.println(Livro.getTotalLivros());

        a.devolver();

        
    }
}
