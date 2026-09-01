/* Leia os 03 ângulos e os 03 lados de um triângulo, e classifique-o como: retângulo,
obtusângulo ou acutângulo; e equilátero, escaleno ou isósceles. */
package Lista1;

import java.util.Scanner;

public class Ex03 {

    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);

        float[] lados = new float[3];
        float[] angulos = new float[3];

        for (int i = 0; i < lados.length; i++) {
            System.out.println("Digite o tamanho do lado " + (i + 1));
            lados[i] = s.nextFloat();

            System.out.println("Digite o tamanho do ângulo " + (i + 1));
            angulos[i] = s.nextFloat();
        }

        float soma = 0;
        for (float angulo : angulos) {
            soma += angulo;
        }

        if(soma != 180){
            System.out.println("Ângulos inválidos");
            return;
        }

        
        float maiorAngulo = 0;
        for(float angulo : angulos)
        {
            if(angulo > maiorAngulo)
            {
                maiorAngulo = angulo;
            }
        }

        

        // Cria um conjunto para guardar os floats únicos
        java.util.Set<Float> conjuntoLados = new java.util.HashSet<>();

        // Passa cada lado do seu array float[] para dentro do conjunto
        for (float lado : lados) {
            conjuntoLados.add(lado);
        }

        // Agora você tem a quantidade exata de lados diferentes!
        int ladosDif = conjuntoLados.size();

        if (ladosDif == 1) {
            System.out.println("Equilátero");
        } else if (ladosDif == 2) {
            System.out.println("Isósceles");
        } else if (ladosDif == 3) {
            System.out.println("Escaleno");
        }

        float maiorAngulo = 0;
        for(float angulo : angulos)
        {
            if(angulo > maiorAngulo)
            {
                maiorAngulo = angulo;
            }
        }

        if(maiorAngulo == 90)
        {
            System.out.println("Retângulo");
        }
        else if(maiorAngulo > 90)
        {
            System.out.println("Obtusângulo");
        }
        else
        {
            System.out.println("Acutângulo");
        }
    }

}
