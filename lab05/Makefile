CC = gcc
CFLAGS = -g -c -Wall
ADDLIB = -lm

all: main1 main2

main1: libfunc1.so main1.c
	$(CC) main1.c -L. -lfunc1 -o main1 $(ADDLIB)

main2: libfunc1.so libfunc2.so main2.c $(ADDLIB)
	$(CC) main2.c -ldl -o main2 

libfunc1.so: func1.o 
	$(CC) -shared func1.o -o libfunc1.so $(ADDLIB)

libfunc2.so: func2.o 
	$(CC) -shared func2.o -o libfunc2.so $(ADDLIB)

func1.o: func1.c
	$(CC) -fPIC -c func1.c -o func1.o

func2.o: func2.c
	$(CC) -fPIC -c func2.c -o func2.o

clean:
	rm -r *.so *.o main1 main2
