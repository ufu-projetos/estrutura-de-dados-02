# Makefile

# Nome do executável
EXEC = trabalho02

# Compilador a ser usado
CC = gcc

# Flags do compilador (opcional)
CFLAGS = -Wall -Wextra -g

# Diretório de construção (onde os arquivos objeto serão colocados)
BUILD_DIR = build

# Arquivos objeto (arquivos .o)
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/Huffman.o

# Regra principal: como criar o executável
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

# Regras para compilar os arquivos .c em .o
$(BUILD_DIR)/Huffman.o: Huffman.c Huffman.h
	@mkdir -p $(BUILD_DIR)  # Cria o diretório se não existir
	$(CC) $(CFLAGS) -c Huffman.c -o $@

$(BUILD_DIR)/main.o: main.c Huffman.h
	@mkdir -p $(BUILD_DIR)  # Cria o diretório se não existir
	$(CC) $(CFLAGS) -c main.c -o $@


# Limpar os arquivos objeto e o executável
clean:
	rm -rf $(EXEC) $(BUILD_DIR)