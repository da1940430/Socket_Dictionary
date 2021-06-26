/*
 * isock_server : listen on an internet socket ; fork ;
 *                child does lookup ; replies down same socket
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "dict.h"

int main(int argc, char **argv) {
	static struct sockaddr_in server, client;
	int sd,cd,n;
	Dictrec tryit;
	strcpy(tryit.text, "");

	if (argc != 2) {
		fprintf(stderr,"Usage : %s <datafile>\n",argv[0]);
		exit(1);
	}

	/* Create the socket.
	 * Fill in code. */
	if((sd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		DIE("Create socket failed");

	/* Initialize address.
	 * Fill in code. */
	server.sin_family = AF_INET;
	server.sin_port = htons(7000);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	/* Name and activate the socket.
	 * Fill in code. */
	if(bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
		DIE("Bind failed");
	if(listen(sd, 10) == -1)
		DIE("Listen failed");

	/* main loop : accept connection; fork a child to have dialogue */
	for (;;) {

		/* Wait for a connection.
		 * Fill in code. */
		socklen_t client_len = sizeof(client);
		if((cd = accept(sd, (struct sockaddr *)&client, &client_len)) == -1)
			DIE("Accept failed");

		/* Handle new client in a subprocess. */
		switch (fork()) {
			case -1 : 
				DIE("fork");
			case 0 :
				close (sd);	/* Rendezvous socket is for parent only. */
				/* Get next request.
				 * Fill in code. */
				char recv_data[100];
				while (recv(cd, recv_data, sizeof(recv_data), 0) != -1) {
					/* Lookup the word , handling the different cases appropriately */
					strcpy(tryit.word, recv_data);
					switch(lookup(&tryit,argv[1]) ) {
						/* Write response back to the client. */
						case FOUND:
							/* Fill in code. */
							if(send(cd, tryit.text, sizeof(tryit.text), 0) == -1)
								DIE("Send failed");
							break;
						case NOTFOUND:
							if(send(cd, tryit.text, sizeof(tryit.text), 0) == -1)
								DIE("Send failed");
							/* Fill in code. */
							 break;
						case UNAVAIL:
							DIE(argv[1]);
					} /* end lookup switch */
				} /* end of client dialog */
				exit(0); /* child does not want to be a parent */

			default :
				close(cd);
				break;
		} /* end fork switch */
	} /* end forever loop */
} /* end main */
