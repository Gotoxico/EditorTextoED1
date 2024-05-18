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
        if(c == -32){
            c = getch();
            if(c == 72){
                printf("\033[1A");
            }else if(c == 80){
                printf("\033[1B");
            }else if(c == 77){
                printf("\033[1C");
            }else if(c == 75){
                printf("\033[1D");
            }
        }else{

        if(c == 8){
            printf("\033[1M");
        }
        else if(c == 13){
            printf("\n");
        }else{
            printf("%c", c);
        }
    }

        

}
    return 0;
}
 