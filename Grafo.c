#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "Grafo.h"
#include <limits.h>
#include <stdint.h>

struct no {
    int rotulo; // id do vertice adjacente
    double peso;
    struct no *proximo;
};

struct grafo {
    int eh_ponderado;
    int eh_digrafo;
    int numero_vertices;
    No **arestas;
    int *grau;
    int *label; // id do vertice
};

Grafo *criarGrafo(int numero_vertices, int eh_ponderado, int eh_digrafo) {
    Grafo *G = (Grafo *)malloc(sizeof(Grafo));
    if (G == NULL) {
        printf("[!] Nao foi possivel criar o grafo!\n");
        exit(1);
    }

    G->numero_vertices = numero_vertices;
    G->eh_ponderado = eh_ponderado ? 1 : 0;
    G->eh_digrafo = eh_digrafo ? 1 : 0;
    G->grau = (int *)calloc(numero_vertices, sizeof(int));
    G->arestas = (No **)malloc(numero_vertices * sizeof(No *));
    G->label = (int *)malloc(numero_vertices * sizeof(int));

    for (int i = 0; i < numero_vertices; i++) {
        G->arestas[i] = NULL;
        G->grau[i] = 0;
        G->label[i] = i; // Inicializa o label com o índice do vértice
    }

    return G;
}

int liberarGrafo(Grafo *G) {
    if(G == NULL) {
        printf("Nao foi possivel liberar o grafo!");
        return 0;
    }

    // Libera as arestas de cada vertice
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
        printf("\n[%s%d] -> ", (G->label[i] < 10) ? "0" : "", G->label[i]);

        No *p = G->arestas[i];
        while(p != NULL) {
            printf("%d", p->rotulo);
            if(G->eh_ponderado) printf(" (%lf)", p->peso);
            if(p->proximo != NULL) printf(", ");
            
            p = p->proximo;
        }
        printf("\n");
    }
    printf("\n");
}

int inserirAresta(Grafo *G, int origem, int destino, double peso) {
    if (G == NULL) {
        printf("[!] Nao foi possivel inserir aresta - grafo nulo.\n");
        exit(1);
    }

    if ((origem < 0 || origem >= G->numero_vertices) || (destino < 0 || destino >= G->numero_vertices)) {
        return 0;
    }

    No *aresta = malloc(sizeof(No));
    aresta->rotulo = destino;
    if (G->eh_ponderado) aresta->peso = peso;
    aresta->proximo = G->arestas[origem];
    G->arestas[origem] = aresta;
    G->grau[origem]++;

    // Verifica se o grafo não é direcionado
    if (!G->eh_digrafo) {
        No *aresta_inversa = malloc(sizeof(No));
        aresta_inversa->rotulo = origem;
        if (G->eh_ponderado) aresta_inversa->peso = peso;
        aresta_inversa->proximo = G->arestas[destino];
        G->arestas[destino] = aresta_inversa;
        G->grau[destino]++;
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

    // Se o grafo nao for direcionado, remove a aresta (destino, origem)
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

    // verifica se o vertice e válido
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

// Verifica se o grafo e ponderado
int grafoEhPonderado(Grafo *G) {
    if(G == NULL) {
        printf("[!] Nao foi possivel verificar se o grafo e ponderado - grafo nulo.");
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

            // ---------- Criacao manual -----------
            if(selecao == 1) {
                // Opcao de grafo ponderado ou nao ponderado
                do {
                    printf("\n[?] O grafo e ponderado? (1 - Sim, 0 - Nao)\n> ");
                    scanf("%d", &eh_ponderado);
    
                    if(eh_ponderado != 0 && eh_ponderado != 1) {
                        printf("\n[!] Opcao invalida! Digite novamente uma opcao: \n");
                    }
                } while(eh_ponderado != 0 && eh_ponderado != 1);
    
                // Opcao de grafo direcionado ou nao direcionado
                do {
                    printf("\n[?] O grafo e direcionado? (1 - Sim, 0 - Nao)\n> ");
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
                // Se o grafo e ponderado ou nao
                do {
                    printf("\n[?] O grafo e ponderado? (1 - Sim, 0 - Nao)\n> ");
                    scanf("%d", &eh_ponderado);
    
                    if(eh_ponderado != 0 && eh_ponderado != 1) {
                        printf("\n[!] Opcao invalida! Digite novamente uma opcao: \n");
                    }
                } while(eh_ponderado != 0 && eh_ponderado != 1);

                char nome_arquivo[100];
                printf("\nDigite o nome do arquivo: ");
                scanf("%s", nome_arquivo);

                G = carregarArquivo(G, nome_arquivo, (eh_ponderado == 1) ? 1 : 0);
            }

            

            if(G == NULL) {
                printf("\n[!] Erro ao criar grafo!\n");
            } else {
                printf("\nGrafo criado com sucesso!\n");
            }

            break;
        
        case 2:
            if(G == NULL) {
                printf("\n[!] O grafo ainda nao foi criado!\n");
            } else {
                visaoGeral(G);
            }

            break;
        
        case 3:
            if(G == NULL) {
                printf("\n[!] Nao e possivel adicionar aresta. O grafo nao foi criado ou teve problemas na criacao.\nTente novamente.\n");
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
        
        case 4:
            if(G == NULL) {
                printf("\n[!] Nao e possivel remover aresta. O grafo nao foi criado ou teve problemas na criacao.\nTente novamente.\n");
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
        
        case 5:
            if(G == NULL) {
                printf("\n[!] Nao e possivel verificar o grau de um vertice. O grafo nao foi criado ou teve problemas na criacao.\nTente novamente.\n");
            } else {
                int vertice;
                printf("\nDigite o vertice que deseja verificar o grau: ");
                scanf("%d", &vertice);

                int grau_vertice = grau(G, vertice);
                if(grau_vertice == -1) {
                    printf("\n[!] Vertice invalido!\n");
                } else {
                    printf("\nO grau do vertice %d e: %d\n", vertice, grau_vertice);
                }
            }

            break;
        
        case 6:
            if(G == NULL) {
                printf("\n[!] Nao e possivel verificar o grau medio do grafo. O grafo nao foi criado ou teve problemas na criacao.\nTente novamente.\n");
            } else {
                printf("\nO grau medio do grafo e: %.2f\n", grauMedio(G));
            }

            break;
        
        case 7:
            int vertice = 0;
            if(G == NULL) {
                printf("\n[!] Nao e possivel verificar o grau maximo do grafo. O grafo nao foi criado ou teve problemas na criacao.\nTente novamente.\n");
            } else {
                int grauMaximo = grauMax(G, &vertice);
                printf("\nO grau maximo do grafo e: %d no vertice %d", grauMaximo, vertice);
            }

            break;

        case 8:
            if(G == NULL) {
                printf("\n[!] Nao e possivel imprimir o grafo. O grafo nao foi criado ou teve problemas na criacao.\nTente novamente.\n");
            } else {
                imprimirGrafo(G);
            }

            break;
        
        case 9:
            if(G == NULL) {
                printf("\n[!] Nao e possivel adicionar um novo vertice ao grafo. O grafo nao foi criado ou teve problemas na criacao.\nTente novamente.\n");
            } else {
                do {
                    printf("\nDigite o vertice que deseja adicionar: ");
                    scanf("%d", &vertice);

                    if(vertice < 0) {
                        printf("\n[!] Vertice invalido! Digite um vertice valido.\n");
                        continue;
                    }
                } while(vertice < 0);

                inserirVertice(G, vertice);
            }

        break;

        case 10:
            if(G == NULL) {
                printf("\n[!] Nao e possivel verificar as componentes conexas. O grafo nao foi criado ou teve problemas na criacao.\nTente novamente.\n");
            } else {
                int num_componentes, maior_componente;
                componentesConexas(G, &num_componentes, &maior_componente);
                printf("\nO grafo possui %d componentes conexas e a maior componente tem tamanho %d.\n", num_componentes, maior_componente);
            }

            break;
        
        case 11:
            if(G == NULL) {
                printf("\n[!] Nao e possivel verificar o menor caminho medio. O grafo nao foi criado ou teve problemas na criacao.\nTente novamente.\n");
            } else {
                double menor_caminho_medio = menorCaminhoMedio(G);
                if(menor_caminho_medio == -1) {
                    printf("\n[!] Nao foi possivel calcular o menor caminho medio.\n");
                } else {
                    printf("\nO menor caminho medio do grafo e: %lf\n", menor_caminho_medio);
                }
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

// Carrega um arquivo de um grafo
Grafo *carregarArquivo(Grafo *G, const char *nome_arquivo, int eh_ponderado) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("[!] Erro ao abrir o arquivo '%s'.\n", nome_arquivo);
        return NULL;
    }

    char primeira_linha[256];
    if (fgets(primeira_linha, sizeof(primeira_linha), arquivo) != NULL && primeira_linha[0] == '%') {
        printf("[!] Este dataset possivelmente esta fora do padrao.\n");
    }
    rewind(arquivo);

    int origem, destino;
    double peso;
    int maior_vertice = -1;
    int eh_digrafo = 0;
    int tem_peso = 0;

    while (fscanf(arquivo, "%d %d", &origem, &destino) == 2) {
        if (fscanf(arquivo, "%lf", &peso) == 1) {
            tem_peso = 1;
        } else {
            fseek(arquivo, -1, SEEK_CUR);
            char c;
            while ((c = fgetc(arquivo)) != '\n' && c != EOF); 
            peso = 1.0; 
        }


        if (origem > maior_vertice) maior_vertice = origem;
        if (destino > maior_vertice) maior_vertice = destino;

    }

    rewind(arquivo);

    if (G == NULL) {
        G = criarGrafo(maior_vertice + 1, tem_peso, eh_digrafo);
    }

    G->eh_ponderado = tem_peso;

    rewind(arquivo);
    while (fscanf(arquivo, "%d %d", &origem, &destino) == 2) {
        if (tem_peso) {
           if (fscanf(arquivo, "%lf", &peso) != 1) {
               printf("Erro ao ler o peso da aresta (%d, %d).\n", origem, destino);
               fclose(arquivo);
               return NULL;
            }
        } else {
            peso = 1.0;
            char c;
            while ((c = fgetc(arquivo)) != '\n' && c != EOF); // Avanca ate o final da linha atual
        }
        inserirAresta(G, origem, destino, peso);
    }


    fclose(arquivo);

    G->eh_ponderado = eh_ponderado;

    printf("\n\n[*] Arquivo '%s' carregado com sucesso. Grafo %s e %s.\n", nome_arquivo, eh_digrafo ? "direcionado" : "nao direcionado", G->eh_ponderado ? "ponderado" : "nao ponderado");
    return G;
}


int inserirVertice(Grafo *G, int vertice) {
    if (G == NULL) {
        printf("[!] Nao foi possivel inserir vertice - grafo nulo.\n");
        exit(1);
    }

    // Verifica se o vertice já está dentro do tamanho atual
    if (vertice < G->numero_vertices) {
        printf("[*] O vertice %d já existe no grafo.\n", vertice);
        return 1;
    }

    // Calcula o novo tamanho do grafo (aumenta em ate 50%)
    int novo_tamanho = G->numero_vertices++;
    if (vertice >= novo_tamanho) {
        novo_tamanho = vertice + 1; // Garante que o novo tamanho acomode o vertice
    }

    // Realoca memória para os vetores do grafo
    G->grau = (int *) realloc(G->grau, novo_tamanho * sizeof(int));
    G->arestas = (No **) realloc(G->arestas, novo_tamanho * sizeof(No *));
    G->label = (int *) realloc(G->label, novo_tamanho * sizeof(int));

    if (G->grau == NULL || G->arestas == NULL || G->label == NULL) {
        printf("[!] Erro ao realocar memória para o grafo.\n");
        exit(1);
    }

    // Inicializa os novos vertices
    for (int i = G->numero_vertices; i < novo_tamanho; i++) {
        G->grau[i] = 0;
        G->arestas[i] = NULL;
        G->label[i] = i; // Define o rótulo como o indice do vertice
    }

    G->numero_vertices = novo_tamanho; // Atualiza o número de vertices
    printf("[*] O grafo foi expandido para %d vertices.\n", G->numero_vertices);

    return 1;
}

// Funcao auxiliar para DFS
void dfs(Grafo *G, int vertice, int *visitado, int *tamanho_componente) {
    visitado[vertice] = 1;
    (*tamanho_componente)++;

    No *adjacente = G->arestas[vertice];
    while (adjacente != NULL) {
        if (!visitado[adjacente->rotulo]) {
            dfs(G, adjacente->rotulo, visitado, tamanho_componente);
        }
        adjacente = adjacente->proximo;
    }
}

// Determina o número de componentes conexas e o tamanho da maior componente
void componentesConexas(Grafo *G, int *num_componentes, int *maior_componente) {
    if (G == NULL) {
        printf("[!] Grafo nulo.\n");
        return;
    }

    int *visitado = (int *)calloc(G->numero_vertices, sizeof(int));
    *num_componentes = 0;
    *maior_componente = 0;

    for (int i = 0; i < G->numero_vertices; i++) {
        if (!visitado[i]) {
            int tamanho_componente = 0;
            dfs(G, i, visitado, &tamanho_componente);
            (*num_componentes)++;
            if (tamanho_componente > *maior_componente) {
                *maior_componente = tamanho_componente;
            }
        }
    }

    free(visitado);
}

// Algoritmo de Dijkstra para encontrar o menor caminho de um vertice
void dijkstra(Grafo *G, int origem, double *dist) {
    int *visitado = (int *)calloc(G->numero_vertices, sizeof(int));

    for (int i = 0; i < G->numero_vertices; i++) {
        dist[i] = INT_MAX;
    }
    dist[origem] = 0;

    for (int i = 0; i < G->numero_vertices; i++) {
        int u = -1;
        for (int j = 0; j < G->numero_vertices; j++) {
            if (!visitado[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }

        if (dist[u] == INT_MAX) break;

        visitado[u] = 1;

        No *adjacente = G->arestas[u];
        while (adjacente != NULL) {
            if (dist[u] + adjacente->peso < dist[adjacente->rotulo]) {
                dist[adjacente->rotulo] = dist[u] + adjacente->peso;
            }
            adjacente = adjacente->proximo;
        }
    }

    free(visitado);
}

// Calcula o menor caminho medio do grafo
double menorCaminhoMedio(Grafo *G) {
    if (G == NULL) {
        printf("[!] Grafo nulo.\n");
        return -1;
    }

    double soma_caminhos = 0;
    int total_caminhos = 0;

    for (int i = 0; i < G->numero_vertices; i++) {
        double *dist = (double *)malloc(G->numero_vertices * sizeof(double));
        dijkstra(G, i, dist);

        for (int j = 0; j < G->numero_vertices; j++) {
            if (i != j && dist[j] != INT_MAX) {
                soma_caminhos += dist[j];
                total_caminhos++;
            }
        }

        free(dist);
    }

    if (total_caminhos == 0) return -1;

    return soma_caminhos / total_caminhos;
}

// Visao geral do grafo
void visaoGeral(Grafo *G) {
    if (G == NULL) {
        printf("[!] O grafo ainda nao foi criado.\n");
        return;
    }

    printf("\n\t\t\t\t\t\t--- Visao Geral do Grafo ---\n\n\n");
    printf("[-] Numero de vertices: %d\n", G->numero_vertices);
    printf("[-] Grafo %s\n", G->eh_digrafo ? "direcionado" : "nao direcionado");
    printf("[-] Grafo %s\n", G->eh_ponderado ? "ponderado" : "nao ponderado");
    printf("[-] Grau medio do grafo: %.2f\n", grauMedio(G));
    int vertice;
    int grau = grauMax(G, &vertice);
    printf("[-] Grau maximo do grafo: %d no vertice %d\n", grau, vertice);

    int num_componentes = 0, maior_componente = 0;
    componentesConexas(G, &num_componentes, &maior_componente);
    printf("[-] Numero de componentes conexas: %d\n", num_componentes);
    printf("[-] Tamanho da maior componente: %d\n", maior_componente);

    double caminho_medio = menorCaminhoMedio(G);
    if (caminho_medio < 0) {
        printf("[-] Menor caminho medio: Nao aplicavel (grafo desconexo).\n");
    } else {
        printf("[-] Menor caminho medio: %.2f\n", caminho_medio);
    }
}