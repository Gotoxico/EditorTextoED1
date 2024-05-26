#ifndef EDITORTEXTOED1_H
#define EDITORTEXTOED1_H

typedef struct no{
    unsigned char c;
    struct no *ant;
    struct no *prox;
}NO;

typedef struct bytes{
    NO *inicio;
    NO *fim;
    struct bytes * prox;
    struct bytes * ant;
}BYTES;

typedef struct linha{
    BYTES *inicio;
    BYTES *fim;
    struct linha *cima;
    struct linha *baixo;
}LINHA;

typedef struct pagina{
    LINHA *inicio;
}PAGINA;

PAGINA * inicializar();
LINHA * inicializarLinha();
BYTES * inicializarBytes();
BYTES * recuperarInicio(LINHA *linha);
void inserirBytes(BYTES *bytes, unsigned char caractere);
void novaLinha(LINHA *linha, LINHA *cima);
void inserirCaractereLinha(LINHA *linha,BYTES * byte, int * posicaoFinalEscrita, int posicaoAtualColuna);
void removerCaractereLinha(LINHA *linha);
void clearScreen();
void moverLinhaCima(LINHA *linha);
void gotoxy(int x, int y);
void inserirTexto(char nomeArquivo[], char c);
void imprimirLista(LINHA * linha);
void salvarArquivo(char nomeArquivo[], PAGINA *pagina);
void apagar(PAGINA *pagina, int x, int y);
void abrirArquivo( LINHA *linha, char nomeArquivo[], int *posicaoAtualLinha);
int getLarguraTerminal();
void RecuperarPosicaoFinal(PAGINA *pagina, int posicaoAtualLinha, int *posicaoFinalEscrita);
void inserir(LINHA * linha, BYTES * byte);
int Menu();
LINHA * Apontamento(PAGINA *pagina,  int posicaoAtualLinha);
LINHA * Reapontar(PAGINA * pagina, int posicaoAtualLinha, int posicaoAtualColuna, int *posicaoFinalEscrita);
 void DeslocarLinha(LINHA * linha);
int is_cursor_on_last_line();

#include "EditorTextoED1.c"
#endif