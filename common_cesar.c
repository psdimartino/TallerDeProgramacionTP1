#include <string.h>
#include <stdio.h>
#include "./common_cesar.h"

int cesar_init(void *_self, unsigned int key ) {
    cesar_state *self = (cesar_state *)_self;
    self->key = key;
    return 0;
}

int cesar_encoder(void *_self, unsigned char const* input,
                    unsigned char* output, unsigned int input_length ) {
    if  ( !output || !input ) {
        return 1;
    }
    cesar_state *self = (cesar_state *)_self;
    for ( unsigned int i = 0; i < input_length; i++ ) {
        output[i] = (input[i] + (self->key)) % 256;
    }
    return 0;
}

int cesar_decoder(void *_self, unsigned char const* input,
                    unsigned char* output, unsigned int input_length ) {
    if ( !output || !input ) {
        return 1;
    }
    cesar_state *self = (cesar_state *)_self;
    for ( unsigned int i = 0; i < input_length; i++ ) {
        output[i] = (input[i] - (self->key)) % 256;
    }
    return 0;
}
