
#ifndef COMMON_SOCKET_H
#define COMMON_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

typedef struct Socket{
    int sfd;
    struct addrinfo hints;
    struct sockaddr peer_addr;
} socket_t;

int socket_init(socket_t *self);

int socket_connect(socket_t *self, const char *service, const char *port);

int socket_send_chunk(socket_t *self, const char *data, unsigned int length);

int socket_read(socket_t *self, unsigned char *buffer, unsigned int length);

int socket_uninit(socket_t *self);

int socket_bind(socket_t *self, const char *port);

int socket_accept(socket_t *self);

int socket_listen(socket_t *self);

bool socket_isUp(socket_t *self);

#endif