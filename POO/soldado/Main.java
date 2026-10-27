package soldado;

public class Main {
    public static void main(String[] args) {
        Inimigo i = new Inimigo(100, 200);

        Soldado a = new Soldado("Abelardo");
        Soldado b = new Soldado("Bernardo");
        Soldado c = new Soldado("Chester Bernington");
        Soldado d = new Soldado("Dadinho");
        Soldado e = new Soldado("Elias");
        Soldado f = new Soldado("Flávio");
        Soldado g = new Soldado("Gargamel");
        Soldado h = new Soldado("Hebert Richards");
        Soldado j = new Soldado("Jazzghost");
        Soldado k = new Soldado("Padre Kelmon");
        Soldado l = new Soldado("Lula");
        Soldado m = new Soldado("Maya");

        a.deslocar(190);
        a.atacar(i, "baioneta");

        System.out.println("Vida do inimigo: " + i.getVida());
        System.out.println("Vida do soldado A: " + a.getVida());
        a.atacar(i);
    }
}
