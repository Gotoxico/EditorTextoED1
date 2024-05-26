#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "EditorTextoED1.h"
#include <windows.h>

#define GORight printf("\033[1C")
#define GOUp printf("\033[1A")
#define GODown printf("\033[1B")
#define GOLeft printf("\033[1D")
#define Deslocamento printf("\033[1L")
#define DeleteLine printf("\033[1M")

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

BYTES * recuperarInicio(LINHA *linha){
    BYTES *aux = linha->inicio;
    return aux;
}

BYTES * inicializarBytes(){
    BYTES *byte = (BYTES*) malloc(sizeof(BYTES));
    byte->inicio = NULL;
    byte->prox = NULL;
    byte->ant = NULL;
    byte->fim = NULL;
    return byte;
}
void novaLinha(LINHA *linha , LINHA *cima){
    linha->inicio = NULL;
    linha->fim = NULL;
    linha->cima = cima;
    linha->baixo = NULL;
    cima->baixo = linha;
}
//Função para inserir byte
void inserirBytes(BYTES *bytes,unsigned char caractere){
    NO *novo = (NO*) malloc(sizeof(NO));
    novo->c = caractere;
    novo->prox = NULL;
    novo->ant = NULL;

    if(bytes->inicio == NULL){
        bytes->inicio = novo;
        bytes->fim = novo;
    }else{
        novo->ant = bytes->fim;
        bytes->fim->prox = novo;
        bytes->fim = novo;
    }
}

//Função para inserir um caractere na linha da página
void inserirCaractereLinha(LINHA *linha, BYTES * byte, int * posicaoFinalEscrita, int posicaoAtualColuna){
    BYTES * aux;

    byte->prox = NULL;
    if(linha->inicio == NULL){
        linha->inicio = byte;
        linha->fim = byte;
        byte->ant = NULL;

    }else{
        if((posicaoAtualColuna) == (*posicaoFinalEscrita)){
            byte->ant = linha->fim;
            linha->fim->prox = byte;
            linha->fim = byte;
        }else if((*posicaoFinalEscrita) > (posicaoAtualColuna)){
             aux = linha->inicio;
            int i = 0;
            while(aux != NULL && i < (posicaoAtualColuna+2)){
                aux = aux->prox;
                i++;
            }
            byte->prox = aux;
            byte->ant = aux->ant;
            if (aux->ant != NULL) {
                aux->ant->prox = byte;
                aux->ant = byte;
            } else {
                linha->inicio = byte;
            }

            printf("\033[1@");
        }
    }
    (*posicaoFinalEscrita)++;
    
}

void removerCaractereLinha(LINHA *linha){
    if(linha->fim == NULL){
        return;
    }
    BYTES *aux = linha->fim;
    linha->fim = linha->fim->ant;
    linha->fim->prox = NULL;
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

//Função para mover o cursor do teclado, mas não está¡ em uso por enquanto
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
    BYTES *auxBytes;
    NO * auxNo;
    while(aux != NULL){
        auxBytes = aux->inicio;

        while(auxBytes!=NULL){
            auxNo = auxBytes->inicio;
            while(auxNo != NULL){
                printf("%c", auxNo->c);
                auxNo = auxNo->prox;
            }
            auxBytes = auxBytes->prox;
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
    BYTES * byte;
    NO * caracteres;
    while(linha != NULL){
        byte = linha->inicio;
        while(byte != NULL){
            caracteres = byte->inicio;
            while(caracteres != NULL){
                fputc(caracteres->c, arquivo);
                caracteres = caracteres->prox;
            }
            byte = byte->prox;
        }
        linha = linha->baixo;
        if(linha != NULL) fputc('\n', arquivo);
    }
    fclose(arquivo);
}

//Função para apagar um caractere da página
void apagar(PAGINA * pagina, int x, int y){
    
    LINHA *linha = pagina->inicio;
    BYTES *byte;
    int contador = 0;

    while(linha != NULL && contador < y){
        linha = linha->baixo;
        contador++;
    }
    contador = 0;
    byte = linha->inicio;
    while(byte != NULL && contador < x-1){
        byte = byte->prox;
        contador++;
    }
    if(byte != NULL){
        BYTES *aux = byte;
        if(byte->ant != NULL){
            byte->ant->prox = byte->prox;
        }
        if(byte->prox != NULL){
            byte->prox->ant = byte->ant;
        }
        if(byte == linha->inicio){
            linha->inicio = byte->prox;
        }
        if(byte == linha->fim){
            linha->fim = byte->ant;
        }
        free(aux);
    }
}


//Função para abrir um arquivo existente e armazenar na página. AlÃ©m disso, os ponteiros de linhas devem ser todos ligados e alocados, de modo qua a linha atual seja a última linha do arquivo, representado pelo ponteiro linha da Função. A Função deve imprimir o arquivo na tela
void abrirArquivo( LINHA * linha, char nomeArquivo[], int *posicaoAtualLinha){
    PAGINA * pagina = inicializar();
    pagina->inicio = linha;
    FILE *arquivo = fopen(nomeArquivo, "r");
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo");
        return;
    }
    LINHA *aux = NULL;
    BYTES * byte;

    unsigned char c;
    while((c = fgetc(arquivo)) != EOF){
        if(c == '\n'){
            imprimirLinha(linha);
            aux = inicializarLinha();
            novaLinha(aux, linha);
            linha->baixo = aux;
            aux->cima = linha;
            linha = aux;
            (*posicaoAtualLinha)++;
            printf("\n");
        }else{
            byte = inicializarBytes();
            if(c <= 127){
                inserirBytes(byte, c); 
                // printf("%c", c); 
            }else if(c >= 192 && c <= 223){
                inserirBytes(byte, c);
                // printf("%c", c);
                c = fgetc(arquivo);
                // printf("%c", c);
                inserirBytes(byte, c);
            }else if(c >= 224 && c <= 239){
                inserirBytes(byte, c);
                // printf("%c", c);
                c = fgetc(arquivo);
                // printf("%c", c);
                inserirBytes(byte, c);
                c = fgetc(arquivo);
                inserirBytes(byte, c);
                // printf("%c", c);
            }else if(c >= 240 && c <= 247){
                inserirBytes(byte, c);
                // printf("%c", c);
                c = fgetc(arquivo);
                // printf("%c", c);
                inserirBytes(byte, c);
                c = fgetc(arquivo);
                // printf("%c", c);
                inserirBytes(byte, c);
                c = fgetc(arquivo);
                // printf("%c", c);
                inserirBytes(byte, c);
            }  
            inserir(linha, byte);
        }
        
    }
    printf("\nAqui\n");
    imprimirLinha(linha);
    fclose(arquivo);

    // imprimirLista(pagina->inicio);    
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
    BYTES *aux = linha->inicio;
    while(aux != NULL){
        (*posicaoFinalEscrita)++;
        aux = aux->prox;
    }
}

//Função de inserção de linha na página
void inserir(LINHA *linha, BYTES * byte){
   
    byte->prox = NULL;

    if(linha->inicio == NULL){
        linha->inicio = byte;
        linha->fim = byte;
        byte->ant = NULL;
    }else{
        byte->ant = linha->fim;
        linha->fim->prox = byte;
        linha->fim = byte;
    }
}

//Função para ajeitar o ponteiro
LINHA *  Apontamento(PAGINA * pagina, int posicaoLinhaAtual){
    
    LINHA *aux = pagina->inicio;
    LINHA *aux1 = NULL;
    int i = 0;
    while(aux != NULL && i < posicaoLinhaAtual){
        aux1 = aux;
        aux = aux->baixo;
        aux->cima = aux1;
        aux1->baixo = aux;
        i++;
    }
    return aux;
}

//Função para imprimir uma linha
void imprimirLinha(LINHA *linha){
    BYTES *aux = linha->inicio;
    NO * aux1;
    while(aux != NULL){
        aux1 = aux->inicio;
        while(aux1 != NULL){
            printf("%c", aux1->c);
            aux1 = aux1->prox;
        }
        aux = aux->prox;
    }
}

//Função para mover a linha para baixo
LINHA * Reapontar(PAGINA * pagina, int posicaoAtualLinha, int posicaoAtualColuna, int *posicaoFinalEscrita){
    LINHA * linha = pagina->inicio;
    int i = 0, conferidor = 0;
    while(linha != NULL && i < posicaoAtualLinha){
        linha = linha->baixo;
        i++;
    }
    LINHA * linhaAux = linha->baixo;
    BYTES * aux = linha->inicio;
    BYTES * liberar = NULL;
    i = 0;
    while(aux != NULL && i < posicaoAtualColuna){
        aux = aux->prox;
        i++;
    }
    LINHA * linha2 = inicializarLinha();
    aux->ant->prox = NULL;
    linha->fim = aux->ant;
    i = posicaoAtualColuna;

    
    if(linhaAux == NULL){
        linhaAux = inicializarLinha();
        novaLinha(linhaAux, linha);
        while(aux != NULL && i < (*posicaoFinalEscrita)){
            inserir(linhaAux, aux);
            liberar = aux;
            aux = aux->prox;
            i++;
            free(liberar);
        }
        
        conferidor = 0;
    }else{
        novaLinha(linha2, linha);
        novaLinha(linhaAux, linha2);
        while(aux != NULL && i < (*posicaoFinalEscrita)){
            inserir(linha2, aux);
            liberar = aux;
            aux = aux->prox;
            i++;
            free(liberar);
        }
        conferidor = 1;
    }
    int contador = (*posicaoFinalEscrita) - posicaoAtualColuna;
    (*posicaoFinalEscrita) = posicaoAtualColuna;
    printf("\033[%dC", contador);
    for(i = 0; i < contador; i++){
        printf("\b");
        printf(" ");
        printf("\033[1D");
    }
    printf("\b");
    printf("\033[1E");

   
    if(conferidor == 0){

        imprimirLinha(linhaAux);
        return linhaAux;
    }else{
        Deslocamento;
        imprimirLinha(linha2);
        return linha2;
    }
}

void DeslocarLinha(LINHA * linha){
    LINHA * auxLinha = linha->baixo;

    linha->baixo = auxLinha->baixo;
    auxLinha->baixo->cima = linha;
    

    BYTES * aux = auxLinha->inicio;
    NO * aux1 = NULL;
    while(aux != NULL){
        aux1 = aux->inicio;
        while(aux1 != NULL){
            printf("%c", aux1->c);
            aux1 = aux1->prox;
        }
        inserir(linha, aux);
        aux = aux->prox;
    }
    aux = auxLinha->inicio;
    BYTES * liberar = NULL;
    while(aux != NULL){
        liberar = aux;
        aux = aux->prox;
        free(liberar);
    }
    free(auxLinha);

}

// int is_cursor_on_last_line() {
//     int cursor_y, cursor_x;
//     int max_y, max_x;
    
//     // Inicializa a tela
//     initscr();
    
//     // Obtém a posição atual do cursor
//     getyx(stdscr, cursor_y, cursor_x);
    
//     // Obtém o tamanho da janela
//     getmaxyx(stdscr, max_y, max_x);
    
//     // Finaliza o modo ncurses
//     endwin();
    
//     // Verifica se o cursor está na última linha
//     return (cursor_y == max_y - 1);
// }