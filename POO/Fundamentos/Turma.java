package Fundamentos;

import java.util.Scanner;

public class Turma {
    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);

        float[] notas = new float[3];
        float maior = 0, menor = 0, media, soma = 0;

        for (int i = 0; i < notas.length; i++) {
            System.out.print("Digite uma nota: ");
            notas[i] = s.nextFloat();

            if (notas[i] > maior)
            {
                maior = notas[i];
            }

            if(notas[i] < menor || i == 0)
            {
                menor = notas[i];
            }

            soma += notas[i];
        }

        media = soma/notas.length;

        System.out.println("Maior nota: " + maior);
        System.out.println("Menor nota: " + menor);
        System.out.println("Media: " + media);
    }
}
