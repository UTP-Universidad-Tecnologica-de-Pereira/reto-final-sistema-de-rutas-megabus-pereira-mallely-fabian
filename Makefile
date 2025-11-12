CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
SRC = main.c graph.c algorithms.c statistics.c
OBJ = $(SRC:.c=.o)
TARGET = megabus

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)
	@echo "✓ Compilación exitosa"

%.o: %.c graph.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
	@echo "✓ Archivos limpiados"

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean valgrind run
