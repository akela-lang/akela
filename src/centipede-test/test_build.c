#include <zinc/error_unit_test.h>

#include "centipede/build.h"
#include "zinc/unit_test.h"
#include "test_parse_tools.h"
#include "centipede/parse.h"
#include "centipede/parse_tools.h"
#include "test_build_tools.h"
#include "centipede/check_value_types.h"

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

    assert_ptr(root, "ptr root");
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
    expect_str(&root->name, "Test", "name root");

    Cent_value* foo = root->data.object.head;
    assert_ptr(foo, "ptr foo");
    expect_int_equal(foo->type, Cent_value_type_object, "type foo");
    expect_str(&foo->name, "Foo", "name foo");

    Cent_value* bar = foo->data.object.head;
    assert_ptr(bar, "ptr bar");
    expect_int_equal(bar->type, Cent_value_type_object, "type bar");
    expect_str(&bar->name, "Bar", "name bar");

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
    expect_str(&root->name, "Test", "name root");

    Cent_value* foo = root->data.object.head;
    assert_ptr(foo, "ptr foo");
    expect_int_equal(foo->type, Cent_value_type_object, "type foo");
    expect_str(&foo->name, "Foo", "name foo");

    Cent_value* bar = Cent_value_get_str(foo, "x");
    assert_ptr(bar, "ptr bar");
    expect_int_equal(bar->type, Cent_value_type_object, "type bar");
    expect_str(&bar->name, "Bar", "name bar");

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

void test_build_property_set_variable()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "element Source\n"
        "    children\n"
        "        String\n"
        "    end\n"
        "end\n"
        "element Test\n"
        "    properties\n"
        "        source: Source\n"
        "    end\n"
        "end\n"
        "a = Source {\"x + 5\"}\n"
        "Test {\n"
        "    .source = a\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    assert_ptr(root, "ptr value");
    expect_int_equal(root->type, Cent_value_type_object, "type root");
    expect_str(&root->name, "Test", "name root");

    Cent_value* source = Cent_value_get_str(root, "source");
    assert_ptr(source, "ptr source");
    expect_int_equal(source->type, Cent_value_type_object, "type source");
    expect_str(&source->name, "Source", "name source");

    Cent_value* line0 = source->data.object.head;
    assert_ptr(line0, "ptr line0");
    expect_int_equal(line0->type, Cent_value_type_string, "type line0");
    expect_str(&line0->name, "String", "name line0");
    expect_str(&line0->data.string, "x + 5", "string line0");

    test_build_teardown(&pd, &pr, root);
}

void test_build_namespace_enum()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "use types\n"
        "Groceries {\n"
        "    types::Grocery_item::Milk\n"
        "    types::Grocery_item::Carrots\n"
        "}\n"
    );

    test_parse_add_comp_unit(&pd, "types.aken",
        "enum Grocery_item\n"
        "    Milk\n"
        "    Cereal\n"
        "    Steak\n"
        "    Potatoes\n"
        "    Carrots\n"
        "end\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    assert_ptr(root, "ptr value");
    expect_int_equal(root->type, Cent_value_type_object, "type root");
    expect_str(&root->name, "Groceries", "name root");

    Cent_value* item0 = root->data.object.head;
    assert_ptr(item0, "ptr item0");
    expect_int_equal(item0->type, Cent_value_type_enum, "type item0");
    expect_str(&item0->data.enumeration.display, "Grocery_item::Milk", "display item0");
    expect_size_t_equal(item0->data.enumeration.number, 0, "number item0");

    Cent_value* item1 = item0->next;
    assert_ptr(item0, "ptr item1");
    expect_int_equal(item1->type, Cent_value_type_enum, "type item1");
    expect_str(&item1->data.enumeration.display, "Grocery_item::Carrots", "display item1");
    expect_size_t_equal(item1->data.enumeration.number, 4, "number item1");

    test_build_teardown(&pd, &pr, root);
}

void test_build_namespace_variable()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "use variables\n"
        "variables::a\n"
    );

    test_parse_add_comp_unit(&pd, "variables.aken",
        "a = 190\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    assert_ptr(root, "ptr value");
    expect_int_equal(root->type, Cent_value_type_number, "type root");
    expect_int_equal(root->number_type, Cent_number_type_integer, "number root");
    expect_long_long_equal(root->data.integer, 190, "integer root");

    test_build_teardown(&pd, &pr, root);
}

void test_build_namespace_submodules()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "use foo::bar\n"
        "foo::bar::a\n"
    );

    test_parse_add_comp_unit(&pd, "foo/bar.aken",
        "a = 190\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    assert_ptr(root, "ptr value");
    expect_int_equal(root->type, Cent_value_type_number, "type root");
    expect_int_equal(root->number_type, Cent_number_type_integer, "number root");
    expect_long_long_equal(root->data.integer, 190, "integer root");

    test_build_teardown(&pd, &pr, root);
}

void test_build_namespace_glob_value()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "use foo::bar::*\n"
        "a\n"
    );

    test_parse_add_comp_unit(&pd, "foo/bar.aken",
        "a = 190\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    assert_ptr(root, "ptr value");
    expect_int_equal(root->type, Cent_value_type_number, "type root");
    expect_int_equal(root->number_type, Cent_number_type_integer, "number root");
    expect_long_long_equal(root->data.integer, 190, "integer root");

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
    test_build_object_function_file_name();
    test_build_property_set_variable();
    test_build_namespace_enum();
    test_build_namespace_variable();
    test_build_namespace_submodules();
    test_build_namespace_glob_value();
}