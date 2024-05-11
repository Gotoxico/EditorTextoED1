#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
//#include <curses.h>
#include "EditorTextoED1.h"
#include <locale.h>


int main(){
    setlocale(LC_ALL, "Portuguese");
    //initscr();
    //cbreak();
    //keypad(stdscr, TRUE);e
    // PAGINA * pagina = iniciarPagina();
    // LINHA  * linhaAtual = pagina->inicio;
    clearScreen();

    //Chama o menu de opções do editor para o usuário escolher o que deseja fazer, se deseja abrir um arquivo ou criar um novo.
    int opcao = Menu();
    
    //int x = 0, y= 0;
    //gotoxy(x,y);
    char caractere, auxiliar;
    int fim;
    char nomeArquivo[100];

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
            break;
        
        default:
            break;
    }


    //Falta implementar a função de abrir um arquivo existente para edição
    while(1){
        caractere = getch();

        switch(caractere){
            case 13:
                //pularLinha();
                break;
            case 8:
                apagarTexto(nomeArquivo);
                break;

            case -32:
                auxiliar = getch();
                switch(auxiliar){
                    case 72:
                        // moverLinhaCima(linhaAtual);
                        printf("\033[1A");
                        break;
                    case 80:
                        // moverLinhaBaixo(linhaAtual);
                        printf("\033[1B");
                        break;
                    case 75:
                        // moverCaractereEsquerda(linhaAtual);
                        printf("\033[1D");
                        break;
                    case 77:
                        // moverCaractereDireita(linhaAtual);
                        printf("\033[1C");
                        break;

                    default:
                    break;
                }
            break;
            
            case 27:
                printf("\nFim da inserção de texto:\n1- Imprimir Lista e Salvar;\n2-Sair\nOpção: ");
                scanf("%d", &fim);
                if(fim == 1){
                    printf("\n\n");
                    //imprimirLista(pagina->inicio);
                }   
                else{
                    return 0;
                }
                
                break;

            default:
                inserirTexto(nomeArquivo, caractere);
                setbuf(stdin, NULL);
                break;
        }
      
    }
      
    return 0;
}