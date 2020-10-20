#include "common_encoder.h"
#include <string.h>

int argInterpreter_obtaintAlgoritm(char *arg, enum encoder_e *algoritm){
    if(strcmp(arg,"--method=cesar") == 0){
        *algoritm = CESAR;
    }else if(strcmp(arg,"--method=vigenere") == 0){
        *algoritm = VIGENERE;
    }else if(strcmp(arg,"--method=rc4") == 0){
        *algoritm = RC4;
    }else{
        return 1;
    }
    return 0;
}

int argInterpreter_obtainKey(char *arg,char **key){
    if(strncmp(arg,"--key=",6) == 0){
        *key = &arg[6];
    }
    return 0;
}

int argInterpreter_server(int argc, char *argv[], char **key, enum encoder_e *algoritm, char **port){
    if(argc != 4){
        return 1;
    }
    *port = argv[1];
    if(argInterpreter_obtaintAlgoritm(argv[2],algoritm)){
        return 1;
    }
    if(argInterpreter_obtainKey(argv[3],key)){
        return 1;
    }
    return 0;
}

int argInterpreter_client(int argc, char *argv[], char **key, enum encoder_e *algoritm, char **service, char **port){
    if(argc != 5){
        return 1;
    }
    *service = argv[1];
    *port = argv[2];
    if(argInterpreter_obtaintAlgoritm(argv[3],algoritm)){
        return 1;
    }
    if(argInterpreter_obtainKey(argv[4],key)){
        return 1;
    }
    return 0;
}