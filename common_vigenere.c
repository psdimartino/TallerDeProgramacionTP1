#include <string.h>
#include <stdio.h>
#include "common_vigenere.h"

int vigenere_init(void *_self, unsigned char *key, unsigned int key_length ) {
    vigenere_state *self = (vigenere_state *)_self;
    self->key = key;
    self->key_length = key_length;
    self->key_pos = 0;
    return 0;
}

int vigenere_encoder(void *_self, unsigned char const* input,
                        unsigned char* output, unsigned int input_length ) {
    vigenere_state *self = (vigenere_state *)_self;
    if ( !output || !input ) {
        return 1;
    }
    for ( unsigned int i = 0; i < input_length; i++, self->key_pos++ ) {
        if ( (self->key_pos) == (self->key_length) ) {
            self->key_pos = 0;
        }
        output[i] = (input[i] + (self->key)[self->key_pos]) % 256;
    }
    return 0;
}

int vigenere_decoder(void *_self, unsigned char const* input,
                        unsigned char* output, unsigned int input_length ) {
    vigenere_state *self = (vigenere_state *)_self;
    if ( !output || !input ) {
        return 1;
    }
    for ( unsigned int i = 0; i < input_length; i++, self->key_pos++ ) {
        if ( (self->key_pos) == (self->key_length) ) {
            self->key_pos = 0;
        }
        output[i] = (input[i] - (self->key)[self->key_pos]) % 256;
    }
    return 0;
}
