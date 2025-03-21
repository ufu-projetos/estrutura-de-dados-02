#ifndef GRAFO_H
#define GRAFO_H

typedef struct no No;
typedef struct grafo Grafo;


Grafo *criarGrafo(int numero_vertices, int eh_ponderado, int eh_digrafo);
int liberarGrafo(Grafo *G);
void imprimirGrafo(Grafo *G);
int inserirAresta(Grafo *G, int origem, int destino, double peso);
int removerAresta(Grafo *G, int origem, int destino);
int grau(Grafo *G, int vertice);
float grauMedio(Grafo *G);
int grauMax(Grafo *G, int *vertice);
int grafoEhPonderado(Grafo *G);
Grafo *menu(Grafo *G, int opcao);
void encerrar(Grafo *G);
Grafo *carregarArquivo( Grafo *G, const char *nome_arquivo, int eh_ponderado);
int inserirVertice(Grafo *G, int vertice);
void dfs(Grafo *G, int vertice, int *visitado, int *tamanho_componente);
void componentesConexas(Grafo *G, int *num_componentes, int *maior_componente);
void dijkstra(Grafo *G, int origem, double *dist);
double menorCaminhoMedio(Grafo *G);
void visaoGeral(Grafo *G);

#endif