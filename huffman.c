#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdint.h> 
#include "Huffman.h"

typedef unsigned char byte;

/** Definição da árvore */
typedef struct nodeArvore
{
    int                 frequencia;
    byte                c;
    struct nodeArvore   *esquerda;
    struct nodeArvore   *direita;
} nodeArvore;

/** Definição da fila de prioridade (implementada como lista simplesmente encadeada) */

typedef struct nodeLista
{
    nodeArvore          *n;
    struct nodeLista    *proximo;
} nodeLista;

typedef struct lista
{
    nodeLista   *head;
    int         elementos;
} lista;

/**
* A função strdup é dependente de implementação nas plataformas não POSIX (Windows, etc)
* Após alguns bugs em diferentes SOs, essa foi a solução que buscada que deu certo
*/

char *strdup(const char *s)
{
    // Adicionado tratamento para string NULL ou vazia
    if (s == NULL) return NULL;
    char *p = malloc(strlen(s) + 1);
    if (p) strcpy(p, s);
    return p;
}

/** Função que faz alocação de memória e trata os ponteiros soltos acerca de nós da lista encadeada.
* Obs: cada nó da lista encadeada é conectado a um nó 'raiz' de árvore.
* @param: um nó de uma árvore.
*/

nodeLista *novoNodeLista(nodeArvore *nArv)
{
    // Aloca memória
    nodeLista *novo;
    if ( (novo = malloc(sizeof(*novo))) == NULL ) {
        perror("Erro ao alocar memoria para nodeLista");
        exit(1); // Encerra se a alocação falhar
    }

    // Adiciona a árvore ao nó
    novo->n = nArv;

    // Faz o campo próximo apontar para NULL
    novo->proximo = NULL;

    return novo;
}

/** Função que faz alocação de memória e trata os ponteiros soltos acerca de nós da árvore
* @param: o byte a ser gravado no nó, a frequencia do byte, ponteiros para os nós filhos
*/

nodeArvore *novoNodeArvore(byte c, int frequencia, nodeArvore *esquerda, nodeArvore *direita)
{
    // Aloca memória
    nodeArvore *novo;

    if ( ( novo = malloc(sizeof(*novo)) ) == NULL ) {
        perror("Erro ao alocar memoria para nodeArvore");
        exit(1); // Encerra se a alocação falhar
    }


    // Atribui na árvore os valores passados como parâmetro
    novo->c = c;
    novo->frequencia = frequencia;
    novo->esquerda = esquerda;
    novo->direita = direita;

    return novo;
}

/** Função que um novo nó na lista encadeada que representa a fila de prioridade.
* @param: um nó previamente criado, a lista que receberá o nó
*/

void insereLista(nodeLista *n, lista *l)
{
     // Verifica se o nó a ser inserido ou a lista são nulos
    if (!n || !l) {
        fprintf(stderr, "Erro: Tentativa de inserir nó nulo ou em lista nula.\n");
        return;
    }
    // Se a lista passada como parâmetro não tem um nó no início (vazia), insira o nó no início
    if (!l->head)
    {
        l->head = n;
    }
   
    else if (n->n->frequencia < l->head->n->frequencia)
    {
        n->proximo = l->head;
        l->head = n;
    }
    else
    {
        // nó auxiliar que inicia apontando para o segundo nó da lista (head->proximo)
        nodeLista *aux = l->head->proximo;
        // nó auxiliar que inicia apontando para o primeiro nó da lista
        nodeLista *aux2 = l->head;

        // Laço que percorre a lista e insere o nó na posição certa de acordo com sua frequência.
        // Se sabe que aux começa apontando para o segundo item da lista e aux2 apontando para o primeiro.
        while (aux && aux->n->frequencia <= n->n->frequencia)
        {
            aux2 = aux;
            aux = aux2->proximo;
        }

        // Se insere o nó na posição certa.
        aux2->proximo = n;
        n->proximo = aux;
    }

    // Incrementa quantidade de elementos
    l->elementos++;
}

/** Função que 'solta' o nó apontado por 'head' da lista (o de menor frequência)
* (faz backup do nó e o desconecta da lista)
* @param: uma lista encadeada.
*/

nodeArvore *popMinLista(lista *l)
{
    // Verifica se a lista está vazia
    if (!l || !l->head) {
         fprintf(stderr, "Erro: Tentativa de remover de lista vazia ou nula.\n");
         return NULL; // Ou tratar o erro de forma apropriada
    }

    // Ponteiro auxilar que aponta para o primeiro nó da lista
    nodeLista *aux = l->head;

    nodeArvore *arvoreRetirada = aux->n;

    // Aponta o 'head' da lista para o segundo elemento dela
    l->head = aux->proximo;

    free(aux);

    // Decrementa a quantidade de elementos
    l->elementos--;

    return arvoreRetirada;
}

/** Função que conta a frequência de ocorrências dos bytes de um dado arquivo
* @param: um arquivo, uma lista de bytes (array de unsigned int)
*/
void getByteFrequency(FILE *entrada, unsigned int *listaBytes)
{
    // Verifica se os ponteiros são válidos
    if (!entrada || !listaBytes) {
        fprintf(stderr, "Erro: Ponteiro de arquivo ou lista de bytes nulo em getByteFrequency.\n");
        return; // Ou tratar o erro
    }

    // Zera a lista de frequência antes de começar (boa prática)

    byte c;
    long initial_pos = ftell(entrada); // Guarda a posição inicial

    while (fread(&c, 1, 1, entrada) == 1) // Verifica se leu 1 byte com sucesso
    {
        listaBytes[c]++; // Usa o valor do byte diretamente como índice
    }

    // Verifica se ocorreu erro durante a leitura (exceto fim de arquivo)
    if (ferror(entrada)) {
        perror("Erro durante a leitura do arquivo em getByteFrequency");
    }

    // Volta o ponteiro para a posição inicial, caso precise reler o arquivo
    fseek(entrada, initial_pos, SEEK_SET);
}

//  Obtem o código começando no nó n, utilizando o byte salvo em 'c', preenchendo 'buffer', desde o bucket 'tamanho'

/**
/ Função recursiva que percorre uma árvore de huffman e para ao encontrar o byte procurado (c)
/ @param: nó para iniciar a busca, byte a ser buscado, buffer para salvar os nós percorridos, posição para escrever
**/

int pegaCodigo(nodeArvore *n, byte c, char *buffer, int tamanho)
{
    // Verifica ponteiros nulos
    if (!n || !buffer) {
        fprintf(stderr, "Erro: Nó ou buffer nulo em pegaCodigo.\n");
        return 0;
    }

    // Caso base da recursão:
    // Se o nó for folha e o seu valor for o buscado, colocar o caractere terminal no buffer e encerrar
    int ehFolha = !(n->esquerda || n->direita);
    if (ehFolha) {
        if (n->c == c) {
            // Verifica se há espaço no buffer (evita overflow)
             if (tamanho < 1024) { // Usando 1024 como exemplo
                buffer[tamanho] = '\0';
                return 1;
            } else {
                fprintf(stderr, "Erro: Buffer pequeno demais em pegaCodigo.\n");
                return 0; // Indica falha
            }
        } else {
            return 0; // É folha, mas não é o caractere procurado
        }
    }
    else // Não é folha, continua a busca
    {
        int encontrado = 0;

        // Se existir um nó à esquerda
        if (n->esquerda)
        {
             // Verifica se há espaço no buffer antes de escrever e chamar recursão
            if (tamanho + 1 < 1024) { // +1 para o próximo char e +1 para o futuro '\0'
                buffer[tamanho] = '0';
                encontrado = pegaCodigo(n->esquerda, c, buffer, tamanho + 1);
            } else {
                 fprintf(stderr, "Erro: Buffer pequeno demais em pegaCodigo (ramo esquerdo).\n");
                 return 0;
            }
        }

        // Se não encontrou na esquerda E existe um nó à direita
        if (!encontrado && n->direita)
        {
             if (tamanho + 1 < 1024) { // +1 para o próximo char e +1 para o futuro '\0'
                buffer[tamanho] = '1';
                encontrado = pegaCodigo(n->direita, c, buffer, tamanho + 1);
            } else {
                 fprintf(stderr, "Erro: Buffer pequeno demais em pegaCodigo (ramo direito).\n");
                 return 0;
            }
        }

        return encontrado;
    }
}

/** Função que constrói a árvore de huffman
* @param: a lista de frequências dos bytes (array de unsigned int).
*/
nodeArvore *BuildHuffmanTree(unsigned int *listaBytes)
{
     if (!listaBytes) {
        fprintf(stderr, "Erro: Lista de bytes nula passada para BuildHuffmanTree.\n");
        return NULL;
    }
    // Lista com head apontando pra NULL e com campo 'elementos' valendo 0;
    lista l = {NULL, 0};

    // Popula a fila de prioridade (lista encadeada ordenada) com nós folha da árvore.
    // Cada nó folha representa um byte que ocorre no arquivo.
    for (int i = 0; i < 256; i++)
    {
        if (listaBytes[i] > 0) // Se existe ocorrência do byte (frequência > 0)
        {
            // Cria um nó de árvore (folha) para o byte 'i' com sua frequência.
            nodeArvore* noFolha = novoNodeArvore((byte)i, listaBytes[i], NULL, NULL);
            if (!noFolha) {
                 // Tratamento de erro se novoNodeArvore falhar (já tem exit dentro, mas por segurança)
                 fprintf(stderr, "Falha ao criar nó folha para byte %d\n", i);
                 // Limpar memória já alocada na lista 'l' antes de sair seria ideal aqui.
                 exit(1);
            }
            // Cria um nó da lista encadeada que aponta para o nó da árvore.
            nodeLista* itemLista = novoNodeLista(noFolha);
             if (!itemLista) {
                 // Tratamento de erro se novoNodeLista falhar
                 fprintf(stderr, "Falha ao criar item de lista para byte %d\n", i);
                 free(noFolha); // Libera o nó da árvore criado
                 // Limpar memória já alocada na lista 'l'
                 exit(1);
             }
            // Insere o nó da lista na fila de prioridade (mantendo a ordem por frequência).
            insereLista(itemLista, &l);
        }
    }

     // Caso especial: Arquivo vazio ou com apenas um tipo de byte.
    // Se a lista tem 0 elementos (arquivo vazio), retorna NULL ou uma árvore vazia simbólica.
    if (l.elementos == 0) {
        return NULL; // Ou tratar como preferir
    }

    if (l.elementos == 1) {
        nodeArvore *unicoFilho = popMinLista(&l);
        // Cria um nó pai com frequência igual à do filho, apontando para ele (ex: esquerda)
        // O caractere '#' ou outro valor não-byte pode ser usado para indicar nó interno.
        nodeArvore *pai = novoNodeArvore('#', unicoFilho->frequencia, unicoFilho, NULL);
         if (!pai) {
             fprintf(stderr, "Falha ao criar nó pai para árvore de um único byte.\n");
             free(unicoFilho); // Libera o nó filho
             exit(1);
         }
         // Insere o nó pai de volta na lista para ser retornado.
         // (Alternativamente, poderíamos retornar 'pai' diretamente aqui, pois a lista ficaria vazia depois)
         insereLista(novoNodeLista(pai), &l);
    }


    // Combina os nós com menores frequências até restar apenas um nó na lista (a raiz da árvore).
    while (l.elementos > 1)
    {
        // Retira os dois nós de árvore com as menores frequências da fila.
        nodeArvore *nodeEsquerdo = popMinLista(&l);
        nodeArvore *nodeDireito = popMinLista(&l);

         // Verifica se popMinLista retornou NULL (improvável se l.elementos > 1, mas seguro checar)
        if (!nodeEsquerdo || !nodeDireito) {
            fprintf(stderr, "Erro inesperado: popMinLista retornou NULL durante construção da árvore.\n");
            // Liberar memória alocada seria necessário aqui antes de sair.
            exit(1);
        }
        
        nodeArvore *soma = novoNodeArvore(
                               '#', // Caractere indicativo de nó interno
                               nodeEsquerdo->frequencia + nodeDireito->frequencia,
                               nodeEsquerdo,
                               nodeDireito
                           );
        if (!soma) {
             fprintf(stderr, "Falha ao criar nó interno da árvore.\n");
             // Liberar nodeEsquerdo, nodeDireito e a lista 'l' adequadamente.
             exit(1);
        }

        // Insere o novo nó (agora encapsulado em um nodeLista) de volta na fila de prioridade.
        insereLista(novoNodeLista(soma), &l);
    }

    // Ao final do loop, a lista contém exatamente um nó, que é a raiz da árvore de Huffman completa.
    // Retira e retorna a raiz da árvore.
    return popMinLista(&l); // Retorna o nó da árvore (nodeArvore*)
}


/** Função que libera memória da árvore de huffman (percurso pós-ordem)
* @param: nó raiz de uma (sub)árvore.
*/
void FreeHuffmanTree(nodeArvore *n)
{
    // Caso base da recursão: se o nó for NULL, não há nada a fazer.
    if (!n) return;

    // Recursivamente libera a subárvore esquerda.
    FreeHuffmanTree(n->esquerda);
    // Recursivamente libera a subárvore direita.
    FreeHuffmanTree(n->direita);
    // Libera o nó atual após liberar seus filhos.
    free(n);
}


/** Função que lê bits de um arquivo de entrada.
 * Mantém um buffer interno (byte aux) e a posição do bit atual.
 * @param entrada Ponteiro para o arquivo de onde ler.
 * @param posicao Ponteiro para a posição atual do bit (será atualizado).
 * @param aux Ponteiro para o byte atual lido do arquivo (será atualizado quando necessário).
 * @return O valor do próximo bit (0 ou 1), ou -1 em caso de erro ou EOF inesperado.
 */
int geraBit(FILE *entrada, unsigned int *posicao, byte *aux) {
    if (!entrada || !posicao || !aux) {
        fprintf(stderr, "Erro: Argumentos nulos para geraBit.\n");
        return -1; // Indica erro
    }

    // Se a posição atual é múltiplo de 8, significa que precisamos ler o próximo byte do arquivo.
    if (*posicao % 8 == 0) {
        // Tenta ler 1 byte. fread retorna o número de itens lidos com sucesso.
        size_t lidos = fread(aux, 1, 1, entrada);
        if (lidos < 1) {
            if (feof(entrada)) {
                // A função chamadora (DecompressFile) deve lidar com isso.
                return -1; // Sinaliza EOF ou erro
            } else {
                // Ocorreu um erro de leitura.
                perror("Erro ao ler byte em geraBit");
                return -1; // Sinaliza erro
            }
        }
    }

    // Extrai o bit da posição atual dentro do byte 'aux'.
    // Usa uma máscara de bits (1 << (*posicao % 8)) e o operador AND (&).
    // O resultado será 0 se o bit for 0, ou diferente de zero se o bit for 1.
    // A dupla negação (!!) converte qualquer valor não-zero para 1, e 0 para 0.
    int bit = !!((*aux) & (1 << (*posicao % 8)));

    // Incrementa a posição do bit para a próxima chamada.
    (*posicao)++;

    return bit;
}


/** Função para notificar ausência ou erro ao abrir o arquivo. Encerra o programa em seguida.
 * @param filename O nome do arquivo que falhou ao abrir.
*/
void erroArquivo(const char *filename) // Modificado para receber o nome do arquivo
{
    // perror mostra a mensagem de erro específica do sistema
    fprintf(stderr, "Erro ao abrir o arquivo: %s\n", filename);
    perror("Detalhes do erro");
    exit(1); // Usa 1 para indicar erro
}


/** Função que comprime um arquivo utilizando a compressão de huffman
* @param: nome do arquivo a comprimir, nome do arquivo resultado da compressão
*/
void CompressFile(const char *arquivoEntrada, const char *arquivoSaida)
{
    printf("Iniciando compressao de '%s' para '%s'...\n", arquivoEntrada, arquivoSaida);
    clock_t inicio, final;
    double tempoGasto;
    inicio = clock();

    // Array para guardar a frequência de cada byte (0-255)
    // Inicializa todas as frequências com 0
    unsigned int listaBytes[256] = {0};

    // Abre arquivo de entrada no modo leitura binária ("rb")
    FILE *entrada = fopen(arquivoEntrada, "rb");
    if (!entrada) {
        erroArquivo(arquivoEntrada); // Passa o nome do arquivo para a função de erro
        // A linha abaixo não será alcançada devido ao exit() em erroArquivo()
        // return;
    }


    // Abre arquivo de saída no modo escrita binária ("wb")
    FILE *saida = fopen(arquivoSaida, "wb");
    if (!saida) {
        fclose(entrada); // Fecha o arquivo de entrada antes de sair
        erroArquivo(arquivoSaida);
        // return;
    }


    // 1. Calcula a frequência de cada byte no arquivo de entrada
    printf("Calculando frequencia dos bytes...\n");
    getByteFrequency(entrada, listaBytes);

    // 2. Constrói a Árvore de Huffman baseada nas frequências
    printf("Construindo arvore de Huffman...\n");
    nodeArvore *raiz = BuildHuffmanTree(listaBytes);

    // Se a árvore for NULL (arquivo vazio ou erro na construção), não há o que comprimir.
    if (!raiz) {
        fprintf(stderr, "Nao foi possivel construir a arvore. O arquivo pode estar vazio ou ocorreu um erro.\n");
        fclose(entrada);
        fclose(saida);
        // Considerar remover o arquivo de saída vazio, se criado.
        remove(arquivoSaida);
        return;
    }


    // 3. Escreve o cabeçalho no arquivo de saída:
    //    a) A tabela de frequências (para reconstruir a árvore na descompressão)
    printf("Escrevendo cabecalho (frequencias)...\n");
    size_t escritoFreq = fwrite(listaBytes, sizeof(unsigned int), 256, saida);
    if (escritoFreq != 256) {
         perror("Erro ao escrever tabela de frequencias no arquivo de saida");
         fclose(entrada);
         fclose(saida);
         FreeHuffmanTree(raiz);
         remove(arquivoSaida); // Remove arquivo incompleto
         exit(1);
    }


    //    b) Espaço reservado para o número total de bits codificados (será preenchido no final)
    unsigned int tamanhoTotalBits = 0; // Será o número total de bits no corpo comprimido
    // Escreve um valor temporário (0) que será sobrescrito depois
    size_t escritoTam = fwrite(&tamanhoTotalBits, sizeof(unsigned int), 1, saida);
     if (escritoTam != 1) {
         perror("Erro ao escrever espaco para tamanho dos bits no arquivo de saida");
         fclose(entrada);
         fclose(saida);
         FreeHuffmanTree(raiz);
         remove(arquivoSaida);
         exit(1);
     }


    // 4. Percorre o arquivo de entrada novamente, byte a byte
    printf("Codificando e escrevendo dados...\n");
    rewind(entrada); // Volta para o início do arquivo de entrada para ler os dados

    byte c; // Byte lido do arquivo de entrada
    byte bufferSaida = 0; // Byte que está sendo montado para escrita
    int bitsNoBuffer = 0; // Contador de quantos bits já foram colocados em bufferSaida
    unsigned int bitsEscritosTotal = 0; // Contador total de bits codificados

    // Enquanto conseguir ler um byte do arquivo de entrada
    while (fread(&c, 1, 1, entrada) == 1)
    {
        // Buffer para armazenar o código de Huffman do byte 'c'
        char codigo[1024] = {0}; // Tamanho grande o suficiente para códigos (profundidade da árvore)

        // Obtém o código binário (ex: "0110") para o byte 'c' usando a árvore
        // A função retorna 1 se encontrou o código, 0 caso contrário (não deve acontecer se a árvore foi bem construída)
        if (!pegaCodigo(raiz, c, codigo, 0)) {
             fprintf(stderr, "Erro critico: Nao foi encontrado o codigo para o byte %d (ASCII %c)\n", (int)c, c);
             // Limpeza e saída
             fclose(entrada);
             fclose(saida);
             FreeHuffmanTree(raiz);
             remove(arquivoSaida); // Arquivo corrompido
             exit(1);
        }

        // Para cada bit ('0' ou '1') no código obtido
        for (char *bitPtr = codigo; *bitPtr; bitPtr++)
        {
            // Se o bit atual é '1'
            if (*bitPtr == '1')
            {
                // Define o bit correspondente em bufferSaida usando OR bit a bit
                // A posição do bit é determinada por bitsNoBuffer
                bufferSaida = bufferSaida | (1 << bitsNoBuffer);
            }
            // Se o bit for '0', não fazemos nada, pois o bufferSaida já está inicializado com 0s

            bitsNoBuffer++; // Incrementa o contador de bits no buffer
            bitsEscritosTotal++; // Incrementa o contador total de bits

            // Se o buffer de saída (bufferSaida) está cheio (8 bits)
            if (bitsNoBuffer == 8)
            {
                // Escreve o byte completo no arquivo de saída
                size_t escritoByte = fwrite(&bufferSaida, 1, 1, saida);
                if (escritoByte != 1) {
                    perror("Erro ao escrever byte de dados comprimidos");
                     fclose(entrada);
                     fclose(saida);
                     FreeHuffmanTree(raiz);
                     remove(arquivoSaida);
                     exit(1);
                }

                // Reseta o buffer e o contador para o próximo byte
                bufferSaida = 0;
                bitsNoBuffer = 0;
            }
        }
    }
    
    if (bitsNoBuffer > 0)
    {
        size_t escritoUltimo = fwrite(&bufferSaida, 1, 1, saida);
        if (escritoUltimo != 1) {
             perror("Erro ao escrever ultimo byte de dados");
             fclose(entrada);
             fclose(saida);
             FreeHuffmanTree(raiz);
             remove(arquivoSaida);
             exit(1);
        }

    }

    printf("Finalizando cabecalho (tamanho dos dados)...\n");
    // Pula a tabela de frequência (256 * tamanho de unsigned int)
    fseek(saida, 256 * sizeof(unsigned int), SEEK_SET);
    // Escreve o número total de bits codificados que foram escritos (bitsEscritosTotal)
    size_t escritoTamFinal = fwrite(&bitsEscritosTotal, sizeof(unsigned int), 1, saida);
     if (escritoTamFinal != 1) {
             perror("Erro ao escrever o tamanho final dos bits no cabecalho");
             fclose(entrada);
             fclose(saida);
             FreeHuffmanTree(raiz);
             remove(arquivoSaida);
             exit(1);
     }

    // Calcula tempo e tamanhos
    final = clock();
    tempoGasto = (double)(final - inicio) / CLOCKS_PER_SEC;

    // Obtém tamanho dos arquivos (move ponteiro para o fim e lê a posição)
    fseek(entrada, 0L, SEEK_END);
    long tamanhoEntradaBytes = ftell(entrada); // Usar long para tamanhos de arquivo

    fseek(saida, 0L, SEEK_END);
    long tamanhoSaidaBytes = ftell(saida);


    // Libera a memória da árvore
    printf("Liberando memoria da arvore...\n");
    FreeHuffmanTree(raiz);

    // Fecha os arquivos
    fclose(entrada);
    fclose(saida);

    // Exibe estatísticas
    printf("----------------------------------------\n");
    printf(" Compressao Concluida\n");
    printf("----------------------------------------\n");
    printf("Arquivo de entrada: %s (%.2f KB)\n", arquivoEntrada, (double)tamanhoEntradaBytes / 1024.0);
    printf("Arquivo de saida:   %s (%.2f KB)\n", arquivoSaida, (double)tamanhoSaidaBytes / 1024.0);
    printf("Tempo gasto: %.3f segundos\n", tempoGasto);
    if (tamanhoEntradaBytes > 0) {
        double taxaCompressao = (double)tamanhoSaidaBytes * 100.0 / tamanhoEntradaBytes;
        printf("Taxa de compressao: %.2f%%\n", taxaCompressao);
    } else {
        printf("Taxa de compressao: N/A (arquivo de entrada vazio)\n");
    }
     printf("----------------------------------------\n");
}

/** Função que descomprime um arquivo utilizando a compressão de huffman
* @param: nome do arquivo a descomprimir, nome do arquivo resultado da descompressão
*/
void DecompressFile(const char *arquivoEntrada, const char *arquivoSaida)
{
    printf("Iniciando descompressao de '%s' para '%s'...\n", arquivoEntrada, arquivoSaida);
    clock_t inicio, final;
    double tempoGasto;
    inicio = clock();

    // Array para ler a frequência de cada byte do cabeçalho
    unsigned int listaBytes[256] = {0};

    // Abre arquivo de entrada (comprimido) no modo leitura binária ("rb")
    FILE *entrada = fopen(arquivoEntrada, "rb");
    if (!entrada) {
        erroArquivo(arquivoEntrada);
    }

    // Abre arquivo de saída (descomprimido) no modo escrita binária ("wb")
    FILE *saida = fopen(arquivoSaida, "wb");
     if (!saida) {
        fclose(entrada);
        erroArquivo(arquivoSaida);
    }

    // 1. Lê o cabeçalho do arquivo de entrada:
    //    a) A tabela de frequências
    printf("Lendo cabecalho (frequencias)...\n");
    size_t lidoFreq = fread(listaBytes, sizeof(unsigned int), 256, entrada);
    if (lidoFreq != 256) {
        fprintf(stderr, "[x] Erro ao ler tabela de frequencias do arquivo '%s'. Arquivo pode estar corrompido ou incompleto.\n", arquivoEntrada);
        perror("[*] Detalhes do erro fread");
        fclose(entrada);
        fclose(saida);
        remove(arquivoSaida); // Remove arquivo de saída potencialmente vazio/inválido
        exit(1);
    }

    //    b) O número total de bits codificados
    unsigned int totalBits;
    printf("Lendo cabecalho (tamanho dos dados)...\n");
    size_t lidoTam = fread(&totalBits, sizeof(unsigned int), 1, entrada);
     if (lidoTam != 1) {
        fprintf(stderr, "[x] Erro ao ler o tamanho total de bits do arquivo '%s'. Arquivo pode estar corrompido.\n", arquivoEntrada);
        perror("[*] Detalhes do erro fread");
        fclose(entrada);
        fclose(saida);
        remove(arquivoSaida);
        exit(1);
    }

     // Se totalBits for 0, significa que o arquivo original estava vazio.
     if (totalBits == 0) {
        printf("[!] Arquivo original estava vazio (0 bits codificados). Arquivo de saida sera vazio.\n");
        fclose(entrada);
        fclose(saida);
        // O arquivo de saída já foi criado vazio, então está correto.
        // Medir tempo e tamanho aqui pode ser útil.
        final = clock();
        tempoGasto = (double)(final - inicio) / CLOCKS_PER_SEC;
        printf("Tempo gasto: %.3f segundos\n", tempoGasto);
        printf("----------------------------------------\n");
        printf(" Descompressao Concluida (arquivo vazio)\n");
        printf("----------------------------------------\n");
        return; // Termina a descompressão
     }


    // 2. Reconstrói a Árvore de Huffman a partir da tabela de frequências lida
    printf("Reconstruindo arvore de Huffman...\n");
    nodeArvore *raiz = BuildHuffmanTree(listaBytes);
    if (!raiz) {
         // Isso pode acontecer se a tabela de frequência lida for inválida (todos zeros, mas totalBits > 0?)
         fprintf(stderr, "Erro: Falha ao reconstruir a arvore de Huffman a partir das frequencias lidas.\n");
         fclose(entrada);
         fclose(saida);
         remove(arquivoSaida);
         exit(1);
    }

    // 3. Lê os dados comprimidos bit a bit e percorre a árvore para decodificar
    printf("Decodificando e escrevendo dados...\n");
    unsigned int bitsDecodificados = 0; // Contador de bits processados
    nodeArvore *nodeAtual = raiz; // Começa na raiz da árvore
    byte byteLido = 0; // Buffer para o byte sendo lido do arquivo de entrada
    unsigned int posBitNoByte = 0; // Posição do próximo bit a ser lido de byteLido (0 a 7)

    // Continua enquanto não tivermos decodificado o número esperado de bits
    while (bitsDecodificados < totalBits)
    {
        // Usa a função geraBit para obter o próximo bit (0 ou 1) do arquivo de entrada
        // Passamos o ponteiro para posBitNoByte e byteLido para que sejam atualizados
        int bit = geraBit(entrada, &posBitNoByte, &byteLido);

        // Verifica se geraBit retornou erro (-1)
        if (bit == -1) {
             fprintf(stderr, "[x] Erro: Fim de arquivo inesperado ou erro de leitura durante a decodificacao (bit %u de %u).\n", bitsDecodificados, totalBits);
             fclose(entrada);
             fclose(saida);
             FreeHuffmanTree(raiz);
             remove(arquivoSaida); // Arquivo de saída está incompleto/corrompido
             exit(1);
        }

        // Navega na árvore de Huffman com base no bit lido
        if (bit == 1) { // Bit '1' -> vai para a direita
            nodeAtual = nodeAtual->direita;
        } else { // Bit '0' -> vai para a esquerda
            nodeAtual = nodeAtual->esquerda;
        }

        // Verifica se chegamos a um nó inválido (não deveria acontecer em árvore bem formada)
        if (!nodeAtual) {
             fprintf(stderr, "[x] Erro: Encontrado caminho invalido na arvore de Huffman durante a decodificacao (bit %u de %u).\n", bitsDecodificados, totalBits);
             fclose(entrada);
             fclose(saida);
             FreeHuffmanTree(raiz);
             remove(arquivoSaida);
             exit(1);
        }


        // Se o nó atual é uma folha (não tem filhos)
        if (!(nodeAtual->esquerda || nodeAtual->direita))
        {
            // Encontramos um byte original! Escreve o caractere (byte) no arquivo de saída.
            size_t escritoByte = fwrite(&(nodeAtual->c), 1, 1, saida);
             if (escritoByte != 1) {
                perror("[x] Erro ao escrever byte decodificado no arquivo de saida");
                fclose(entrada);
                fclose(saida);
                FreeHuffmanTree(raiz);
                remove(arquivoSaida);
                exit(1);
            }

            // Volta para a raiz da árvore para começar a decodificar o próximo caractere
            nodeAtual = raiz;
        }
        // Se não é folha, continua no próximo loop para ler o próximo bit e descer mais na árvore.

        bitsDecodificados++; // Incrementa o contador de bits processados
    }


    // Libera a memória da árvore
    printf("Liberando memoria da arvore...\n");
    FreeHuffmanTree(raiz);

    // Calcula tempo e tamanhos
    final = clock();
    tempoGasto = (double)(final - inicio) / CLOCKS_PER_SEC;

    fseek(entrada, 0L, SEEK_END);
    long tamanhoEntradaBytes = ftell(entrada);

    fseek(saida, 0L, SEEK_END);
    long tamanhoSaidaBytes = ftell(saida);

    // Fecha os arquivos
    fclose(saida);
    fclose(entrada);

     // Exibe estatísticas
    printf("----------------------------------------\n");
    printf(" Descompressao Concluida\n");
    printf("----------------------------------------\n");
    printf("[<-] Arquivo de entrada: %s (%.2f KB)\n", arquivoEntrada, (double)tamanhoEntradaBytes / 1024.0);
    printf("[->] Arquivo de saida:   %s (%.2f KB)\n", arquivoSaida, (double)tamanhoSaidaBytes / 1024.0);
    printf("Tempo gasto: %.3f segundos\n", tempoGasto);
    if (tamanhoEntradaBytes > 0) {
         // A taxa de descompressão não é tão significativa quanto a de compressão,
         // mas podemos mostrar a relação inversa se desejado.
         // double taxaDescompressao = (double)tamanhoSaidaBytes * 100.0 / tamanhoEntradaBytes;
         // printf("Tamanho relativo ao comprimido: %.2f%%\n", taxaDescompressao);
    }
    printf("----------------------------------------\n");
}


// Função auxiliar para ler uma linha de forma segura e remover o \n
// Retorna 1 se leu algo, 0 se erro ou EOF imediato.
int lerLinha(char *buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin) == NULL) {
        return 0; // Erro ou EOF
    }
    // Remove o \n do final, se existir
    buffer[strcspn(buffer, "\n")] = 0;
    return 1;
}