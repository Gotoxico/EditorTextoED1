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
    cima->baixo = linha;
}

//função para inserir um caractere na linha da página
void inserirCaractereLinha(LINHA *linha, char caractere){
    NO *novo = (NO*) malloc(sizeof(NO));
    novo->c = caractere;
    novo->prox = NULL;
    novo->ant = NULL;

    if(linha->inicio == NULL){
        linha->inicio = novo;
        linha->fim = novo;
    }else{
        novo->ant = linha->fim;
        linha->fim->prox = novo;
        linha->fim = novo;
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
    //inserirCaractereLinha(linha , c);
    FILE *file = fopen(nomeArquivo, "a+");
    char caractere;
    int contadorCaracteresLinha;
   // PAGINA pagina;
    //LINHA linha, anterior;

    fseek(file, 0, SEEK_SET);
    //iniciarPagina(&pagina);
    //primeiraLinha(&linha);
    
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

    putch(c);
    fputc(c, file);
    fclose(file);
}


//Função para apagar o texto do arquivo
/*void apagarTexto(char nomeArquivo[]){
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
}*/

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

// Função para apagar o último caractere de um arquivo
void apagarUltimoCaractere(char nomeArquivo[]) {
    FILE *arquivo = fopen(nomeArquivo, "r+");
    if (arquivo == NULL) {
        printf("Não foi possível abrir o arquivo.\n");
        return;
    }

    fseek(arquivo, -1, SEEK_END); // Move o ponteiro para o penúltimo caractere
    long posicao = ftell(arquivo); // Obtém a posição do penúltimo caractere

    int resultado = ftruncate(fileno(arquivo), posicao); // Trunca o arquivo na posição do penúltimo caractere

   

    fclose(arquivo);
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
        fputc('\n', arquivo);
        linha = linha->baixo;
    }
    fclose(arquivo);
}

//Função para apagar um caractere da página
void apagar(PAGINA * pagina, int x, int y){
    
    LINHA *linha = pagina->inicio;
    NO *caractere;
    int contador = 1;
    while(linha != NULL && contador < y){
        linha = linha->baixo;
        contador++;
    }
    contador = 1;
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
void abrirArquivo(char nomeArquivo[], PAGINA *pagina){
    FILE *arquivo = fopen(nomeArquivo, "r");
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo");
        return;
    }

    char caractere;
    LINHA *linha = inicializarLinha();
    pagina->inicio = linha;
    while((caractere = fgetc(arquivo)) != EOF){
        if(caractere == '\n'){
            LINHA *aux = inicializarLinha();
            novaLinha(aux, linha);
            linha = aux;
        }else{
            inserirCaractereLinha(linha, caractere);
        }
    }
    fclose(arquivo);

    imprimirLista(pagina->inicio);
}
