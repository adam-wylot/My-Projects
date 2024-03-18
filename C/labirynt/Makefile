#							---=== MAKEFILE ===---

#							    --- ZMIENNE ---
CC = cc		# kompilator
CFLAGS = -Wall -Wextra -ansi -pedantic -std=c99 -Iinc # flagi

SRC_DIR = src
OBJ_DIR = obj

TARGET = labirynt 	# Nazwa wynikowego pliku wykonywalnego

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

#							    --- CELE ---
all: $(TARGET)	# cel główny

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

# Kompilacja plików źródłowych do plików obiektowych
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Stworzenie katalogu obj, jeśli nie istnieje
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Usuń pliki obiektowe i plik wykonywalny
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Kompiluje i tworzy plik do debugowania
debug:
	CFLAGS = -g -Wall -Wextra -ansi -pedantic -std=c99 -Iinc
	$(TARGET)
