#include <zinc/error_unit_test.h>

#include "centipede/build.h"
#include "zinc/unit_test.h"
#include "test_parse_tools.h"
#include "centipede/parse.h"
#include "centipede/parse_tools.h"
#include "test_build_tools.h"

void test_build_number_integer()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd, "241");

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    assert_ptr(root, "ptr value");
    expect_int_equal(root->type, Cent_value_type_number, "type root");
    expect_int_equal(root->number_type, Cent_number_type_integer, "number_type root");
    expect_long_long_equal(root->data.integer, 241, "integer root");

    test_build_teardown(&pd, &pr, root);
}

void test_build_number_fp()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd, "1.5");

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    assert_ptr(root, "ptr value");
    expect_int_equal(root->type, Cent_value_type_number, "type root");
    expect_int_equal(root->number_type, Cent_number_type_fp, "number_type root");
    expect_double_equal(root->data.fp, 1.5, "fp root");

    test_build_teardown(&pd, &pr, root);
}

void test_build_string()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd, "\"hello\"");

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    assert_ptr(root, "ptr value");
    expect_int_equal(root->type, Cent_value_type_string, "type root");
    expect_str(&root->data.string, "hello", "string root");

    test_build_teardown(&pd, &pr, root);
}

void test_build_boolean_true()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd, "true");

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    assert_ptr(root, "ptr value");
    expect_int_equal(root->type, Cent_value_type_boolean, "type root");
    expect_true(root->data.boolean, "string root");

    test_build_teardown(&pd, &pr, root);
}

void test_build_boolean_false()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd, "false");

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    assert_ptr(root, "ptr value");
    expect_int_equal(root->type, Cent_value_type_boolean, "type root");
    expect_false(root->data.boolean, "string root");

    test_build_teardown(&pd, &pr, root);
}

void test_build_enum0()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "enum Symbol_type\n"
        "    Variable\n"
        "    Type\n"
        "    Info\n"
        "end\n"
        "Symbol_type::Variable\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    assert_ptr(root, "ptr value");
    expect_int_equal(root->type, Cent_value_type_enum, "type root");
    expect_str(&root->data.enumeration.id1, "Symbol_type", "id1 root");
    expect_str(&root->data.enumeration.id2, "Variable", "id2 root");
    expect_str(&root->data.enumeration.display, "Symbol_type::Variable", "display root");
    expect_size_t_equal(root->data.enumeration.number, 0, "enumeration number root");

    test_build_teardown(&pd, &pr, root);
}

void test_build_enum1()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "enum Symbol_type\n"
        "    Variable\n"
        "    Type\n"
        "    Info\n"
        "end\n"
        "Symbol_type::Type\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    assert_ptr(root, "ptr value");
    expect_int_equal(root->type, Cent_value_type_enum, "type root");
    expect_str(&root->data.enumeration.id1, "Symbol_type", "id1 root");
    expect_str(&root->data.enumeration.id2, "Type", "id2 root");
    expect_str(&root->data.enumeration.display, "Symbol_type::Type", "display root");
    expect_size_t_equal(root->data.enumeration.number, 1, "enumeration number root");

    test_build_teardown(&pd, &pr, root);
}

void test_build_enum2()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "enum Symbol_type\n"
        "    Variable\n"
        "    Type\n"
        "    Info\n"
        "end\n"
        "Symbol_type::Info\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    assert_ptr(root, "ptr value");
    expect_int_equal(root->type, Cent_value_type_enum, "type root");
    expect_str(&root->data.enumeration.id1, "Symbol_type", "id1 root");
    expect_str(&root->data.enumeration.id2, "Info", "id2 root");
    expect_str(&root->data.enumeration.display, "Symbol_type::Info", "display root");
    expect_size_t_equal(root->data.enumeration.number, 2, "enumeration number root");

    test_build_teardown(&pd, &pr, root);
}

void test_build_assign()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "a = 1\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    expect_ptr_equal(root, NULL, "ptr equal root");

    Cent_environment* top = Cent_get_environment(pr.root);
    assert_ptr(top, "ptr top");

    Cent_symbol* sym = Cent_environment_get_str(top, "a");
    assert_ptr(sym, "ptr sym");
    expect_int_equal(sym->type, Cent_symbol_type_variable, "type sym");

    Cent_value* value = sym->data.variable.value;
    assert_ptr(value, "ptr value");
    expect_int_equal(value->type, Cent_value_type_number, "type number");
    expect_int_equal(value->number_type, Cent_number_type_integer, "number_type value");
    expect_long_long_equal(value->data.integer, 1, "integer value");

    test_build_teardown(&pd, &pr, root);
}

void test_build_object()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "Test {}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    expect_ptr(root, "ptr root");
    expect_int_equal(root->type, Cent_value_type_object, "type root");

    test_build_teardown(&pd, &pr, root);
}

void test_build_object_prop_set()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "Test {\n"
        "    .a = 1\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    expect_ptr(root, "ptr root");
    expect_int_equal(root->type, Cent_value_type_object, "type root");
    Cent_value* a = Cent_value_get_str(root, "a");
    assert_ptr(a, "ptr a");
    expect_int_equal(a->type, Cent_value_type_number, "type number");
    expect_int_equal(a->number_type, Cent_number_type_integer, "number_type value");
    expect_long_long_equal(a->data.integer, 1, "integer value");

    test_build_teardown(&pd, &pr, root);
}

void test_build_object_assign()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "Test {\n"
        "    count = 20\n"
        "    .count_value = count\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    expect_ptr(root, "ptr root");
    expect_int_equal(root->type, Cent_value_type_object, "type root");
    Cent_value* count_value = Cent_value_get_str(root, "count_value");
    assert_ptr(count_value, "ptr count_value");
    expect_int_equal(count_value->type, Cent_value_type_number, "type number");
    expect_int_equal(count_value->number_type, Cent_number_type_integer, "number_type value");
    expect_long_long_equal(count_value->data.integer, 20, "integer value");

    test_build_teardown(&pd, &pr, root);
}

void test_build_object_child_of()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "Test {\n"
        "    foo = Foo {}\n"
        "    bar = Bar {\n"
        "        .@child_of(foo)\n"
        "    }\n"
        "    foo\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    expect_ptr(root, "ptr root");
    expect_int_equal(root->type, Cent_value_type_object, "type root");
    expect_str(&root->data.name, "Test", "name root");

    Cent_value* foo = root->head;
    assert_ptr(foo, "ptr foo");
    expect_int_equal(foo->type, Cent_value_type_object, "type foo");
    expect_str(&foo->data.name, "Foo", "name foo");

    Cent_value* bar = foo->head;
    assert_ptr(bar, "ptr bar");
    expect_int_equal(bar->type, Cent_value_type_object, "type bar");
    expect_str(&bar->data.name, "Bar", "name bar");

    test_build_teardown(&pd, &pr, root);
}

void test_build_object_property_of()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "Test {\n"
        "    foo = Foo {}\n"
        "    bar = Bar {\n"
        "        .@property_of(foo, \"x\")\n"
        "    }\n"
        "    foo\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Cent_value_type_object, "type root");
    expect_str(&root->data.name, "Test", "name root");

    Cent_value* foo = root->head;
    assert_ptr(foo, "ptr foo");
    expect_int_equal(foo->type, Cent_value_type_object, "type foo");
    expect_str(&foo->data.name, "Foo", "name foo");

    Cent_value* bar = Cent_value_get_str(foo, "x");
    assert_ptr(bar, "ptr bar");
    expect_int_equal(bar->type, Cent_value_type_object, "type bar");
    expect_str(&bar->data.name, "Bar", "name bar");

    test_build_teardown(&pd, &pr, root);
}

void test_build_object_function_top()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "Test {\n"
        "    @top()\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Cent_value_type_object, "type root");
    expect_str(&root->data.name, "Test", "name root");

    Cent_value* top = root->head;
    expect_ptr_equal(top, root, "ptr equal top");

    test_build_teardown(&pd, &pr, root);
}

void test_build_object_function_file_name()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "@file_name()\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Cent_value_type_string, "type root");
    expect_str(&root->data.string, "**string**", "string root");

    test_build_teardown(&pd, &pr, root);
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
    test_build_object_function_top();
    test_build_object_function_file_name();
}