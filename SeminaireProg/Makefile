all: run_client run_server

run_client: build_client
	./clientDirectory/tcpclient

run_server: build_server
	./serverDirectory/tcpserver

build_client: ./clientDirectory/tcpclient.c
	gcc ./clientDirectory/tcpclient.c -o ./clientDirectory/tcpclient

build_server: ./serverDirectory/tcpserver.c
	gcc ./serverDirectory/tcpserver.c -o ./serverDirectory/tcpserver
