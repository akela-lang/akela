#ifndef AKELA_ENVIRONMENT_H
#define AKELA_ENVIRONMENT_H

#include "api.h"
#include "zinc/hash.h"
#include "symbol.h"

#define ENVIRONMENT_HASH_TABLE_SIZE 32

typedef struct Ake_environment {
    struct hash_table ht;
    struct Ake_environment* prev;
} Ake_environment;

struct Ake_symbol;

AKELA_API void Ake_environment_init(struct Ake_environment* env, struct Ake_environment* p);
AKELA_API void Ake_environment_create(struct Ake_environment** env, struct Ake_environment* p);
AKELA_API void Ake_environment_put(struct Ake_environment* env, struct buffer* value, struct Ake_symbol* sym);
AKELA_API struct Ake_symbol* Ake_environment_get(struct Ake_environment* env, struct buffer* value);
AKELA_API struct Ake_symbol* Ake_environment_get_local(struct Ake_environment* env, struct buffer* value);
AKELA_API void Ake_environment_destroy(struct Ake_environment* env);
AKELA_API void Ake_environment_destroy_symbol(struct Ake_symbol* sym);

#endif