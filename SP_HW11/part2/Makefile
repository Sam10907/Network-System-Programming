all: isock_server isock_client
isock_server: isock_server.c lookup5.c dict.h
	gcc isock_server.c lookup5.c dict.h -o isock_server
isock_client: main.c lookup8.c dict.h
	gcc main.c lookup8.c dict.h -o isock_client