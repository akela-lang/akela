#ifndef _COMP_TABLE_H
#define _COMP_TABLE_H

#define COMP_TABLE_HASH_TABLE_SIZE 32

#include "api.h"
#include "zinc/hash.h"
#include <stdbool.h>
#include "comp_unit.h"

struct comp_table {
	struct hash_table ht;
};

AKELA_API void comp_table_init(struct comp_table* ct);
AKELA_API void comp_table_put(struct comp_table* ct, struct buffer* path, struct comp_unit* cu);
AKELA_API struct comp_unit* comp_table_get(struct comp_table* ct, struct buffer* path);
AKELA_API void comp_table_destroy_comp_unit(struct comp_unit* cu);
AKELA_API void comp_table_destroy(struct comp_table* ct);
AKELA_API bool include_base(struct comp_table* ct, struct comp_unit* cu, struct comp_unit** base);

#endif
