#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "EditorTextoED1.h"
#include <windows.h>


void iniciarPagina( PAGINA *pagina){
    pagina->inicio = NULL;
}

void primeiraLinha(LINHA *linha){
    linha->inicio = NULL;
    linha->fim = NULL;
    linha->cima = NULL;
    linha->baixo = NULL;
}

void novaLinha(LINHA *linha , LINHA *cima){
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

//Função para mover o cursor do teclado, mas não está em uso por enquanto
void gotoxy(int x, int y){
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// void pularLinha(LINHA linha){
    
// }

//A ideia e basicamente: Toda vez que for inserir ou apagar texto ler tudo do arquivo texto e mostrar no terminal. A inserção e remoção estão no final da página por enquanto

//Algumas partes foram comentadas para testar a inserção de texto no arquivo
void inserirTexto(char nomeArquivo[], char c){
    //clearScreen();
    FILE *file = fopen(nomeArquivo, "a+");
    char caractere;
    int contadorCaracteresLinha;
    PAGINA pagina;
    LINHA linha, anterior;

    fseek(file, 0, SEEK_SET);
    iniciarPagina(&pagina);
    primeiraLinha(&linha);
    
    // while((caractere = fgetc(file)) != EOF){
    //     if(contadorCaracteresLinha == 80){
    //         contadorCaracteresLinha = 0;
    //         anterior = linha;
    //         novaLinha(&linha, &anterior);
    //     }
    //     inserirCaractereLinha(&linha, caractere);
    //     putch(caractere);
    //     contadorCaracteresLinha++;
    // }

    // if(contadorCaracteresLinha != 0){
    //     inserirCaractereLinha(&linha, c);
    //     contadorCaracteresLinha++;
    // }

    // if(contadorCaracteresLinha == 80){
    //     contadorCaracteresLinha = 0;
    //     anterior = linha;
    //     novaLinha(&linha, &anterior);
    //     inserirCaractereLinha(&linha, c);
    //     contadorCaracteresLinha++;
    // }

    inserirCaractereLinha(&linha , c);
    putch(c);
    fputc(c, file);
    fclose(file);
}


//Função para apagar o texto do arquivo
void apagarTexto(char nomeArquivo[]){
    //clearScreen();
    FILE *file = fopen(nomeArquivo, "a+");
    char caractere;
    int contadorCaracteresLinha;
    PAGINA pagina;
    LINHA linha, anterior;

    fseek(file, 0, SEEK_SET);
    iniciarPagina(&pagina);
    primeiraLinha(&linha);
    
    while((caractere = fgetc(file)) != EOF){
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

//Função para imprimir a lista de caracteres, para testes
void imprimirLista(LINHA *linha){
    if(linha != NULL){
        NO *aux = linha->inicio;
        while(aux!=NULL){
            printf("%c", aux->c);
            aux = aux->prox;
        }
        printf("\n");
        linha = linha->baixo;
        imprimirLista(linha);
    }
}

// Menu de opções do editor
int Menu(){
    int opcao;
    printf("\tBem vindo ao Editor de Texto\n");
    printf("1- Inserir texto\n");
    printf("2- Abrir e Editar arquivo\n");
    printf("3- Sair\n");
    printf("Digite a opção desejada: ");

    scanf("%d", &opcao);

    return opcao;
}
