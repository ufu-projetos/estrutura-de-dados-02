#include <stdio.h>
#include "Grafo.h"
#include <unistd.h>
#include <ctype.h>

// Menu do terminal
Grafo *menu(Grafo *G, int opcao) {
    switch(opcao) {
        case 1:
            // Caso o grafo ja tenha sido criado
            if(G != NULL) {
                printf("\n[!] O grafo já foi criado!\n");
                break;
            }

            int selecao, eh_ponderado, eh_digrafo, numero_vertices;

            printf("\n[?] Deseja criar um grafo manulamente ou carregar um arquivo?\n");
            printf("\n1 - Criar manualmente\n2 - Carregar arquivo\n");

            // Opcao de criar manualmente ou carregar arquivo
            do {
                printf("\n> ");
                scanf("%d", &selecao);

                if(selecao != 1 && selecao != 2) {
                    printf("\n[!] Opcao invalida! Digite novamente uma opcao: \n");
                }
            } while (selecao != 1 && selecao != 2);

            // ---------- Criação manual -----------
            if(selecao == 1) {
                // Opcao de grafo ponderado ou nao ponderado
                do {
                    printf("\n[?] O grafo eh ponderado? (1 - Sim, 0 - Nao)\n> ");
                    scanf("%d", &eh_ponderado);
    
                    if(eh_ponderado != 0 && eh_ponderado != 1) {
                        printf("\n[!] Opcao invalida! Digite novamente uma opcao: \n");
                    }
                } while(eh_ponderado != 0 && eh_ponderado != 1);
    
                // Opcao de grafo direcionado ou nao direcionado
                do {
                    printf("\n[?] O grafo eh direcionado? (1 - Sim, 0 - Nao)\n> ");
                    scanf("%d", &eh_digrafo);
    
                    if(eh_digrafo != 0 && eh_digrafo != 1) {
                        printf("\n[!] Opcao invalida! Digite novamente uma opcao: \n");
                    }
                } while(eh_digrafo != 0 && eh_digrafo != 1);

                // Numero de vertices do grafo
                do {
                    printf("\n[*] Digite o numero de vertices do grafo: ");
                    scanf("%d", &numero_vertices);
    
                    if(numero_vertices <= 0) {
                        printf("\n[!] Numero de vertices invalido! Digite novamente um numero valido: \n");
                    }
                } while(numero_vertices <= 0);

                G = criarGrafo(numero_vertices, eh_ponderado, eh_digrafo);
            }
            // ------------ Carregar arquivo --------------
            // else {
            //     char nome_arquivo[100];
            //     printf("\nDigite o nome do arquivo: ");
            //     scanf("%s", nome_arquivo);

            //     G = carregarArquivo(nome_arquivo);
            // }

            

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

                if(grafoEhPonderado(G)) {
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
        
        case 3:
            if(G == NULL) {
                printf("\n[!] Nao eh possivel remover aresta. O grafo nao foi criado ou teve problemas na criacao.\nTente novamente.\n");
            } else {
                int origem, destino;
                printf("\nDigite a origem da aresta: ");
                scanf("%d", &origem);
                printf("Digite o destino da aresta: ");
                scanf("%d", &destino);

                if(removerAresta(G, origem, destino)) {
                    printf("\nAresta removida com sucesso!\n");
                } else {
                    printf("\n[!] Erro ao remover aresta!\n");
                }
            }

            break;
        
            case 4:
                if(G == NULL) {
                    printf("\n[!] Nao eh possivel verificar o grau de um vertice. O grafo nao foi criado ou teve problemas na criacao.\nTente novamente.\n");
                } else {
                    int vertice;
                    printf("\nDigite o vertice que deseja verificar o grau: ");
                    scanf("%d", &vertice);

                    int grau_vertice = grau(G, vertice);
                    if(grau_vertice == -1) {
                        printf("\n[!] Vertice invalido!\n");
                    } else {
                        printf("\nO grau do vertice %d eh: %d\n", vertice, grau_vertice);
                    }
                }

                break;

        case 7:
            if(G == NULL) {
                printf("\n[!] Nao eh possivel imprimir o grafo. O grafo nao foi criado ou teve problemas na criacao.\nTente novamente.\n");
            } else {
                imprimirGrafo(G);
            }

            break;

        default:
            printf("Opcao invalida! Digite novamente uma opcao: \n");
            break;
    }

    return G;

}

// Encerra o programa
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
        printf("Escolha uma opcao abaixo:\n\n");

        if(G == NULL) {
            printf("\n[!] Ainda nao ha nenhum grafo criado!");
            printf("\n\n1 - Criar/carregar novo grafo\n");
        } else {
            printf("2 - Inserir aresta\n");
            printf("3 - Remover aresta\n");
            printf("4 - Verificar grau de um vertice\n");
            printf("5 - Verificar grau medio do grafo\n");
            printf("6 - Verificar grau maximo do grafo\n");
            printf("7 - Imprimir grafo\n");
            printf("0 - Sair\n");
        }

        printf("\n\n> ");
        if(scanf("%d", &opcao) != 1) {
            printf("\n[!] Entrada inválida! Digite apenas números.\n\n\n");
            while(getchar() != '\n');
            opcao = 0;
            break;
        }

        if(opcao)
            G = menu(G, opcao);
    } while(opcao);
    
    encerrar(G);

    return 0;
}