# Kompilator i flagi kompilatora
CC = gcc
CFLAGS = -ansi -pedantic -Wall -Wextra -Iinc -std=c90

# Ścieżki do katalogów
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj
BIN_DIR = bin
EXE_NAME = filecalc

# Nazwa pliku wykonywalnego
TARGET = $(BIN_DIR)/$(EXE_NAME).exe

# Zbieramy wszystkie pliki źródłowe z katalogu src
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Tworzymy listę plików obiektowych .o odpowiadających plikom źródłowym
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Reguła domyślna
all: $(TARGET)

# Reguła kompilująca plik wykonywalny
$(TARGET): $(OBJS)
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Reguła kompilująca pliki .c na pliki .o i umieszczająca je w obj
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Czyszczenie plików obiektowych i pliku wykonywalnego
clean:
	if exist $(OBJ_DIR) del /Q $(OBJ_DIR)\*.o
	if exist $(TARGET) del /Q $(TARGET)

# Pomoc
help:
	@echo "Użycie make:"
	@echo "  make          | kompiluje program"
	@echo "  make clean    | usuwa pliki obiektowe i plik wykonywalny"
