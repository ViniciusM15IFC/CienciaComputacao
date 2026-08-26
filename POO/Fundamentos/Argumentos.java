package Fundamentos;
public class Argumentos {
    public static void main(String[] args)
    {
        int soma = 0;
        for (String a: args)
        {
            try {
                soma += Integer.parseInt(a);
            } catch (Exception e) {
                //nada
            }
            
            System.out.println(a);
        }

        System.out.println("Soma dos valores numéricos: " + soma);
    }
}
