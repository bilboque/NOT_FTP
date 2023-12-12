CC= gcc
FLAGS= -Wall -g
LIB= -lreadline

all: server client lightserver
	./out/lightserver

server: protocole src/server.c
	$(CC) $(FLAGS) $(LIB) src/server.c out/protocole.o -o out/server

lightserver: src/lightserver.c server
	$(CC) $(FLAGS) src/lightserver.c -o out/lightserver

client: options protocole src/client.c
	$(CC) $(FLAGS) $(LIB) src/client.c out/protocole.o out/opt.o -o out/client 

protocole: src/protocole.c src/protocole.h
	$(CC) -c src/protocole.c -o out/protocole.o

options: src/opt.h src/opt.c
	$(CC) -c src/opt.c -o out/opt.o

clean:
	rm out/*.o out/client out/server out/lightserver
