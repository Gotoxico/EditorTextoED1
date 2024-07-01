#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "EditorTextoED1.h"
#include <locale.h>
#include <math.h>
#include <dirent.h>  // Biblioteca para manipulação de diretórios, como opendir e readdir
#include <sys/stat.h> // Biblioteca para obter informações sobre arquivos e diretórios, como stat
#include <string.h>


#define Backspace 8
#define Enter 13
#define Tab 9
#define ESC 27
#define CTRL_S 19
#define SETAS 224
#define CIMA 72
#define BAIXO 80
#define ESQUERDA 75
#define DIREITA 77
#define NEW 1
#define OPEN 2
#define Espace 32
#define GORight printf("\033[1C")
#define GOUp printf("\033[1A")
#define GODown printf("\033[1B")
#define GOLeft printf("\033[1D")
#define GOStartDown printf("\033[1E")
#define Deslocamento printf("\033[1L")
#define DeleteLine printf("\033[1M")
#define GOUpStart printf("\033[1F")
#define ScrollUp printf("\033[1S")
#define ScrollDown printf("\033[1T")
#define InsertLine printf("\033[1L")


int main(){
    //setlocale(LC_ALL, "Portuguese");
    PAGINA * pagina = inicializar();
    LINHA  * linhaAtual = inicializarLinha();
    LINHA * aux;
    pagina->inicio = linhaAtual;
    clearScreen();
    BYTES * byte = inicializarBytes();  
    // linhaAtual->inicio = byte;
    char path[100] = "../arquivos";
    int opcao = 0;
    //Chama o menu de opções do editor para o usuário escolher o que deseja fazer, se deseja abrir um arquivo ou criar um novo.
    unsigned char caractere, auxiliar;
    int fim;
    char nomeArquivo[100];

    int i, larguraTerminal = getLarguraTerminal, posicaoAtualColuna = 0, posicaoAtualLinha = 0,posicaoFinalEscrita = 0, conferidor, salvamento, abertura;
    do{
        
        opcao = Menu();
        //Caso o usuário escolha a opção de abrir um arquivo, ele deve digitar o nome do arquivo que deseja abrir, caso contrário, ele deve digitar o nome do arquivo que deseja criar. Para cada opção, o nome do arquivo é armazenado na variável nomeArquivo.
        switch(opcao){
            case NEW:
                //system("t.txt");
                getchar();
                printf("digite o nome do arquivo (nome.txt): ");
                gets(nomeArquivo);
                strcat(nomeArquivo, ".txt");
                //fopen("teste.txt", "w");
                system("cls");
                break;

            case OPEN:
               // system("cls");
                printf("\n\nArquivos existentes no diretorio: \n");
                listFiles(path);
                getchar();
                printf("digite o nome do arquivo: ");
                gets(nomeArquivo);
                //strcat(nomeArquivo, ".txt");
                abertura = abrirArquivo(linhaAtual, nomeArquivo, &posicaoAtualLinha);
                if(abertura == 0){
                    printf("\n\nArquivo nao encontrado!\n\n\n");
                    continue;
                }
                RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                posicaoAtualColuna = posicaoFinalEscrita;
                GOUp;

                for(i = 0; i < posicaoFinalEscrita; i++){
                    GORight;
                } 
                linhaAtual = Apontamento(pagina, posicaoAtualLinha);
                break;

            default:
                return 0;
                break;
        }

        setbuf(stdin, NULL);
        
        //Falta implementar a função de abrir um arquivo existente para edição
        while(1){
            salvamento = 0;
            caractere = getch();

            switch(caractere){
                case Tab:
                    //Tabulação
                    for(i = 0; i < 4; i++){
                        inserirBytes(byte, ' ');
                        inserirCaractereLinha(linhaAtual, byte, &posicaoFinalEscrita, posicaoAtualColuna);
                        byte = byte->prox;
                        printf(" ");
                        if(posicaoAtualColuna == larguraTerminal){
                            if(verificarCursorFundo()){
                                ScrollUp;
                                /*if(linhaAtual->baixo != NULL){
                                    imprimirLinha(linhaAtual->baixo);
                                }*/
                            }
                            GOStartDown;
                            posicaoAtualColuna = 0;
                            posicaoAtualLinha++;
                        }
                        else{
                            posicaoAtualColuna++;
                        }
                    }
                    break;
                case Enter:
                        //posicaoAtualLinha++;
                        if(verificarCursorFundo()){
                            ScrollUp;
                            /*if(linhaAtual->baixo != NULL){
                                imprimirLinha(linhaAtual->baixo);
                            }*/
                        }
                        if(posicaoAtualColuna < posicaoFinalEscrita){
                            if(posicaoAtualColuna == 0){
                                aux = inicializarLinha();
                                LINHA * sub = linhaAtual->cima;
                                novaLinha(linhaAtual, aux);
                                aux->cima = sub;
                                if(sub != NULL){
                                    sub->baixo = aux;
                                }
                                posicaoAtualLinha++;
                                RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                                Deslocamento;
                                GOStartDown;
                                break;
                            }
                            linhaAtual = Reapontar(pagina, posicaoAtualLinha, posicaoAtualColuna, &posicaoFinalEscrita);
                            posicaoAtualLinha++;
                            
                            
                            RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                            posicaoAtualColuna = posicaoFinalEscrita;
                            //linhaAtual = linhaAtual->baixo;
                        }else{
                            aux = inicializarLinha();
                            if(linhaAtual->baixo == NULL){  
                                novaLinha(aux, linhaAtual);
                                linhaAtual = aux;
                                posicaoAtualLinha++;
                                RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                                GOStartDown;
                                posicaoAtualColuna = 0;
                            }else{
                                GODown;
                                Deslocamento;
                                LINHA * sub = linhaAtual->baixo;
                                novaLinha(aux, linhaAtual);
                                aux->baixo = sub;
                                sub->cima = aux;
                                linhaAtual = aux;
                                posicaoAtualLinha++;
                                posicaoAtualColuna = 0;
                                RecuperarPosicaoFinal( pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                            imprimirLinha(linhaAtual);
                            }
                        }
                    
                    break;
                case Backspace:
                    //Apagar caractere terminal
                    //Caso esteja no começo de uma linha, pular para o final da linha acima
                    if(verificarCursorTopo()){
                        InsertLine;
                        imprimirLinha(linhaAtual->cima);
                    }
                    if(posicaoAtualColuna == 0 && posicaoAtualLinha != 0){
                            if(linhaAtual->inicio == NULL){
                                posicaoAtualLinha--;
                                RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                                GOUpStart;
                                printf("\033[%dC", posicaoFinalEscrita);
                                posicaoAtualColuna = posicaoFinalEscrita;
                                aux = linhaAtual;
                                linhaAtual = linhaAtual->cima;
                                linhaAtual->baixo = NULL;
                                free(aux);
                            }else{
                                //linhaAtual = linhaAtual->cima;
                                posicaoAtualLinha--;
                                DeleteLine;
                                RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                                GOUpStart;
                                printf("\033[%dC", posicaoFinalEscrita);
                                linhaAtual = DeslocarLinha(linhaAtual);
                                posicaoAtualColuna = posicaoFinalEscrita;
                                RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                                posicaoAtualColuna = posicaoFinalEscrita;

                            }
                        
                        break;
                    }

                    if(posicaoAtualColuna != 0){
                        GOLeft;
                        printf("\033[1P");
                    }
                 
                    apagar(pagina, posicaoAtualColuna, posicaoAtualLinha);
                    if(posicaoAtualColuna != 0) posicaoAtualColuna--;
                    posicaoFinalEscrita--;

                    break;

                case SETAS:
                    auxiliar = getch();
                    switch(auxiliar){
                        case CIMA:
                            // moverLinhaCima(linhaAtual);
                            if(posicaoAtualLinha!=0){
                                if(verificarCursorTopo()){
                                    InsertLine;
                                    imprimirLinha(linhaAtual->cima);
                                }
                                posicaoAtualLinha--;
                                conferidor = posicaoAtualColuna;
                                RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                                if(conferidor > posicaoFinalEscrita){
                                    GOUpStart;
                                    printf("\033[%dC", posicaoFinalEscrita);
                                    posicaoAtualColuna = posicaoFinalEscrita;
                                }else{
                                    GOUpStart;
                                    printf("\033[%dC", posicaoAtualColuna);
                                }
                                linhaAtual = linhaAtual->cima;
                            
                            } 
                            
                            
                            break;
                        case BAIXO:
                            // moverLinhaBaixo(linhaAtual);
                            if(linhaAtual->baixo!= NULL){
                                if(verificarCursorFundo()){
                                    ScrollUp;
                                    imprimirLinha(linhaAtual->baixo);
                                }
                                linhaAtual = linhaAtual->baixo;
                                posicaoAtualLinha++;
                                conferidor = posicaoAtualColuna;
                                RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                                if(conferidor > posicaoFinalEscrita){
                                    GODown ;
                                    printf("\033[%dD", conferidor - posicaoFinalEscrita);
                                    posicaoAtualColuna = posicaoFinalEscrita;
                                }else{
                                    GODown ;
                                }
                                
                            }
                            break;
                        case ESQUERDA:
                            // moverCaractereEsquerda(linhaAtual);
                            GOLeft;
                            if(posicaoAtualColuna == 0 && posicaoAtualLinha != 0){
                                if(verificarCursorTopo()){
                                    InsertLine;
                                    imprimirLinha(linhaAtual->cima);
                                }
                                posicaoAtualLinha--;
                                RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                                GOUpStart;
                                printf("\033[%dC", posicaoFinalEscrita);
                                
                                posicaoAtualColuna = posicaoFinalEscrita;
                                linhaAtual = linhaAtual->cima;
                            }
                            else{

                                if(posicaoAtualColuna != 0)posicaoAtualColuna--;
                            }
                            break;
                        case DIREITA:
                            // moverCaractereDireita(linhaAtual);
                            if(posicaoAtualColuna < posicaoFinalEscrita){
                                GORight;
                                posicaoAtualColuna++;
                            }else if(posicaoAtualColuna == posicaoFinalEscrita){
                                if(linhaAtual->baixo != NULL){
                                    if(verificarCursorFundo()){
                                        ScrollUp;
                                        /*if(linhaAtual->baixo != NULL){
                                            imprimirLinha(linhaAtual->baixo);
                                        }*/
                                    }
                                    linhaAtual = linhaAtual->baixo;
                                    posicaoAtualLinha++;
                                    posicaoAtualColuna = 0;
                                    GOStartDown;
                                    RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                                }
                            }
                            break;

                        default:
                        break;
                    }
                    break;            
                case Espace:
                    inserirBytes(byte, ' ');
                    inserirCaractereLinha(linhaAtual, byte, &posicaoFinalEscrita, posicaoAtualColuna);
                    byte = inicializarBytes();
                    printf(" ");
                    if(posicaoAtualColuna == larguraTerminal){
                        if(verificarCursorFundo()){
                            ScrollUp;
                            /*if(linhaAtual->baixo != NULL){
                                imprimirLinha(linhaAtual->baixo);
                            }*/
                        }
                        posicaoAtualColuna = 0;
                        posicaoAtualLinha++;
                        aux = inicializarLinha();
                        novaLinha(aux, linhaAtual);
                        linhaAtual = aux;
                        RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                    }
                    else{
                        posicaoAtualColuna++;
                    }
                    break;

                case ESC:
                    system("cls");
                    printf("\n\nFim da insercao de texto:\n1- Imprimir Lista e Salvar;\n2- Nao-Salvar\nOpcao: ");
                    scanf("%d", &fim);
                    setbuf(stdin, NULL);
                    if(fim == 1){
                        printf("\n\n");
                        imprimirLista(pagina->inicio);
                        salvarArquivo(nomeArquivo, pagina);
                        printf("\n\nArquivo salvo com sucesso!\n\n");
                    }else{
                        printf("\n\n");
                    }   
                   salvamento = 1;
                    break;
                
                

                //CTRL+S = 16 (Salvar e sair)
                case CTRL_S:
                    printf("Fim da inserção de texto:\n");
                    setbuf(stdin, NULL);
                    printf("\n\n");
                    imprimirLista(pagina->inicio);
                    salvarArquivo(nomeArquivo, pagina);
                    return 0;

                default:
                    // x++;
                printf("\033[1@");
                if(caractere <= 127){
                    printf("%c", caractere);
                    inserirBytes(byte, caractere);
                    
                }else if(caractere >= 192 && caractere <= 223){
                    printf("%c", caractere);
                    inserirBytes(byte, caractere);
                    caractere = getch();
                    printf("%c", caractere);
                    inserirBytes(byte, caractere);
                }else if(caractere >= 224 && caractere <= 239){
                    printf("%c", caractere);
                    inserirBytes(byte, caractere);
                    caractere = getch();
                    printf("%c", caractere);
                    inserirBytes(byte, caractere);
                    caractere = getch();
                    printf("%c", caractere);
                    inserirBytes(byte, caractere);
                }else if(caractere >= 240 && caractere <= 247){
                    printf("%c", caractere);
                    inserirBytes(byte, caractere);
                    caractere = getch();
                    printf("%c", caractere);
                    inserirBytes(byte, caractere);
                    caractere = getch();
                    printf("%c", caractere);
                    inserirBytes(byte, caractere);
                    caractere = getch();
                    printf("%c", caractere);
                    inserirBytes(byte, caractere);
                }
                inserirCaractereLinha(linhaAtual, byte, &posicaoFinalEscrita, posicaoAtualColuna);
                    if(posicaoAtualColuna == larguraTerminal-1){
                        if(verificarCursorFundo()){
                            ScrollUp;
                            /*if(linhaAtual->baixo != NULL){
                                imprimirLinha(linhaAtual->baixo);
                            }*/
                        }
                        posicaoAtualColuna = 0;
                        posicaoAtualLinha++;
                        aux = inicializarLinha();
                        novaLinha(aux, linhaAtual);
                        linhaAtual = aux;
                        RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                    }
                    else{
                        posicaoAtualColuna++;
                    }
                    setbuf(stdin, NULL);
                    byte = inicializarBytes();
                    break;
            }
            if(salvamento == 1) break;
        }
    } while(opcao != 0);
      
    return 0;
}