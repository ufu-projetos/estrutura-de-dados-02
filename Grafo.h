typedef struct grafo Grafo;

// Cria o grafo.
Grafo* criarGrafo(int numero_vertices, int grau_maximo, int eh_ponderado, int eh_digrafo);
// Libera o grafo.
void liberarGrafo(Grafo *G);
// Insere vertice no grafo.
int inserirVertice(Grafo *G, char *label);
// Insere aresta no grafo.
int inserirAresta(Grafo *G, int vertice_origem, int vertice_destino, float peso);
// Remove aresta do grafo.
int removeAresta(Grafo *G, int vertice_origem, int vertice_destino);
// Grau do vertice.
int grau(Grafo *G, int vertice);
// Grau medio do grafo.
int grauMedio(Grafo *G);
// Grau maximo do vertice.
int grauMax(Grafo *G, int *v);
// Imprime o grafo.
void imprime_Grafo(Grafo *G);