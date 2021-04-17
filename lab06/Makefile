CC = g++
FINALFLAGS =-lzmq -o

all: client server

client: client.cpp
    $(CC) client.cpp $(FINALFLAGS) client
server: server.cpp
    $(CC) server.cpp $(FINALFLAGS) server
clean:
    rm -r client server
