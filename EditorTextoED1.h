#ifndef EDITORTEXTOED1_H
#define EDITORTEXTOED1_H

typedef struct no{
    char c;
    NO *ant;
    NO *prox;
}NO;

typedef struct linha{
    NO *inicio;
    NO *fim;
    LINHA *cima;
    LINHA *baixo;
}LINHA;

typedef struct pagina{
    LINHA *inicio;
}PAGINA;

void iniciarPagina(PAGINA *pagina);
void primeiraLinha(LINHA *linha);
void novaLinha(LINHA *linha, LINHA *cima);
void inserirCaractereLinha(LINHA *linha, char caractere);
void removerCaractereLinha(LINHA *linha);
void inserirTexto();
void apagarTexto();


#include "EditorTextoED1.c"
#endif