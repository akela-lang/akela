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

    test_parse_teardown(&pd, &pr);
}

void test_parse()
{
    test_parse_element();
}