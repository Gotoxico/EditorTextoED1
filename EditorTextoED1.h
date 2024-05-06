#ifndef EDITORTEXTOED1_H
#define EDITORTEXTOED1_H

typedef struct no{
    char c;
    struct no *ant;
    struct no *prox;
}NO;

typedef struct linha{
    NO *inicio;
    NO *fim;
    struct linha *cima;
    struct linha *baixo;
}LINHA;

typedef struct pagina{
    LINHA *inicio;
}PAGINA;

void iniciarPagina(PAGINA *pagina);
void primeiraLinha(LINHA *linha);
void novaLinha(LINHA *linha, LINHA *cima);
void inserirCaractereLinha(LINHA *linha, char caractere);
void removerCaractereLinha(LINHA *linha);
void inserirTexto(char c);
void apagarTexto();
void pularLinha();
void clearScreen();
void moverLinhaCima(LINHA *linha);
void gotoxy(int x, int y);

#include "EditorTextoED1.c"
#endif