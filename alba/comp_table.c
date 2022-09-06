#include "comp_table.h"
#include "hash.h"
#include "comp_unit.h"

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
