#include <string.h>
#include <zinc/error_unit_test.h>

#include "zinc/unit_test.h"
#include "lava/parse.h"
#include "zinc/input_unicode_string.h"

void unit_parse_header()
{
    Zinc_test_name(__func__);

    Lava_result pr = Lava_parse_str("# Test Suite\n");

    Zinc_expect_no_errors(pr.errors);

    Zinc_expect_ptr(pr.root, "ptr root");
    Zinc_expect_int_equal(pr.root->kind, LAVA_DOM_HEADER, "kind root");
    Zinc_expect_int_equal(pr.root->data.LAVA_DOM_HEADER.level, 1, "level root");
    Zinc_expect_string(&pr.root->data.LAVA_DOM_HEADER.title, " Test Suite", "string root");
    Zinc_expect_size_t_equal(pr.root->data.LAVA_DOM_HEADER.items.count, 0, "count root");

    Lava_result_destroy(&pr);
}

void unit_parse()
{
    unit_parse_header();
}