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

uint32_t QKD_OPEN(uint32_t destination, qos_t qos, key_handle_t* key_handle) {
  printf("%s\n", *key_handle);
  strcpy(*key_handle, "specified_key_handle");

	return 0;
}

uint32_t QKD_CONNECT(key_handle_t* key_handle, uint32_t timeout) {
  printf("%s\n", *key_handle);
	return 0;
}

uint32_t QKD_GET_KEY(key_handle_t* key_handle, uint8_t* key_buffer) {

  if (strcmp(*key_handle, "specified_key_handle") != 0) {
    return -1;
  }

  const char* hostname="localhost";
  const char* portname="8080";
  char buf[BUFSIZE];
  bzero(buf, BUFSIZE);
  struct addrinfo hints;
  memset(&hints,0,sizeof(hints));
  hints.ai_family=AF_UNSPEC;
  hints.ai_socktype=SOCK_STREAM;
  hints.ai_protocol=0;
  hints.ai_flags=AI_ADDRCONFIG;
  struct addrinfo* res=0;
  int err=getaddrinfo(hostname,portname,&hints,&res);
  if (err!=0) {
      error("ERROR with getaddrinfo");
  }

  int sd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
  if (sd==-1) {
      error("ERROR opening socket");
  }

  int reuseaddr=1;
  if (setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&reuseaddr,sizeof(reuseaddr))==-1) {
      error("ERROR with setsockopt");
  }

  if (bind(sd,res->ai_addr,res->ai_addrlen)==-1) {
    printf("ERROR on binding -> it's a CLIENT\n");
    /*
     *  CLIENT
     */
    if (connect(sd,res->ai_addr,res->ai_addrlen) < 0) {
      error("ERROR connecting");
    }
    puts("-> client requested the server to generate key");
  	strcpy(buf, "send key plz\n");
    int n = write(sd, buf, strlen(buf));
    if (n < 0) {
      error("ERROR writing to socket");
    }
    /* print the server's reply */
    bzero(buf, BUFSIZE);
    n = read(sd, buf, BUFSIZE);
    if (n < 0) {
      error("ERROR reading from socket");
    }
    for (size_t i = 0; i < KEYSIZE; i++) {
      key_buffer[i] = (uint8_t) buf[i];
    }
  } else {
    printf("SUCCESSFUL binding -> it's a SERVER\n");
    /*
     *  SERVER
     */
    if (listen(sd,SOMAXCONN)) {
        error("FAILED to listen for connections");
    }
    int session_fd=accept(sd,0,0);
    puts("-> key generated on the server");
    for (size_t i = 0; i < KEYSIZE; i++) {
      key_buffer[i] = (uint8_t) rand() % 256;
      buf[i] = key_buffer[i];
    }
    int n = write(session_fd, buf, strlen(buf));
    if (n < 0) {
      error("ERROR writing to socket");
    }
    close(session_fd);
  }

  freeaddrinfo(res);
  close(sd);
  return 0;
}

uint32_t QKD_CLOSE(key_handle_t* key_handle) {

	return 0;
}
