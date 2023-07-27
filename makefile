CC = gcc
CFLAGS = -Wall -Wextra
OBJ = src/main.o src/libtermio.o src/matrix.o src/piece.o
DEPS = src/libtermio.h src/matrix.h src/piece.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: tetric clean

tetric: $(OBJ)
	$(CC) -o $@ $^

clean:
	rm -f $(OBJ)

purge:
	rm -f $(OBJ) tetric
