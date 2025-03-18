typedef struct no No;
typedef struct grafo Grafo;

Grafo *criarGrafo(int numero_vertices, int eh_ponderado, int eh_digrafo);
int liberarGrafo(Grafo *G);
void imprimirGrafo(Grafo *G);
int inserirAresta(Grafo *G, int origem, int destino, int peso);
int removerAresta(Grafo *G, int origem, int destino);
int grau(Grafo *G, int vertice);
float grauMedio(Grafo *G);
int grauMax(Grafo *G, int *vertice);
void lerDataSet(Grafo *G, char *arquivo);