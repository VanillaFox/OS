CC = gcc
CFLAGS = -g -c -Wall
ADDLIB = -lm

all: main1 main2

main1: libfunc.a main1.c
	$(CC) main1.c -L. -lfunc $(ADDLIB) -o main1

main2: libfunc1.so libfunc2.so main2.c
	$(CC) main2.c -ldl -o main2

libfunc1.so: libfunc1.o
	$(CC) -shared func1.o $(ADDLIB) -o libfunc1.so

libfunc2.so: libfunc2.o
	$(CC) -shared func2.o $(ADDLIB) -o libfunc2.so 

libfunc.a: libfunc2.o
	ar cr libfunc.a func2.o


libfunc1.o: 
	$(CC) -fPIC -c func1.c -o func1.o

libfunc2.o:
	$(CC) -fPIC -c func2.c -o func2.o

clean:
	rm -r *.o *.a *.so main1 main2
