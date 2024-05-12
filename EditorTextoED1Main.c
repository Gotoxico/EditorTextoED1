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
            abrirArquivo(nomeArquivo, pagina);
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
                    inserirCaractereLinha(linhaAtual, ' ' );
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
                
                break;
            case 8:
                //Apagar caractere terminal
                //Caso esteja no começo de uma linha, pular para o final da linha acima

                if(posicaoAtualColuna == 0){
                    printf("\033[1F");
                    printf("\033[%dC", larguraTerminal);
                    printf(" ");
                    posicaoAtualColuna = larguraTerminal-1;
                    apagar(pagina, posicaoAtualColuna, posicaoAtualLinha);
                    break;
                }
               
                //Apagar normal
                printf("\b");
                printf(" ");
                printf("\033[1D");
                if(posicaoAtualColuna != 0) posicaoAtualColuna--;
                apagar(pagina, posicaoAtualColuna, posicaoAtualLinha);

                break;

            case -32:
                auxiliar = getch();
                switch(auxiliar){
                    case 72:
                        // moverLinhaCima(linhaAtual);
                        printf("\033[1A");
                        if(posicaoAtualLinha!=0) posicaoAtualLinha--;
                        break;
                    case 80:
                        // moverLinhaBaixo(linhaAtual);
                        printf("\033[1B");
                        posicaoAtualLinha++;
                        break;
                    case 75:
                        // moverCaractereEsquerda(linhaAtual);
                        printf("\033[1D");
                        if(posicaoAtualColuna == 0){
                            printf("\033[1F");
                            printf("\033[%dC", larguraTerminal);
                            posicaoAtualColuna = larguraTerminal-1;
                            if(posicaoAtualLinha!=0) posicaoAtualLinha--;
                        }
                        else{
                            posicaoAtualColuna--;
                        }
                        
                        break;
                    case 77:
                        // moverCaractereDireita(linhaAtual);
                        printf("\033[1C");
                        if(posicaoAtualColuna == larguraTerminal-1){
                            printf("\033[1E");
                            posicaoAtualColuna = 0;
                            posicaoAtualLinha++;
                        }
                        else{
                            posicaoAtualColuna++;
                        }

                        break;

                    default:
                    break;
                }
            break;
            
            /*case 27:
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
            */

            //CTRL+P = 16 (Salvar e sair)
            case 16:
                printf("Fim da inserção de texto:\n");
                setbuf(stdin, NULL);
                printf("\n\n");
                imprimirLista(pagina->inicio);
                salvarArquivo(nomeArquivo, pagina);
                return 0;

            default:
                // x++;
                inserirTexto(nomeArquivo, caractere);
                inserirCaractereLinha(linhaAtual, caractere);
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