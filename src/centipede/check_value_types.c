#include "parse_data.h"
#include "parse_tools.h"
#include <assert.h>
#include "zinc/hash_map_size_t.h"
#include "cast.h"

void Cent_check_value_types_value(Cent_value* value);
void Cent_check_value_types_property(struct Zinc_string* name, Cent_value* value);
void Cent_check_value_types_property2(Cent_value* value);
void Cent_check_value_types_child(Cent_parse_result* pr, Cent_value* object_value, Cent_value* value);

Cent_parse_result* Cent_check_value_types_pr = NULL;
Zinc_hash_map_size_t* Cent_check_value_types_hm = NULL;
void Cent_check_value_types(Cent_parse_result* pr, Cent_value* value)
{
    if (!pr->errors->head) {
        if (value) {
            Cent_check_value_types_pr = pr;

            Zinc_hash_map_size_t* hm = NULL;
            Zinc_hash_map_size_t_create(&hm, 64);
            Cent_check_value_types_hm = hm;

            Cent_check_value_types_value(value);

            Zinc_hash_map_size_t_destroy(hm);
            free(hm);
        }
    }
}

Cent_value* Cent_parse_types_object_value = NULL;
/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_check_value_types_value(Cent_value* value)
{
    Cent_parse_result* pr = Cent_check_value_types_pr;
    Zinc_hash_map_size_t* hm = Cent_check_value_types_hm;

    Zinc_hash_map_size_t_add(hm, (size_t)value, value);

    if (value->type == Cent_value_type_dag) {
        Cent_parse_types_object_value = value;
        Zinc_string_finish(&value->name);
        Zinc_hash_map_string_map_name(
            &value->data.dag.properties,
            (Zinc_hash_map_string_func_name)Cent_check_value_types_property);
        Zinc_hash_map_string_map(
            &value->data.dag.properties,
            (Zinc_hash_map_string_func)Cent_check_value_types_property2);

        Cent_value* p = value->data.dag.head;
        while (p) {
            Cent_check_value_types_child(pr, value, p);
            p = p->next;
        }
    } else if (value->type == Cent_value_type_dict) {
        Cent_parse_types_object_value = value;
        Zinc_hash_map_string_map_name(
            &value->data.dict.properties,
            (Zinc_hash_map_string_func_name)Cent_check_value_types_property);
        Zinc_hash_map_string_map(
            &value->data.dict.properties,
            (Zinc_hash_map_string_func)Cent_check_value_types_property2);
    } else if (value->type == Cent_value_type_list) {
        Cent_value* p = value->data.list.head;
        while (p) {
            Cent_check_value_types_child(pr, value, p);
            p = p->next;
        }
    }
}

void Cent_check_value_types_property(Zinc_string* name, Cent_value* value)
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

    Cent_ast* p = object_n->head;
    Cent_ast* tag = NULL;
    while (p) {
        if (p->type == Cent_ast_type_variant_set) {
            tag = p;
            break;
        }

        if (p->type == Cent_ast_type_prop_set) {
            Cent_ast* prop_name = Cent_ast_get(p, 0);
            if (Zinc_string_compare_str(&prop_name->text, "@tag")) {
                Cent_ast* ns = Cent_ast_get(p, 1);
                Cent_ast* id1 = Cent_ast_get(ns, 0);
                Cent_ast* id2 = Cent_ast_get(ns, 1);
                tag = id2;
                break;
            }
        }
        p = p->next;
    }

    Cent_variant_type* vt = NULL;
    if (tag) {
        vt = object_element->variants.head;
        while (vt) {
            if (Zinc_string_compare(&vt->tag_name, &tag->text)) {
                break;
            }
            vt = vt->next;
        }
    }

    Cent_property_type* prop_type = NULL;
    // variant
    if (vt) {
        prop_type = Cent_variant_type_get(vt, name);
    }

    // element
    if (!prop_type) {
        prop_type = Zinc_hash_map_string_get(&object_element->properties, name);
    }

    // catch all variant
    if (vt && !prop_type) {
        Zinc_string name2;
        Zinc_string_init(&name2);
        Zinc_string_add_str(&name2, "_");
        prop_type = Cent_variant_type_get(vt, &name2);
        Zinc_string_destroy(&name2);
    }

    // catch all element
    if (!prop_type) {
        Zinc_string name2;
        Zinc_string_init(&name2);
        Zinc_string_add_str(&name2, "_");
        prop_type = Zinc_hash_map_string_get(&object_element->properties, &name2);
        Zinc_string_destroy(&name2);
    }

    Zinc_string_finish(name);
    if (!prop_type) {
        Cent_ast* n = value->n;
        Zinc_error_list_set(
            pr->errors,
            &n->loc,
            "property name (%bf) not defined in element (%bf)",
            name,
            &object_value->name);
        value->has_error = true;
        n->has_error = true;
        return;
    }

    if (prop_type->type == Cent_types_element) {
        Cent_element_type* prop_element = prop_type->data.et;

        Cent_cast_value(value, prop_element, pr->errors);

        Cent_symbol* value_sym = Cent_environment_get(top, &value->name);
        if (!value_sym) {
            Cent_ast* n = value->n;
            Zinc_error_list_set(
                pr->errors,
                &n->loc,
                "for property (%bf--%bf), found type (%bf) but expected (%bf)",
                &object_value->name,
                name,
                &value->name,
                &prop_element->name);
            value->has_error = true;
            n->has_error = true;
            return;
            /* test case: test_check_types_property_error_object */
        }

        assert(value_sym->type == Cent_symbol_type_element);
        Cent_element_type* value_element = value_sym->data.element;

        if ((prop_element->type != Cent_value_type_any) && (prop_element != value_element)) {
            Cent_ast* n = value->n;
            Zinc_error_list_set(
                pr->errors,
                &n->loc,
                "for property (%bf--%bf), found type (%bf) but expected (%bf)",
                &object_value->name,
                name,
                &value_element->name,
                &prop_element->name);
            value->has_error = true;
            n->has_error = true;
            /* test case: test_check_types_property_error_number */
        }
    } else if (prop_type->type == Cent_types_enum) {
        Cent_enum_type* prop_en = prop_type->data.en;

        if (value->type != Cent_value_type_enum) {
            Cent_ast* n = value->n;
            Zinc_error_list_set(pr->errors, &n->loc, "value is not an enum value: %bf", &value->name);
            value->has_error = true;
            n->has_error = true;
            return;
            /* test case: test_check_types_property_error_not_enum */
        }

        Cent_symbol* value_sym = Cent_environment_get(top, &value->data.enumeration.enum_type->name);
        assert(value_sym && "is checked in update values");

        assert(value_sym->type == Cent_symbol_type_enumerate);
        Cent_enum_type* value_en = value_sym->data.enumerate;

        if (prop_en != value_en) {
            Cent_ast* n = value->n;
            Zinc_error_list_set(pr->errors, &n->loc, "invalid value enum type: %bf", &value_en->name);
            value->has_error = true;
            n->has_error = true;
            /* test case: test_check_types_property_enum_error_not_match */
        }
    } else {
        assert(false && "invalid property type type");
    }
}

void Cent_check_value_types_property2(Cent_value* value)
{
    Zinc_hash_map_size_t* hm = Cent_check_value_types_hm;
    Cent_value* value2 = Zinc_hash_map_size_t_get(hm, (size_t)value);
    if (!value2) {
        Cent_check_value_types_value(value);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_check_value_types_child(Cent_parse_result* pr, Cent_value* object_value, Cent_value* value)
{
    Cent_environment* top = Cent_get_environment(object_value->n);
    Cent_ast* object_n = object_value->n;
    Cent_symbol* object_sym = Cent_environment_get(top, &object_n->text);

    if (object_sym) {
        assert(object_sym->type == Cent_symbol_type_element);
        Cent_element_type* object_element = object_sym->data.element;

        Cent_symbol* value_sym = Cent_environment_get(top, &value->name);
        if (!value_sym) {
            if (object_element->children.head) {
                Cent_ast* n = value->n;
                struct Zinc_string bf;
                Zinc_string_init(&bf);
                Cent_types_node* p = object_element->children.head;
                while (p) {
                    if (bf.size > 0) {
                        Zinc_string_add_char(&bf, ' ');
                    }
                    if (p->type == Cent_types_element) {
                        Zinc_string_copy(&p->data.et->name, &bf);
                    } else if (p->type == Cent_types_enum) {
                        Zinc_string_copy(&p->data.en->name, &bf);
                    } else {
                        assert(false && "invalid types type");
                    }
                    p = p->next;
                }
                Zinc_error_list_set(pr->errors, &n->loc, "value has no type; looking for %bf", &bf);
                value->has_error = true;
                n->has_error = true;
                Zinc_string_destroy(&bf);
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
            Zinc_error_list_set(pr->errors, &n->loc, "invalid child type: %bf", &value->name);
            value->has_error = true;
            n->has_error = true;
            /* test case: test_check_types_child_error_number */
        }
    }

    Zinc_hash_map_size_t* hm = Cent_check_value_types_hm;
    Cent_value* value2 = Zinc_hash_map_size_t_get(hm, (size_t)value);
    if (!value2) {
        Cent_check_value_types_value(value);
    }
}