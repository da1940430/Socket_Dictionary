/*
 * udp_server : listen on a UDP socket ;reply immediately
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>

#include "dict.h"

int main(int argc, char **argv) {
	static struct sockaddr_in server,client;
	int sockfd,siz;
	Dictrec dr, *tryit = &dr;
	strcpy(tryit->text, "");

	if (argc != 2) {
		fprintf(stderr,"Usage : %s <datafile>\n",argv[0]);
		exit(errno);
	}

	/* Create a UDP socket.
	 * Fill in code. */
	if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
		DIE("Create socket failed");

	/* Initialize address.
	 * Fill in code. */
	server.sin_family = AF_INET;
	server.sin_port = htons(7000);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	/* Name and activate the socket.
	 * Fill in code. */
	if(bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
		DIE("Bind failed");

	for (;;) { /* await client packet; respond immediately */

		siz = sizeof(client); /* siz must be non-zero */

		/* Wait for a request.
		 * Fill in code. */

		socklen_t client_len = sizeof(client);
		char recv_data[100];
		while (recvfrom(sockfd, recv_data, sizeof(recv_data), 0, (struct sockaddr *)&client, &client_len) != -1) {
			/* Lookup request and respond to user. */
			strcpy(tryit->word, recv_data);
			switch(lookup(tryit,argv[1]) ) {
				case FOUND: 
					if(sendto(sockfd, tryit->text, sizeof(tryit->text), 0, (struct sockaddr *)&client, client_len) == -1)
						DIE("Send failed");
					/* Send response.
					 * Fill in code. */
					break;
				case NOTFOUND : 
					if(sendto(sockfd, tryit->text, sizeof(tryit->text), 0, (struct sockaddr *)&client, client_len) == -1)
						DIE("Send failed");
					/* Send response.
					 * Fill in code. */
					break;
				case UNAVAIL:
					DIE(argv[1]);
			} /* end lookup switch */
		} /* end while */
	} /* end forever loop */
} /* end main */
