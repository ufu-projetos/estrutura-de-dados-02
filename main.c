#include <stdio.h>
#include "Grafo.h"

int main() {
    Grafo *G = NULL; // Grafo a ser criado
    int opcao; // Opcao do menu

    printf("\n\n\t\t\t\t\t\t\tTRABALHO 01");
    printf("\n\n\t\t\t\t\t\t\t - Grupo 10 - ");
    printf("\n\n>> Integrantes:");

    printf("\n\n\n[-] Joao Guilherme Araujo Viana");
    printf("\n[-] Matheus Guimaraes de Oliveira");
    printf("\n[-] Gabriel Faria de Angelis");
    printf("\n[-] Gustavo Antonio Teixeira de Matos");
    printf("\n[-] Hugo Alves Viana\n\n");

    do {
        printf("\n\t\t --- ANALISE DE REDES ---\n\n");
        printf("Escolha uma opcao abaixo:\n\n");

        if(G == NULL) {
            printf("\n[!] Ainda nao ha nenhum grafo criado!");
            printf("\n\n1 - Criar/carregar novo grafo\n");
        } else {
            printf("2 - Visao Geral do grafo\n");
            printf("3 - Inserir aresta\n");
            printf("4 - Remover aresta\n");
            printf("5 - Verificar grau de um vertice\n");
            printf("6 - Verificar grau medio do grafo\n");
            printf("7 - Verificar grau maximo do grafo\n");
            printf("8 - Imprimir grafo\n");
            printf("9 - Adicionar novo vertice\n");
            printf("10 - Verificar componentes conexas\n");
            printf("11 - Verificar menor caminho medio\n");
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