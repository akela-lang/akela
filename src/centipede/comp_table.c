#include "comp_table.h"
#include "zinc/memory.h"
#include <assert.h>
#include "base.h"
#include "module_finder_string.h"
#include "module_finder_file.h"
#include "zinc/input_unicode_file.h"
#include <string.h>

void Cent_comp_table_init(
    Cent_comp_table* ct,
    void* module_finder_obj,
    Cent_module_finder_vtable* module_finder_vtable)
{
    ct->primary = NULL;
    Zinc_hash_map_string_init(&ct->ht, 32);
    ct->module_finder_obj = module_finder_obj;
    ct->module_finder_vtable = module_finder_vtable;
    ct->base = Cent_base_create();

}

void Cent_comp_table_create(
    Cent_comp_table** ct,
    void* module_finder_obj,
    Cent_module_finder_vtable* module_finder_vtable)
{
    Zinc_malloc_safe((void**)ct, sizeof(Cent_comp_table));
    Cent_comp_table_init(*ct, module_finder_obj, module_finder_vtable);
}

void Cent_comp_table_init_str(Cent_comp_table* ct, char* s)
{
    Cent_module_finder_string* ms = NULL;
    Cent_module_finder_string_create(&ms);

    Cent_comp_table_init(ct, ms, ms->vtable);

    Zinc_string_slice file_name;
    file_name.p = "**string**";
    file_name.size = strlen(file_name.p);
    Zinc_string name;
    Zinc_string_init(&name);
    Zinc_string_add(&name, file_name.p, file_name.size);
    Zinc_string_finish(&name);

    Cent_module_finder_string_add_module_str_str(ct->module_finder_obj, name.buf, s);
    Cent_input_data data = Cent_module_finder_find(
        ms,
        ms->vtable,
        &name);
    assert(data.input);
    assert(data.input_vtable);

    Cent_comp_unit* cu = NULL;
    Cent_comp_unit_create(
        &cu,
        data.input,
        data.input_vtable,
        file_name,
        ct->base);
    cu->ct = ct;
    cu->pd.ct = ct;
    ct->primary = cu;
    Cent_comp_table_add(ct, &name, cu);
    Zinc_string_destroy(&name);
}

void Cent_comp_table_create_str(Cent_comp_table** ct, char* s)
{
    Zinc_malloc_safe((void**)ct, sizeof(Cent_comp_table));
    Cent_comp_table_init_str(*ct, s);
}

void Cent_comp_table_init_fp(Cent_comp_table* ct, Zinc_string* dir_path, Zinc_string* name, FILE* fp)
{
    Cent_module_finder_file* mff = NULL;
    Cent_module_finder_file_create(&mff, dir_path);

    Cent_comp_table_init(ct, mff, mff->vtable);

    Zinc_input_unicode_file* input = NULL;
    Zinc_input_unicode_file_create(&input, fp);

    Zinc_string_slice name_slice;
    name_slice.p = name->buf;
    name_slice.size = name->size;

    Cent_comp_unit* cu = NULL;
    Cent_comp_unit_create(
        &cu,
        input,
        input->vtable,
        name_slice,
        ct->base);
    cu->ct = ct;
    cu->pd.ct = ct;
    ct->primary = cu;
    Cent_comp_table_add(ct, name, cu);
}

void Cent_comp_table_create_fp(
    Cent_comp_table** ct,
    Zinc_string* dir_path,
    Zinc_string* name,
    FILE* fp)
{
    Zinc_malloc_safe((void**)ct, sizeof(Cent_comp_table));
    Cent_comp_table_init_fp(*ct, dir_path, name, fp);
}

void Cent_comp_table_add_module_str(Cent_comp_table* ct, char* name, char* s)
{
    assert(ct->module_finder_vtable == &Cent_module_finder_string_vtable);
    Cent_module_finder_string_add_module_str_str(ct->module_finder_obj, name, s);
}

void Cent_comp_table_destroy(Cent_comp_table* table)
{
    if (table) {
        Cent_value_destroy_setup();
        Cent_module_finder_destroy(table->module_finder_obj, table->module_finder_vtable);
        free(table->module_finder_obj);
        Zinc_hash_map_string_map(&table->ht, (Zinc_hash_map_string_func)Cent_comp_unit_free);
        Zinc_hash_map_string_destroy(&table->ht);
        Cent_environment_destroy(table->base);
        free(table->base);
        Cent_value_destroy_teardown();
    }
}

void Cent_comp_table_add(Cent_comp_table* table, struct Zinc_string* name, Cent_comp_unit *unit)
{
    Zinc_hash_map_string_add(&table->ht, name, unit);
}

void Cent_comp_table_add_str(Cent_comp_table* table, char* name, Cent_comp_unit *unit)
{
    Zinc_hash_map_string_add_str(&table->ht, name, unit);
}

Cent_comp_unit* Cent_comp_table_get(Cent_comp_table* table, struct Zinc_string* name)
{
    return Zinc_hash_map_string_get(&table->ht, name);
}

Cent_comp_unit* Cent_comp_table_get_str(Cent_comp_table* table, char* name)
{
    return Zinc_hash_map_string_get_str(&table->ht, name);
}

void Cent_comp_table_map(Cent_comp_table* table, Cent_comp_table_func func)
{
    Zinc_hash_map_string_map(&table->ht, (Zinc_hash_map_string_func)func);
}

void Cent_comp_table_map_name(Cent_comp_table* table, Cent_comp_table_func_name func)
{
    Zinc_hash_map_string_map_name(&table->ht, (Zinc_hash_map_string_func_name)func);
}

Cent_comp_unit* Cent_comp_table_find_unit(Cent_comp_table* ct, struct Zinc_string* name)
{
    Cent_comp_unit* cu = Cent_comp_table_get(ct, name);
    if (cu) return cu;

    Cent_input_data data = Cent_module_finder_find(
        ct->module_finder_obj,
        ct->module_finder_vtable, name);

    if (!data.input) {
        return NULL;
    }
    assert(data.input);
    assert(data.input_vtable);

    Zinc_string_slice file_name;
    file_name.p = name->buf;
    file_name.size = name->size;
    Cent_comp_unit_create(
        &cu,
        data.input,
        data.input_vtable,
        file_name,
        ct->base);
    cu->input = data.input;
    cu->vtable = data.input_vtable;
    cu->pd.cu = cu;
    cu->pd.ct = ct;

    return cu;
}