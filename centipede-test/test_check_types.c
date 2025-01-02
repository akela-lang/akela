#include "zinc/unit_test.h"
#include "test_check_types.h"

#include <zinc/error_unit_test.h>

#include "test_build_tools.h"
#include "test_parse_tools.h"
#include "centipede/parse.h"
#include "centipede/build.h"

void test_check_types_property()
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
        "Test {\n"
        "    .source = Source {\n"
        "        \"a = 1\"\n"
        "        \"a + 10\"\n"
        "    }\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);
    assert_ptr(root, "ptr value");
    test_build_teardown(&pd, &pr, root);
}

void test_check_types_property_error_number()
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
        "Test {\n"
        "    .source = 150\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_has_errors(pr.errors);
    assert_ptr(root, "ptr value");
    expect_source_error(pr.errors, "invalid value element type: Integer");
    test_build_teardown(&pd, &pr, root);
}


void test_check_types_property_error_string()
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
        "Test {\n"
        "    .source = \"hello\"\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_has_errors(pr.errors);
    assert_ptr(root, "ptr value");
    expect_source_error(pr.errors, "invalid value element type: String");
    test_build_teardown(&pd, &pr, root);
}

void test_check_types_property_error_boolean()
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
        "Test {\n"
        "    .source = true\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_has_errors(pr.errors);
    assert_ptr(root, "ptr value");
    expect_source_error(pr.errors, "invalid value element type: Bool");
    test_build_teardown(&pd, &pr, root);
}

void test_check_types_property_error_object()
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
        "Test {\n"
        "    .source = Foo {}\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_has_errors(pr.errors);
    assert_ptr(root, "ptr value");
    expect_source_error(pr.errors, "invalid value element type: Foo");
    test_build_teardown(&pd, &pr, root);
}

void test_check_types_property_variable()
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
    expect_no_errors(pr.errors);
    test_build_teardown(&pd, &pr, root);
}

void test_check_types_property_error_variable_object()
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
        "a = Foo {}\n"
        "Test {\n"
        "    .source = a\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_has_errors(pr.errors);
    assert_ptr(root, "ptr value");
    expect_source_error(pr.errors, "invalid value element type: Foo");
    test_build_teardown(&pd, &pr, root);
}

void test_check_types_property_enum()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "enum Vehicle_type\n"
        "    Car\n"
        "    Truck\n"
        "    Van\n"
        "    Motorcycle\n"
        "end\n"
        "element Vehicle\n"
        "    properties\n"
        "        type: Vehicle_type\n"
        "    end\n"
        "end\n"
        "Vehicle {\n"
        "    .type = Vehicle_type::Truck\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);
    assert_ptr(root, "ptr value");

    test_build_teardown(&pd, &pr, root);
}

void test_check_types_property_enum_error_id1()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "enum Vehicle_type\n"
        "    Car\n"
        "    Truck\n"
        "    Van\n"
        "    Motorcycle\n"
        "end\n"
        "element Vehicle\n"
        "    properties\n"
        "        type: Vehicle_type\n"
        "    end\n"
        "end\n"
        "Vehicle {\n"
        "    .type = Coin_type::Truck\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_has_errors(pr.errors);
    expect_source_error(pr.errors, "could not find enum: Coin_type");

    test_build_teardown(&pd, &pr, root);
}

void test_check_types_property_enum_error_id2()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "enum Vehicle_type\n"
        "    Car\n"
        "    Truck\n"
        "    Van\n"
        "    Motorcycle\n"
        "end\n"
        "element Vehicle\n"
        "    properties\n"
        "        type: Vehicle_type\n"
        "    end\n"
        "end\n"
        "Vehicle {\n"
        "    .type = Vehicle_type::Pencil\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_has_errors(pr.errors);
    expect_source_error(pr.errors, "could not find enum id: Pencil");

    test_build_teardown(&pd, &pr, root);
}

void test_check_types_child()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "element Source\n"
        "    children\n"
        "        String\n"
        "    end\n"
        "end\n"
        "Source {\n"
        "    \"one\"\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    test_build_teardown(&pd, &pr, root);
}

void test_check_types_child_error_number()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "element Source\n"
        "    children\n"
        "        String\n"
        "    end\n"
        "end\n"
        "Source {\n"
        "    \"one\"\n"
        "    24.3\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_has_errors(pr.errors);
    struct error* e =expect_source_error(pr.errors, "invalid child type: Float");
    expect_size_t_equal(e->loc.line, 8, "line e");
    expect_size_t_equal(e->loc.col, 5, "col e");

    test_build_teardown(&pd, &pr, root);
}

void test_check_types_child_enum()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "enum Vehicle_type\n"
        "    Car\n"
        "    Truck\n"
        "    Van\n"
        "    Motorcycle\n"
        "end\n"
        "element Vehicle\n"
        "    children\n"
        "        Vehicle_type\n"
        "    end\n"
        "end\n"
        "Vehicle {\n"
        "    Vehicle_type::Car\n"
        "    Vehicle_type::Van\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_no_errors(pr.errors);

    test_build_teardown(&pd, &pr, root);
}

void test_check_types_child_enum_error_id1_not_found()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "enum Vehicle_type\n"
        "    Car\n"
        "    Truck\n"
        "    Van\n"
        "    Motorcycle\n"
        "end\n"
        "element Vehicle\n"
        "    children\n"
        "        Vehicle_type\n"
        "    end\n"
        "end\n"
        "Vehicle {\n"
        "    Vehicle_type::Car\n"
        "    Building_type::Van\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_has_errors(pr.errors);
    struct error* e = expect_source_error(pr.errors, "could not find enum: Building_type");
    assert_ptr(e, "ptr e");
    expect_size_t_equal(e->loc.line, 14, "line e");
    expect_size_t_equal(e->loc.col, 5, "col e");

    test_build_teardown(&pd, &pr, root);
}

void test_check_types_child_enum_error_id1_not_match()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "enum Vehicle_type\n"
        "    Car\n"
        "    Truck\n"
        "    Van\n"
        "    Motorcycle\n"
        "end\n"
        "enum Building_type\n"
        "    Brick\n"
        "    Cement\n"
        "    Wood\n"
        "end\n"
        "element Vehicle\n"
        "    children\n"
        "        Vehicle_type\n"
        "    end\n"
        "end\n"
        "Vehicle {\n"
        "    Vehicle_type::Car\n"
        "    Building_type::Brick\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_has_errors(pr.errors);
    struct error* e = expect_source_error(pr.errors, "invalid child type: Building_type");
    assert_ptr(e, "ptr e");
    expect_size_t_equal(e->loc.line, 19, "line e");
    expect_size_t_equal(e->loc.col, 5, "col e");

    test_build_teardown(&pd, &pr, root);
}

void test_check_types_child_enum_error_id2_not_valid()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "enum Vehicle_type\n"
        "    Car\n"
        "    Truck\n"
        "    Van\n"
        "    Motorcycle\n"
        "end\n"
        "element Vehicle\n"
        "    children\n"
        "        Vehicle_type\n"
        "    end\n"
        "end\n"
        "Vehicle {\n"
        "    Vehicle_type::Car\n"
        "    Vehicle_type::Bike\n"
        "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    Cent_value* root = Cent_build(&pr);

    expect_has_errors(pr.errors);
    struct error* e = expect_source_error(pr.errors, "could not find enum id: Bike");
    assert_ptr(e, "ptr e");
    expect_size_t_equal(e->loc.line, 14, "line e");
    expect_size_t_equal(e->loc.col, 19, "col e");

    test_build_teardown(&pd, &pr, root);
}

void test_check_types()
{
    test_check_types_property();
    test_check_types_property_error_number();
    test_check_types_property_error_string();
    test_check_types_property_error_boolean();
    test_check_types_property_error_object();
    test_check_types_property_variable();
    test_check_types_property_error_variable_object();
    test_check_types_property_enum();
    test_check_types_property_enum_error_id1();
    test_check_types_property_enum_error_id2();
    test_check_types_child();
    test_check_types_child_error_number();
    test_check_types_child_enum();
    test_check_types_child_enum_error_id1_not_found();
    test_check_types_child_enum_error_id1_not_match();
    test_check_types_child_enum_error_id2_not_valid();
}