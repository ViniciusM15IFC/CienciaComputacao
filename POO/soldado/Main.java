package soldado;

public class Main {
    public static void main(String[] args) {
        Inimigo i = new Inimigo(100, 200);

        Soldado a = new Soldado();
        Soldado b = new Soldado();
        Soldado c = new Soldado();
        Soldado d = new Soldado();
        Soldado e = new Soldado();
        Soldado f = new Soldado();
        Soldado g = new Soldado();
        Soldado h = new Soldado();
        Soldado j = new Soldado();
        Soldado k = new Soldado();
        Soldado l = new Soldado();
        Soldado m = new Soldado();

        a.deslocar(190);
        a.atacar(i, "baioneta");

        System.out.println("Vida do inimigo: " + i.getVida());
    }
}
