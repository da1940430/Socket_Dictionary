/*
 * lookup7 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Unix TCP Sockets
 * The name of the socket is passed as resource.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	static int sockfd;
	static struct sockaddr_un server;
	static int first_time = 1;
	int n;

	if (first_time) {     /* connect to socket ; resource is socket name */
		first_time = 0;

		/* Set up destination address.
		 * Fill in code. */
		if((sockfd = socket(PF_UNIX, SOCK_STREAM, 0)) == -1)
			DIE("Create socket failed");

		/* Allocate socket. */
		server.sun_family = AF_UNIX;
		strcpy(server.sun_path,resource);

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
