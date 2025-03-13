#include <stdio.h>
#include "Grafo.h"

int main() {
    Grafo *gr = criarGrafo(3, 0, 0);
    if(gr != NULL) printf("Deu certo!");
    else printf("Deu rum!");

    imprimirGrafo(gr);

    inserirAresta(gr, 1, 2, 0);

    imprimirGrafo(gr);

    //inserirAresta(gr, 0, 1, 0);

    imprimirGrafo(gr);

    liberarGrafo(gr);

    return 0;
}