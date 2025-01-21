#include "environment.h"
#include "parse_data.h"

Cent_environment* Cent_base_create(Cent_parse_data* pd)
{
    Cent_environment* env = NULL;
    Cent_environment_create(&env);

    Cent_symbol* sym = NULL;
    Cent_element_type* element = NULL;

    Cent_element_create(&element);
    Zinc_string_add_str(&element->name, "Integer");
    element->type = Cent_value_type_number;
    element->number_type = Cent_number_type_integer;
    Cent_symbol_create(&sym);
    Cent_symbol_set_type(sym, Cent_symbol_type_element);
    sym->data.element = element;
    Zinc_hash_map_string_add(&env->symbols, &element->name, sym);

    Cent_element_create(&element);
    Zinc_string_add_str(&element->name, "Any");
    element->type = Cent_value_type_any;
    Cent_symbol_create(&sym);
    Cent_symbol_set_type(sym, Cent_symbol_type_element);
    sym->data.element = element;
    Zinc_hash_map_string_add(&env->symbols, &element->name, sym);

    Cent_element_create(&element);
    Zinc_string_add_str(&element->name, "Float");
    element->type = Cent_value_type_number;
    element->number_type = Cent_number_type_fp;
    Cent_symbol_create(&sym);
    Cent_symbol_set_type(sym, Cent_symbol_type_element);
    sym->data.element = element;
    Zinc_hash_map_string_add(&env->symbols, &element->name, sym);

    Cent_element_create(&element);
    Zinc_string_add_str(&element->name, "String");
    element->type = Cent_value_type_string;
    Cent_symbol_create(&sym);
    Cent_symbol_set_type(sym, Cent_symbol_type_element);
    sym->data.element = element;
    Zinc_hash_map_string_add(&env->symbols, &element->name, sym);

    Cent_element_create(&element);
    Zinc_string_add_str(&element->name, "Bool");
    element->type = Cent_value_type_boolean;
    Cent_symbol_create(&sym);
    Cent_symbol_set_type(sym, Cent_symbol_type_element);
    sym->data.element = element;
    Zinc_hash_map_string_add(&env->symbols, &element->name, sym);

    return env;
}
