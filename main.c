#include <stdio.h>
#include "Grafo.h"

int main() {
    Grafo *G = NULL; // Grafo a ser criado
    int opcao; // Opcao do menu

    do {
        printf("\n\t\t --- ANALISE DE GRAFOS ---\n\n");
        printf("Escolha uma opcao abaixo:\n\n");

        if(G == NULL) {
            printf("\n[!] Ainda nao ha nenhum grafo criado!");
            printf("\n\n1 - Criar/carregar novo grafo\n");
        } else {
            printf("2 - Inserir aresta\n");
            printf("3 - Remover aresta\n");
            printf("4 - Verificar grau de um vertice\n");
            printf("5 - Verificar grau medio do grafo\n");
            printf("6 - Verificar grau maximo do grafo\n");
            printf("7 - Imprimir grafo\n");
        }
        printf("0 - Sair\n");

        printf("\n\n> ");
        if(scanf("%d", &opcao) != 1) {
            printf("\n[!] Entrada inválida! Digite apenas números.\n\n\n");
            while(getchar() != '\n');
            opcao = 0;
            break;
        }

        if(opcao)
            G = menu(G, opcao);
    } while(opcao);
    
    encerrar(G);

    return 0;
}