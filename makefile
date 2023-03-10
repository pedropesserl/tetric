CFLAGS = -Wall -g -Wextra

all: prod debug

prod: tetric.c
	gcc $(CFLAGS) -o prod tetric.c

debug: tetric.c
	gcc -DDEBUG_FLAG $(CFLAGS) -o debug tetric.c
