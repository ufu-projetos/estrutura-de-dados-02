#include <stdio.h>
#include "Grafo.h"

int menu(char c) {
    
}

int main() {
    Grafo *gr = criarGrafo(8, 0, 0);
    if(gr != NULL) printf("Deu certo!");
    else printf("Deu rum!");

    liberarGrafo(gr);

    return 0;
}