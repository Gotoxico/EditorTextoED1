//Prgorama que implemente recebe entrada do teclado sem apertar enter
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
int main(){
    int c ;
    FILE * file = fopen("teste3.txt", "r");
    if(file == NULL){
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }
    
    while((c != 27)){
        c = getch();
        if(c <= 127){
            
            // printf("%c", c);
            printf(", %d\n", c);
            // fputc(c, file);

        }else if(c >= 192 && c <= 223){
            // printf("%c", c);
            printf(", %d", c);
            // fputc(c, file);

            c = getch();
            // printf("%c", c);
            printf(", %d\n", c);
            // fputc(c, file);

        }else if(c >= 224 && c <= 239){
            // printf("%c", c);
            printf(", %d", c);
            // fputc(c, file);

            c = getch();
            // fputc(c, file);
            // printf("%c", c);
            printf(", %d", c);

            c = getch();
            // fputc(c, file);
            // printf("%c", c);
            printf(", %d\n", c);

        }else if(c >= 240 && c <= 247){
            // printf("%c", c);
            printf(", %d", c);
            // fputc(c, file);

            c = getch();
            // printf("%c", c);
            printf(", %d", c);
            // fputc(c, file);

            c = getch();
            // fputc(c, file);
            // printf("%c", c);
            printf(", %d", c);

            c = getch();
            // fputc(c, file);
            // printf("%c", c);
            printf(", %d\n", c);
        }
        printf("\033[2S");
    }

    // while((c = fgetc(file)) != EOF){
       
    //     if(c <= 127){
            
    //         printf("%c", c);
    //         // printf(", %d\n", c);
    //         // fputc(c, file);

    //     }else if(c >= 192 && c <= 223){
    //         printf("%c", c);
    //         // printf(", %d", c);
    //         // fputc(c, file);

    //         c = fgetc(file);
    //         printf("%c", c);
    //         // printf(", %d\n", c);
    //         // fputc(c, file);

    //     }else if(c >= 224 && c <= 239){
    //         printf("%c", c);
    //         // printf(", %d", c);
    //         // fputc(c, file);

    //         c = fgetc(file);
    //         // fputc(c, file);
    //         printf("%c", c);
    //         // printf(", %d", c);

    //         c = fgetc(file);
    //         // fputc(c, file);
    //         printf("%c", c);
    //         // printf(", %d\n", c);

    //     }else if(c >= 240 && c <= 247){
    //         printf("%c", c);
    //         // printf(", %d", c);
    //         // fputc(c, file);

    //         c = fgetc(file);
    //         printf("%c", c);
    //         // printf(", %d", c);
    //         // fputc(c, file);

    //         c = fgetc(file);
    //         // fputc(c, file);
    //         printf("%c", c);
    //         // printf(", %d", c);

    //         c = fgetc(file);
    //         // fputc(c, file);
    //         printf("%c", c);
    //         // printf(", %d\n", c);

    //     }
    // }
    fclose(file);
}
 