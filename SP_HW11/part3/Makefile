all: udp_server udp_client
udp_server: udp_server.c lookup5.c dict.h
	gcc udp_server.c lookup5.c dict.h -o udp_server
udp_client: main.c lookup9.c dict.h
	gcc main.c lookup9.c dict.h -o udp_client