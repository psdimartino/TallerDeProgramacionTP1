#ifndef COMMON_RC4_H
#define COMMON_RC4_H

#include <string.h>

typedef struct RC4_state{
    unsigned char S[256];
    int i, j;
}rc4_state;


int RC4_init(void *self, unsigned char *key, unsigned int key_length);

int RC4_encoder(void *self, const unsigned char *input, unsigned char *output, unsigned int input_length);

#endif