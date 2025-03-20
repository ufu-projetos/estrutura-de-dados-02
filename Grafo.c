#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "Grafo.h"

struct no {
    int rotulo;
    double peso;
    struct no *proximo;
};

struct grafo {
    int eh_ponderado;
    int eh_digrafo;
    int numero_vertices;
    No **arestas;
    int *grau;
    int *label;
};

Grafo *criarGrafo(int numero_vertices, int eh_ponderado, int eh_digrafo) {
    Grafo *G = (Grafo *) malloc(sizeof(Grafo));
    if(G == NULL) {
        printf("[!] Nao foi possivel criar o grafo!");
        exit(1);
    }
    G->numero_vertices = numero_vertices;
    G->eh_ponderado = eh_ponderado ? 1 : 0;
    G->eh_digrafo = eh_digrafo ? 1 : 0;
    G->grau = (int *) calloc(numero_vertices, sizeof(int));
    G->arestas = (No **) malloc(numero_vertices * sizeof(No *));
    for(int i = 0; i < numero_vertices; i++) {
        G->arestas[i] = NULL;
    }
    G->label = (char **) calloc(numero_vertices, sizeof(char *));
    
    return G;
}

int liberarGrafo(Grafo *G) {
    if(G == NULL) {
        printf("Nao foi possivel liberar o grafo!");
        return 0;
    }

    // Libera as arestas de cada vértice
    for(int i = 0; i < G->numero_vertices; i++) {
        No *atual = G->arestas[i];
        while(atual != NULL) {
            No *temp = atual;
            atual = atual->proximo;
            free(temp);  // Libera cada nó da lista
        }
    }
    
    free(G->arestas);  // Libera o vetor de arestas
    free(G->grau);     // Libera o vetor de graus
    free(G);           // Libera a estrutura do grafo

    return 1;
}

void imprimirGrafo(Grafo *G) {
    if(G == NULL) {
        printf("[!] Nao foi possivel imprimir o grafo - grafo nulo!");
        exit(1);
    }
    printf("\t\t\t\t\t\t\t\t\n\n ---- GRAFO IMPRESSO ----\n\n");

    for(int i = 0; i < G->numero_vertices; i++) {
        // Imprimi o label do vertice
        if(G->grau[i] > 0) {
            printf("\n[%s%d] -> ", (G->label[i] < 10) ? "0" : "", G->label[i]);
        }

        No *p = G->arestas[i];
        while(p != NULL) {
            printf("%d", p->rotulo);
            if(G->eh_ponderado) printf(" (%lf)", p->peso);
            if(p->proximo != NULL) printf(", ");
            
            p = p->proximo;
        }
    }
    printf("\n");
}

int inserirAresta(Grafo *G, int origem, int destino, double peso) {
    if(G == NULL) {
        printf("[!] Nao foi possivel inserir aresta - grafo nulo.");
        exit(1);
    }
    
    if((origem < 0 || origem >= G->numero_vertices) || (destino < 0 || destino >= G->numero_vertices)) {
        return 0;
    }

    No *aresta = malloc(sizeof(No));
    aresta->rotulo = destino;
    if(G->eh_ponderado) aresta->peso = peso;
    aresta->proximo = G->arestas[origem];
    G->arestas[origem] = aresta;
    G->grau[origem]++;
    G->label[origem] = origem;


    // verifica se ele nao eh digrafo
    if(!G->eh_digrafo) {
        if(origem < 0 || origem >= G->numero_vertices) return 0;
        if(destino < 0 || destino >= G->numero_vertices) return 0;

        No *aresta = malloc(sizeof(No));
        aresta->rotulo = origem;
        if(G->eh_ponderado) aresta->peso = peso;
        aresta->proximo = G->arestas[destino];
        G->arestas[destino] = aresta;
    
        G->grau[destino]++;
        G->label[destino] = destino;
    } 

    return 1;
}

int removerAresta(Grafo *G, int origem, int destino) {
    if(G == NULL) {
        perror("\n[!] Nao foi possivel remover aresta - grafo nulo.");
        exit(1);
    }

    if((origem < 0 || origem >= G->numero_vertices) || (destino < 0 || destino >= G->numero_vertices)) {
        return 0;
    }

    // Remove a aresta (origem, destino)
    No *atual = G->arestas[origem];
    No *anterior = NULL;
    
    while(atual != NULL) {
        if(atual->rotulo == destino) {
            if(anterior == NULL) {
                G->arestas[origem] = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            G->grau[origem]--;
            free(atual);
            break;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    // Se o grafo não for direcionado, remove a aresta (destino, origem)
    if(!G->eh_digrafo) {
        atual = G->arestas[destino];
        anterior = NULL;

        while(atual != NULL) {
            if(atual->rotulo == origem) {
                if(anterior == NULL) {
                    G->arestas[destino] = atual->proximo;
                } else {
                    anterior->proximo = atual->proximo;
                }
                G->grau[destino]--;
                free(atual);
                return 1;
            }
            anterior = atual;
            atual = atual->proximo;
        }
    }

    return 0;
}

int grau(Grafo *G, int vertice) {
    if(G == NULL) {
        printf("[!] Nao foi possivel verfificar o grau do vertice %d - grafo nulo.", vertice);
        exit(1);
    }

    // Verifica se o vértice é válido
    if(vertice < 0 || vertice >= G->numero_vertices) {
        return -1;
    }

    return G->grau[vertice];
}

float grauMedio(Grafo *G) {
    if(G == NULL) {
        printf("[!] Nao foi possivel calcular o grau medio - grafo nulo.");
        exit(1);
    }

    if(G->numero_vertices == 0) {
        return -1;
    }

    int soma = 0;
    for(int i = 0; i < G->numero_vertices; i++) {
        soma += G->grau[i];
    }

    float resultado = (float) soma / (float) G->numero_vertices;

    return resultado;
}

int grauMax(Grafo *G, int *vertice) {
    if(G == NULL) {
        printf("[!] Nao foi possivel calcular o grau maximo - grafo nulo.");
        exit(1);
    }

    if(G->numero_vertices == 0) {
        return -1;
    }

    int max = G->grau[0];
    *vertice = 0;
    for(int i = 0; i < G->numero_vertices; i++) {
        if(G->grau[i] > max) {
            max = G->grau[i];
            *vertice = i;
        }
    }
    
    return max;
}

// Verifica se o grafo eh ponderado
int grafoEhPonderado(Grafo *G) {
    if(G == NULL) {
        printf("[!] Nao foi possivel verificar se o grafo eh ponderado - grafo nulo.");
        exit(1);
    }

    return G->eh_ponderado;
}

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
            else {
                char nome_arquivo[100];
                printf("\nDigite o nome do arquivo: ");
                scanf("%s", nome_arquivo);

                G = carregarArquivo(G, nome_arquivo);
            }

            

            if(G == NULL) {
                printf("\n[!] Erro ao criar grafo!\n");
            } else {
                printf("\nGrafo criado com sucesso!\n");
            }

            // inserirAresta(G, 0, 1, 10);
            // inserirAresta(G, 0, 2, 15);
            // inserirAresta(G, 1, 2, 20);
            // inserirAresta(G, 1, 3, 25);
            // inserirAresta(G, 2, 3, 30);
            // inserirAresta(G, 2, 4, 35);
            // inserirAresta(G, 3, 4, 40);
            // inserirAresta(G, 3, 5, 45);
            // inserirAresta(G, 4, 5, 50);
            // inserirAresta(G, 4, 6, 55);
            // inserirAresta(G, 5, 6, 60);
            // inserirAresta(G, 5, 7, 65);
            // inserirAresta(G, 6, 7, 70);
            // inserirAresta(G, 0, 7, 75);
            // inserirAresta(G, 1, 5, 80);

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
        
        case 5:
            if(G == NULL) {
                printf("\n[!] Nao eh possivel verificar o grau medio do grafo. O grafo nao foi criado ou teve problemas na criacao.\nTente novamente.\n");
            } else {
                printf("\nO grau medio do grafo eh: %.2f\n", grauMedio(G));
            }

            break;
        
        case 6:
            int vertice = 0;
            if(G == NULL) {
                printf("\n[!] Nao eh possivel verificar o grau maximo do grafo. O grafo nao foi criado ou teve problemas na criacao.\nTente novamente.\n");
            } else {
                int grauMaximo = grauMax(G, &vertice);
                printf("\nO grau maximo do grafo eh: %d no vertice %d", grauMaximo, vertice);
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
            printf("\n[x] Opcao invalida! Digite novamente uma opcao: \n\n");
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

// Carrega o dataset de um arquivo
Grafo *carregarArquivo(Grafo *G, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("[!] Erro ao abrir o arquivo '%s'.\n", nome_arquivo);
        return NULL;
    }

    int origem, destino;
    double peso;
    int maior_vertice = -1;

    // Lê o arquivo linha por linha
    while (fscanf(arquivo, "%d %d %lf", &origem, &destino, &peso) == 3) {
        // Determina o maior índice de vértice
        if (origem > maior_vertice) maior_vertice = origem;
        if (destino > maior_vertice) maior_vertice = destino;

        // Se o grafo não foi criado ou precisa ser expandido
        if (G == NULL || maior_vertice >= G->numero_vertices) {
            int novo_tamanho = maior_vertice + 1; // Ajusta o tamanho para acomodar o maior vértice
            if (G == NULL) {
                G = criarGrafo(novo_tamanho, 1, 0); // Cria um grafo inicial (ponderado e não direcionado)
            } else {
                // Expande o grafo dinamicamente
                G->grau = (int *) realloc(G->grau, novo_tamanho * sizeof(int));
                G->arestas = (No **) realloc(G->arestas, novo_tamanho * sizeof(No *));
                for (int i = G->numero_vertices; i < novo_tamanho; i++) {
                    G->grau[i] = 0;
                    G->arestas[i] = NULL;
                }
                G->numero_vertices = novo_tamanho;
            }
        }

        // Insere a aresta no grafo
        inserirAresta(G, origem, destino, peso);
    }

    fclose(arquivo);
    printf("[*] Arquivo '%s' carregado com sucesso.\n", nome_arquivo);
    return G;
}

int insereVertice(Grafo *G, int label) {
    if(G == NULL) {
        printf("[!] Nao foi possivel inserir vertice - grafo nulo.");
        exit(1);
    }

    

    return 1;
}