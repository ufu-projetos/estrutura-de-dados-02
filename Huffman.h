#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdint.h>

typedef unsigned char byte;

typedef struct nodeArvore nodeArvore;

// Definição da fila de prioridade (lista simplesmente encadeada)
typedef struct nodeLista nodeLista;

typedef struct lista lista;

// Protótipos das funções
char *strdup(const char *s);
nodeLista *novoNodeLista(nodeArvore *nArv);
nodeArvore *novoNodeArvore(byte c, int frequencia, nodeArvore *esquerda, nodeArvore *direita);
void insereLista(nodeLista *n, lista *l);
nodeArvore *popMinLista(lista *l);
void getByteFrequency(FILE *entrada, unsigned int *listaBytes);
int pegaCodigo(nodeArvore *n, byte c, char *buffer, int tamanho);
nodeArvore *BuildHuffmanTree(unsigned int *listaBytes);
void FreeHuffmanTree(nodeArvore *n);
int geraBit(FILE *entrada, unsigned int *posicao, byte *aux);
void erroArquivo(const char *filename);
void CompressFile(const char *arquivoEntrada, const char *arquivoSaida);
void DecompressFile(const char *arquivoEntrada, const char *arquivoSaida);
int lerLinha(char *buffer, int tamanho);

#endif