#include <zinc/error_unit_test.h>
#include "zinc/unit_test.h"
#include "centipede/parse_tools.h"
#include "centipede/comp_table.h"

void test_build_number_integer()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct, "241");

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_natural, "type root");
    Zinc_expect_uint64_t_equal(root->data.natural, 241, "integer root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_number_fp()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct, "1.5");

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_real, "type root");
    Zinc_expect_double_equal(root->data.real, 1.5, "fp root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_string()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct, "\"hello\"");

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_string, "type root");
    Zinc_expect_string(&root->data.string, "hello", "string root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_boolean_true()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct, "true");

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_boolean, "type root");
    Zinc_expect_true(root->data.boolean, "string root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_boolean_false()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct, "false");

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_boolean, "type root");
    Zinc_expect_false(root->data.boolean, "string root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_enum0()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Symbol_type {\n"
        "    Variable\n"
        "    Type\n"
        "    Info\n"
        "}\n"
        "Symbol_type::Variable\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr root");
    Zinc_expect_int_equal(root->type, Cent_value_type_enum, "type root");
    Zinc_expect_string(&root->data.enumeration.enum_type->name, "Symbol_type", "id1 root");
    Zinc_expect_string(&root->data.enumeration.enum_value->display, "Variable", "id2 root");
    Zinc_expect_size_t_equal(root->data.enumeration.number, 0, "enumeration number root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_enum1()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Symbol_type {\n"
        "    Variable\n"
        "    Type\n"
        "    Info\n"
        "}\n"
        "Symbol_type::Type\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_enum, "type root");
    Zinc_expect_string(&root->data.enumeration.enum_type->name, "Symbol_type", "id1 root");
    Zinc_expect_string(&root->data.enumeration.enum_value->display, "Type", "id2 root");
    Zinc_expect_size_t_equal(root->data.enumeration.number, 1, "enumeration number root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_enum2()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Symbol_type {\n"
        "    Variable\n"
        "    Type\n"
        "    Info\n"
        "}\n"
        "Symbol_type::Info\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_enum, "type root");
    Zinc_expect_string(&root->data.enumeration.enum_type->name, "Symbol_type", "id1 root");
    Zinc_expect_string(&root->data.enumeration.enum_value->display, "Info", "id2 root");
    Zinc_expect_size_t_equal(root->data.enumeration.number, 2, "enumeration number root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_assign()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "const a = 1;\n"
        "a\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_expect_ptr(root, "ptr");

    Cent_environment* top = Cent_get_environment(ct->primary->pr.root);
    Zinc_assert_ptr(top, "ptr top");

    Cent_symbol* sym = Cent_environment_get_str(top, "a");
    Zinc_assert_ptr(sym, "ptr sym");
    Zinc_expect_int_equal(sym->type, Cent_symbol_type_variable, "type sym");

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_natural, "type number");
    Zinc_expect_uint64_t_equal(root->data.natural, 1, "integer value");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_object()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "Test {}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr root");
    Zinc_expect_int_equal(root->type, Cent_value_type_dag, "type root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_object_prop_set()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "Test {\n"
        "    .a = 1\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr root");
    Zinc_expect_int_equal(root->type, Cent_value_type_dag, "type root");
    Cent_value* a = Cent_value_get_str(root, "a");
    Zinc_assert_ptr(a, "ptr a");
    Zinc_expect_int_equal(a->type, Cent_value_type_natural, "type number");
    Zinc_expect_long_long_equal(a->data.natural, 1, "integer value");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_property_set_variable()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Source {\n"
        "    children {\n"
        "        String\n"
        "    }\n"
        "}\n"
        "element Test {\n"
        "    properties {\n"
        "        source: Source\n"
        "    }\n"
        "}\n"
        "const a = Source {\"x + 5\"};\n"
        "Test {\n"
        "    .source = a\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_dag, "type root");
    Zinc_expect_string(&root->name, "Test", "name root");

    Cent_value* source = Cent_value_get_str(root, "source");
    Zinc_assert_ptr(source, "ptr source");
    Zinc_expect_int_equal(source->type, Cent_value_type_dag, "type source");
    Zinc_expect_string(&source->name, "Source", "name source");

    Cent_value* line0 = source->data.dag.head;
    Zinc_assert_ptr(line0, "ptr line0");
    Zinc_expect_int_equal(line0->type, Cent_value_type_string, "type line0");
    Zinc_expect_string(&line0->name, "String", "name line0");
    Zinc_expect_string(&line0->data.string, "x + 5", "string line0");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_namespace_enum()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "use types;\n"
        "Groceries {\n"
        "    types::Grocery_item::Milk\n"
        "    types::Grocery_item::Carrots\n"
        "}\n"
    );

    Cent_comp_table_add_module_str(ct, "types.cent",
        "enum Grocery_item {\n"
        "    Milk\n"
        "    Cereal\n"
        "    Steak\n"
        "    Potatoes\n"
        "    Carrots\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_dag, "type root");
    Zinc_expect_string(&root->name, "Groceries", "name root");

    Cent_value* item0 = root->data.dag.head;
    Zinc_assert_ptr(item0, "ptr item0");
    Zinc_expect_int_equal(item0->type, Cent_value_type_enum, "type item0");
    Zinc_expect_size_t_equal(item0->data.enumeration.number, 0, "number item0");

    Cent_value* item1 = item0->next;
    Zinc_assert_ptr(item0, "ptr item1");
    Zinc_expect_int_equal(item1->type, Cent_value_type_enum, "type item1");
    Zinc_expect_size_t_equal(item1->data.enumeration.number, 4, "number item1");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_namespace_variable()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "use variables\n"
        "const a = 1\n"
        "const b = 2\n"
        "const c = 3\n"
        "Foo {\n"
        "    a\n"
        "    b\n"
        "    c\n"
        "    variables::a\n"
        "}\n"
    );

    Cent_comp_table_add_module_str(ct, "variables.cent",
        "use original\n"
        "const a = original::c\n"
        "const b = 5\n"
        "const c = 6\n"
    );

    Cent_comp_table_add_module_str(ct, "original.cent",
    "const a = 7\n"
    "const b = 8\n"
    "const c = 9\n"
);

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_dag, "type root");

    Cent_value* a = root->data.dag.head;
    Zinc_assert_ptr(a, "ptr a2");

    Cent_value* b = a->next;
    Zinc_assert_ptr(b, "ptr a2");

    Cent_value* c = b->next;
    Zinc_assert_ptr(c, "ptr a2");

    Cent_value* a2 = c->next;
    Zinc_assert_ptr(a2, "ptr a2");
    Zinc_expect_int_equal(a2->type, Cent_value_type_natural, "type a2");
    Zinc_expect_uint64_t_equal(a2->data.natural, 9, "integer a2");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_namespace_submodules()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "use foo::bar;\n"
        "foo::bar::a\n"
    );

    Cent_comp_table_add_module_str(ct, "foo/bar.cent",
        "const a = 190;\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_natural, "type root");
    Zinc_expect_uint64_t_equal(root->data.natural, 190, "integer root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_namespace_glob_value()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "use foo::bar::*;\n"
        "a\n"
    );

    Cent_comp_table_add_module_str(ct, "foo/bar.cent",
        "const a = 190;\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_natural, "type root");
    Zinc_expect_uint64_t_equal(root->data.integer, 190, "integer root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_const()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "const a = 245;\n"
        "a\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_natural, "type root");
    Zinc_expect_uint64_t_equal(root->data.natural, 245, "integer root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_cast_natural_to_integer()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Integer\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = 100\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;
    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_dag, "type root");
    Zinc_expect_string(&root->name, "Foo", "name root");

    Cent_value* x = Cent_value_get_str(root, "x");
    Zinc_assert_ptr(x, "ptr x");
    Zinc_expect_int_equal(x->type, Cent_value_type_integer, "type x");
    Zinc_expect_string(&x->name, "Integer", "name x");
    Zinc_expect_int64_t_equal(x->data.integer, 100, "integer x");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_cast_natural_to_integer_error_too_large()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Integer\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = 9223372036854775808\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(
        errors,
        "Natural (9223372036854775808) too large to cast to Integer");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_cast_integer_to_natural_error()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Natural\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = -100\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "Integer (-100) is negative; cannot cast to Natural");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_cast_natural_to_real()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Real\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = 100\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_dag, "type root");
    Zinc_expect_string(&root->name, "Foo", "name root");

    Cent_value* x = Cent_value_get_str(root, "x");
    Zinc_assert_ptr(x, "ptr x");
    Zinc_expect_int_equal(x->type, Cent_value_type_real, "type x");
    Zinc_expect_string(&x->name, "Real", "name x");
    Zinc_expect_double_equal(x->data.real, 100, "integer x");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_cast_real_to_natural()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Natural\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = 100.0\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_dag, "type root");
    Zinc_expect_string(&root->name, "Foo", "name root");

    Cent_value* x = Cent_value_get_str(root, "x");
    Zinc_assert_ptr(x, "ptr x");
    Zinc_expect_int_equal(x->type, Cent_value_type_natural, "type x");
    Zinc_expect_string(&x->name, "Natural", "name x");
    Zinc_expect_uint64_t_equal(x->data.natural, 100, "integer x");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_cast_real_to_natural_error_negative()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Natural\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = -100.0\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "Real (-100.000000) is negative; cannot cast to Natural");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_cast_real_to_natural_error_too_large()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Natural\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = 1.0e20\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(
        errors,
        "Real (100000000000000000000.000000) is too large; cannot cast to Natural");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_cast_integer_to_real()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Real\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = -100\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_dag, "type root");
    Zinc_expect_string(&root->name, "Foo", "name root");

    Cent_value* x = Cent_value_get_str(root, "x");
    Zinc_assert_ptr(x, "ptr x");
    Zinc_expect_int_equal(x->type, Cent_value_type_real, "type x");
    Zinc_expect_string(&x->name, "Real", "name x");
    Zinc_expect_double_equal(x->data.real, -100.000000, "integer x");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_cast_real_to_integer()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Integer\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = 100.0\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_dag, "type root");
    Zinc_expect_string(&root->name, "Foo", "name root");

    Cent_value* x = Cent_value_get_str(root, "x");
    Zinc_assert_ptr(x, "ptr x");
    Zinc_expect_int_equal(x->type, Cent_value_type_integer, "type x");
    Zinc_expect_string(&x->name, "Integer", "name x");
    Zinc_expect_int64_t_equal(x->data.integer, 100, "integer x");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_cast_real_to_integer_too_large_of_a_negative()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Integer\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = -1.0e20\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(
        errors,
        "Real (-100000000000000000000.000000) is too large of a negative; cannot cast to Integer");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_cast_real_to_integer_too_large()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Integer\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = 1.0e20\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(
        errors,
        "Real (100000000000000000000.000000) is too large; cannot cast to Integer");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_variant()
{
    Zinc_test_name(__func__);
    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
    "element Monster(MonsterKind) {\n"
    "    properties {\n"
    "        hp: Integer\n"
    "        attack: Integer\n"
    "        defense: Integer\n"
    "    }\n"
    "}\n"
    "enum MonsterKind {\n"
    "    Bat\n"
    "    Rat\n"
    "    Orc\n"
    "    Goblin\n"
    "    Skeleton\n"
    "    Human\n"
    "    Dog\n"
    "}\n"
    "variant Monster::Bat {\n"
    "    properties {\n"
    "        siphon: Integer\n"
    "    }\n"
    "}\n"
    "variant Monster::Human {\n"
    "    properties {\n"
    "        name: String\n"
    "    }\n"
    "}\n"
    "Monsters {\n"
    "    Monster::Bat {"
    "       .hp = 10\n"
    "       .attack = 2\n"
    "       .defense = 1\n"
    "       .siphon = 1\n"
    "    }\n"
    "    Monster {\n"
    "        .@tag = MonsterKind::Human\n"
    "        .hp = 20\n"
    "        .attack = 1\n"
    "        .defense = 1\n"
    "        .name = \"Tom\"\n"
    "    }\n"
    "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;
    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_dag, "type root");
    Zinc_expect_string(&root->name, "Monsters", "name root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_cycle()
{
    Zinc_test_name(__func__);
    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
    "const foo = Foo {\n"
    "    bar\n"
    "}\n"
    "const bar = Bar {\n"
    "    foo\n"
    "}\n"
    "foo\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;
    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "cycle detected");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build_cycle2()
{
    Zinc_test_name(__func__);
    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
    "const foo = Foo {\n"
    "    foo\n"
    "}\n"
    "foo\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;
    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "cycle detected");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_build()
{
    test_build_number_integer();
    test_build_number_fp();
    test_build_string();
    test_build_boolean_true();
    test_build_boolean_false();
    test_build_enum0();
    test_build_enum1();
    test_build_enum2();
    test_build_assign();
    test_build_object();
    test_build_object_prop_set();
    test_build_property_set_variable();
    test_build_namespace_enum();
    test_build_namespace_variable();
    test_build_namespace_submodules();
    test_build_namespace_glob_value();
    test_build_const();

    test_build_cast_natural_to_integer();
    test_build_cast_natural_to_integer_error_too_large();
    test_build_cast_integer_to_natural_error();

    test_build_cast_natural_to_real();
    test_build_cast_real_to_natural();
    test_build_cast_real_to_natural_error_negative();
    test_build_cast_real_to_natural_error_too_large();

    test_build_cast_integer_to_real();
    test_build_cast_real_to_integer();
    test_build_cast_real_to_integer_too_large_of_a_negative();
    test_build_cast_real_to_integer_too_large();

    test_build_variant();

    test_build_cycle();
    test_build_cycle2();
}
