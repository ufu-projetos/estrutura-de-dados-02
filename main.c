#include <stdio.h>
#include "Grafo.h"
#include <unistd.h>

// Menu do terminal
Grafo *menu(Grafo *G, int opcao) {
    switch(opcao) {
        case 1:
            int eh_ponderado, eh_digrafo, selecao;

            printf("\n[?] Deseja criar um grafo manulamente ou carregar um arquivo?\n");
            printf("\1 - Criar manualmente\2 - Carregar arquivo\n\n> ");

            // Opcao de criar manualmente ou carregar arquivo
            do {
                printf("\n> ");
                scanf("%d", &selecao);

                if(selecao != 1 && selecao != 2) {
                    printf("\n[!] Opcao invalida! Digite novamente uma opcao: \n");
                }
            } while (selecao != 1 && selecao != 2);

            if(G == NULL) {
                printf("\n[!] Erro ao criar grafo!\n");
            } else {
                printf("\nGrafo criado com sucesso!\n");
            }



            break;
        
        case 2:
            if(G == NULL) {
                printf("\n[!] Nao eh possivel adicionar aresta. O grafo nao foi criado ou teve problemas na criacao.\nTente novamente.\n");
            } else {
                int origem, destino, peso;
                printf("\nDigite a origem da aresta: ");
                scanf("%d", &origem);
                printf("Digite o destino da aresta: ");
                scanf("%d", &destino);

                if(G->eh_ponderado) {
                    printf("Digite o peso da aresta: ");
                    scanf("%d", &peso);
                } else {
                    peso = 0;
                }

                if(inserirAresta(G, origem, destino, peso)) {
                    printf("\nAresta inserida com sucesso!\n");
                } else {
                    printf("\n[!] Erro ao inserir aresta!\n");
                }
            }

            break;

        default:
            printf("Opcao invalida! Digite novamente uma opcao: \n");
            break;
    }

    return G;

}

void encerrar(Grafo *G) {
    printf("Saindo do programa...\n");
    if(G != NULL) {
        liberarGrafo(G);
    }

    // Efeito loading
    int counter = 0;
    while(counter < 90) {
        printf("*");
        fflush(stdout);
        usleep(20000);
        counter++;
    }

    printf("\n\nPrograma encerrado com exito! :)\n\n");
}

int main() {
    Grafo *G = NULL; // Grafo a ser criado
    int opcao; // Opcao do menu

    do {
        printf("\n\t\t --- ANALISE DE GRAFOS ---\n\n");
        printf("Escolha uma opcao abaixo:\n");

        printf("1 - Criar/carregar novo grafo\n");
        printf("2 - Inserir aresta\n");
        printf("3 - Remover aresta\n");
        printf("4 - Verificar grau de um vertice\n");
        printf("5 - Verificar grau medio do grafo\n");
        printf("6 - Verificar grau maximo do grafo\n");
        printf("7 - Imprimir grafo\n");
        printf("0 - Sair\n");
        
        printf("\n\n> ");
        scanf("%d", &opcao);

        if(opcao)
            G = menu(G, opcao);
    } while(opcao);
    
    encerrar(G);

    return 0;
}