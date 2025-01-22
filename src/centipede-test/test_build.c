#include <zinc/error_unit_test.h>

#include "centipede/build.h"
#include "zinc/unit_test.h"
#include "test_parse_tools.h"
#include "centipede/parse.h"
#include "centipede/parse_tools.h"
#include "centipede/check_value_types.h"

void test_build_number_integer()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct, "241");

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_number, "type root");
    Zinc_expect_int_equal(root->number_type, Cent_number_type_integer, "number_type root");
    Zinc_expect_long_long_equal(root->data.integer, 241, "integer root");

    test_parse_teardown(ct);
}

void test_build_number_fp()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct, "1.5");

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_number, "type root");
    Zinc_expect_int_equal(root->number_type, Cent_number_type_real, "number_type root");
    Zinc_expect_double_equal(root->data.fp, 1.5, "fp root");

    test_parse_teardown(ct);
}

void test_build_string()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct, "\"hello\"");

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_string, "type root");
    Zinc_expect_string(&root->data.string, "hello", "string root");

    test_parse_teardown(ct);
}

void test_build_boolean_true()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct, "true");

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_boolean, "type root");
    Zinc_expect_true(root->data.boolean, "string root");

    test_parse_teardown(ct);
}

void test_build_boolean_false()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct, "false");

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_boolean, "type root");
    Zinc_expect_false(root->data.boolean, "string root");

    test_parse_teardown(ct);
}

void test_build_enum0()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct,
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

    test_parse_teardown(ct);
}

void test_build_enum1()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct,
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

    test_parse_teardown(ct);
}

void test_build_enum2()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct,
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

    test_parse_teardown(ct);
}

void test_build_assign()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct,
        "let a = 1;\n"
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

    Cent_value* value = sym->data.variable.value;
    Zinc_assert_ptr(value, "ptr value");
    Zinc_expect_int_equal(value->type, Cent_value_type_number, "type number");
    Zinc_expect_int_equal(value->number_type, Cent_number_type_integer, "number_type value");
    Zinc_expect_long_long_equal(value->data.integer, 1, "integer value");

    test_parse_teardown(ct);
}

void test_build_object()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct,
        "Test {}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr root");
    Zinc_expect_int_equal(root->type, Cent_value_type_dag, "type root");

    test_parse_teardown(ct);
}

void test_build_object_prop_set()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct,
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
    Zinc_expect_int_equal(a->type, Cent_value_type_number, "type number");
    Zinc_expect_int_equal(a->number_type, Cent_number_type_integer, "number_type value");
    Zinc_expect_long_long_equal(a->data.integer, 1, "integer value");

    test_parse_teardown(ct);
}

void test_build_object_assign()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct,
        "Test {\n"
        "    let count = 20\n"
        "    .count_value = count\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr root");
    Zinc_expect_int_equal(root->type, Cent_value_type_dag, "type root");
    Cent_value* count_value = Cent_value_get_str(root, "count_value");
    Zinc_assert_ptr(count_value, "ptr count_value");
    Zinc_expect_int_equal(count_value->type, Cent_value_type_number, "type number");
    Zinc_expect_int_equal(count_value->number_type, Cent_number_type_integer, "number_type value");
    Zinc_expect_long_long_equal(count_value->data.integer, 20, "integer value");

    test_parse_teardown(ct);
}

void test_build_object_child_of()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct,
        "Test {\n"
        "    let foo = Foo {}\n"
        "    let bar = Bar {\n"
        "        .@child_of(foo)\n"
        "    }\n"
        "    foo\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr root");
    Zinc_expect_int_equal(root->type, Cent_value_type_dag, "type root");
    Zinc_expect_string(&root->name, "Test", "name root");

    Cent_value* foo = root->data.dag.head;
    Zinc_assert_ptr(foo, "ptr foo");
    Zinc_expect_int_equal(foo->type, Cent_value_type_dag, "type foo");
    Zinc_expect_string(&foo->name, "Foo", "name foo");

    Cent_value* bar = foo->data.dag.head;
    Zinc_assert_ptr(bar, "ptr bar");
    Zinc_expect_int_equal(bar->type, Cent_value_type_dag, "type bar");
    Zinc_expect_string(&bar->name, "Bar", "name bar");

    test_parse_teardown(ct);
}

void test_build_object_property_of()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct,
        "Test {\n"
        "    let foo = Foo {}\n"
        "    let bar = Bar {\n"
        "        .@property_of(foo, \"x\")\n"
        "    }\n"
        "    foo\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr root");
    Zinc_expect_int_equal(root->type, Cent_value_type_dag, "type root");
    Zinc_expect_string(&root->name, "Test", "name root");

    Cent_value* foo = root->data.dag.head;
    Zinc_assert_ptr(foo, "ptr foo");
    Zinc_expect_int_equal(foo->type, Cent_value_type_dag, "type foo");
    Zinc_expect_string(&foo->name, "Foo", "name foo");

    Cent_value* bar = Cent_value_get_str(foo, "x");
    Zinc_assert_ptr(bar, "ptr bar");
    Zinc_expect_int_equal(bar->type, Cent_value_type_dag, "type bar");
    Zinc_expect_string(&bar->name, "Bar", "name bar");

    test_parse_teardown(ct);
}

void test_build_object_function_file_name()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct,
        "@file_name()\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr root");
    Zinc_expect_int_equal(root->type, Cent_value_type_string, "type root");
    Zinc_expect_string(&root->data.string, "**string**", "string root");

    test_parse_teardown(ct);
}

void test_build_property_set_variable()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct,
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
        "let a = Source {\"x + 5\"};\n"
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

    test_parse_teardown(ct);
}

void test_build_namespace_enum()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct,
        "use types;\n"
        "Groceries {\n"
        "    types::Grocery_item::Milk\n"
        "    types::Grocery_item::Carrots\n"
        "}\n"
    );

    test_parse_add_comp_unit(ct->module_finder_obj, "types.cent",
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

    test_parse_teardown(ct);
}

void test_build_namespace_variable()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct,
        "use variables;\n"
        "variables::a\n"
    );

    test_parse_add_comp_unit(ct->module_finder_obj, "variables.cent",
        "let a = 190;\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_number, "type root");
    Zinc_expect_int_equal(root->number_type, Cent_number_type_integer, "number root");
    Zinc_expect_long_long_equal(root->data.integer, 190, "integer root");

    test_parse_teardown(ct);
}

void test_build_namespace_submodules()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct,
        "use foo::bar;\n"
        "foo::bar::a\n"
    );

    test_parse_add_comp_unit(ct->module_finder_obj, "foo/bar.cent",
        "let a = 190;\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_number, "type root");
    Zinc_expect_int_equal(root->number_type, Cent_number_type_integer, "number root");
    Zinc_expect_long_long_equal(root->data.integer, 190, "integer root");

    test_parse_teardown(ct);
}

void test_build_namespace_glob_value()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct,
        "use foo::bar::*;\n"
        "a\n"
    );

    test_parse_add_comp_unit(ct->module_finder_obj, "foo/bar.cent",
        "let a = 190;\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_number, "type root");
    Zinc_expect_int_equal(root->number_type, Cent_number_type_integer, "number root");
    Zinc_expect_long_long_equal(root->data.integer, 190, "integer root");

    test_parse_teardown(ct);
}

void test_build_let()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct,
        "let a = 245;\n"
        "a\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_number, "type root");
    Zinc_expect_int_equal(root->number_type, Cent_number_type_integer, "number root");
    Zinc_expect_long_long_equal(root->data.integer, 245, "integer root");

    test_parse_teardown(ct);
}

void test_build_object_let()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    test_parse_setup(&ct,
        "Foo {\n"
        "    let bar = Bar {}\n"
        "    bar\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_int_equal(root->type, Cent_value_type_dag, "type root");
    Zinc_expect_string(&root->name, "Foo", "name root");

    Cent_value* bar = root->data.dag.head;
    Zinc_expect_int_equal(bar->type, Cent_value_type_dag, "type bar");
    Zinc_expect_string(&bar->name, "Bar", "name bar");

    test_parse_teardown(ct);
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
    test_build_object_assign();
    test_build_object_child_of();
    test_build_object_property_of();
    test_build_object_function_file_name();
    test_build_property_set_variable();
    test_build_namespace_enum();
    test_build_namespace_variable();
    test_build_namespace_submodules();
    test_build_namespace_glob_value();
    test_build_let();
    test_build_object_let();
}