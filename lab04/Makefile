CC = gcc
CFLAGS = -pthread -lrt

all: main child

main: main.c
	$(CC) main.c $(CFLAGS) -o main

child: child.c
	$(CC) child.c $(CFLAGS) -o child

clean:
	rm -r main child
