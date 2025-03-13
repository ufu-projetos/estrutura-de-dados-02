#include <stdio.h>
#include "Grafo.h"

int main() {
    Grafo *gr = criarGrafo(8, 0, 0);
    if(gr != NULL) printf("Deu certo!");
    else printf("Deu rum!");

    imprimirGrafo(gr);

    inserirAresta(gr, 1, 2, 5);
    inserirAresta(gr, 0, 4, 1);
    inserirAresta(gr, 0, 6, 90);

    imprimirGrafo(gr);

    //inserirAresta(gr, 0, 1, 0);

    liberarGrafo(gr);

    return 0;
}