#include <stdio.h>
#include <stdlib.h>
#include "common_cesar.h"
#include "common_vigenere.h"
#include "common_RC4.h"
#include "common_encoder.h"

int encoder_init(encoder_t *self, enum encoder_e algoritm, unsigned char *key, unsigned int key_lenght){
    switch (algoritm){
    case NONE:
        return 1;
        break;
    case CESAR:
        // printf("Cesar selected, key = %i\n",atoi((char *)key));
        self->pEncoder = &cesar_encoder;
        self->pDecoder = &cesar_decoder;
        self->state = malloc(sizeof(cesar_state));
        cesar_init(self->state,atoi((char *)key));
        // printf("state->key:%u\n", (((cesar_state *)(self->state))->key) );
        break;
    case VIGENERE:
        self->pEncoder = &vigenere_encoder;
        self->pDecoder = &vigenere_decoder;
        self->state = malloc(sizeof(vigenere_state));
        vigenere_init(self->state,key,key_lenght);
        break;
    case RC4:
        self->pEncoder = &RC4_encoder;
        self->pDecoder = &RC4_encoder;
        self->state = malloc(sizeof(rc4_state));
        RC4_init(self->state,key,key_lenght);
        break;
    default:
        return 1;
    }
    return 0;
}

int encoder_encode(encoder_t *self, unsigned char *input, unsigned char *output,unsigned int input_length){
    return (*(self->pEncoder))(self->state, input, output, input_length);
    // printf("encoderr state->key:%u\n", (((cesar_state *)(self->state))->key) );
    // printf("encoder_encode - Input:%s - Output:%s",input,output);
    // return 0;
}

int encoder_decode(encoder_t *self, unsigned char *input, unsigned char *output,unsigned int input_length){
    return (*(self->pDecoder))(self->state, input, output, input_length);
    // printf("encoder_decode - Input:%s - Output:%s",input,output);
    // return 0;
}

int encoder_uninit(encoder_t *self){
    free(self->state);
    return 0;
}
