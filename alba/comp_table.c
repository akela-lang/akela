#include "comp_table.h"
#include "hash.h"
#include "comp_unit.h"
#include "zinc/memory.h"

void comp_table_init(struct comp_table* ct)
{
	hash_table_init(&ct->ht, COMP_TABLE_HASH_TABLE_SIZE);
}

void comp_table_put(struct comp_table* ct, struct buffer* path, struct comp_unit* cu)
{
	hash_table_add(&ct->ht, path, cu);
}

struct comp_unit* comp_table_get(struct comp_table* ct, struct buffer* path)
{
	return hash_table_get(&ct->ht, path);
}

void comp_table_destroy_comp_unit(struct comp_unit* cu)
{
	comp_unit_destroy(cu);
	free(cu);
}

void comp_table_destroy(struct comp_table* ct)
{
	hash_table_map(&ct->ht, (void (*)(void*))comp_table_destroy_comp_unit);
	hash_table_destroy(&ct->ht);
}

struct comp_unit* include_base(struct comp_table* ct, struct comp_unit* cu)
{
	struct buffer bf_base;
	buffer_init(&bf_base);
	buffer_copy_str(&bf_base, "function sqrt(x::Int64)::Int64 1 end");
	struct string_data sd_base;
	string_data_init(&bf_base, &sd_base);

	struct comp_unit* cu_base = NULL;
	malloc_safe(&cu_base, sizeof(struct comp_unit));
	comp_unit_init(cu_base);
	array2buffer("|math|", &cu_base->path);
	comp_table_put(ct, &cu_base->path, cu_base);

	comp_unit_compile(cu_base, string_getchar, &sd_base);

	transfer_global_symbols(&cu_base->st, &cu->st);

	buffer_destroy(&bf_base);

	return cu_base;
}
