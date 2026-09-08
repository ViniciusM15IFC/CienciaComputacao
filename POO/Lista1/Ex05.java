// 5. Em um campeonato de futebol existem cinco times e cada um possui 11 jogadores.
// Faça um programa que receba a idade, o peso e a altura de cada um dos jogadores,
// calcule e mostre:
// • a quantidade de jogadores com idade inferior a 18 anos;
// • a média das idades dos jogadores de cada time;
// • a média das alturas de todos os jogadores do campeonato;
// • a porcentagem de jogadores com mais de 80kg entre todos os jogadores do cam-
// peonato.

package Lista1;

import java.util.Scanner;

public class Ex05 {
    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);

        int nTimes = 2;
        int nJogadores = 3;

        int contMenores = 0;
        int cont80 = 0;
        double somaAlturas = 0;
        double[] somaIdades = new double[nTimes];

        for(int i = 0; i < nTimes; i++)
        {   somaIdades[i] = 0;
            System.out.println("\n===Time " + (i+1) + "===\n");
            for (int j = 0; j < nJogadores; j++)
            {
                System.out.printf("\nDados do Jogador %d do time %d\n", (j+1), (i+1));

                System.out.println("Digite a idade: ");
                int idade = s.nextInt();

                System.out.println("Digite o peso: ");
                double peso = s.nextDouble();
                
                System.out.println("Digite a altura: ");
                double altura = s.nextDouble();

                somaIdades[i] += idade;
                somaAlturas += altura;

                if(idade < 18)
                {
                    contMenores ++;
                }

                if(peso > 80)
                {
                    cont80 ++;
                }
            }
        }

        System.out.println("Jogadores com idade menor que 18: " + contMenores);
        System.out.println("Média de idade de cada time:");
        for (int i = 0; i < nTimes; i++)
        {
            System.out.printf("    Time %d: %.2f\n", (i+1), ((double)somaIdades[i]/nJogadores));
        }
        System.out.println("Média de altura do campeonato: " + (somaAlturas/(nJogadores*nTimes)));
        System.out.println("Porcentagem de jogadores com mais de 80kg: " + ((cont80/(nJogadores*nTimes))*100));
    }
}
