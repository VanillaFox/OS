CC = g++
FLAGS = -lzmq

all: A B C

A: progA.cpp
	$(CC) progA.cpp $(FLAGS) -o A
	
B: progB.cpp
	$(CC) progB.cpp $(FLAGS) -o B

C: progC.cpp
	$(CC) progC.cpp $(FLAGS) -o C

clean:
	rm A B C
