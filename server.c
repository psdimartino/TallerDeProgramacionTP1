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

int main(int argc, char *argv[]){
    char *key;
    char *port;
    enum encoder_e algoritm = RC4;

    if(argInterpreter_server(argc,argv,&key,&algoritm, &port)){
        return 1;
    }
    
    encoder_t encoder;
    if(encoder_init(&encoder,algoritm,(unsigned char *)key,strlen(key))){
        return 1;
    }
    
    socket_t skt;
    socket_init(&skt);
    unsigned char buf[BUF_SIZE],buf2[BUF_SIZE];

    if(socket_bind(&skt,argv[1])){
        return 1;
    }
    if(socket_listen(&skt)){
        return 1;
    }
    if(socket_accept(&skt)){
        return 1;
    }
    while (socket_isUp(&skt)){
        int read;   
        if((read = socket_read(&skt,buf,BUF_SIZE)) == -1){
            return 1;
        }
        if(encoder_decode(&encoder,buf,buf2,BUF_SIZE)){
            return 1;
        }
        write(0,buf2,read);
    }
    if(socket_uninit(&skt)){
        return 1;
    }
    if(encoder_uninit(&encoder)){
        return 1;
    }
}
