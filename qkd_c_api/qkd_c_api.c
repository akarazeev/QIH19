#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "qkd_c_api.h"

#define KEYSIZE 8
#define BUFSIZE 1024

void error(char *msg) {
  perror(msg);
  exit(1);
}

uint32_t QKD_OPEN(uint32_t destination, qos_t qos, context_t* key_handle) {
	// if ((key_handle = (context_t*) malloc(1)) == NULL) {
	// 	return -1;
	// }

	return 0;
}

uint32_t QKD_CONNECT(context_t* key_handle, uint32_t timeout) {

	return 0;
}

uint32_t QKD_GET_KEY(context_t* key_handle, uint8_t** key_buffer) {

  if ((*key_buffer = malloc(sizeof(uint8_t) * KEYSIZE)) == NULL) {
		return -1;
	}

  if (key_handle->host == 'c') {
    /* Client */

    int sockfd, n;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char buf[BUFSIZE];

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(key_handle->hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", key_handle->hostname);
        exit(0);
    }

    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(key_handle->portno);

    /* connect: create a connection with the server */
    if (connect(sockfd, (const struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
      error("ERROR connecting");

    /* send the message line to the server */
    puts("-> client requested the server to generate key");
    bzero(buf, BUFSIZE);
  	strcpy(buf, "send key plz\n");

    n = write(sockfd, buf, strlen(buf));
    if (n < 0)
      error("ERROR writing to socket");

    /* print the server's reply */
    bzero(buf, BUFSIZE);
    n = read(sockfd, buf, BUFSIZE);
    if (n < 0)
      error("ERROR reading from socket");
    printf("Echo from server: %s\n", buf);

    *key_buffer = (unsigned char*) buf;

    close(sockfd);

  } else if (key_handle->host == 's') {
    /* Server */

    int parentfd; /* parent socket */
    int childfd; /* child socket */
    unsigned int clientlen; /* byte size of client's address */
    struct sockaddr_in serveraddr; /* server's addr */
    struct sockaddr_in clientaddr; /* client addr */
    struct hostent *hostp; /* client host info */
    char buf[BUFSIZE]; /* message buffer */
    char *hostaddrp; /* dotted decimal host addr string */
    int optval; /* flag value for setsockopt */
    int n; /* message byte size */

    /*
     * socket: create the parent socket
     */
    parentfd = socket(AF_INET, SOCK_STREAM, 0);
    if (parentfd < 0)
      error("ERROR opening socket");

    optval = 1;
    setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR,
  	     (const void *)&optval , sizeof(int));

    bzero((char *) &serveraddr, sizeof(serveraddr));

    /* this is an Internet address */
    serveraddr.sin_family = AF_INET;

    /* let the system figure out our IP address */
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* this is the port we will listen on */
    serveraddr.sin_port = htons((unsigned short)key_handle->portno);

    /*
     * bind: associate the parent socket with a port
     */
    if (bind(parentfd, (struct sockaddr *) &serveraddr,
  	   sizeof(serveraddr)) < 0)
      error("ERROR on binding");

    /*
     * listen: make this socket ready to accept connection requests
     */
    if (listen(parentfd, 5) < 0) /* allow 5 requests to queue up */
      error("ERROR on listen");

    /*
     * accept: wait for a connection request
     */
    clientlen = sizeof(clientaddr);
    childfd = accept(parentfd, (struct sockaddr *) &clientaddr, &clientlen);
    if (childfd < 0)
      error("ERROR on accept");
    /*
     * gethostbyaddr: determine who sent the message
     */
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
			  sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    if (hostp == NULL) {
      error("ERROR on gethostbyaddr");
    }
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL) {
      error("ERROR on inet_ntoa\n");
    }
    printf("server established connection with %s (%s)\n",
	   hostp->h_name, hostaddrp);

    /*
     * read: read input string from the client
     */
    bzero(buf, BUFSIZE);
    n = read(childfd, buf, BUFSIZE);
    if (n < 0) {
      error("ERROR reading from socket");
    }
    printf("server received %d bytes: %s", n, buf);

    /*
     * write
     */
     puts("-> key generated on the server");
     bzero(buf, BUFSIZE);
     for (size_t i = 0; i < KEYSIZE; i++) {
       (*key_buffer)[i] = rand() % 255;
       buf[i] = (*key_buffer)[i];
     }

     n = write(childfd, buf, strlen(buf));
     if (n < 0) {
       error("ERROR writing to socket");
     }

     close(childfd);
     close(parentfd);
  }

	return 0;
}

uint32_t QKD_CLOSE(context_t* key_handle) {
  free(key_handle);
	return 0;
}
