#include "comp_table.h"
#include "zinc/memory.h"
#include <assert.h>

void Cent_comp_table_init(
    Cent_comp_table* ct,
    void* module_finder_obj,
    Cent_module_vtable* module_finder_vtable,
    Cent_environment* base)
{
    ct->primary = NULL;
    hash_table_init(&ct->ht, 32);
    ct->module_finder_obj = module_finder_obj;
    ct->module_finder_vtable = module_finder_vtable;
    ct->base = base;
}

void Cent_comp_table_create(
    Cent_comp_table** ct,
    void* module_finder_obj,
    Cent_module_vtable* module_finder_vtable,
    Cent_environment* base)
{
    malloc_safe((void**)ct, sizeof(Cent_comp_table));
    Cent_comp_table_init(*ct, module_finder_obj, module_finder_vtable, base);
}

void Cent_comp_table_destroy(Cent_comp_table* table)
{
    hash_table_map(&table->ht, Cent_comp_unit_free);
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

Cent_comp_unit* Cent_comp_table_find_unit(Cent_comp_table* ct, struct buffer* name)
{
    Cent_comp_unit* cu = Cent_comp_table_get(ct, name);
    if (cu) return cu;

    Cent_input_data data = Cent_module_find_interface(
        ct->module_finder_obj,
        ct->module_finder_vtable, name);

    assert(data.input);
    assert(data.input_vtable);

    String_slice file_name;
    file_name.p = name->buf;
    file_name.size = name->size;
    Cent_comp_unit_create(
        &cu,
        data.input,
        data.input_vtable,
        file_name,
        ct->module_finder_obj,
        ct->module_finder_vtable,
        ct->base);
    cu->input = data.input;
    cu->input_vtable = data.input_vtable;

    return cu;
}