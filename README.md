# Trabalho 02 - Programa de Compressão de Arquivos por Alg. de Huffman

Este projeto implementa estruturas e algoritmos para compressão e descompressão de arquivos usando algoritmo de Huffman e árvore.

## Pré-requisitos

Antes de executar o programa, é necessário ter um compilador C instalado na sua máquina (ex: GCC).

## Compilação

Utilizamos o `Make` para facilitar a compilação e ter um melhor controle da saída. Entretanto, o projeto não depende dele.

### Execução

#### Utilizando o `Make`

1. Abra o terminal e navegue até o diretório do projeto.
2. Execute o seguinte comando para compilar o projeto:

    ```bash
    make
    ```

3. Após a compilação, execute o programa com:

    ```bash
    ./trabalho02
    ```

#### Sem utilizar o `Make`

1. Abra o terminal e navegue até o diretório do projeto.
2. Execute o seguinte comando para compilar o projeto:

    ```bash
    gcc -o trabalho02 Huffman.c main.c
    ```

   **Note:** Se estiver no Windows, execute:

    ```bash
    gcc -o trabalho02.exe Huffman.c main.c
    ```

3. Após a compilação, execute o programa com:
    * No Linux/MacOS:

    ```bash
    ./trabalho02
    ```

   * No Windows:

    ```bash
    ./trabalho02.exe
    ```
