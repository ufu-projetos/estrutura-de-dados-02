# Trabalho 01 - Grafos

Este projeto implementa estruturas e algoritmos para manipulação de grafos em C.

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
    ./trabalho01
    ```

#### Sem utilizar o `Make`

1. Abra o terminal e navegue até o diretório do projeto.
2. Execute o seguinte comando para compilar o projeto:

    ```bash
    gcc -o trabalho01 Grafo.c main.c
    ```

   **Note:** Se estiver no Windows, execute:

    ```bash
    gcc -o trabalho01.exe Grafo.c main.c
    ```

3. Após a compilação, execute o programa com:
    * No Linux/MacOS:

    ```bash
    ./trabalho01
    ```

   * No Windows:

    ```bash
    ./trabalho01.exe
    ```
