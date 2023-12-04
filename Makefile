CC= gcc
FLAGS= -Wall -g

all: server client

server: protocole src/server.c
	$(CC) $(FLAGS) src/server.c out/protocole.o -o out/server

client: options protocole src/client.c
	$(CC) $(FLAGS) src/client.c out/protocole.o out/opt.o -o out/client 

protocole: src/protocole.c src/protocole.h
	$(CC) -c src/protocole.c -o out/protocole.o

options: src/opt.h src/opt.c
	$(CC) -c src/opt.c -o out/opt.o

clean:
	rm out/*.o out/client out/server
