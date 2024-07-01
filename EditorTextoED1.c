#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "EditorTextoED1.h"
#include <windows.h>
#include <dirent.h>  // Biblioteca para manipulação de diretórios, como opendir e readdir
#include <sys/stat.h> // Biblioteca para obter informações sobre arquivos e diretórios, como stat

#define GORight printf("\033[1C")
#define GOUp printf("\033[1A")
#define GODown printf("\033[1B")
#define GOLeft printf("\033[1D")
#define Deslocamento printf("\033[1L")
#define DeleteLine printf("\033[1M")
#define GOStartDown printf("\033[1E")
#define GOUpStart printf("\033[1F")

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
    //linha->inicio = NULL;
    //linha->fim = NULL;
    linha->cima = cima;
    //linha->baixo = NULL;
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
    char caminho[100] = "../arquivos/";
    strcat(caminho, nomeArquivo);
    FILE *arquivo = fopen(caminho, "w");
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
int abrirArquivo( LINHA * linha, char nomeArquivo[], int *posicaoAtualLinha){
    PAGINA * pagina = inicializar();
    pagina->inicio = linha;
     char caminho[100] = "../arquivos/";
    strcat(caminho, nomeArquivo);
    FILE *arquivo = fopen(caminho, "r");
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo");
        return 0;
    }
    LINHA *aux = NULL;
    BYTES * byte;

    int c;
    while((c = fgetc(arquivo)) != EOF){
        if(c == '\n'){
            aux = inicializarLinha();
            novaLinha(aux, linha);
            linha = aux;
            (*posicaoAtualLinha)++;
            printf("\n");
        }else{
            byte = inicializarBytes();
            if(c <= 127){
                inserirBytes(byte, (unsigned char) c); 
            }else if(c >= 192 && c <= 223){
                inserirBytes(byte,(unsigned char) c);
                c = fgetc(arquivo);
                inserirBytes(byte,(unsigned char) c);
            }else if(c >= 224 && c <= 239){
                inserirBytes(byte,(unsigned char) c);
                c = fgetc(arquivo);
                inserirBytes(byte,(unsigned char) c);
                c = fgetc(arquivo);
                inserirBytes(byte,(unsigned char) c);
            }else if(c >= 240 && c <= 247){
                inserirBytes(byte,(unsigned char) c);
                c = fgetc(arquivo);
                inserirBytes(byte,(unsigned char) c);
                c = fgetc(arquivo);
                inserirBytes(byte,(unsigned char) c);
                c = fgetc(arquivo);
                inserirBytes(byte,(unsigned char) c);
            }  
            inserir(linha, byte);
        }
        
    }
    imprimirLista(pagina->inicio);
    fclose(arquivo);
    return 1;
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
    printf("0- Fechar Editor\n");
    printf("Digite a opcao desejada: ");

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

//imprimir Byte
void imprimirByte(BYTES *byte){
    NO *aux = byte->inicio;
    while(aux != NULL){
        printf("%c", aux->c);
        aux = aux->prox;
    }
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
        linhaAux->inicio = aux;
        while(aux->prox != NULL){
            //apagar caractere do terminal
            aux = aux->prox;
        }
        linhaAux->fim = aux;
        aux = linhaAux->inicio;
        GOStartDown;
        while(aux != NULL){
            imprimirByte(aux);
            aux = aux->prox;
        }
        
        conferidor = 0;
    }else{
        novaLinha(linha2, linha);
        novaLinha(linhaAux, linha2);
        linha2->inicio = aux;
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        linha2->fim = aux;
        aux = linha2->inicio;
        GOStartDown;
        Deslocamento;
        while(aux != NULL){
            imprimirByte(aux);
            aux = aux->prox;
        }
        conferidor = 1;
    }
    
    
    GOUpStart;
    printf("\033[%dC", (*posicaoFinalEscrita));
    i = posicaoAtualColuna;
    for(i; i < (*posicaoFinalEscrita); i++){
        GOLeft;
        printf("\033[1P");
    }

    GOStartDown;
    int contador = (*posicaoFinalEscrita) - posicaoAtualColuna;
    (*posicaoFinalEscrita) = posicaoAtualColuna;
    printf("\033[%dC", contador);
     
    
    if(conferidor == 0){

         return linhaAux;
    }else{
        
        return linha2;
    }
}

//Backspace no início da linha para que o conteúdo da linha de baixo seja movido para cima
LINHA * DeslocarLinha(LINHA * linha){
   LINHA * Cima = linha->cima;
    BYTES * aux = linha->inicio, *sub;
    Cima->fim->prox = aux;
    aux->ant = Cima->fim;
    Cima->fim = linha->fim;
    if(linha->baixo == NULL){
        while( aux != NULL){
        imprimirByte(aux);
            aux = aux->prox;
        }
        Cima->baixo = NULL;
        free(linha);
        return Cima;    
    }else{
        while( aux != NULL){
        imprimirByte(aux);
            aux = aux->prox;
        }
        Cima->baixo = linha->baixo;
        linha->baixo->cima = Cima;
        free(linha);
        return Cima;
    
    }
}



void listFiles(const char *path) {  // Função para listar todos os arquivos em um diretório
    struct dirent *entry;
    DIR *dp = opendir(path); // Abre o diretório especificado pelo caminho e retorna um ponteiro para o diretório
    int i = 1;
    if (dp == NULL) {
        perror("opendir");  // Se o diretório der erro na hora de abrir
        return;
    }
     
    

    while ((entry = readdir(dp))) {
        // Verifica se o nome da entrada não começa com um ponto para ignorar "." e ".."
        if (entry->d_name[0] != '.') {
            printf(" %d - %s\n", i, entry->d_name);  // Imprime o nome do arquivo ou diretório
            i++;
        }
    }

    

    closedir(dp);  // Fecha o diretório após a leitura
}


int createDoc(const char *directory){
    char newDocName[30];
    char fullPath[130];

    printf("Nome do documento (inclua .txt): ");
    fgets(newDocName, sizeof(newDocName), stdin);
    newDocName[strcspn(newDocName, "\n")] = '\0'; // Remove o caractere de nova linha

    snprintf(fullPath, sizeof(fullPath), "%s/%s", directory, newDocName);


    FILE *f = fopen(fullPath, "a+"); // inicializa o arquivo
    if (f == NULL) {
        printf("Erro ao abrir arquivo %s\n", newDocName);
        return FALSE;
    } else {
        printf("Arquivo %s criado com sucesso\n", newDocName);
    }

    //FUNÇÂO PRA EXECUTAR O BGLH DOIDO

    fclose(f); // fecha o arquivo

    return TRUE;

}


