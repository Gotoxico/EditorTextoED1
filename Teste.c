#include <stdio.h>
#include <conio.h>

int main(){
    char c;
    while(c != 27){
        c = getch();
        printf("%c : %d", c, c);
        printf("\n");
    }
}
