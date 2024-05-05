#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <curses.h>
#include "EditorTextoED1.h"

int main(){
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    char caractere;
    while((caractere = getch()) != 27){
        switch(caractere){
            case KEY_BACKSPACE:
                apagarTexto();
                break;

            case KEY_UP:
                gotoxy(wherex(), wherey()+1);
                break;

            case KEY_DOWN:
                gotoxy(wherex(), wherey()-1);
                break;

            case KEY_LEFT:
                gotoxy(wherex()-1, wherey());
                break;

            case KEY_RIGHT:
                gotoxy(wherex()+1, wherey());
                break;

            default:
                inserirTexto();
                break;
        }
    }
    return 0;
}