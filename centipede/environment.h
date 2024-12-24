#ifndef CENTIPEDE_ENVIRONMENT_H
#define CENTIPEDE_ENVIRONMENT_H

#include "zinc/hash.h"

typedef struct Cent_environment {
    struct hash_table symbols;
    struct Cent_environment* prev;
} Cent_environment;

#endif