#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
//Programas que lê o teclado sem precisar de enter até que a tecla ESC seja pressionada. Além disso, ele deve ser capaz de mover o curso do teclado com
int main(){
    char c;
    char caractere, auxiliar;
    while(c != 27){
        c = getch();
        printf("%c : %d", c, c);
        printf("\n");
    }

    // FILE *arquivo;
    // arquivo = fopen("teste.txt", "r");

    // if(arquivo == NULL){
    //     printf("Erro ao abrir o arquivo");
    //     return 0;
    // }

    // while((c = fgetc(arquivo)) != EOF){
    //     printf("%c", c);
    // }
    // fclose(arquivo);

    // arquivo = fopen("teste.txt", "a+");

    // while(1){
    //     caractere = getch();

    //     switch(caractere){
            
    //         case -32:
    //             auxiliar = getch();
    //             switch(auxiliar){
    //                 case 72:
    //                     // moverLinhaCima(linhaAtual);
    //                     printf("\033[1A");
    //                     break;
    //                 case 80:
    //                     // moverLinhaBaixo(linhaAtual);
    //                     printf("\033[1B");
    //                     break;
    //                 case 75:
    //                     // moverCaractereEsquerda(linhaAtual);
    //                     printf("\033[1D");
    //                     break;
    //                 case 77:
    //                     // moverCaractereDireita(linhaAtual);
    //                     printf("\033[1C");
    //                     break;

    //                 default:
                        
    //                 break;
    //             }
    //         break;
            
    //         case 27:
    //                 return 0;
    //             break;

    //         default:
    //             fputc(caractere, arquivo);
    //             break;
    //     }
      
    // }
    // fclose(arquivo);
}
