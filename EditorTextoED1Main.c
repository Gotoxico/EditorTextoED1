#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
//#include <curses.h>
#include "EditorTextoED1.h"

int main(){
    //initscr();
    //cbreak();
    //keypad(stdscr, TRUE);e
    clearScreen();
    int x = 0, y= 0;
    gotoxy(x,y);
    char caractere;
    while((caractere = getch()) != 27){
        switch(caractere){
            case 13:
                pularLinha();
                break;
            case 8:
                apagarTexto();
                break;

            case 72:
                gotoxy(x, y+1);
                y++;
                break;

            case 80:
                gotoxy(x, y-1);
                y--;
                break;

            case 75:
                gotoxy(x-1, y);
                if(x-1 != -1){
                    x--;
                }
                break;

            case 77:
                gotoxy(x+1, y);
                if(x+1 != 80){
                    x++;
                }
                break;

            default:
                inserirTexto(caractere);
                break;
        }
        if(x == 80){
            x = 0;
        }
    }
    return 0;
}