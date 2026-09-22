CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

TARGET = shellforge

SRC = src/main.c \
      src/token.c \
      src/lexer.c \
      src/history.c \
      src/parser.c \
      src/expand.c \
      src/builtin.c \
      src/executor.c

OBJ = $(SRC:.c=.o)

$(TARGET): $(OBJ)
	gcc $(CFLAGS) $(OBJ) -o $(TARGET)

src/%.o: src/%.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
