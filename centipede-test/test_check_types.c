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

void test_check_types_property_error()
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

void test_check_types()
{
    test_check_types_property();
    test_check_types_property_error();
}