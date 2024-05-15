#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
//#include <curses.h>
#include "EditorTextoED1.h"
#include <locale.h>
#include <math.h>

#define Backspace 8
#define Enter 13
#define Tab 9
#define ESC 27
#define CTRL_S 19
#define SETAS -32
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

int main(){
    setlocale(LC_ALL, "Portuguese");
    PAGINA * pagina = inicializar();
    LINHA  * linhaAtual = inicializarLinha();
    LINHA * aux;
    pagina->inicio = linhaAtual;
    clearScreen();

    //Chama o menu de opções do editor para o usuário escolher o que deseja fazer, se deseja abrir um arquivo ou criar um novo.
    int opcao = Menu();
    
    char caractere, auxiliar;
    int fim;
    char nomeArquivo[100];

    int i;
    int larguraTerminal = getLarguraTerminal();
    int posicaoAtualColuna = 0;
    int posicaoAtualLinha = 0;
    int posicaoFinalEscrita = 0;
    int conferidor;

    //Caso o usuário escolha a opção de abrir um arquivo, ele deve digitar o nome do arquivo que deseja abrir, caso contrário, ele deve digitar o nome do arquivo que deseja criar. Para cada opção, o nome do arquivo é armazenado na variável nomeArquivo.
    switch(opcao){
        case NEW:
            getchar();
            printf("digite o nome do arquivo (nome.txt): ");
            gets(nomeArquivo);
            break;

        case OPEN:
            getchar();
            printf("digite o nome do arquivo (nome.txt): ");
            gets(nomeArquivo);
            abrirArquivo(pagina, linhaAtual, nomeArquivo, &posicaoAtualLinha);
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
        caractere = getch();

        switch(caractere){
            case Tab:
                //Tabulação
                for(i = 0; i < 4; i++){
                    inserirCaractereLinha(linhaAtual, ' ', &posicaoFinalEscrita, posicaoAtualColuna);
                    printf(" ");
                    if(posicaoAtualColuna == larguraTerminal){
                        printf("\033[1E");
                        posicaoAtualColuna = 0;
                        posicaoAtualLinha++;
                    }
                    else{
                        posicaoAtualColuna++;
                    }
                }
                break;
            case Enter:
                if(linhaAtual->baixo == NULL){
                    aux = inicializarLinha();
                    novaLinha(aux, linhaAtual);
                    linhaAtual = aux;
                    posicaoAtualLinha++;
                    if(posicaoAtualColuna < posicaoFinalEscrita){
                        Reapontar(pagina, posicaoAtualLinha, posicaoAtualColuna, &posicaoFinalEscrita);
                        RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                        posicaoAtualColuna = posicaoFinalEscrita;
                    }else{
                        posicaoAtualColuna = 0;
                    }
                }
                else{
                    printf("\033[1E");
                    posicaoAtualColuna = 0;
                    posicaoAtualLinha++;
                    linhaAtual = linhaAtual->baixo;
                }
            
                
                break;
            case Backspace:
                //Apagar caractere terminal
                //Caso esteja no começo de uma linha, pular para o final da linha acima
                if(posicaoAtualColuna == 0 && posicaoAtualLinha != 0){
                    posicaoAtualLinha--;
                    RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                    printf("\033[1F");
                    printf("\033[%dC", posicaoFinalEscrita);
                    posicaoAtualColuna = posicaoFinalEscrita;
                    //posicaoAtualColuna = larguraTerminal-1;
                    //apagar(pagina, posicaoAtualColuna, posicaoAtualLinha);
                    linhaAtual = linhaAtual->cima;
                    break;
                }

                if(posicaoAtualColuna != 0)printf("\033[1P");
                //Apagar normal
                printf("\b");
                printf(" ");
                GOLeft;
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
                            posicaoAtualLinha--;
                            conferidor = posicaoAtualColuna;
                            RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                            if(conferidor > posicaoFinalEscrita){
                                printf("\033[1F");
                                printf("\033[%dC", posicaoFinalEscrita);
                                posicaoAtualColuna = posicaoFinalEscrita;
                            }else{
                                printf("\033[1F");
                                printf("\033[%dC", posicaoAtualColuna);
                            }
                            linhaAtual = linhaAtual->cima;
                           
                        } 
                        
                        
                        break;
                    case BAIXO:
                        // moverLinhaBaixo(linhaAtual);
                        if(linhaAtual->baixo!= NULL){
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
                            posicaoAtualLinha--;
                            RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                            printf("\033[1F");
                            printf("\033[%dC", posicaoFinalEscrita);
                            
                            posicaoAtualColuna = posicaoFinalEscrita;
                            linhaAtual = linhaAtual->cima;
                        }
                        else{
                            posicaoAtualColuna--;
                        }
                        break;
                    case DIREITA:
                        // moverCaractereDireita(linhaAtual);
                        if(posicaoAtualColuna < posicaoFinalEscrita){
                            GORight;
                            posicaoAtualColuna++;
                        }
                        break;

                    default:
                    break;
                }
                break;            
            case Espace:
                inserirCaractereLinha(linhaAtual, ' ', &posicaoFinalEscrita, posicaoAtualColuna);
                printf(" ");
                if(posicaoAtualColuna == larguraTerminal){
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
                printf("\nFim da inserção de texto:\n1- Imprimir Lista e Salvar;\n2-Sair\nOpção: ");
                scanf("%d", &fim);
                setbuf(stdin, NULL);
                if(fim == 1){
                    printf("\n\n");
                    imprimirLista(pagina->inicio);
                    salvarArquivo(nomeArquivo, pagina);
                    return 0;
                }   
                else{
                    return 0;
                }
                
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
                putc(caractere, stdout);
                inserirCaractereLinha(linhaAtual, caractere, &posicaoFinalEscrita, posicaoAtualColuna);
                if(posicaoAtualColuna == larguraTerminal-1){
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
                break;
        }
      
    }
      
    return 0;
}