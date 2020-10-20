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

typedef struct Socket{
    int sfd;
    struct addrinfo hints;
    bool isUp;
} socket_t;

int socket_init(socket_t *self){
    memset(self, 0, sizeof(socket_t));
    self->hints.ai_family = AF_INET;
    self->hints.ai_socktype = SOCK_STREAM;//SOCK_DGRAM;//
    self->isUp = false;
    return 0;
}

int socket_connect(socket_t *self, const char *service, const char *port){
    struct addrinfo *result, *result_it;
    if ((getaddrinfo(port,service, &self->hints, &result)) != 0) {
        return 1;
    }
    result_it = result;
    for(int sfd = 0; result_it != NULL; result_it = result_it->ai_next){
        if((sfd = socket(result_it->ai_family, result_it->ai_socktype, result_it->ai_protocol)) != -1){
            if (connect(sfd, result_it->ai_addr, result_it->ai_addrlen) != -1){
                self->sfd = sfd;
                freeaddrinfo(result);
                return 0;
            }else{
                close(sfd);
            }
        }
    }
    freeaddrinfo(result);
    return 1;
}

bool socket_isUp(socket_t *self){
    return self->isUp;
}

int socket_bind(socket_t *self, const char *port){
    struct addrinfo *result, *result_it;

    if ((getaddrinfo(NULL, port, &self->hints, &result)) != 0) {
        return 1;
    }
    result_it = result;
    for(int sfd = 0; result_it != NULL; result_it = result_it->ai_next){
        if((sfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) != -1){
            if (bind(sfd, result_it->ai_addr, result_it->ai_addrlen) != -1){
                self->sfd = sfd;
                self->isUp = true;
                freeaddrinfo(result);
                return 0;
            }else{
                close(sfd);
            }
        }
    }
    freeaddrinfo(result);
    return 1;
}

int socket_accept(socket_t *self){
    struct sockaddr peer_addr;
    socklen_t peer_addr_len = sizeof(peer_addr);
    int cfd;
    if((cfd = accept(self->sfd, &peer_addr,&peer_addr_len)) == -1 ){
        return 1;
    }
    self->sfd = cfd;
    return 0;
}

int socket_listen(socket_t *self){
    if(listen(self->sfd,10) == -1){
        return 1;
    }
    return 0;
}

int socket_send_chunk(socket_t *self, const char *data, unsigned int length){
    for (int total = 0,cicle = 0; total < (int)length; total += cicle)
    {
        if((cicle = send(self->sfd,data,length,0)) == -1){
            // fprintf("Err send -1\n");
            return 1;
        }
        if(cicle){
            // fprintf("\nsent: %i\n",cicle);
            write(1,data,cicle);
        } 
    }
    return 0;
}

int socket_read(socket_t *self, unsigned char *buffer, unsigned int length){
    for (int total = 0,cicle = 0; total < (int)length; total += cicle){
        if((cicle = read(self->sfd,buffer,length)) == -1){
            // printf("Err receive -1\n");
            return -1;
        }
        if (cicle == 0){
            // fprintf("Conection ended\n");
            self->isUp = false;
            return total + cicle;
        }
    }
    return length;
}

int socket_uninit(socket_t *self){
    shutdown(self->sfd, SHUT_RDWR);
    close(self->sfd);
    self->sfd=-1;
    return 0;
}