CC = gcc
CFLAGS = -Wall -g -Wextra
OBJ = tetric.o
DEPS =

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: tetric debug

tetric: $(OBJ)
	$(CC) -o $@ $^

debug: tetric.c
	$(CC) -DDEBUG_FLAG -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJ)

purge:
	rm -f $(OBJ) tetric debug
