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

void handle_session(int session_fd) {
    time_t now=time(0);
    char buffer[80];
    size_t length=strftime(buffer,sizeof(buffer),"%a %b %d %T %Y\r\n",localtime(&now));
    if (length==0) {
        snprintf(buffer,sizeof(buffer),"Error: buffer overflow\r\n");
    }
    size_t index=0;
    while (index<length) {
        ssize_t count=write(session_fd,buffer+index,length-index);
        if (count<0) {
            error("failed to write to socket");
        } else {
            index+=count;
        }
    }
}

uint32_t QKD_GET_KEY(context_t* key_handle, uint8_t* key_buffer) {
  if (key_handle->host == 'c') {
    /* Client */
    const char* hostname="localhost";
    const char* portname="8080";
    struct addrinfo hints;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_protocol=0;
    hints.ai_flags=AI_ADDRCONFIG;
    struct addrinfo* res=0;
    int err=getaddrinfo(hostname,portname,&hints,&res);
    if (err!=0) {
        error("failed to resolve remote socket address");
    }

    int fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if (fd==-1) {
        error("ERROR opening socket");
    }

    /* connect: create a connection with the server */
    if (connect(fd,res->ai_addr,res->ai_addrlen) < 0)
      error("ERROR connecting");

    /* send the message line to the server */
    puts("-> client requested the server to generate key");
    char buf[BUFSIZE];
    bzero(buf, BUFSIZE);
  	strcpy(buf, "send key plz\n");

    int n = write(fd, buf, strlen(buf));
    if (n < 0)
      error("ERROR writing to socket");

    /* print the server's reply */
    bzero(buf, BUFSIZE);
    n = read(fd, buf, BUFSIZE);
    if (n < 0)
      error("ERROR reading from socket");

    for (size_t i = 0; i < KEYSIZE; i++) {
      key_buffer[i] = buf[i];
    }

    close(fd);

  } else if (key_handle->host == 's') {
    /* Server */
    const char* hostname="localhost";
    const char* portname="8080";
    struct addrinfo hints;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_protocol=0;
    hints.ai_flags=AI_PASSIVE|AI_ADDRCONFIG;
    struct addrinfo* res=0;
    int err=getaddrinfo(hostname,portname,&hints,&res);
    if (err!=0) {
        error("ERROR with getaddrinfo");
    }

    int server_fd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if (server_fd==-1) {
        error("ERROR opening socket");
    }

    int reuseaddr=1;
    if (setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&reuseaddr,sizeof(reuseaddr))==-1) {
        error("ERROR with setsockopt");
    }

    if (bind(server_fd,res->ai_addr,res->ai_addrlen)==-1) {
        error("ERROR on binding");
    }

    freeaddrinfo(res);

    if (listen(server_fd,SOMAXCONN)) {
        error("FAILED to listen for connections");
    }

    int session_fd=accept(server_fd,0,0);
    puts("-> key generated on the server");
    char buf[BUFSIZE]; /* message buffer */
    bzero(buf, BUFSIZE);
    for (size_t i = 0; i < KEYSIZE; i++) {
      key_buffer[i] = rand() % 256;
      buf[i] = key_buffer[i];
    }

    int n = write(session_fd, buf, strlen(buf));
    if (n < 0) {
      error("ERROR writing to socket");
    }

    close(session_fd);
    close(server_fd);
   }

   return 0;
}

uint32_t QKD_CLOSE(context_t* key_handle) {
  free(key_handle);
	return 0;
}
