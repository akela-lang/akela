#ifndef CENTIPEDE_COMP_TABLE_H
#define CENTIPEDE_COMP_TABLE_H

#include "comp_unit.h"
#include "module_finder.h"

typedef struct Cent_comp_table {
    Cent_comp_unit* primary;
    Zinc_hash_map_string ht;
    void* module_finder_obj;
    Cent_module_finder_vtable* module_finder_vtable;
    Cent_environment* base;
} Cent_comp_table;

typedef void (*Cent_comp_table_func)(Cent_comp_unit* cu);
typedef void (*Cent_comp_table_func_name)(struct Zinc_string* name, Cent_comp_unit* cu);
void Cent_comp_table_init(
    Cent_comp_table* ct,
    void* module_finder_obj,
    Cent_module_finder_vtable* module_finder_vtable);
void Cent_comp_table_create(
    Cent_comp_table** ct,
    void* module_finder_obj,
    Cent_module_finder_vtable* module_finder_vtable);
void Cent_comp_table_init_str(Cent_comp_table* ct, char* s);
void Cent_comp_table_create_str(Cent_comp_table** ct, char* s);
void Cent_comp_table_add_module_str(Cent_comp_table* ct, char* name, char* s);
void Cent_comp_table_init_fp(Cent_comp_table* ct, Zinc_string* dir_path, Zinc_string* name, FILE* fp);
void Cent_comp_table_create_fp(
    Cent_comp_table** ct,
    Zinc_string* dir_path,
    Zinc_string* name,
    FILE* fp);
void Cent_comp_table_destroy(Cent_comp_table* table);
void Cent_comp_table_add(Cent_comp_table* table, struct Zinc_string* name, Cent_comp_unit *unit);
void Cent_comp_table_add_str(Cent_comp_table* table, char* name, Cent_comp_unit *unit);
Cent_comp_unit* Cent_comp_table_get(Cent_comp_table* table, struct Zinc_string* name);
Cent_comp_unit* Cent_comp_table_get_str(Cent_comp_table* table, char* name);
void Cent_comp_table_map(Cent_comp_table* table, Cent_comp_table_func func);
void Cent_comp_table_map_name(Cent_comp_table* table, Cent_comp_table_func_name func);
Cent_comp_unit* Cent_comp_table_find_unit(Cent_comp_table* ct, struct Zinc_string* name);

#endif
