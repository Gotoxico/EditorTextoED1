#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
//#include <curses.h>
#include "EditorTextoED1.h"
#include <locale.h>
#include <math.h>

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
        case 1:
            getchar();
            printf("digite o nome do arquivo (nome.txt): ");
            gets(nomeArquivo);
            break;

        case 2:
            getchar();
            printf("digite o nome do arquivo (nome.txt): ");
            gets(nomeArquivo);
            abrirArquivo(pagina, nomeArquivo, linhaAtual, &posicaoAtualLinha);
            RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
            posicaoAtualColuna = posicaoFinalEscrita;
            //printf("%d, %d, %d", posicaoAtualColuna, posicaoAtualLinha, posicaoFinalEscrita);
            printf("\033[1A");

            for(i = 0; i < posicaoFinalEscrita; i++){
                printf("\033[1C");
            } 
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
            case 9:
                //Tabulação
                for(i = 0; i < 4; i++){
                    inserirCaractereLinha(linhaAtual, ' ', &posicaoFinalEscrita, &posicaoAtualColuna);
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
            case 13:
                aux = inicializarLinha();
                novaLinha(aux, linhaAtual);
                linhaAtual = aux;
                printf("\033[1E");
                posicaoAtualColuna = 0;
                posicaoAtualLinha++;
                RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
            
                
                break;
            case 8:
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
                    break;
                }
               
                //Apagar normal
                printf("\b");
                printf(" ");
                printf("\033[1D");
                apagar(pagina, posicaoAtualColuna, posicaoAtualLinha);
                if(posicaoAtualColuna != 0) posicaoAtualColuna--;
                posicaoFinalEscrita--;

                break;

            case -32:
                auxiliar = getch();
                switch(auxiliar){
                    case 72:
                        // moverLinhaCima(linhaAtual);
                        if(posicaoAtualLinha!=0){
                            posicaoAtualLinha--;
                            conferidor = posicaoAtualColuna;
                            RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                            if(conferidor > posicaoFinalEscrita){
                                printf("\033[1F");
                                
                                printf("\033[%dC", posicaoFinalEscrita);
                            }else{
                                printf("\033[1F");
                                printf("\033[%dC", posicaoAtualColuna);
                            }
                            linhaAtual = linhaAtual->cima;
                        } 
                        
                        
                        break;
                    case 80:
                        // moverLinhaBaixo(linhaAtual);
                        if(linhaAtual->baixo!= NULL){
                            printf("\033[1B");
                            posicaoAtualLinha++;
                            RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                            linhaAtual = linhaAtual->baixo;
                        }
                        break;
                    case 75:
                        // moverCaractereEsquerda(linhaAtual);
                        printf("\033[1D");
                        if(posicaoAtualColuna == 0 && posicaoAtualLinha != 0){
                            posicaoAtualLinha--;
                            RecuperarPosicaoFinal(pagina, posicaoAtualLinha, &posicaoFinalEscrita);
                            printf("\033[1F");
                            printf("\033[%dC", posicaoFinalEscrita);
                            
                            posicaoAtualColuna = posicaoFinalEscrita;
                            // if(posicaoAtualLinha!=0){
                            //     posicaoAtualLinha--;
                            //     posicaoAtualColuna = larguraTerminal-1;
                            // } 

                        }
                        else{
                            posicaoAtualColuna--;
                        }
                        break;
                    case 77:
                        // moverCaractereDireita(linhaAtual);
                        if(posicaoAtualColuna != posicaoFinalEscrita){
                            printf("\033[1C");
                            posicaoAtualColuna++;
                        }
                        break;

                    default:
                    break;
                }
            break;
            
            case 27:
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
            case 19:
                printf("Fim da inserção de texto:\n");
                setbuf(stdin, NULL);
                printf("\n\n");
                imprimirLista(pagina->inicio);
                salvarArquivo(nomeArquivo, pagina);
                return 0;

            default:
                // x++;
                putc(caractere, stdout);
                inserirCaractereLinha(linhaAtual, caractere, &posicaoFinalEscrita, &posicaoAtualColuna);
                if(posicaoAtualColuna == larguraTerminal-1){
                    posicaoAtualColuna = 0;
                    posicaoAtualLinha++;
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