#include <string.h>
#include <stdio.h>
#include "common_RC4.h"

void swap(unsigned char *s, unsigned int i, unsigned int j) {
    unsigned char temp = s[i];
    s[i] = s[j];
    s[j] = temp;
}

int RC4_init(void *_self, unsigned char *key, unsigned int key_length ) {
    rc4_state *self = (rc4_state *)_self;
    for ( int k = 0; k < 256; k++ ) {
        (self->S)[k] = k;
    }
    for ( int i = 0, j = 0; i < 256; i++ ) {
        j = (j + key[i % key_length] + (self->S)[i] ) & 255;
        swap(self->S, i, j);
    }
    self->i = self->j = 0;
    return 0;
}

int RC4_encoder(void *_self, const unsigned char *input, unsigned char *output,
                                                  unsigned int input_length ) {
    rc4_state *self = (rc4_state *)_self;
    for ( unsigned int y = 0; y < input_length; y++ ) {
        self->i = (self->i + 1) & 255;
        self->j = (self->j + self->S[self->i]) & 255;
        swap(self->S, self->i, self->j);
        output[y] =
            input[y] ^ self->S[(self->S[self->i] + self->S[self->j]) & 255];
    }
    return 0;
}
