#ifndef COMMON_VIGENERE_H
#define COMMON_VIGENERE_H

#include <string.h>

typedef struct Vigenere_state{
    unsigned char *key;
    unsigned int key_pos, key_length;
}vigenere_state;

int vigenere_init(void *self, unsigned char *key, unsigned int key_length);

int vigenere_encoder(void *self,unsigned char const* input, unsigned char* output, unsigned int input_length);

int vigenere_decoder(void *self,unsigned char const* input, unsigned char* output, unsigned int input_length);

#endif