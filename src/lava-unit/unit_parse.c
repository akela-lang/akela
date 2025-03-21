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

void unit_parse_header_text()
{
    Zinc_test_name(__func__);

    Lava_result pr = Lava_parse_str(
        "# Test Suite\n"
        "Define constant and use\n"
        );

    Zinc_expect_no_errors(pr.errors);

    Zinc_expect_ptr(pr.root, "ptr root");
    Zinc_expect_int_equal(pr.root->kind, LAVA_DOM_HEADER, "kind root");
    Zinc_expect_int_equal(pr.root->data.LAVA_DOM_HEADER.level, 1, "level root");
    Zinc_expect_string(&pr.root->data.LAVA_DOM_HEADER.title, " Test Suite", "string root");
    Zinc_expect_size_t_equal(pr.root->data.LAVA_DOM_HEADER.items.count, 1, "count root");

    Lava_dom* header_text = (Lava_dom*)ZINC_VECTOR_PTR(&pr.root->data.LAVA_DOM_HEADER.items, 0);
    Zinc_expect_ptr(header_text, "ptr header_text");
    Zinc_expect_int_equal(header_text->kind, LAVA_DOM_TEXT, "kind header_text");
    Zinc_expect_string(
        &header_text->data.LAVA_DOM_TEXT,
        "Define constant and use",
        "text header_text");

    Lava_result_destroy(&pr);
}

void unit_parse_header_text_backquote()
{
    Zinc_test_name(__func__);

    Lava_result pr = Lava_parse_str(
        "# Test Suite\n"
        "Define constant and use\n"
        "```\n"
        "use lib::base::*\n"
        "TestSuite {\n"
        "  .solo = false\n"
        "  .mute = false\n"
        "  .headers = headers\n"
        "}\n"
        "```\n"
        );

    Zinc_expect_no_errors(pr.errors);

    Zinc_expect_ptr(pr.root, "ptr root");
    Zinc_expect_int_equal(pr.root->kind, LAVA_DOM_HEADER, "kind root");
    Zinc_expect_int_equal(pr.root->data.LAVA_DOM_HEADER.level, 1, "level root");
    Zinc_expect_string(&pr.root->data.LAVA_DOM_HEADER.title, " Test Suite", "string root");
    Zinc_expect_size_t_equal(pr.root->data.LAVA_DOM_HEADER.items.count, 2, "count root");

    Lava_dom* header_text = (Lava_dom*)ZINC_VECTOR_PTR(&pr.root->data.LAVA_DOM_HEADER.items, 0);
    Zinc_expect_ptr(header_text, "ptr header_text");
    Zinc_expect_int_equal(header_text->kind, LAVA_DOM_TEXT, "kind header_text");
    Zinc_expect_string(
        &header_text->data.LAVA_DOM_TEXT,
        "Define constant and use",
        "text header_text");

    Lava_dom* header_backquote = (Lava_dom*)ZINC_VECTOR_PTR(&pr.root->data.LAVA_DOM_HEADER.items, 1);
    Zinc_expect_ptr(header_backquote, "ptr header_backquote");
    Zinc_expect_int_equal(header_backquote->kind, LAVA_DOM_BACKQUOTE, "kind header_backquote");
    Zinc_expect_string(&header_backquote->data.LAVA_DOM_BACKQUOTE.text,
        "use lib::base::*\n"
        "TestSuite {\n"
        "  .solo = false\n"
        "  .mute = false\n"
        "  .headers = headers\n"
        "}\n",
        "text header_backquote");

    Lava_result_destroy(&pr);
}

void unit_parse()
{
    unit_parse_header();
    unit_parse_header_text();
    unit_parse_header_text_backquote();
}