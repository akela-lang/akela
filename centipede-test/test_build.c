#include <zinc/error_unit_test.h>

#include "centipede/build.h"
#include "zinc/unit_test.h"
#include "test_parse_tools.h"
#include "centipede/parse.h"

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

    test_parse_teardown(&pd, &pr);
    Cent_value_free(root);
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

    test_parse_teardown(&pd, &pr);
    Cent_value_free(root);
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

    test_parse_teardown(&pd, &pr);
    Cent_value_free(root);
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

    test_parse_teardown(&pd, &pr);
    Cent_value_free(root);
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

    test_parse_teardown(&pd, &pr);
    Cent_value_free(root);
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

    test_parse_teardown(&pd, &pr);
    Cent_value_free(root);
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

    test_parse_teardown(&pd, &pr);
    Cent_value_free(root);
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

    test_parse_teardown(&pd, &pr);
    Cent_value_free(root);
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
}