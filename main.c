#include <stdio.h>
#include "funcoes.h"


int main() {
    ListaPacientes lista;
    lista.quantidade = 0;

    FilaAtendimento fila;
    fila.head = NULL;
    fila.tail = NULL;
    fila.qtde = 0;


    ABB arvore;
    arvore.raiz = NULL;
    arvore.qtde = 0;

    NoPilha *topo = NULL; 

    int opcao;
    do {
        int cod;
        menu();
        printf("--------------------------\n");
        scanf("%d", &opcao);
        fflush(stdin);

        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif

        switch (opcao) {
            case 1:
                cadastrar(&lista, &arvore);
                break;
            case 2:
                atendimento(&lista, &fila, &topo);  
                break;
            case 3:
                pesquisar(&arvore);
                break;
            case 4:  
                desfazer_operacao(&fila, &topo);
                break;
            case 5:

                cod = lerarquivo(&lista,&arvore);
                if (cod == 1) {
                    printf("Não foi possível ler o arquivo!\n");
                }
                break;
            case 6:
                salvararquivo(&lista);
                break;
            case 7:
                printf("Projeto Desenvolvido pelos Alunos:\n");
                printf("Gustavo Mendes Franco Lapin Atui RA:24.123.072-1\n");
                printf("Renan Casemiro Hessel RA:24.123.019-2\n");
                printf("Alunos de Ciência da Computação 4 Ciclo\n");
                printf("Projeto de Atendimento Hospitalar, utilizando lista,fila,pilha e arvore binaria\n");
                printf("Projeto da Materia Estrutura de Dados\n");
                printf("Professor: Luciano Rossi\n");
                printf("Data de Entrega: 08/11/2024\n");
                break;
            case 8:
                printf("Saindo do programa.\n");
                break;
            default:
                printf("Opção inválida.\n");
                break;
        }
    } while (opcao != 8);

    return 0;
}