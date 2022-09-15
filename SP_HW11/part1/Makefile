all: usock_server usock_client
usock_server: usock_server.c lookup5.c dict.h
	gcc usock_server.c lookup5.c dict.h -o usock_server
usock_client: main.c lookup7.c dict.h
	gcc main.c lookup7.c dict.h -o usock_client