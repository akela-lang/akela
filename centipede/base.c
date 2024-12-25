#include "environment.h"

Cent_environment* Cent_base_create()
{
    Cent_environment* env = NULL;
    Cent_environment_create(&env);

    Cent_symbol* sym = NULL;
    Cent_element* element = NULL;

    Cent_element_create(&element);
    buffer_copy_str(&element->name, "Integer");
    element->type = Cent_value_type_number;
    element->number_type = Cent_number_type_integer;
    Cent_symbol_create(&sym);
    sym->type = Cent_symbol_type_element;
    sym->element = element;
    hash_table_add(&env->symbols, &element->name, element);

    Cent_element_create(&element);
    buffer_copy_str(&element->name, "Float");
    element->type = Cent_value_type_number;
    element->number_type = Cent_number_type_fp;
    Cent_symbol_create(&sym);
    sym->type = Cent_symbol_type_element;
    sym->element = element;
    hash_table_add(&env->symbols, &element->name, element);

    Cent_element_create(&element);
    buffer_copy_str(&element->name, "String");
    element->type = Cent_value_type_string;
    Cent_symbol_create(&sym);
    sym->type = Cent_symbol_type_element;
    sym->element = element;
    hash_table_add(&env->symbols, &element->name, element);

    Cent_element_create(&element);
    buffer_copy_str(&element->name, "Bool");
    element->type = Cent_value_type_boolean;
    Cent_symbol_create(&sym);
    sym->type = Cent_symbol_type_element;
    sym->element = element;
    hash_table_add(&env->symbols, &element->name, element);

    return env;
}
