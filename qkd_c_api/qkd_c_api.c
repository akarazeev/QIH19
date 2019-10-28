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

void error(char *msg) {
    perror(msg);
    exit(1);
}

qos_t current_qos;

uint32_t QKD_OPEN(ip_address_t destination, qos_t qos, key_handle_t* key_handle) {
    current_qos.requested_length = qos.requested_length;

    int all_zeros = 1;
    for (size_t i = 0; i < KEYHANDLE_SIZE; i++) {
        if ((*key_handle)[i] != 0) {
            all_zeros = 0;
            break;
        }
    }

    if (all_zeros == 1) {
        for (size_t i = 0; i < KEYHANDLE_SIZE; i++) {
            (*key_handle)[i] = (uint8_t) rand() % 256;
        }
    }

    return SUCCESS;
}

uint32_t QKD_CONNECT_NONBLOCKING(key_handle_t* key_handle, uint32_t timeout) {

    return SUCCESS;
}

uint32_t QKD_CONNECT_BLOCKING(key_handle_t* key_handle, uint32_t timeout) {

    return SUCCESS;
}

uint32_t QKD_GET_KEY(key_handle_t* key_handle, uint8_t* key_buffer) {
    const char* hostname = "localhost";
    const char* portname = "8080";
    char buf[BUFSIZE];
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_ADDRCONFIG;
    struct addrinfo* res = 0;
    int err = getaddrinfo(hostname, portname, &hints, &res);
    if (err != 0) {
        error("ERROR with getaddrinfo");
    }

    int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sd == -1) {
        error("ERROR opening socket");
    }

    int reuseaddr = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)) == -1) {
        error("ERROR with setsockopt");
    }

    if (bind(sd, res->ai_addr, res->ai_addrlen) == -1) {
        /************
         *  CLIENT  *
         ************/
        printf("Port busy - acting as client\n");
        if (connect(sd, res->ai_addr, res->ai_addrlen) < 0) {
            error("ERROR connecting");
        }

        /* Send key_handle first */
        int n = write(sd, key_handle, KEYHANDLE_SIZE * sizeof(uint8_t));
        if (n < 0) {
            error("ERROR writing to socket");
        }

        /* Check whether key_handle was accepted */
        n = read(sd, buf, sizeof(uint8_t));
        if (n < 0) {
            error("ERROR reading from socket");
        }

        if (strncmp(buf, "0", 1) != 0) {
            /* key_handle is different from what was expected */
            printf("--> WRONG key_handle\n");
            freeaddrinfo(res);
            close(sd);
            return QKD_GET_KEY_FAILED;
        }

        /* key_handle is correct */
        n = read(sd, key_buffer, current_qos.requested_length * sizeof(uint8_t));
        if (n < 0) {
            error("ERROR reading from socket");
        }
        freeaddrinfo(res);
        close(sd);
        return SUCCESS;
    } else {
        /************
         *  SERVER  *
         ************/
        printf("Open port %s - acting as server\n", portname);
        if (listen(sd, SOMAXCONN)) {
            error("FAILED to listen for connections");
        }
        int session_fd = accept(sd, 0, 0);

        /* Check whether key_handle is correct */
        int n = read(session_fd, buf, BUFSIZE);
        if (n < 0) {
            error("ERROR reading from socket");
        }

        if (memcmp(*key_handle, buf, KEYHANDLE_SIZE) != 0) {
            /* key_handle is different from what was expected */
            n = write(session_fd, "1", sizeof(uint8_t));
            if (n < 0) {
                error("ERROR writing to socket");
            }
            freeaddrinfo(res);
            close(sd);
            close(session_fd);
            return QKD_GET_KEY_FAILED;
        }

        /* key_handle is correct */
        strcpy(buf, "0");
        n = write(session_fd, buf, sizeof(uint8_t));
        if (n < 0) {
            error("ERROR writing to socket");
        }

        /* Key generation */
        for (size_t i = 0; i < current_qos.requested_length; i++) {
            key_buffer[i] = (uint8_t) rand() % 256;
        }

        n = write(session_fd, key_buffer, current_qos.requested_length * sizeof(uint8_t));
        if (n < 0) {
            error("ERROR writing to socket");
        }
        close(session_fd);
        freeaddrinfo(res);
        close(sd);
        return SUCCESS;
    }
}

uint32_t QKD_CLOSE(key_handle_t* key_handle) {

    return SUCCESS;
}
