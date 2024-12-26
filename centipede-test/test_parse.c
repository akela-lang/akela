#include <string.h>
#include <string.h>

#include "zinc/unit_test.h"
#include "centipede/parse.h"
#include "centipede/parse_data.h"
#include "zinc/input_unicode_string.h"
#include "zinc/error.h"
#include "test_parse_tools.h"

void test_parse_element()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "element Test_suite\n"
        "    properties\n"
        "        name: String `required`\n"
        "        solo: Bool      # only run this suite\n"
        "        mute: Bool      # exclude this suite from running\n"
        "    end\n"
        "    children\n"
        "        Test\n"
        "    end\n"
        "end\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);

    assert_ptr(pr.root, "ptr pr.root");
    expect_int_equal(pr.root->type, Cent_ast_type_stmts, "type pr.root");

    Cent_ast* element = Cent_ast_get(pr.root, 0);
    assert_ptr(element, "ptr element");
    expect_int_equal(element->type, Cent_ast_type_element, "type element");

    Cent_ast* prop = Cent_ast_get(element, 0);
    assert_ptr(prop, "ptr prop");
    expect_int_equal(prop->type, Cent_ast_type_prop, "type prop");

    Cent_ast* dec0 = Cent_ast_get(prop, 0);
    assert_ptr(dec0, "ptr dec0");
    expect_int_equal(dec0->type, Cent_ast_type_prop_dec, "type dec0");

    Cent_ast* name0 = Cent_ast_get(dec0, 0);
    assert_ptr(name0, "ptr name0");
    expect_int_equal(name0->type, Cent_ast_type_id, "type name0");
    expect_str(&name0->value, "name", "value name0");

    Cent_ast* type0 = Cent_ast_get(dec0, 1);
    assert_ptr(type0, "ptr type0");
    expect_int_equal(type0->type, Cent_ast_type_id, "type type0");
    expect_str(&type0->value, "String", "value type0");

    Cent_ast* mod0 = Cent_ast_get(dec0, 2);
    assert_ptr(mod0, "ptr mod0");
    expect_int_equal(mod0->type, Cent_ast_type_modifier, "type mod0");
    expect_str(&mod0->value, "required", "value mod0");

    Cent_ast* dec1 = Cent_ast_get(prop, 1);
    assert_ptr(dec1, "ptr dec1");
    expect_int_equal(dec1->type, Cent_ast_type_prop_dec, "type dec1");

    Cent_ast* name1 = Cent_ast_get(dec1, 0);
    assert_ptr(name1, "ptr name1");
    expect_int_equal(name1->type, Cent_ast_type_id, "type name1");
    expect_str(&name1->value, "solo", "value name1");

    Cent_ast* type1 = Cent_ast_get(dec1, 1);
    assert_ptr(type1, "ptr type1");
    expect_int_equal(type1->type, Cent_ast_type_id, "type type1");
    expect_str(&type1->value, "Bool", "value type1");

    Cent_ast* dec2 = Cent_ast_get(prop, 2);
    assert_ptr(dec2, "ptr dec2");
    expect_int_equal(dec2->type, Cent_ast_type_prop_dec, "type dec2");

    Cent_ast* name2 = Cent_ast_get(dec2, 0);
    assert_ptr(name2, "ptr name2");
    expect_int_equal(name2->type, Cent_ast_type_id, "type name2");
    expect_str(&name2->value, "mute", "value name2");

    Cent_ast* type2 = Cent_ast_get(dec2, 1);
    assert_ptr(type2, "ptr type2");
    expect_int_equal(type2->type, Cent_ast_type_id, "type type2");
    expect_str(&type2->value, "Bool", "value type2");

    test_parse_teardown(&pd, &pr);
}

void test_parse()
{
    test_parse_element();
}