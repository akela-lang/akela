#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include <stdbool.h>
#include "hash.h"
#include "token.h"

#define ENVIRONMENT_HASH_TABLE_SIZE 32

struct environment {
	struct hash_table ht;
	struct environment* prev;
};

struct symbol {
	enum token_enum tk_type;
	struct dag_node* dec;
};

struct symbol_table {
	struct environment* initial;
	struct environment* top;
};

/* dynamic-output env{} */
/* transfer p -> env */
void environment_init(struct environment* env, struct environment* p);

/* dynamic-output env{} */
/* transfer sym -> env{} */
void environment_put(struct environment* env, struct buffer* value, struct symbol* sym);

/* dynamic-output-none */
struct symbol* environment_get(struct environment* env, struct buffer* value);

/* dynamic-output-none */
struct symbol* environment_get_local(struct environment* env, struct buffer* value);

/* dynamic-output_none */
void symbol_init(struct symbol* sym);

/* dynamic-destroy env env{} */
void environment_destroy(struct environment* env);

void symbol_table_init(struct symbol_table* st);

void symbol_table_destroy(struct symbol_table* st);

bool symbol_table_is_global(struct symbol_table* st);

#endif
