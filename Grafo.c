#include <stdio.h>
#include <stdlib.h>
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
        printf("\n[%d] -> ", i);

        No *p = G->arestas[i];
        while(p != NULL) {
            printf("%d", p->rotulo);
            if(G->eh_ponderado) printf(" (%.2f)", p->peso);
            if(p->proximo != NULL) printf(", ");
            
            p = p->proximo;
        }
    }
    printf("\n");
}

int inserirAresta(Grafo *G, int origem, int destino, int peso) {
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

    return soma / G->numero_vertices;
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