#ifndef CENTIPEDE_COMP_UNIT_H
#define CENTIPEDE_COMP_UNIT_H

#include "zinc/input_unicode.h"
#include "zinc/hash.h"

typedef struct Cent_comp_unit {
    void* input;
    InputUnicodeVTable* input_vtable;
    struct error_list* errors;
} Cent_comp_unit;

typedef struct Cent_comp_table {
    Cent_comp_unit* primary;
    struct hash_table ht;
} Cent_comp_table;

void Comp_unit_init(Cent_comp_unit *unit);
void Comp_unit_create(Cent_comp_unit **unit);
void Comp_table_init(Cent_comp_table* table);
void Comp_table_create(Cent_comp_table** table);
void Comp_table_destroy(Cent_comp_table* table);
void Comp_table_add(Cent_comp_table* table, struct buffer* name, Cent_comp_unit *unit);
void Comp_table_add_str(Cent_comp_table* table, char* name, Cent_comp_unit *unit);
Cent_comp_unit* Comp_table_get(Cent_comp_table* table, struct buffer* name);
Cent_comp_unit* Comp_table_get_str(Cent_comp_table* table, char* name);

#endif