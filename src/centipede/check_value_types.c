#include "parse_data.h"
#include "parse_tools.h"
#include <assert.h>

void Cent_check_value_types_dispatch(Cent_value* value);
void Cent_check_value_types_property(struct buffer* name, Cent_value* value);
void Cent_check_value_types_child(Cent_parse_result* pr, Cent_value* object_value, Cent_value* value);

Cent_parse_result* Cent_check_value_types_pr = NULL;
void Cent_check_value_types(Cent_parse_result* pr, Cent_value* value)
{
    if (!pr->errors->head) {
        if (value) {
            Cent_check_value_types_pr = pr;
            Cent_check_value_types_dispatch(value);
        }
    }
}

Cent_value* Cent_parse_types_object_value = NULL;
void Cent_check_value_types_dispatch(Cent_value* value)
{
    Cent_parse_result* pr = Cent_check_value_types_pr;

    if (value->type == Cent_value_type_object) {
        Cent_parse_types_object_value = value;
        hash_table_map_name(
            &value->data.object.properties,
            (hash_table_func_name)Cent_check_value_types_property);

        Cent_value* p = value->data.object.head;
        while (p) {
            Cent_check_value_types_child(pr, value, p);
            p = p->next;
        }
    }
}

void Cent_check_value_types_property(struct buffer* name, Cent_value* value)
{
    Cent_parse_result* pr = Cent_check_value_types_pr;

    /* parent object of property */
    Cent_value* object_value = Cent_parse_types_object_value;
    Cent_environment* top = Cent_get_environment(object_value->n);
    Cent_ast* object_n = object_value->n;
    Cent_symbol* object_sym = Cent_environment_get(top, &object_n->text);
    if (!object_sym) return;
    assert(object_sym->type == Cent_symbol_type_element);
    Cent_element_type* object_element = object_sym->data.element;

    Cent_property_type* prop_type = hash_table_get(&object_element->properties, name);

    if (prop_type->type == Cent_types_element) {
        Cent_element_type* prop_element = prop_type->data.et;

        Cent_symbol* value_sym = Cent_environment_get(top, &value->name);
        if (!value_sym) {
            Cent_ast* n = value->n;
            error_list_set(pr->errors, &n->loc, "invalid value element type: %b", &value->name);
            value->has_error = true;
            n->has_error = true;
            return;
            /* test case: test_check_types_property_error_object */
        }

        assert(value_sym->type == Cent_symbol_type_element);
        Cent_element_type* value_element = value_sym->data.element;

        if (prop_element != value_element) {
            Cent_ast* n = value->n;
            error_list_set(pr->errors, &n->loc, "invalid value element type: %b", &value_element->name);
            value->has_error = true;
            n->has_error = true;
            /* test case: test_check_types_property_error_number */
        }
    } else if (prop_type->type == Cent_types_enum) {
        Cent_enum_type* prop_en = prop_type->data.en;

        if (value->type != Cent_value_type_enum) {
            Cent_ast* n = value->n;
            error_list_set(pr->errors, &n->loc, "value is not an enum value: %b", &value->name);
            value->has_error = true;
            n->has_error = true;
            return;
            /* test case: test_check_types_property_error_not_enum */
        }

        Cent_symbol* value_sym = Cent_environment_get(top, &value->data.enumeration.id1);
        assert(value_sym && "is checked in update values");

        assert(value_sym->type == Cent_symbol_type_enumerate);
        Cent_enum_type* value_en = value_sym->data.enumerate;

        if (prop_en != value_en) {
            Cent_ast* n = value->n;
            error_list_set(pr->errors, &n->loc, "invalid value enum type: %b", &value_en->name);
            value->has_error = true;
            n->has_error = true;
            /* test case: test_check_types_property_enum_error_not_match */
        }
    } else {
        assert(false && "invalid property type type");
    }
}

void Cent_check_value_types_child(Cent_parse_result* pr, Cent_value* object_value, Cent_value* value)
{
    Cent_environment* top = Cent_get_environment(object_value->n);
    Cent_ast* object_n = object_value->n;
    Cent_symbol* object_sym = Cent_environment_get(top, &object_n->text);
    if (!object_sym) return;
    assert(object_sym->type == Cent_symbol_type_element);
    Cent_element_type* object_element = object_sym->data.element;

    Cent_symbol* value_sym = Cent_environment_get(top, &value->name);
    if (!value_sym) {
        if (object_element->children.head) {
            Cent_ast* n = value->n;
            struct buffer bf;
            buffer_init(&bf);
            Cent_types_node* p = object_element->children.head;
            while (p) {
                if (bf.size > 0) {
                    buffer_add_char(&bf, ' ');
                }
                if (p->type == Cent_types_element) {
                    buffer_copy(&p->data.et->name, &bf);
                } else if (p->type == Cent_types_enum) {
                    buffer_copy(&p->data.en->name, &bf);
                } else {
                    assert(false && "invalid types type");
                }
                p = p->next;
            }
            error_list_set(pr->errors, &n->loc, "value has no type; looking for %b", &bf);
            value->has_error = true;
            n->has_error = true;
            buffer_destroy(&bf);
            /* test case: test_check_types_child_error_no_type */
        }

        return;
    }

    bool found = false;
    if (value_sym->type == Cent_symbol_type_element) {
        Cent_element_type* value_element = value_sym->data.element;
        Cent_types_node* p = object_element->children.head;
        while (p) {
            if (p->type == Cent_types_element) {
                if (p->data.et == value_element) {
                    found = true;
                    break;
                }
            }
            p = p->next;
        }
    } else if (value_sym->type == Cent_symbol_type_enumerate) {
        Cent_enum_type* value_enum = value_sym->data.enumerate;
        Cent_types_node* p = object_element->children.head;
        while (p) {
            if (p->type == Cent_types_enum) {
                if (p->data.en == value_enum) {
                    found = true;
                    break;
                }
            }
            p = p->next;
        }
    }

    if (!found) {
        Cent_ast* n = value->n;
        error_list_set(pr->errors, &n->loc, "invalid child type: %b", &value->name);
        value->has_error = true;
        n->has_error = true;
        /* test case: test_check_types_child_error_number */
    }
}