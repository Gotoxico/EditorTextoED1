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

PAGINA * inicializar();
LINHA * inicializarLinha();
void novaLinha(LINHA *linha, LINHA *cima);
void inserirCaractereLinha(LINHA *linha, char caractere);
void removerCaractereLinha(LINHA *linha);
void clearScreen();
void moverLinhaCima(LINHA *linha);
void gotoxy(int x, int y);
void inserirTexto(char nomeArquivo[], char c);
void imprimirLista(LINHA * cima);
void salvarArquivo(char nomeArquivo[], PAGINA *pagina);
void apagar(PAGINA *pagina, int x, int y);
void abrirArquivo(char nomeArquivo[], PAGINA *pagina);
int getLarguraTerminal();
int Menu();


#include "EditorTextoED1.c"
#endif