/*
 * lookup8 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Internet TCP Sockets
 * The name of the server is passed as resource. PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	static int sockfd;
	static struct sockaddr_in server;
	struct hostent *host;
	static int first_time = 1;

	if (first_time) {        /* connect to socket ; resource is server name */
		first_time = 0;

		/* Set up destination address. */
		if((host = gethostbyname(resource)) == NULL){
			perror("gethostbyname error");
			exit(1);
		}
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = inet_addr("127.0.0.1");
		server.sin_port = htons(PORT);
		/* Allocate socket.*/
		sockfd = socket(AF_INET,SOCK_STREAM,0);
		if(sockfd < 0){
			perror("socket error");
			exit(1);
		}
		/* Connect to the server.*/
		if(connect(sockfd,(struct sockaddr*) &server,sizeof(server)) < 0){
			perror("connect error");
			exit(1);
		}
	}

	/* write query on socket ; await reply*/
	if(send(sockfd,sought->word,strlen(sought->word),0) < 0){
		perror("send error");
		exit(1);
	}
	if(recv(sockfd,sought->text,sizeof(sought->text),0) < 0){
		perror("recv error");
		exit(1);
	}
	if (strcmp(sought->text,"XXXX") != 0) {
		return FOUND;
	}

	return NOTFOUND;
}
