#include "centipede/parse.h"
#include "centipede/build.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <centipede/base.h>
#include "zinc/input_unicode_file.h"
#include "centipede/comp_table.h"
#include <assert.h>
#include <akela/comp_unit.h>
#include "zinc/spec_error.h"
#include "zinc/input_unicode_string.h"
#include "test_data.h"

void Apt_run_test(Apt_test_data* data, Cent_value* test_value);
void Apt_error(Apt_test_data* data, Ake_ast* n, Cent_value* value, Zinc_string* message);
void Apt_compare_node(Apt_test_data* data, Ake_ast* n, Cent_value* value);
void Apt_compare_ast(Apt_test_data* data, Ake_ast* n, Cent_value* value);

void Apt_run(Zinc_string* file_name, Cent_comp_unit* cu)
{
    if (!cu->value) return;

    Cent_value* test_suite_value = cu->value;
    assert(test_suite_value->type == Cent_value_type_dag);

    Cent_value* mute = Cent_value_get_str(test_suite_value, "mute");
    if (mute && mute->data.boolean) return;

    Cent_value* name = Cent_value_get_str(test_suite_value, "name");
    Zinc_string_finish(&name->data.string);
    printf("%s\n", name->data.string.buf);

    Cent_value* test_value = test_suite_value->data.dag.head;
    while (test_value) {
        Apt_test_data data;
        Apt_test_data_init(&data);

        Zinc_string_add_str(&data.name1, "source");
        Zinc_string_add_string(&data.name2, file_name);
        Apt_run_test(&data, test_value);

        Apt_test_data_destroy(&data);

        test_value = test_value->next;
    }
}

void Apt_run_test(Apt_test_data* data, Cent_value* test_value)
{
    Cent_value* name = Cent_value_get_str(test_value, "name");
    Zinc_string_finish(&name->data.string);
    printf("%s\n", name->data.string.buf);
    Cent_value* source = Cent_value_get_str(test_value, "source");
    Zinc_string source_string;
    Zinc_string_init(&source_string);
    Cent_value* line = source->data.dag.head;
    while (line) {
        Zinc_string_add_string(&source_string, &line->data.string);
        line = line->next;
    }
    Zinc_string_finish(&source_string);
    printf("%s", source_string.buf);

    Zinc_vector* text = NULL;
    Zinc_vector_create(&text, sizeof(char));
    Zinc_vector_add(text, source_string.buf, source_string.size);

    Zinc_input_unicode_string* input_obj = NULL;
    Zinc_input_unicode_string_create(&input_obj, text);

    Ake_comp_unit* cu = NULL;
    Ake_comp_unit_create(&cu);
    bool valid = Ake_comp_unit_compile(cu, input_obj, input_obj->input_vtable);
    if (valid) {
        printf("compile is valid\n");
    } else {
        printf("compile has errors\n");
        Zinc_error* e = cu->el.head;
        while (e) {
            Zinc_string_finish(&e->message);
            printf("(%zu,%zu): %s\n", e->loc.line, e->loc.col, e->message.buf);
            e = e->next;
        }
    }

    Cent_value* value = test_value->data.dag.head;
    Ake_ast* root = cu->root;

    Apt_compare_node(data, root, value);

    Ake_comp_unit_destroy(cu);
    free(cu);
    free(input_obj);
    Zinc_vector_destroy(text);
    free(text);
    Zinc_string_destroy(&source_string);
}

void Apt_error(Apt_test_data* data, Ake_ast* n, Cent_value* value, Zinc_string* message)
{
    Zinc_spec_error* error = NULL;
    Zinc_spec_error_create(&error);
    if (n) {
        error->loc1 = n->loc;
    }
    if (value) {
        Cent_ast* n2 = value->n;
        error->loc2 = n2->loc;
    }
    Zinc_string_add_string(&error->name1, &data->name1);
    Zinc_string_add_string(&error->name2, &data->name2);
    Zinc_string_add_string(&error->message, message);
    Zinc_spec_error_list_add(&data->list, error);
}

void Apt_compare_node(Apt_test_data* data, Ake_ast* n, Cent_value* value)
{
    if (!n && !value) {
        return;
    }

    if (n && !value) {
        Zinc_string message;
        Zinc_string_init(&message);
        Zinc_string_add_format(&message, "value is null");
        Apt_error(data, n, value, &message);
        Zinc_string_destroy(&message);
        return;
    }

    if (!n && value) {
        Zinc_string message;
        Zinc_string_init(&message);
        Zinc_string_add_format(&message, "node is null");
        Apt_error(data, n, value, &message);
        Zinc_string_destroy(&message);
        return;
    }

    if (Zinc_string_compare_str(&value->name, "Ast")) {
        Apt_compare_ast(data, n, value);
    } else {
        Zinc_string message;
        Zinc_string_init(&message);
        Zinc_string_add_format(&message, "unhandled type %b", &value->name);
        Zinc_string_destroy(&message);
    }
}

void Apt_compare_ast(Apt_test_data* data, Ake_ast* n, Cent_value* value)
{
    Cent_value* type = Cent_value_get_str(value, "type");
    assert(type);
    if (n->type != type->data.enumeration.enum_value->value) {
        Zinc_string_finish(&type->data.enumeration.enum_value->display);
        fprintf(
            stderr,
            "Ast type does not match (%d-%s) (%d-%s)\n",
            n->type,
            Ast_type_name(n->type),
            type->data.enumeration.enum_value->value,
            type->data.enumeration.enum_value->display.buf);
    }

    Cent_value* value2 = Cent_value_get_str(value, "value");
    if (n->value.size > 0) {
        if (!value2) {
            fprintf(stderr, "value not expected\n");
        } else {
            if (!Zinc_string_compare(&n->value, &value->data.string)) {
                Zinc_string_finish(&n->value);
                Zinc_string_finish(&value->data.string);
                fprintf(stderr, "AST values do not match (%s) (%s)\n", n->value.buf, value->data.string.buf);
            }
        }
    } else {
        if (value2) {
            fprintf(stderr, "value expected\n");
        }
    }
}