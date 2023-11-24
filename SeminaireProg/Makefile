all: run_client run_server

run_client: build_client
	./clientDirectory/out/tcpclient

run_server: build_server
	./serverDirectory/out/tcpserver


build_client: ./clientDirectory/src/tcpclient.c build_optlib build_cmdlib
	gcc ./clientDirectory/src/tcpclient.c ./clientDirectory/out/cmd.o ./clientDirectory/out/opt.o -o ./clientDirectory/out/tcpclient

build_optlib: ./clientDirectory/src/options.c ./clientDirectory/src/options.h
	gcc -c ./clientDirectory/src/options.c -o ./clientDirectory/out/opt.o

build_cmdlib: ./clientDirectory/src/cmd.c ./clientDirectory/src/cmd.h
	gcc -c ./clientDirectory/src/cmd.c -o ./clientDirectory/out/cmd.o


build_server: ./serverDirectory/src/tcpserver.c
	gcc ./serverDirectory/src/tcpserver.c -o ./serverDirectory/out/tcpserver


clean: clean_client clean_server

clean_client:
	rm ./clientDirectory/out/*

clean_server:
	rm ./serverDirectory/out/*
