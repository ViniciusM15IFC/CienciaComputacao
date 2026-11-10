//  Desenvolva um sistema para uma livraria. Crie classes para representar ‘Livro‘, ‘Autor‘
// e ‘Editora‘, onde um livro é escrito por um autor e publicado por uma editora

package Ex01;

public class Main {
    
    public static void main(String[] args) {
        Autor autor1 = new Autor("Charles Dickens", "Reino Unido", "31/07/1812");
        Autor autor2 = new Autor("Machado de Assis", "Brasil", "21/06/1839");
        Editora editora1 = new Editora("Companhia das Letras", "Brasil", "01/01/1925");
        Livro livro1 = new Livro("Dom Casmurro", autor2, editora1, "01/01/1899", 300);
        Livro livro2 = new Livro("Memórias Póstumas de Brás Cubas", autor2, editora1, "01/01/1838", 400);
        Livro livro3 = new Livro("David Copperfield", autor1, editora1, "01/01/1850", 500);
        System.out.println(livro1);
        System.out.println(livro2);
        System.out.println(livro3);
    }
}
