#include "zinc/unit_test.h"
#include "worm/parse.h"
#include "worm/lex_data.h"
#include "worm/parse_data.h"
#include "worm/lex.h"
#include "zinc/input_unicode_string.h"
#include <string.h>
#include "test_parse_tools.h"
#include "zinc/error_unit_test.h"

void test_parse_type_int32()
{   
    Zinc_test_name(__func__);

    Worm_parse_data* pd = NULL;

    test_parse_setup(&pd,
        "element Scalar {\n"
        "    children {\n"
        "        Integer\n"
        "    }\n"
        "}\n"
        "element Integer {\n"
        "    properties {\n"
        "        name: String\n"
        "        bit_count: Nat32\n"
        "        is_signed: Bool\n"
        "    }\n"
        "}\n"
        "Scalar {}\n"
        ".Integer {.name=\"Int32\";.bit_count=32;.is_signed=true}\n"
    );

    Worm_ast* root = Worm_parse(pd);
    Zinc_expect_no_errors(pd->errors);

    /* root */
    Zinc_assert_ptr(root, "ptr root");
    Zinc_expect_int_equal(root->type, Worm_ast_type_stmts, "type root");

    /* Scaler type */
    Worm_ast* element0 = Worm_ast_get(root, 0);
    Zinc_assert_ptr(element0, "ptr element0");
    Zinc_expect_int_equal(element0->type, Worm_ast_type_element, "type element0");
    Zinc_expect_string(&element0->value, "Scalar", "value element0");

    Worm_ast* children0 = Worm_ast_get(element0, 0);
    Zinc_assert_ptr(children0, "ptr children0");
    Zinc_expect_int_equal(children0->type, Worm_ast_type_children, "type children0");

    Worm_ast* type0 = Worm_ast_get(children0, 0);
    Zinc_assert_ptr(type0, "ptr type0");
    Zinc_expect_int_equal(type0->type, Worm_ast_type_id, "type type0");
    Zinc_expect_string(&type0->value, "Integer", "value type0");

    /* Integer type */
    Worm_ast* int_type = Worm_ast_get(root, 1);
    Zinc_assert_ptr(int_type, "ptr int_type");
    Zinc_expect_int_equal(int_type->type, Worm_ast_type_element, "type int_type");
    Zinc_expect_string(&int_type->value, "Integer", "value int_type");

    Worm_ast* int_type_props = Worm_ast_get(int_type, 0);
    Zinc_assert_ptr(int_type_props, "ptr int_type_props");
    Zinc_expect_int_equal(int_type_props->type, Worm_ast_type_properties, "type int_type_props");

    Worm_ast* int_type_prop0 = Worm_ast_get(int_type_props, 0);
    Zinc_assert_ptr(int_type_prop0, "ptr int_type_prop0");
    Zinc_expect_int_equal(int_type_prop0->type, Worm_ast_type_property_type, "type int_type_prop0");

    Worm_ast* int_type_prop0_id0 = Worm_ast_get(int_type_prop0, 0);
    Zinc_assert_ptr(int_type_prop0_id0, "ptr int_type_prop0_id0");
    Zinc_expect_int_equal(int_type_prop0_id0->type, Worm_ast_type_id, "type int_type_prop0_id0");
    Zinc_expect_string(&int_type_prop0_id0->value, "name", "value int_type_prop0_id0");

    Worm_ast* int_type_prop0_id1 = Worm_ast_get(int_type_prop0, 1);
    Zinc_assert_ptr(int_type_prop0_id1, "ptr int_type_prop0_id1");
    Zinc_expect_int_equal(int_type_prop0_id1->type, Worm_ast_type_id, "type int_type_prop0_id1");
    Zinc_expect_string(&int_type_prop0_id1->value, "String", "value int_type_prop0_id1");

    /* Scaler */
    Worm_ast* node0 = Worm_ast_get(root, 2);
    Zinc_assert_ptr(node0, "ptr node0");
    Zinc_expect_int_equal(node0->type, Worm_ast_type_node, "type node0");

    Worm_ast* level0 = Worm_ast_get(node0, 0);
    Zinc_assert_ptr(level0, "ptr level0");
    Zinc_expect_int_equal(level0->type, Worm_ast_type_natural, "type level0");
    Zinc_expect_unsigned_long_equal(level0->number.natural, 0, "natrual level0");

    Worm_ast* scalar0 = Worm_ast_get(node0, 1);
    Zinc_assert_ptr(scalar0, "ptr scaler0");
    Zinc_expect_int_equal(scalar0->type, Worm_ast_type_object, "type scalar0");
    Zinc_expect_string(&scalar0->value, "Scalar", "value scalar0");

    /* Integer */
    Worm_ast* node1 = Worm_ast_get(root, 3);
    Zinc_assert_ptr(node1, "ptr node1");
    Zinc_expect_int_equal(node1->type, Worm_ast_type_node, "type node1");

    Worm_ast* level1 = Worm_ast_get(node1, 0);
    Zinc_assert_ptr(level1, "ptr level1");
    Zinc_expect_int_equal(level1->type, Worm_ast_type_natural, "type level1");
    Zinc_expect_unsigned_long_equal(level1->number.natural, 1, "natrual level1");

    Worm_ast* integer1 = Worm_ast_get(node1, 1);
    Zinc_assert_ptr(integer1, "ptr integer1");
    Zinc_expect_int_equal(integer1->type, Worm_ast_type_object, "type integer1");
    Zinc_expect_string(&integer1->value, "Integer", "value integer1");

    Worm_ast* name_prop = Worm_ast_get(integer1, 0);
    Zinc_assert_ptr(name_prop, "ptr name_prop");
    Zinc_expect_int_equal(name_prop->type, Worm_ast_type_property, "type name_prop");

    Worm_ast* id = Worm_ast_get(name_prop, 0);
    Zinc_assert_ptr(id, "ptr id");
    Zinc_expect_int_equal(id->type, Worm_ast_type_id, "type id");
    Zinc_expect_string(&id->value, "name", "value id");

    Worm_ast* name_expr = Worm_ast_get(name_prop, 1);
    Zinc_assert_ptr(name_expr, "ptr name_expr");
    Zinc_expect_int_equal(name_expr->type, Worm_ast_type_string, "type name_expr");
    Zinc_expect_string(&name_expr->value, "Int32", "value name_expr");

    Worm_ast* bit_count_prop = Worm_ast_get(integer1, 1);
    Zinc_assert_ptr(bit_count_prop, "ptr bit_count_prop");
    Zinc_expect_int_equal(bit_count_prop->type, Worm_ast_type_property, "type bit_count_prop");

    Worm_ast* id1 = Worm_ast_get(bit_count_prop, 0);
    Zinc_assert_ptr(id1, "ptr id1");
    Zinc_expect_int_equal(id1->type, Worm_ast_type_id, "type id1");
    Zinc_expect_string(&id1->value, "bit_count", "value id1");

    Worm_ast* expr1 = Worm_ast_get(bit_count_prop, 1);
    Zinc_assert_ptr(expr1, "ptr expr1");
    Zinc_expect_int_equal(expr1->type, Worm_ast_type_natural, "type expr1");
    Zinc_expect_unsigned_long_equal(expr1->number.natural, 32, "value expr1");

    Worm_ast* is_signed_prop = Worm_ast_get(integer1, 2);
    Zinc_assert_ptr(is_signed_prop, "ptr is_signed_prop");
    Zinc_expect_int_equal(is_signed_prop->type, Worm_ast_type_property, "type is_signed_prop");

    Worm_ast* id2 = Worm_ast_get(is_signed_prop, 0);
    Zinc_assert_ptr(id2, "ptr id2");
    Zinc_expect_int_equal(id2->type, Worm_ast_type_id, "type id2");
    Zinc_expect_string(&id2->value, "is_signed", "value id2");

    Worm_ast* expr2 = Worm_ast_get(is_signed_prop, 1);
    Zinc_assert_ptr(expr2, "ptr expr2");
    Zinc_expect_int_equal(expr2->type, Worm_ast_type_boolean, "type expr2");
    Zinc_expect_true(expr2->number.boolean, "boolean expr2");

    test_parse_teardown(pd);
    Worm_ast_destroy(root);
    free(root);
}

void test_parse()
{
    test_parse_type_int32();
}
