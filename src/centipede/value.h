#ifndef CENTIPEDE_VALUE_H
#define CENTIPEDE_VALUE_H

#include "token.h"
#include "zinc/hash_map_string.h"
#include "enumerate.h"

typedef enum Cent_value_type {
    Cent_value_type_none,
    Cent_value_type_integer,
    Cent_value_type_natural,
    Cent_value_type_real,
    Cent_value_type_string,
    Cent_value_type_boolean,
    Cent_value_type_list,
    Cent_value_type_dict,
    Cent_value_type_dag,
    Cent_value_type_enum,
    Cent_value_type_any,
} Cent_value_type;

typedef union Cent_data
{
    int64_t integer;
    uint64_t natural;
    double real;
    Zinc_string string;
    bool boolean;
    struct {
        Cent_enum_type* enum_type;
        Cent_enum_value* enum_value;
        size_t number;
    } enumeration;
    struct {
        struct Cent_value* head;
        struct Cent_value* tail;
    } list;
    struct {
        Zinc_hash_map_string properties;
    } dict;
    struct {
        Zinc_hash_map_string properties;
        struct Cent_value* head;
        struct Cent_value* tail;
    } dag;
} Cent_data;

typedef struct Cent_value {
    Zinc_string name;
    Cent_value_type type;
    bool has_error;
    Cent_data data;
    struct Cent_value* next;
    struct Cent_value* prev;
    void* n;
    void* parent;
} Cent_value;

void Cent_data_init(Cent_data *data, Cent_value_type type);
void Cent_data_destroy(Cent_data *data, Cent_value_type type);
void Cent_data_copy(Cent_data *src, Cent_data *dst, Cent_value_type type);
void Cent_data_clone(Cent_data *data, Cent_value_type type);

void Cent_value_init(Cent_value *value);
void Cent_value_create(Cent_value **value);
void Cent_value_set_type(Cent_value *value, Cent_value_type type);
void Cent_value_destroy(Cent_value *value);
void Cent_value_free(Cent_value *value);
void Cent_value_set(Cent_value* value, struct Zinc_string* name, Cent_value* value2);
void Cent_value_set_str(Cent_value* value, char* name, Cent_value* value2);
void Cent_value_add(Cent_value* parent, Cent_value* child);
Cent_value* Cent_value_get(Cent_value* value, struct Zinc_string* name);
Cent_value* Cent_value_get_str(Cent_value* value, char* name);

bool Cent_value_destroy_setup();
void Cent_value_destroy_teardown();

void Cent_value_copy(Cent_value* src, Cent_value* dst);
Cent_value* Cent_value_clone(Cent_value* value);

void Cent_value_print(Cent_value* value, bool is_property);

#endif
