#define _POSIX_C_SOURCE 200112L
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "common_socket.h"
#include "common_encoder.h"
#include "common_argInterpreter.h"

#define BUFFER_SIZE 64
#define KEY_MAXLENGTH 64

int main(int argc, char *argv[]) {
    char *key = NULL , *service = NULL, *port = NULL;
    enum encoder_e algoritm = RC4;
    bool stream_ended = false;
    unsigned char buf[BUFFER_SIZE];
    if ( argInterpreter_client(argc, argv, &key, &algoritm, &service, &port) ) {
        return 1;
    }
    encoder_t encoder;
    if ( encoder_init(&encoder, algoritm, (unsigned char*)key, strlen(key)) ) {
        return 1;
    }
    socket_t skt;
    if ( socket_init(&skt) ) {
        encoder_uninit(&encoder);
        return 1;
    }
    if ( socket_connect(&skt, argv[2], argv[1]) ) {
        encoder_uninit(&encoder);
        return 1;
    }
    while (!stream_ended) {
        int input_size;
        if ( (input_size = read(0, buf, BUFFER_SIZE) ) == -1 ) {
            socket_uninit(&skt);
            encoder_uninit(&encoder);
            return 1;
        }
        if ( input_size < BUFFER_SIZE) stream_ended = true;
        if ( encoder_encode(&encoder, buf, buf, input_size) ) {
            socket_uninit(&skt);
            encoder_uninit(&encoder);
            return 1;
        }
        // for (int i = 0; i < 3; i++){
        //     printf("%02X|", buf[i]);
        // }
        if ( socket_send(&skt, (char *)buf, input_size) ) {
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
    return 0;
}
