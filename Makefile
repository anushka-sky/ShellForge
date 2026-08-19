CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

TARGET = shellforge

SRC = src/main.c \
      src/token.c \
      src/lexer.c \
      src/history.c \
      src/parser.c \
      src/expand.c

OBJ = $(SRC:.c=.o)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
