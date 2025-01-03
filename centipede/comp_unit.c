#include "comp_unit.h"
#include "zinc/memory.h"

void Cent_comp_unit_init(Cent_comp_unit *unit)
{
    unit->errors = NULL;
    unit->input = NULL;
    unit->input_vtable = NULL;
}

void Cent_comp_unit_create(Cent_comp_unit **unit)
{
    malloc_safe((void**)unit, sizeof(Cent_comp_unit));
    Cent_comp_unit_init(*unit);
}

void Cent_comp_table_init(Cent_comp_table* table)
{
    table->primary = NULL;
    hash_table_init(&table->ht, 32);
}

void Cent_comp_table_create(Cent_comp_table** table)
{
    malloc_safe((void**)table, sizeof(Cent_comp_table));
    Cent_comp_table_init(*table);
}

void Cent_comp_table_destroy(Cent_comp_table* table)
{
    hash_table_destroy(&table->ht);
}

void Cent_comp_table_add(Cent_comp_table* table, struct buffer* name, Cent_comp_unit *unit)
{
    hash_table_add(&table->ht, name, unit);
}

void Cent_comp_table_add_str(Cent_comp_table* table, char* name, Cent_comp_unit *unit)
{
    hash_table_add_str(&table->ht, name, unit);
}

Cent_comp_unit* Cent_comp_table_get(Cent_comp_table* table, struct buffer* name)
{
    return hash_table_get(&table->ht, name);
}

Cent_comp_unit* Cent_comp_table_get_str(Cent_comp_table* table, char* name)
{
    return hash_table_get_str(&table->ht, name);
}

void Cent_comp_table_map(Cent_comp_table* table, Cent_comp_table_func func)
{
    hash_table_map(&table->ht, (hash_table_func)func);
}

void Cent_comp_table_map_name(Cent_comp_table* table, Cent_comp_table_func_name func)
{
    hash_table_map_name(&table->ht, (hash_table_func_name)func);
}