/*
 * lookup8 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Internet TCP Sockets
 * The name of the server is passed as resource. PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	static int sockfd;
	static struct sockaddr_in server;
	struct hostent *host;
	static int first_time = 1;

	if (first_time) {        /* connect to socket ; resource is server name */
		first_time = 0;

		/* Set up destination address. */
		server.sin_family = AF_INET;
		/* Fill in code. */
		if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
			DIE("Create socket failed");

		/* Allocate socket.
		 * Fill in code. */
		if((host = gethostbyname(resource)) == NULL)
			DIE("Gethostbyname failed");

		server.sin_port = htons(7000);
		memcpy((char *)&server.sin_addr, host->h_addr, host->h_length);

		/* Connect to the server.
		 * Fill in code. */
		if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
			DIE("Connect failed");
	}

	/* write query on socket ; await reply
	 * Fill in code. */
	if(send(sockfd, sought->word, sizeof(sought->word), 0) == -1)
		DIE("Send failed");
	
	char recv_data[1000];
	if(recv(sockfd, recv_data, sizeof(recv_data), 0) == -1)
		DIE("Receive failed");

	if (strcmp(sought->text,recv_data) != 0) {
		strcpy(sought->text, recv_data);
		return FOUND;
	}

	return NOTFOUND;
}
