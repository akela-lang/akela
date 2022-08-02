#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include "hash.h"

#define ENVIRONMENT_HASH_TABLE_SIZE 32

struct environment {
	struct hash_table ht;
	struct environment* prev;
};

struct symbol {
	struct buffer type;
	struct dag_node* dec;
};

/* dynamic-output env{} */
/* transfer p -> env */
void environment_init(struct environment* env, struct environment* p);

/* dynamic-output env{} */
/* transfer sym -> env{} */
void environment_put(struct environment* env, struct buffer* value, struct symbol* sym);

/* dynamic-output-none */
struct symbol* environment_get(struct environment* env, struct buffer* value);

/* dynamic-output_none */
void symbol_init(struct symbol* sym);

/* dynamic-destroy env env{} */
void environment_destroy(struct environment* env);

#endif
