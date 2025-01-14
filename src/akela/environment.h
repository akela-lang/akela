#ifndef AKELA_ENVIRONMENT_H
#define AKELA_ENVIRONMENT_H

#include "api.h"
#include "zinc/hash.h"
#include "symbol.h"

#define ENVIRONMENT_HASH_TABLE_SIZE 32

struct environment {
    struct hash_table ht;
    struct environment* prev;
};

struct symbol;

AKELA_API void environment_init(struct environment* env, struct environment* p);
AKELA_API void environment_create(struct environment** env, struct environment* p);
AKELA_API void environment_put(struct environment* env, struct buffer* value, struct symbol* sym);
AKELA_API struct symbol* environment_get(struct environment* env, struct buffer* value);
AKELA_API struct symbol* environment_get_local(struct environment* env, struct buffer* value);
AKELA_API void environment_destroy(struct environment* env);
AKELA_API void environment_destroy_symbol(struct symbol* sym);

#endif