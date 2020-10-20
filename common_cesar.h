#ifndef COMMON_CESAR_H
#define COMMON_CESAR_H

typedef struct{
    unsigned int key;
}cesar_state;

int cesar_init(void *self, unsigned int key);

int cesar_encoder(void *self,unsigned char const* input, unsigned char* output, unsigned int input_length);

int cesar_decoder(void *self,unsigned char const* input, unsigned char* output, unsigned int input_length);

#endif