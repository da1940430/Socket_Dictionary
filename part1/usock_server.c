/*
 * usock_server : listen on a Unix socket ; fork ;
 *                child does lookup ; replies down same socket
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include "dict.h"

int main(int argc, char **argv) {
    struct sockaddr_un server, client;
    int sd,cd,n;
    Dictrec tryit;
	strcpy(tryit.text, "");

    if (argc != 3) {
      fprintf(stderr,"Usage : %s <dictionary source>"
          "<Socket name>\n",argv[0]);
      exit(errno);
    }

    /* Setup socket.
     * Fill in code. */
	if((sd = socket(PF_UNIX, SOCK_STREAM, 0)) == -1)
		DIE("Create socket failed");
    
    /* Initialize address.
     * Fill in code. */
	server.sun_family = AF_UNIX;
	strcpy(server.sun_path, argv[2]);

    /* Name and activate the socket.
     * Fill in code. */
	unlink(argv[2]);
	if(bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
		DIE("Bind failed");

	if(listen(sd, 100) == -1){
		DIE("Listen failed");
	}

    /* main loop : accept connection; fork a child to have dialogue */
    for (;;) {
		/* Wait for a connection.
		 * Fill in code. */
		socklen_t len = sizeof(client);
		if((cd = accept(sd, (struct sockaddr *)&client, &len)) == -1){
			perror("Accept failed");
			continue;
		}

		/* Handle new client in a subprocess. */
		switch (fork()) {
			case -1 : 
				DIE("fork");
			case 0 :
				close (sd);	/* Rendezvous socket is for parent only. */
				/* Get next request.
				 * Fill in code. */
				char recv_data[50];
				while (recv(cd, recv_data, sizeof(recv_data), 0) != -1) {
					/* Lookup request. */
					strcpy(tryit.word, recv_data);
					switch(lookup(&tryit,argv[1]) ) {
						/* Write response back to client. */
						case FOUND: 
							/* Fill in code. */
							if(send(cd, tryit.text, sizeof(tryit.text), 0) == -1)
								DIE("Send failed");
							break;
						case NOTFOUND: 
							/* Fill in code. */
							if(send(cd, tryit.text, sizeof(tryit.text), 0) == -1)
								DIE("Send failed");
							break;
						case UNAVAIL:
							DIE(argv[1]);
					} /* end lookup switch */

				} /* end of client dialog */

				/* Terminate child process.  It is done. */
				exit(0);

			/* Parent continues here. */
			default :
				close(cd);
				break;
		} /* end fork switch */
    } /* end forever loop */
} /* end main */
