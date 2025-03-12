#include <stdio.h>
#include <stdlib.h>
#include "Grafo.h"

#define LEN 30 // Tamanho da string do label

// Estrutura do Grafo.
struct grafo {
    int eh_ponderado; // Registra se o grafo eh ponderado ou nao
    int numero_vertices; // Numero de vertices/listas do grafo
    int grau_maximo; // Tamanho de cada lista
    int** arestas; // Array de arestas
    float** pesos; // Array de pesos, caso seja ponderado
    int* grau; // Numero de elementos em cada lista (numero de arestas em cada vertice)
    char **labels; // Identificacao do vertice/lista
    int eh_digrafo; // Registra se o grafo é digrafo
};

Grafo* criarGrafo(int numero_vertices, int grau_maximo, int eh_ponderado, int eh_digrafo) {
    Grafo* G;
    G = (Grafo*) malloc(sizeof(Grafo));

    if(G == NULL) {
        printf("[!] Erro ao criar Grafo!\n");
        exit(1);
    }

    G->eh_ponderado = (eh_ponderado) ? 1 : 0;
    G->numero_vertices = numero_vertices;
    G->grau_maximo = grau_maximo;
    G->eh_digrafo = (eh_digrafo) ? 1 : 0;
    G->grau = (int*) calloc(numero_vertices, sizeof(int));

    if(G->grau == NULL) {
        printf("[!] Erro ao alocar vetor de graus!\n");
        exit(1);
    }

    G->arestas = (int**) malloc(numero_vertices * sizeof(int*));
    for(int i = 0; i < numero_vertices; i++) {
        G->arestas[i] = (int*) malloc(numero_vertices * sizeof(int));
    }

    if(G->eh_ponderado) {
        G->pesos = (float**) malloc(numero_vertices * sizeof(float*));
        for(int i = 0; i < numero_vertices; i++) {
            G->pesos[i] = (float*) malloc(grau_maximo * sizeof(float));
        }

        if(G->pesos == NULL) {
            printf("[!] Erro ao alocar pesos!\n");
            exit(1);
        }
    }

    G->labels = (char**) calloc(numero_vertices, sizeof(char*));
    if (G->labels == NULL) {
        printf("[!] Erro ao alocar memória para os rótulos!\n");
        exit(1);
    }

    return G;
}

int inserirAresta(Grafo *G, int vertice_origem, int vertice_destino, float peso) {
    if(G == NULL) return 0;

    // Verifica se os verticies existem
    if((vertice_origem < 0 || vertice_origem >= G->numero_vertices) || 
    (vertice_destino < 0 || vertice_destino >= G->numero_vertices)) return 0;

    // Insere uma aresta no fim da lista
    G->arestas[vertice_origem][G->grau[vertice_origem]] = vertice_destino;
    if(G->eh_ponderado) {
        G->pesos[vertice_origem][G->grau[vertice_origem]] = peso;
    }
    G->grau[vertice_origem]++;

    if(!G->eh_digrafo) {
        // Verifica se os verticies existem
        if((vertice_origem < 0 || vertice_origem >= G->numero_vertices) || 
        (vertice_destino < 0 || vertice_destino >= G->numero_vertices)) return 0;

        // Insere uma aresta no fim da lista, so que invertido
        G->arestas[vertice_destino][G->grau[vertice_destino]] = vertice_origem;
        if(G->eh_ponderado) {
            G->pesos[vertice_destino][G->grau[vertice_destino]] = peso;
        }
        G->grau[vertice_destino]++;
    }

    return 1;
}

int removeAresta(Grafo* G, int vertice_origem, int vertice_destino) {
    if(G == NULL) return 0;
    if(vertice_origem < 0 || vertice_origem >= G->numero_vertices) return 0;
    if(vertice_destino < 0 || vertice_destino >= G->numero_vertices) return 0;

    // procurar aresta
    int i = 0;
    while(i < G->grau[vertice_origem] && G->arestas[vertice_origem][i] != vertice_destino) {
        i++;
    }

    // elemento nao encontrado
    if(i == G->grau[vertice_origem]) return 0;

    G->grau[vertice_origem]--;
    G->arestas[vertice_origem][i] = G->arestas[vertice_origem][G->grau[vertice_origem]];
    
    if(G->eh_ponderado) {
        G->pesos[vertice_origem][i] = G->pesos[vertice_origem][G->grau[vertice_origem]];
    }

    if(!G->eh_digrafo) {
        i = 0;
        while(i < G->grau[vertice_destino] && G->arestas[vertice_destino][i] != vertice_origem) {
            i++;
        }

        if(i == G->grau[vertice_destino]) return 0;

        G->grau[vertice_destino]--;
        G->arestas[vertice_destino][i] = G->arestas[vertice_destino][G->grau[vertice_destino]];

        if(G->eh_ponderado) {
            G->pesos[vertice_destino][i] = G->pesos[vertice_destino][G->grau[vertice_destino]];
        }
    }

    return 1;
}

int grau(Grafo *G, int vertice) {
    if(G == NULL) return 0;

    // verifica se o vertice existe
    if(vertice < 0 || vertice > G->numero_vertices) return -1;

    return G->grau[vertice];
}

void imprime_Grafo(Grafo* G){
	if(G == NULL)
		return;

	for(int i=0; i < G->numero_vertices; i++){
		printf("%d: ", i);
		for(int j=0; j < G->grau[i]; j++){
			if(G->eh_ponderado)
				printf("%d(%.2f)", G->arestas[i][j], G->pesos[i][j]);
			else
				printf("%d", G->arestas[i][j]);
            
            if(j < G->grau[i] - 1)
                printf(", ");
            else
                printf(".");
		}
		printf("\n");
	}
}