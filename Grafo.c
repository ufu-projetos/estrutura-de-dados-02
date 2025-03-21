#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "Grafo.h"
#include <limits.h>
#include <stdint.h>

struct mapeamento {
    int original;
    int index;
};

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
    Mapeamento *map; // mapeia os rotulos dos vertices
};

Grafo *criarGrafo(int numero_vertices, int eh_ponderado, int eh_digrafo) {
    Grafo *G = (Grafo *)malloc(sizeof(Grafo));
    if (!G) {
        printf("[!] Nao foi possivel criar o grafo!\n");
        exit(1);
    }

    G->numero_vertices = numero_vertices;
    G->eh_ponderado = eh_ponderado;
    G->eh_digrafo = eh_digrafo;
    G->grau = (int *)calloc(numero_vertices, sizeof(int));
    G->arestas = (No **)malloc(numero_vertices * sizeof(No *));
    G->map = (Mapeamento *)malloc(numero_vertices * sizeof(Mapeamento));

    if (!G->grau || !G->arestas || !G->map) {
        printf("[!] Falha na alocacao de memoria.\n");
        exit(1);
    }

    for (int i = 0; i < numero_vertices; i++) {
        G->arestas[i] = NULL;
        G->grau[i] = 0;
        G->map[i].original = -1;
        G->map[i].index = i;
    }

    return G;
}

int liberarGrafo(Grafo *G) {
    if(G == NULL) {
        printf("Nao foi possivel liberar o grafo!");
        return 0;
    }

    for(int i = 0; i < G->numero_vertices; i++) {
        No *atual = G->arestas[i];
        while(atual != NULL) {
            No *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
    
    free(G->arestas);
    free(G->grau);
    free(G->map);
    free(G);

    return 1;
}

void imprimirGrafo(Grafo *G) {
    if (G == NULL) {
        printf("[!] Nao foi possivel imprimir o grafo - grafo nulo!\n");
        return;
    }

    printf("\n ---- GRAFO IMPRESSO ----\n\n");

    for (int i = 0; i < G->numero_vertices; i++) {
        int rotuloOrigem = G->map[i].original;
        if (rotuloOrigem == -1) continue;

        printf("\n[%s%d] -> ", (rotuloOrigem < 10) ? "0" : "", rotuloOrigem);

        No *p = G->arestas[i];
        while (p != NULL) {
            int rotuloDestino = G->map[p->rotulo].original;
            printf("%d", rotuloDestino);
            if (G->eh_ponderado) printf(" (%lf)", p->peso);
            if (p->proximo != NULL) printf(", ");

            p = p->proximo;
        }
        printf("\n");
    }
    printf("\n");
}

int buscarIndice(Grafo *G, int rotulo) {
    for (int i = 0; i < G->numero_vertices; i++) {
        if (G->map[i].original == rotulo) {
            return i;
        }
    }
    return -1;
}

void associarRotulo(Grafo *G, int rotulo, int index) {
    G->map[index].original = rotulo;
}

int inserirAresta(Grafo *G, int origem, int destino, double peso) {
    if (!G) {
        printf("[!] Nao foi possivel inserir aresta - grafo nulo.\n");
        return 0;
    }

    int indexOrigem = buscarIndice(G, origem);
    int indexDestino = buscarIndice(G, destino);

    if (indexOrigem == -1) {
        indexOrigem = buscarIndice(G, origem);
        if(indexOrigem == -1){
             for(int i = 0; i < G->numero_vertices; ++i){
                if(G->map[i].original == -1){
                    indexOrigem = i;
                    associarRotulo(G, origem, i);
                    break;
                }
             }
        }
    }

    if (indexDestino == -1) {
        indexDestino = buscarIndice(G, destino);
        if(indexDestino == -1){
            for(int i = 0; i < G->numero_vertices; ++i){
                if(G->map[i].original == -1){
                    indexDestino = i;
                    associarRotulo(G, destino, i);
                    break;
                }
            }
        }
    }

    if (indexOrigem == -1 || indexDestino == -1) {
        printf("[!] Vertice nao encontrado: Origem %d, Destino %d.\n", origem, destino);
        return 0;
    }

    No *aresta = malloc(sizeof(No));
    if (!aresta) {
        printf("[!] Falha ao alocar memoria para a aresta.\n");
        return 0;
    }
    aresta->rotulo = indexDestino;
    aresta->peso = (G->eh_ponderado) ? peso : 1.0;
    aresta->proximo = G->arestas[indexOrigem];
    G->arestas[indexOrigem] = aresta;
    G->grau[indexOrigem]++;

    if (!G->eh_digrafo) {
        No *aresta_inversa = malloc(sizeof(No));
        if (!aresta_inversa) {
            printf("[!] Falha ao alocar memoria para a aresta inversa.\n");
            return 0;
        }
        aresta_inversa->rotulo = indexOrigem;
        aresta_inversa->peso = (G->eh_ponderado) ? peso : 1.0;
        aresta_inversa->proximo = G->arestas[indexDestino];
        G->arestas[indexDestino] = aresta_inversa;
        G->grau[indexDestino]++;
    }

    return 1;
}

int removerAresta(Grafo *G, int origem, int destino) {
   if(G == NULL) {
        perror("\n[!] Nao foi possivel remover aresta - grafo nulo.");
        exit(1);
    }

    int indexOrigem = buscarIndice(G, origem);
    int indexDestino = buscarIndice(G, destino);

    if (indexOrigem == -1 || indexDestino == -1) {
        printf("[!] Vertice nao encontrado: Origem %d, Destino %d.\n", origem, destino);
        return 0;
    }

    No *atual = G->arestas[indexOrigem];
    No *anterior = NULL;
    
    while(atual != NULL) {
        if(atual->rotulo == indexDestino) {
            if(anterior == NULL) {
                G->arestas[indexOrigem] = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            G->grau[indexOrigem]--;
            free(atual);
            break;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    if(!G->eh_digrafo) {
        atual = G->arestas[indexDestino];
        anterior = NULL;

        while(atual != NULL) {
            if(atual->rotulo == indexOrigem) {
                if(anterior == NULL) {
                    G->arestas[indexDestino] = atual->proximo;
                } else {
                    anterior->proximo = atual->proximo;
                }
                G->grau[indexDestino]--;
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

    int indexVertice = buscarIndice(G, vertice);

    if (indexVertice == -1) {
        printf("[!] Vertice nao encontrado: %d.\n", vertice);
        return -1;
    }
    
    return G->grau[indexVertice];
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
    *vertice = G->map[0].original;

    for(int i = 1; i < G->numero_vertices; i++) {
        if(G->grau[i] > max) {
            max = G->grau[i];
            *vertice = G->map[i].original;
        }
    }
    
    return max;
}

int grafoEhPonderado(Grafo *G) {
   if(G == NULL) {
        printf("[!] Nao foi possivel verificar se o grafo e ponderado - grafo nulo.");
        exit(1);
    }

    return G->eh_ponderado;
}

Grafo *menu(Grafo *G, int opcao) {
    int vertice;

    switch(opcao) {
        case 1:
            if(G != NULL) {
                printf("\n[!] O grafo já foi criado!\n");
                break;
            }

            int selecao, eh_ponderado, eh_digrafo, numero_vertices;

            printf("\n[?] Deseja criar um grafo manulamente ou carregar um arquivo?\n");
            printf("\n1 - Criar manualmente\n2 - Carregar arquivo\n");

            do {
                printf("\n> ");
                scanf("%d", &selecao);

                if(selecao != 1 && selecao != 2) {
                    printf("\n[!] Opcao invalida! Digite novamente uma opcao: \n");
                }
            } while (selecao != 1 && selecao != 2);

            if(selecao == 1) {
                do {
                    printf("\n[?] O grafo e ponderado? (1 - Sim, 0 - Nao)\n> ");
                    scanf("%d", &eh_ponderado);
    
                    if(eh_ponderado != 0 && eh_ponderado != 1) {
                        printf("\n[!] Opcao invalida! Digite novamente uma opcao: \n");
                    }
                } while(eh_ponderado != 0 && eh_ponderado != 1);
    
                do {
                    printf("\n[?] O grafo e direcionado? (1 - Sim, 0 - Nao)\n> ");
                    scanf("%d", &eh_digrafo);
    
                    if(eh_digrafo != 0 && eh_digrafo != 1) {
                        printf("\n[!] Opcao invalida! Digite novamente uma opcao: \n");
                    }
                } while(eh_digrafo != 0 && eh_digrafo != 1);

                do {
                    printf("\n[*] Digite o numero de vertices do grafo: ");
                    scanf("%d", &numero_vertices);
    
                    if(numero_vertices <= 0) {
                        printf("\n[!] Numero de vertices invalido! Digite novamente um numero valido: \n");
                    }
                } while(numero_vertices <= 0);

                G = criarGrafo(numero_vertices, eh_ponderado, eh_digrafo);

                for (int i = 0; i < numero_vertices; i++) {
                    associarRotulo(G, i, i);
                }
            }
            else {
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
                int origem, destino;
                double peso = 1.0;

                printf("\nDigite a origem da aresta: ");
                scanf("%d", &origem);
                printf("Digite o destino da aresta: ");
                scanf("%d", &destino);

                if(grafoEhPonderado(G)) {
                    printf("Digite o peso da aresta: ");
                    scanf("%lf", &peso);
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
            vertice = 0;
            if(G == NULL) {
                printf("\n[!] Nao e possivel verificar o grau maximo do grafo. O grafo nao foi criado ou teve problemas na criacao.\nTente novamente.\n");
            } else {
                int grauMaximo = grauMax(G, &vertice);
                printf("\nO grau maximo do grafo e: %d no vertice %d\n", grauMaximo, vertice);
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
                int vertice;
                printf("\nDigite o vertice que deseja adicionar: ");
                scanf("%d", &vertice);

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

void encerrar(Grafo *G) {
    printf("Saindo do programa...\n");
    if(G != NULL) {
        liberarGrafo(G);
    }

    int counter = 0;
    while(counter < 90) {
        printf("*");
        fflush(stdout);
        usleep(20000);
        counter++;
    }

    printf("\n\nPrograma encerrado com exito! :)\n\n");
}

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
            while ((c = fgetc(arquivo)) != '\n' && c != EOF);
        }
        inserirAresta(G, origem, destino, peso);
    }


    fclose(arquivo);

    G->eh_ponderado = eh_ponderado;

    printf("\n\n[*] Arquivo '%s' carregado com sucesso. Grafo %s e %s.\n", nome_arquivo, eh_digrafo ? "direcionado" : "nao direcionado", G->eh_ponderado ? "ponderado" : "nao ponderado");
    return G;
}

int inserirVertice(Grafo *G, int label) { 
    if (G == NULL) {
        printf("[!] Nao foi possivel inserir vertice - grafo nulo.\n");
        return 0;
    }

    if (buscarIndice(G, label) != -1) {
        printf("[*] O vertice %d já existe no grafo.\n", label);
        return 1;
    }

    int novo_tamanho = G->numero_vertices + 1;
    G->grau = (int *) realloc(G->grau, novo_tamanho * sizeof(int));
    G->arestas = (No **) realloc(G->arestas, novo_tamanho * sizeof(No *));
    G->map = (Mapeamento *) realloc(G->map, novo_tamanho * sizeof(Mapeamento));

    if (G->grau == NULL || G->arestas == NULL || G->map == NULL) {
        printf("[!] Erro ao realocar memória para o grafo.\n");
        exit(1);
    }

    int novo_indice = G->numero_vertices; 
    G->grau[novo_indice] = 0;
    G->arestas[novo_indice] = NULL;
    G->map[novo_indice].original = label;
    G->map[novo_indice].index = novo_indice;

    G->numero_vertices++;
    printf("[*] O vértice %d foi adicionado ao grafo.\n", label);

    return 1;
}

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

void componentesConexas(Grafo *G, int *num_componentes, int *maior_componente) {
    if (G == NULL) {
        printf("[!] Grafo nulo.\n");
        return;
    }

    int *visitado = (int *)calloc(G->numero_vertices, sizeof(int));
    if(!visitado){
        printf("[!] Falha na alocacao de memoria.\n");
        exit(1);
    }
    
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

void dijkstra(Grafo *G, int origem, double *dist) {
    int *visitado = (int *)calloc(G->numero_vertices, sizeof(int));
     if(!visitado){
        printf("[!] Falha na alocacao de memoria.\n");
        exit(1);
    }

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

        if (u == -1 || dist[u] == INT_MAX) break;

        visitado[u] = 1;

        No *adjacente = G->arestas[u];
        while (adjacente != NULL) {
            if (dist[u] != INT_MAX && dist[u] + adjacente->peso < dist[adjacente->rotulo]) {
                dist[adjacente->rotulo] = dist[u] + adjacente->peso;
            }
            adjacente = adjacente->proximo;
        }
    }

    free(visitado);
}

double menorCaminhoMedio(Grafo *G) {
   if (G == NULL) {
        printf("[!] Grafo nulo.\n");
        return -1;
    }

    double soma_caminhos = 0;
    int total_caminhos = 0;

    for (int i = 0; i < G->numero_vertices; i++) {
        double *dist = (double *)malloc(G->numero_vertices * sizeof(double));
         if(!dist){
            printf("[!] Falha na alocacao de memoria.\n");
            exit(1);
        }
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
    int vertice = 0;
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