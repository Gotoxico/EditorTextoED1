//Prgorama que implemente recebe entrada do teclado sem apertar enter
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

int main(){
    char c;
    int contador = 0;
    while(1){
        c = getch();
        if(c == 27){
            break;
        }
        printf("%c", c);
        contador++;
        if(contador % 10 == 0){
            for(int i = 0; i < 5; i++){
                printf("\b");
                printf(" ");
                printf("\033[1D");
            }   
        }
    }

    return 0;
}
    
 