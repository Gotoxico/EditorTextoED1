#include <stdio.h>
#include <conio.h>
#include "EditorTextoED1.h"

int main(){
    char caractere;
    while(caractere = getch() != 27){
        switch(caractere){
            case 8:
                apagarTexto();
                break;

            case 170 || 234:
                gotoxy(wherex(), wherey()+1);
                break;

            case 171 || 235:
                gotoxy(wherex(), wherey()-1);
                break;

            case 148 || 212:
                gotoxy(wherex()-1, wherey());
                break;

            case 151 || 215:
                gotoxy(wherex+1, wherey());
                break;

            default:
                inserirTexto();
                break;
        }
    }
}