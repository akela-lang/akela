#include "parse_data.h"
#include "parse_tools.h"
#include <assert.h>

void Cent_check_types_dispatch(Cent_value* value);
void Cent_check_types_property(struct buffer* name, Cent_value* value);

Cent_parse_result* Cent_check_types_pr = NULL;
void Cent_check_types(Cent_parse_result* pr, Cent_value* value)
{
    if (!pr->errors->head) {
        if (value) {
            Cent_check_types_pr = pr;
            Cent_check_types_dispatch(value);
        }
    }
}

Cent_value* Cent_parse_types_object_value = NULL;
void Cent_check_types_dispatch(Cent_value* value)
{
    Cent_parse_result* pr = Cent_check_types_pr;

    if (value->type == Cent_value_type_object) {
        Cent_parse_types_object_value = value;
        hash_table_map_name(&value->properties, (hash_table_func_name)Cent_check_types_property);
    }
}

void Cent_check_types_property(struct buffer* name, Cent_value* value)
{
    Cent_parse_result* pr = Cent_check_types_pr;

    /* parent object of property */
    Cent_value* object_value = Cent_parse_types_object_value;
    Cent_environment* top = Cent_get_environment(object_value->n);
    Cent_ast* object_n = object_value->n;
    Cent_symbol* object_sym = Cent_environment_get(top, &object_n->text);
    if (!object_sym) return;
    assert(object_sym->type == Cent_symbol_type_element);
    Cent_element_type* object_element = object_sym->data.element;

    Cent_property_type* prop_type = hash_table_get(&object_element->properties, name);
    Cent_element_type* prop_element = prop_type->et;

    Cent_symbol* value_sym;
    if (value->type == Cent_value_type_enum) {
        value_sym = Cent_environment_get(top, &value->data.enumeration.id1);
    } else {
        value_sym = Cent_environment_get(top, &value->name);
    }
    assert(value_sym);
    assert(value_sym->type == Cent_symbol_type_element);
    Cent_element_type* value_element = value_sym->data.element;

    if (prop_element != value_element) {
        Cent_ast* n = value->n;
        error_list_set(pr->errors, &n->loc, "invalid value element type: %b", &value_element->name);
    }
}