#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Huffman.h"

int main(void) // Removidos argc e argv
{
    char operacao;
    char arquivoEntrada[FILENAME_MAX]; // Usa constante de stdio.h para tamanho do nome
    char arquivoSaida[FILENAME_MAX];
    int entradaValida = 0;

    printf("----------------------------------------\n");
    printf(" Compressor/Descompressor Huffman\n");
    printf("----------------------------------------\n");

    // Loop para garantir que o usuário escolha uma operação válida
    while (!entradaValida) {
        printf("Escolha a operacao:\n");
        printf("  c - Comprimir arquivo\n");
        printf("  d - Descomprimir arquivo\n");
        printf("  s - Sair\n");
        printf("Opcao: ");

        // Lê a linha inteira para evitar problemas com \n deixado no buffer
        char bufferEscolha[10];
        if (!lerLinha(bufferEscolha, sizeof(bufferEscolha))) {
            printf("\nErro ou fim de entrada inesperado. Saindo.\n");
            return 1;
        }

        // Pega apenas o primeiro caractere e ignora o resto da linha
        if (strlen(bufferEscolha) > 0) {
            operacao = bufferEscolha[0];
            // Converte para minúscula para facilitar a comparação
            if (operacao >= 'A' && operacao <= 'Z') {
                operacao += 32; // Converte para minúscula ('a' - 'A')
            }

            if (operacao == 'c' || operacao == 'd') {
                entradaValida = 1;
            } else if (operacao == 's') {
                 printf("Saindo do programa.\n");
                 return 0;
            } else {
                printf("\nOpcao invalida '%c'. Tente novamente.\n\n", operacao);
            }
        } else {
             printf("\nEntrada vazia. Tente novamente.\n\n");
        }
    }

    // Pede o nome do arquivo de entrada
    printf("\nDigite o nome do arquivo de ENTRADA: ");
    if (!lerLinha(arquivoEntrada, FILENAME_MAX)) {
         printf("\nErro ao ler nome do arquivo de entrada. Saindo.\n");
         return 1;
    }
     // Verifica se o nome não está vazio
    if (strlen(arquivoEntrada) == 0) {
        printf("\nNome do arquivo de entrada nao pode ser vazio. Saindo.\n");
        return 1;
    }


    // Pede o nome do arquivo de saída
    printf("Digite o nome do arquivo de SAIDA: ");
     if (!lerLinha(arquivoSaida, FILENAME_MAX)) {
         printf("\nErro ao ler nome do arquivo de saida. Saindo.\n");
         return 1;
    }
     // Verifica se o nome não está vazio
    if (strlen(arquivoSaida) == 0) {
        printf("\nNome do arquivo de saida nao pode ser vazio. Saindo.\n");
        return 1;
    }

    // Verifica se os nomes de entrada e saída são iguais
    if (strcmp(arquivoEntrada, arquivoSaida) == 0) {
        printf("\nErro: O nome do arquivo de entrada e saida nao podem ser iguais.\n");
        return 1;
    }


    printf("\n"); // Linha em branco para separar

    // Executa a operação escolhida
    if (operacao == 'c') {
        // Validação da extensão do arquivo de saída para compressão
        if (strstr(arquivoSaida, ".huff") == NULL && strstr(arquivoSaida, ".hx") == NULL) // Permite .huff ou .hx
        {
            printf("Aviso: O arquivo resultante da compressao geralmente tem a extensao '.huff' ou '.hx'.\n");
            // Poderia perguntar se deseja continuar ou forçar a extensão. Por ora, só avisa.
            // Exemplo: ./meuprog -c texto.txt texto_comprimido.huff
        }
         // Adiciona verificação se o arquivo de entrada existe antes de chamar CompressFile
         FILE *testeEntrada = fopen(arquivoEntrada, "rb");
         if (!testeEntrada) {
             erroArquivo(arquivoEntrada); // erroArquivo já termina o programa
         } else {
             fclose(testeEntrada); // Fecha o arquivo se ele existe
             CompressFile(arquivoEntrada, arquivoSaida);
         }

    } else if (operacao == 'd') {
        // Validação da extensão do arquivo de entrada para descompressão
        if (strstr(arquivoEntrada, ".huff") == NULL && strstr(arquivoEntrada, ".hx") == NULL)
        {
            printf("Aviso: O arquivo a ser descomprimido geralmente tem a extensao '.huff' ou '.hx'.\n");
             // Poderia perguntar se deseja continuar. Por ora, só avisa.
            // Exemplo: ./meuprog -x texto_comprimido.huff texto_recuperado.txt
        }
         // Adiciona verificação se o arquivo de entrada existe antes de chamar DecompressFile
         FILE *testeEntrada = fopen(arquivoEntrada, "rb");
         if (!testeEntrada) {
             erroArquivo(arquivoEntrada); // erroArquivo já termina o programa
         } else {
             fclose(testeEntrada); // Fecha o arquivo se ele existe
             DecompressFile(arquivoEntrada, arquivoSaida);
         }
    }
    // Não precisa de 'else' aqui, pois a validação foi feita no loop inicial.


    return 0; // Indica sucesso
}