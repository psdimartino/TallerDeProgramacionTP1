#define _POSIX_C_SOURCE 200112L
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include "common_socket.h"
#include "common_encoder.h"
#include "common_argInterpreter.h"

#define BUF_SIZE 64
#define NI_MAXHOST 1025
#define NI_MAXSERV 32

int main(int argc, char *argv[] ) {
    unsigned char buf[BUF_SIZE];
    enum encoder_e algoritm = RC4;
    encoder_t encoder;
    socket_t skt;
    char *port;
    char *key;

    if ( argInterpreter_server(argc, argv, &key, &algoritm, &port ) ) {
        return 1;
    }
    if ( encoder_init(&encoder, algoritm, (unsigned char *)key, strlen(key)) ) {
        return 1;
    }
    if ( socket_init(&skt) ) {
        encoder_uninit(&encoder);
        return 1;
    }
    if ( socket_bind(&skt, argv[1]) ) {
        encoder_uninit(&encoder);
        return 1;
    }
    if ( socket_listen(&skt) ) {
        socket_uninit(&skt);
        encoder_uninit(&encoder);
        return 1;
    }
    if ( socket_accept(&skt) ) {
        socket_uninit(&skt);
        encoder_uninit(&encoder);
        return 1;
    }
    while ( socket_isUp(&skt) ) {
        int read;
        memset(buf, 0, BUF_SIZE);  // Valgrind warning of uninitialized bytes
        if ( (read = socket_read(&skt, buf, BUF_SIZE)) == -1 ) {
            socket_uninit(&skt);
            encoder_uninit(&encoder);
            return 1;
        }
        if ( encoder_decode(&encoder, buf, buf, BUF_SIZE) ) {
            socket_uninit(&skt);
            encoder_uninit(&encoder);
            return 1;
        }
        if ( write(1, buf, read) == -1 ) {
            socket_uninit(&skt);
            encoder_uninit(&encoder);
            return 1;
        }
    }
    if ( socket_uninit(&skt) ) {
        encoder_uninit(&encoder);
        return 1;
    }
    if ( encoder_uninit(&encoder) ) {
        return 1;
    }
}
