#ifndef AKELA_COMP_TABLE_H
#define AKELA_COMP_TABLE_H

#define COMP_TABLE_HASH_TABLE_SIZE 32

#include "api.h"
#include "zinc/hash.h"
#include <stdbool.h>
#include "comp_unit.h"

typedef struct Ake_comp_table {
	struct hash_table ht;
} Ake_comp_table;

AKELA_API void Ake_comp_table_init(struct Ake_comp_table* ct);
AKELA_API void Ake_comp_table_put(struct Ake_comp_table* ct, struct Zinc_string* path, struct Ake_comp_unit* cu);
AKELA_API struct Ake_comp_unit* Ake_comp_table_get(struct Ake_comp_table* ct, struct Zinc_string* path);
AKELA_API void Ake_comp_table_destroy_comp_unit(struct Ake_comp_unit* cu);
AKELA_API void Ake_comp_table_destroy(struct Ake_comp_table* ct);
AKELA_API bool Ake_include_base(struct Ake_comp_table* ct, struct Ake_comp_unit* cu, struct Ake_comp_unit** base);

#endif
