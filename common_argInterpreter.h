#ifndef COMMON_ARGINTERPRETER_H
#define COMMON_ARGINTERPRETER_H

#include "common_encoder.h"

int argInterpreter_server(int argc, char *argv[], char **key,
                                        enum encoder_e *algoritm, char **port);
int argInterpreter_client(int argc, char *argv[], char **key,
                        enum encoder_e *algoritm, char **service, char **port);

#endif
