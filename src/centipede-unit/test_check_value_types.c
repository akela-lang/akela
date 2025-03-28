#include "zinc/unit_test.h"
#include <zinc/error_unit_test.h>
#include "centipede/build.h"
#include "centipede/comp_table.h"

void test_check_value_types_property()
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
        "Test {\n"
        "    .source = Source {\n"
        "        \"a = 1\"\n"
        "        \"a + 10\"\n"
        "    }\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);
    Zinc_assert_ptr(root, "ptr value");
    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_property_error_number()
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
        "Test {\n"
        "    .source = 150\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_has_errors(errors);
    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_source_error(errors,
        "for property (Test--source), found type (Natural) but expected (Source)");
    Cent_comp_table_destroy(ct);
    free(ct);
}


void test_check_value_types_property_error_string()
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
        "Test {\n"
        "    .source = \"hello\"\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_has_errors(errors);
    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_source_error(errors,
        "for property (Test--source), found type (String) but expected (Source)");
    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_property_error_boolean()
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
        "Test {\n"
        "    .source = true\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_has_errors(errors);
    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_source_error(errors,
        "for property (Test--source), found type (Bool) but expected (Source)");
    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_property_error_object()
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
        "Test {\n"
        "    .source = Foo {}\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_has_errors(errors);
    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_source_error(errors,
        "for property (Test--source), found type (Foo) but expected (Source)");
    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_property_variable()
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
    Zinc_expect_no_errors(errors);
    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_property_error_variable_object()
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
        "const a = Foo {};\n"
        "Test {\n"
        "    .source = a\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_has_errors(errors);
    Zinc_assert_ptr(root, "ptr value");
    Zinc_expect_source_error(errors,
        "for property (Test--source), found type (Foo) but expected (Source)");
    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_property_enum()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Vehicle_type {\n"
        "    Car\n"
        "    Truck\n"
        "    Van\n"
        "    Motorcycle\n"
        "}\n"
        "element Vehicle {\n"
        "    properties {\n"
        "        type: Vehicle_type\n"
        "    }\n"
        "}\n"
        "Vehicle {\n"
        "    .type = Vehicle_type::Truck\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(errors);
    Zinc_assert_ptr(root, "ptr value");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_property_enum_error_id1()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Vehicle_type {\n"
        "    Car\n"
        "    Truck\n"
        "    Van\n"
        "    Motorcycle\n"
        "}\n"
        "element Vehicle {\n"
        "    properties {\n"
        "        type: Vehicle_type\n"
        "    }\n"
        "}\n"
        "Vehicle {\n"
        "    .type = Coin_type::Truck\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "id is not a variable: Coin_type");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_property_enum_error_id2()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Vehicle_type {\n"
        "    Car\n"
        "    Truck\n"
        "    Van\n"
        "    Motorcycle\n"
        "}\n"
        "element Vehicle {\n"
        "    properties {\n"
        "        type: Vehicle_type\n"
        "    }\n"
        "}\n"
        "Vehicle {\n"
        "    .type = Vehicle_type::Pencil\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "invalid enum id: Pencil");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_child()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Source {\n"
        "    children {\n"
        "        String\n"
        "    }\n"
        "}\n"
        "Source {\n"
        "    \"one\"\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_no_errors(errors);

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_child_error_number()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Source {\n"
        "    children {\n"
        "        String\n"
        "    }\n"
        "}\n"
        "Source {\n"
        "    \"one\"\n"
        "    24.3\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_error* e = Zinc_expect_source_error(errors, "invalid child type: Real");
    if (e) {
        Zinc_expect_size_t_equal(e->loc.line, 8, "line e");
        Zinc_expect_size_t_equal(e->loc.col, 5, "col e");
    }

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_child_enum()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Vehicle_type {\n"
        "    Car\n"
        "    Truck\n"
        "    Van\n"
        "    Motorcycle\n"
        "}\n"
        "element Vehicle {\n"
        "    children {\n"
        "        Vehicle_type\n"
        "    }\n"
        "}\n"
        "Vehicle {\n"
        "    Vehicle_type::Car\n"
        "    Vehicle_type::Van\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_no_errors(errors);

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_child_enum_error_id1_not_found()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Vehicle_type {\n"
        "    Car\n"
        "    Truck\n"
        "    Van\n"
        "    Motorcycle\n"
        "}\n"
        "element Vehicle {\n"
        "    children {\n"
        "        Vehicle_type\n"
        "    }\n"
        "}\n"
        "Vehicle {\n"
        "    Vehicle_type::Car\n"
        "    Building_type::Van\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    struct Zinc_error* e = Zinc_expect_source_error(errors, "id is not a variable: Building_type");
    Zinc_assert_ptr(e, "ptr e");
    Zinc_expect_size_t_equal(e->loc.line, 14, "line e");
    Zinc_expect_size_t_equal(e->loc.col, 5, "col e");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_child_enum_error_id1_not_match()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Vehicle_type {\n"
        "    Car\n"
        "    Truck\n"
        "    Van\n"
        "    Motorcycle\n"
        "}\n"
        "enum Building_type {\n"
        "    Brick\n"
        "    Cement\n"
        "    Wood\n"
        "}\n"
        "element Vehicle {\n"
        "    children {\n"
        "        Vehicle_type\n"
        "    }\n"
        "}\n"
        "Vehicle {\n"
        "    Vehicle_type::Car\n"
        "    Building_type::Brick\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    struct Zinc_error* e = Zinc_expect_source_error(errors, "invalid child type: Building_type");
    Zinc_assert_ptr(e, "ptr e");
    Zinc_expect_size_t_equal(e->loc.line, 19, "line e");
    Zinc_expect_size_t_equal(e->loc.col, 5, "col e");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_child_enum_error_id2_not_valid()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Vehicle_type {\n"
        "    Car\n"
        "    Truck\n"
        "    Van\n"
        "    Motorcycle\n"
        "}\n"
        "element Vehicle {\n"
        "    children {\n"
        "        Vehicle_type\n"
        "    }\n"
        "}\n"
        "Vehicle {\n"
        "    Vehicle_type::Car\n"
        "    Vehicle_type::Bike\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    struct Zinc_error* e = Zinc_expect_source_error(errors, "invalid enum id: Bike");
    Zinc_assert_ptr(e, "ptr e");
    Zinc_expect_size_t_equal(e->loc.line, 14, "line e");
    Zinc_expect_size_t_equal(e->loc.col, 19, "col e");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_property_error_not_enum()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Bar {\n"
        "    Filler\n"
        "}\n"
        "element Foo {\n"
        "    properties {\n"
        "        a: Bar\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .a = 87\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    struct Zinc_error* e = Zinc_expect_source_error(errors, "value is not an enum value: Natural");
    Zinc_assert_ptr(e, "ptr e");
    Zinc_expect_size_t_equal(e->loc.line, 10, "line e");
    Zinc_expect_size_t_equal(e->loc.col, 10, "col e");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_property_enum_error_not_match()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Apparel {\n"
        "    Shoes\n"
        "}\n"
        "enum Bar {\n"
        "    Filler\n"
        "}\n"
        "element Foo {\n"
        "    properties {\n"
        "        a: Bar\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .a = Apparel::Shoes\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    struct Zinc_error* e = Zinc_expect_source_error(errors, "invalid value enum type: Apparel");
    Zinc_assert_ptr(e, "ptr e");
    Zinc_expect_size_t_equal(e->loc.line, 13, "line e");
    Zinc_expect_size_t_equal(e->loc.col, 10, "col e");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_child_error_no_type()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Bar {\n"
        "}\n"
        "element Foo {\n"
        "    children {\n"
        "        Bar\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    Bar2 {}\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    struct Zinc_error* e = Zinc_expect_source_error(errors, "value has no type; looking for Bar");
    Zinc_assert_ptr(e, "ptr e");
    Zinc_expect_size_t_equal(e->loc.line, 9, "line e");
    Zinc_expect_size_t_equal(e->loc.col, 5, "col e");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_not_nested()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Bar {\n"
        "    properties {\n"
        "        a: Bool\n"
        "    }\n"
        "}\n"
        "Bar {.a=1}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);

    Zinc_expect_source_error(errors, "for property (Bar--a), found type (Natural) but expected (Bool)");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_nested()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Bar {\n"
        "    properties {\n"
        "        a: Bool\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    Bar {.a=1}\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);

    Zinc_expect_source_error(errors,
        "for property (Bar--a), found type (Natural) but expected (Bool)");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_not_attached()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "const a = Foo {};\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);

    Zinc_expect_source_error(errors, "value is not used");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_not_attached_import()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "use bar;\n"
    );
    Cent_comp_table_add_module_str(ct, "bar.cent",
        "const a = Foo {};\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_no_errors(errors);

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_not_attached_import_glob()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "use bar::*;\n"
    );
    Cent_comp_table_add_module_str(ct, "bar.cent",
        "const a = Foo {};\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_no_errors(errors);

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_not_attached_not_last()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "const a = Foo {};\n"
        "a;\n"
        "Bar {}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);

    Zinc_expect_source_error(errors, "value is not used");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_not_attached_not_last2()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "Foo {};\n"
        "Bar {};\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);

    Zinc_expect_source_error(errors, "value is not used");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types_not_attached_in_object()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "Bar {\n"
        "    const a = 1\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);

    Zinc_expect_source_error(errors, "value is not used");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_check_value_types()
{
    test_check_value_types_property();
    test_check_value_types_property_error_number();
    test_check_value_types_property_error_string();
    test_check_value_types_property_error_boolean();
    test_check_value_types_property_error_object();
    test_check_value_types_property_variable();
    test_check_value_types_property_error_variable_object();
    test_check_value_types_property_enum();
    test_check_value_types_property_enum_error_id1();
    test_check_value_types_property_enum_error_id2();
    test_check_value_types_child();
    test_check_value_types_child_error_number();
    test_check_value_types_child_enum();
    test_check_value_types_child_enum_error_id1_not_found();
    test_check_value_types_child_enum_error_id1_not_match();
    test_check_value_types_child_enum_error_id2_not_valid();
    test_check_value_types_property_error_not_enum();
    test_check_value_types_property_enum_error_not_match();
    test_check_value_types_child_error_no_type();
    test_check_value_types_not_nested();
    test_check_value_types_nested();
    test_check_value_types_not_attached();
    test_check_value_types_not_attached_import();
    test_check_value_types_not_attached_import_glob();
    test_check_value_types_not_attached_not_last();
    test_check_value_types_not_attached_not_last2();
    // test_check_value_types_not_attached_in_object();
}
