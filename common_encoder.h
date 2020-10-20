#ifndef COMMON_ENCODER_H
#define COMMON_ENCODER_H

#include <stdio.h>

enum encoder_e {
    NONE,
    CESAR,
    VIGENERE,
    RC4
};

typedef struct Encoder_t{
    int (*pEncoder)(void *, const unsigned char *, unsigned char *, unsigned);
    int (*pDecoder)(void *, const unsigned char *, unsigned char *, unsigned);
    void *state;
}encoder_t;

int encoder_init(encoder_t *self, enum encoder_e algoritm,unsigned char *key,
                                                    unsigned int key_lenght);

int encoder_encode(encoder_t *self, unsigned char *input,
                            unsigned char *output,unsigned int input_length); 

int encoder_decode(encoder_t *self, unsigned char *input,
                            unsigned char *output, unsigned int input_length);

int encoder_uninit(encoder_t *self);

#endif
