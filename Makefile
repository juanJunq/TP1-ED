SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = include
CC = gcc
CFLAGS = -Wall -std=gnu11 -I$(INC_DIR)

TARGET = $(BIN_DIR)/tp1.out

# Arquivos fonte e objetos
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Regra principal
all: $(TARGET)

$(TARGET): $(OBJS)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ -lm

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o 
	rm -f $(TARGET)

.PHONY: all clean
