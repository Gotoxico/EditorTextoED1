#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "EditorTextoED1.h"

void iniciarPagina(PAGINA *pagina){
    pagina->inicio = NULL;
}

void primeiraLinha(LINHA *linha){
    linha->inicio = NULL;
    linha->fim = NULL;
    linha->cima = NULL;
    linha->baixo = NULL;
}

void novaLinha(LINHA *linha, LINHA *cima){
    linha->inicio = NULL;
    linha->fim = NULL;
    linha->cima = cima;
    linha->baixo = NULL;
}

void inserirCaractereLinha(LINHA *linha, char caractere){
    NO *novoNo = (NO*) malloc(sizeof(NO));
    novoNo->c = caractere;
    novoNo->prox = NULL;

    if(linha->inicio == NULL){
        linha->inicio = novoNo;
        novoNo->ant = NULL;
    }
    else{
        novoNo->ant = linha->fim;
        linha->fim = novoNo;
    }
}

void removerCaractereLinha(LINHA *linha){
    if(linha->fim == NULL){
        return;
    }
    NO *aux = linha->fim;
    linha->fim = linha->fim->ant;
    free(aux);
}

void clearScreen(){
    printf("\033[2J\033[H");  
}

void moverLinhaCima(LINHA *linha){
    LINHA *aux = linha;
    linha = linha->cima;
    free(aux);
}


//A ideia e basicamente: Toda vez que for inserir ou apagar texto ler tudo do arquivo texto e mostrar no terminal. A inserção e remoção estão no final da página por enquanto

void inserirTexto(){
    clearScreen();
    FILE *file = fopen("EditorTextoED1Arquivo.txt", "a+");
    char caractere;
    int contadorCaracteresLinha;
    PAGINA pagina;
    LINHA linha, anterior;

    fseek(file, 0, SEEK_SET);
    iniciarPagina(&pagina);
    primeiraLinha(&linha);
    
    while(caractere = fgetc(file) != NULL){
        if(contadorCaracteresLinha == 80){
            contadorCaracteresLinha = 0;
            anterior = linha;
            novaLinha(&linha, &anterior);
        }
        inserirCaractereLinha(&linha, caractere);
        putch(caractere);
        contadorCaracteresLinha++;
    }

    if(contadorCaracteresLinha != 0){
        inserirCaractereLinha(&linha, caractere);
        contadorCaracteresLinha++;
    }

    if(contadorCaracteresLinha == 80){
        contadorCaracteresLinha = 0;
        anterior = linha;
        novaLinha(&linha, &anterior);
    }

    putch(caractere);
    fputc(caractere, file);
}

void apagarTexto(){
    clearScreen();
    FILE *file = fopen("EditorTextoED1Arquivo.txt", "a+");
    char caractere;
    int contadorCaracteresLinha;
    PAGINA pagina;
    LINHA linha, anterior;

    fseek(file, 0, SEEK_SET);
    iniciarPagina(&pagina);
    primeiraLinha(&linha);
    
    while(caractere = fgetc(file) != NULL){
        if(contadorCaracteresLinha == 80){
            contadorCaracteresLinha = 0;
            anterior = linha;
            novaLinha(&linha, &anterior);
        }
        inserirCaractereLinha(&linha, caractere);
        putch(caractere);
        contadorCaracteresLinha++;
    }

    if(contadorCaracteresLinha != 0){
        removerCaractereLinha(&linha);
        contadorCaracteresLinha--;
    }

    if(contadorCaracteresLinha == 0){
        moverLinhaCima(&linha);
    }

    fseek(file, -1, SEEK_END);
    fputc(32, file);
}