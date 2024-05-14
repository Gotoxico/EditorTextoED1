#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "EditorTextoED1.h"
#include <windows.h>

 PAGINA * inicializar(){
    PAGINA *pagina = (PAGINA*) malloc(sizeof(PAGINA));
    pagina->inicio = NULL;
    return pagina;
 }

LINHA * inicializarLinha(){
    LINHA *linha = (LINHA*) malloc(sizeof(LINHA));
    linha->inicio = NULL;
    linha->fim = NULL;
    linha->cima = NULL;
    linha->baixo = NULL;
    return linha;
}

void novaLinha(LINHA *linha , LINHA *cima){
    linha->inicio = NULL;
    linha->fim = NULL;
    linha->cima = cima;
    linha->baixo = NULL;
    cima->baixo = linha;
}

//função para inserir um caractere na linha da página
void inserirCaractereLinha(LINHA *linha, char caractere, int * posicaoFinalEscrita, int *posicaoAtualColuna){
    NO *novo = (NO*) malloc(sizeof(NO));
    novo->c = caractere;
    novo->prox = NULL;
    novo->ant = NULL;
    NO *aux;
    if(linha->inicio == NULL){
        linha->inicio = novo;
        linha->fim = novo;
    }else{
        if((*posicaoAtualColuna) == (*posicaoFinalEscrita)){
    
            novo->ant = linha->fim;
            linha->fim->prox = novo;
            linha->fim = novo;
        }else if((*posicaoFinalEscrita) > (*posicaoAtualColuna)){
             aux = linha->inicio;
            int i = 0;
            while(aux != NULL && i < (*posicaoAtualColuna)+1){
                aux = aux->prox;
                i++;
            }
            novo->prox = aux;
            novo->ant = aux->ant;
            if (aux->ant != NULL) {
                aux->ant->prox = novo;
            } else {
                linha->inicio = novo;
            }
            aux->ant = novo;
        }
    }
    (*posicaoFinalEscrita)++;
    
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

//Algumas partes foram comentadas para testar a inserção de texto no arquivo
void inserirTexto(char nomeArquivo[], char c){
    FILE *file = fopen(nomeArquivo, "a+");
    fseek(file, 0, SEEK_SET);
    putch(c);
    fputc(c, file);
    fclose(file);
}

//Função para imprimir a lista de caracteres, para testes
void imprimirLista(LINHA *linha){
    LINHA *aux = linha;    
    NO *aux1;
    while(aux != NULL){
        aux1 = aux->inicio;
        while(aux1!=NULL){
            printf("%c", aux1->c);
            aux1 = aux1->prox;
        }
        printf("\n");
        aux = aux->baixo;
    }
}

//Função para salvar o arquivo
void salvarArquivo(char nomeArquivo[], PAGINA *pagina){
    FILE *arquivo = fopen(nomeArquivo, "w");
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo");
        return;
    }

    LINHA *linha = pagina->inicio;
    NO *caractere;
    while(linha != NULL){
        caractere = linha->inicio;
        while(caractere != NULL){
            fputc(caractere->c, arquivo);
            caractere = caractere->prox;
        }
        linha = linha->baixo;
        if(linha != NULL) fputc('\n', arquivo);
    }
    fclose(arquivo);
}

//Função para apagar um caractere da página
void apagar(PAGINA * pagina, int x, int y){
    
    LINHA *linha = pagina->inicio;
    NO *caractere;
    int contador = 0;
    while(linha != NULL && contador < y){
        linha = linha->baixo;
        contador++;
    }
    contador = 0;
    caractere = linha->inicio;
    while(caractere != NULL && contador < x-1){
        caractere = caractere->prox;
        contador++;
    }
    if(caractere != NULL){
        NO *aux = caractere;
        if(caractere->ant != NULL){
            caractere->ant->prox = caractere->prox;
        }
        if(caractere->prox != NULL){
            caractere->prox->ant = caractere->ant;
        }
        if(caractere == linha->inicio){
            linha->inicio = caractere->prox;
        }
        if(caractere == linha->fim){
            linha->fim = caractere->ant;
        }
        free(aux);
    }
}


//Função para abrir um arquivo existente e armazenar na página
void abrirArquivo(PAGINA * pagina, char nomeArquivo[], LINHA *linha, int *posicaoAtualLinha){
    FILE *arquivo = fopen(nomeArquivo, "r");
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo");
        return;
    }
    pagina->inicio = linha;
    char caractere;
    while((caractere = fgetc(arquivo)) != EOF){
        if(caractere == '\n'){
            LINHA *aux = inicializarLinha();
            novaLinha(aux, linha);
            (linha) = aux;
            (*posicaoAtualLinha)++;
        }else{
            inserir(linha, caractere);
        }
    }
    fclose(arquivo);

    imprimirLista(pagina->inicio);
}

//Função para conseguir largura terminal com a biblioteca windows
int getLarguraTerminal(){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwSize.X;
}

// Menu de opções do editor
int Menu(){
    int opcao;
    printf("\tBem vindo ao Editor de Texto\n");
    printf("1- Novo arquivo\n");
    printf("2- Abrir e Editar arquivo existente\n");
    printf("3- Sair\n");
    printf("Digite a opção desejada: ");

    scanf("%d", &opcao);

    return opcao;
}

void RecuperarPosicaoFinal(PAGINA * pagina, int posicaoAtualLinha, int *posicaoFinalEscrita){
    LINHA * linha = pagina->inicio;
    int i = 0;
    *posicaoFinalEscrita = 0;
    while(linha != NULL && i < posicaoAtualLinha){
        linha = linha->baixo;
        i++;
    }
    NO *aux = linha->inicio;
    while(aux != NULL){
        (*posicaoFinalEscrita)++;
        aux = aux->prox;
    }
}

//Função de inserção simples na struct PAGINA
void inserir(LINHA *linha, char c){
    NO *novo = (NO*) malloc(sizeof(NO));
    novo->c = c;
    novo->prox = NULL;
    novo->ant = NULL;
    NO *aux;
    if(linha->inicio == NULL){
        linha->inicio = novo;
        linha->fim = novo;
    }else{
        if(linha->fim == NULL){
            linha->fim = novo;
            linha->inicio->prox = linha->fim;
            linha->fim->ant = linha->inicio;
        }else{
            aux = linha->fim;
            aux->prox = novo;
            novo->ant = aux;
            linha->fim = novo;
        }
    }
}