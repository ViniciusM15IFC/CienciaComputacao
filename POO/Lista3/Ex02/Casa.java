package Ex02;

import java.util.List;
import java.util.ArrayList;

public class Casa {
    private double area;
    private int numero;
    private List<Comodo> comodos;

    public Casa(int numero, double area) {
        this.numero = numero;
        this.area = area;
        comodos = new ArrayList<Comodo>();
    }

    public Casa(int numero, double area, Comodo comodo1) {
        this.numero = numero;
        this.area = area;
        comodos = new ArrayList<Comodo>();
        comodos.add(comodo1);
    }

    public double getAreaOcupada() {
        double total = 0;

        for (Comodo comodo : comodos) {
            total += comodo.getArea();
        }

        return total;
    }

    public Comodo buscarComodo(String busca) {
        Comodo resultado = null; 
        for (Comodo comodo : comodos) {
            if(comodo.getNome() == busca)
            {
                resultado = comodo;
            }
        }
        return resultado;
    }

    public boolean adicionarComodo(String nome, double areaComodo) {
        if (getAreaOcupada() + areaComodo <= this.area) {
            Comodo c = new Comodo(nome, areaComodo);
            comodos.add(c);
            return true;
        }
        return false;
    }

    public boolean removerComodo(String nome)
    {
        Comodo c = this.buscarComodo(nome);

        if(c == null)
        {
            return false;
        }

        this.comodos.remove(c);
        return true;
    }

    public boolean adicionarMovel(String nome, Moveis... moveis)
    {
        Comodo c = buscarComodo(nome);

        if(c == null)
        {
            return false;
        }

        c.adicionarMovel(moveis);
        return true;
    }
    public boolean removerMovel(String nome, Moveis... moveis)
    {
        Comodo c = buscarComodo(nome);

        if(c == null)
        {
            return false;
        }

        c.removerMovel(moveis);
        return true;
    }

    public String toString()
    {
        return "Número da casa: " + numero + "\n" +
                "Área total: " + area + "\n" +
                "Área ocupada: " + getAreaOcupada() + "\n" +
                "Comodos: " + comodos;
    }

}
