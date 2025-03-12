#include <stdio.h>
#include "Grafo.h"

int main() {
    Grafo* gr = criarGrafo(5, 5, 0, 0);

    inserirAresta(gr, 0, 1, 0);
	inserirAresta(gr, 1, 3, 0);
	inserirAresta(gr, 1, 2, 0);
	inserirAresta(gr, 2, 4, 0);
	inserirAresta(gr, 3, 0, 0);
	inserirAresta(gr, 3, 4, 0);
	inserirAresta(gr, 4, 1, 0);

    imprime_Grafo(gr);
	//printf("\n\n%d\n", grau(gr, 2));

    return 0;
}