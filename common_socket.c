#define _POSIX_C_SOURCE 200112L
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#define BUF_SIZE 64

typedef struct Socket {
    int sfd;
    struct addrinfo hints;
    bool isUp;
} socket_t;

int socket_init(socket_t *self ) {
    memset(self, 0, sizeof(socket_t));
    self->hints.ai_family = AF_INET;
    self->hints.ai_socktype = SOCK_STREAM;
    self->isUp = false;
    return 0;
}

int socket_connect(socket_t *self, const char *service, const char *port ) {
    struct addrinfo *result, *it;
    if ((getaddrinfo(port, service, &self->hints, &result)) != 0) return 1;
    for ( self->sfd = 0, it = result; it != NULL; it = it->ai_next ) {
        self->sfd = socket(it->ai_family, it->ai_socktype, it->ai_protocol);
        if (self->sfd != -1) {
            if ( connect(self->sfd, it->ai_addr, it->ai_addrlen) != -1 ) {
                freeaddrinfo(result);
                return 0;
            } else { close(self->sfd); }
        }
    }
    freeaddrinfo(result);
    return 1;
}

int socket_bind(socket_t *self, const char *port ) {
    struct addrinfo *result, *it;
    if ((getaddrinfo(NULL, port, &self->hints, &result)) != 0) return 1;
    it = result;
    for ( self->sfd = 0; it != NULL; it = it->ai_next ) {
        self->sfd = socket(it->ai_family, it->ai_socktype, it->ai_protocol);
        if ( self->sfd != -1 ) {
            if ( bind(self->sfd, it->ai_addr, it->ai_addrlen) == 0 ) {
                self->isUp = true;
                freeaddrinfo(result);
                return 0;
            } else { close(self->sfd); }
        }
    }
    freeaddrinfo(result);
    return 1;
}

bool socket_isUp(socket_t *self ) {
    return self->isUp;
}


int socket_accept(socket_t *self) {
    struct sockaddr peer_addr;
    socklen_t peer_addr_len = sizeof(peer_addr);
    int cfd;
    if ( (cfd = accept(self->sfd, &peer_addr, &peer_addr_len)) == -1 ) {
        return 1;
    }
    close(self->sfd);
    self->sfd = cfd;
    return 0;
}

int socket_listen(socket_t *self ) {
    if ( listen(self->sfd, 10) == -1 ) {
        return 1;
    }
    return 0;
}

int socket_send(socket_t *self, const char *data, int length ) {
    for ( int total = 0, cicle = 0; total < length; total += cicle ) {
        cicle = send(self->sfd, &data[total], length - total, MSG_NOSIGNAL);
        if ( cicle == -1 ) {
            return 1;
        }
    }
    return 0;
}

int socket_read(socket_t *self, unsigned char *buffer, int length ) {
    for ( int total = 0, cicle = 0; total < length; total += cicle ) {
        cicle = recv(self->sfd, &buffer[total], length - total , 0);
        if ( cicle == -1 ) return -1;
        if ( cicle == 0 ) {
            if ( (total + cicle) < length ) self->isUp = false;
            return total + cicle;
        }
    }
    return length;
}

int socket_uninit(socket_t *self ) {
    close(self->sfd);
    self->sfd = -1;
    return 0;
}
