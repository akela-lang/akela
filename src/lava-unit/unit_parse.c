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
    Zinc_expect_size_t_equal(Lava_dom_count(pr.root), 0, "count root");

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
    Zinc_expect_size_t_equal(Lava_dom_count(pr.root), 1, "count root");

    Lava_dom* header_text = Lava_dom_get(pr.root, 0);
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
    char* s =
        "# Test Suite\n"
        "Define constant and use\n"
        "\n"
        "```cent\n"
        "use lib::base::*\n"
        "TestSuite {\n"
        "  .solo = false\n"
        "  .mute = false\n"
        "}\n"
        "```\n";

    Lava_result pr = Lava_parse_str(s);

    Zinc_expect_no_errors(pr.errors);

    Zinc_expect_ptr(pr.root, "ptr root");
    Zinc_expect_int_equal(pr.root->kind, LAVA_DOM_HEADER, "kind root");
    Zinc_expect_int_equal(pr.root->data.LAVA_DOM_HEADER.level, 1, "level root");
    Zinc_expect_string(&pr.root->data.LAVA_DOM_HEADER.title, " Test Suite", "string root");
    Zinc_expect_size_t_equal(Lava_dom_count(pr.root), 2, "count root");

    Lava_dom* header_text = Lava_dom_get(pr.root, 0);
    Zinc_expect_ptr(header_text, "ptr header_text");
    Zinc_expect_int_equal(header_text->kind, LAVA_DOM_TEXT, "kind header_text");
    Zinc_expect_string(
        &header_text->data.LAVA_DOM_TEXT,
        "Define constant and use",
        "text header_text");

    Lava_dom* header_backquote = Lava_dom_get(pr.root, 1);
    Zinc_expect_ptr(header_backquote, "ptr header_backquote");
    Zinc_expect_int_equal(header_backquote->kind, LAVA_DOM_BACKQUOTE, "kind header_backquote");
    Zinc_expect_string(&header_backquote->data.LAVA_DOM_BACKQUOTE.format,
        "cent",
        "format header_backquote");
    Zinc_expect_string(&header_backquote->data.LAVA_DOM_BACKQUOTE.text,
        "use lib::base::*\n"
        "TestSuite {\n"
        "  .solo = false\n"
        "  .mute = false\n"
        "}\n",
        "text header_backquote");
    Zinc_input_bounds bounds = header_backquote->data.LAVA_DOM_BACKQUOTE.bounds;
    Zinc_expect_size_t_equal(bounds.loc.start_pos, 46, "loc start");
    Zinc_expect_size_t_equal(bounds.loc.end_pos, 62, "loc end");
    Zinc_expect_size_t_equal(bounds.loc.line, 5, "loc line");
    Zinc_expect_size_t_equal(bounds.loc.col, 1, "loc col");
    Zinc_expect_size_t_equal(bounds.end, 109, "end");

    Zinc_string_slice quoted_actual_slice = {
        .p = s + bounds.loc.start_pos,
        .size = bounds.end - bounds.loc.start_pos,
    };

    char* quoted_expected =
        "use lib::base::*\n"
        "TestSuite {\n"
        "  .solo = false\n"
        "  .mute = false\n"
        "}\n";

    Zinc_string_slice quoted_expected_slice = {
        .p = quoted_expected,
        .size = strlen(quoted_expected),
    };

    Zinc_expect_true(
        Zinc_string_slice_compare(
            &quoted_actual_slice,
            &quoted_expected_slice),
            "quoted");

    Lava_result_destroy(&pr);
}

void unit_parse_nested_header()
{
    Zinc_test_name(__func__);

    Lava_result pr = Lava_parse_str(
        "# Test Suite\n"
        "Define constant and use\n"
        "\n"
        "```cent\n"
        "use lib::base::*\n"
        "TestSuite {\n"
        "  .solo = false\n"
        "  .mute = false\n"
        "}\n"
        "```\n"
        "\n"
        "## Test\n"
        "Declare and use an integer\n"
        "\n"
        "```cent\n"
        "use lib::base::*\n"
        "Test {\n"
        "  .solo = false\n"
        "  .mute = false\n"
        "  .has_error = false\n"
        "}\n"
        "```\n"
        "\n"
        "```akela\n"
        "const a: Int32 = 105\n"
        "a\n"
        "```"
        "\n"
        "```cent\n"
        "use lib::base::*\n"
        "Ast::Stmts {\n"
        "  Ast::Const {\n"
        "    Ast::Id {\n"
        "      .value = \"a\"\n"
        "    }\n"
        "    Ast::Type {\n"
        "      .tu = Type_use {\n"
        "        .td = Int32\n"
        "      }\n"
        "    }\n"
        "    Ast::Number {\n"
        "      .value = \"105\"\n"
        "    }\n"
        "  }\n"
        "  Ast::Id {\n"
        "    .value = \"a\"\n"
        "  }\n"
        "}\n"
        "```\n"
        );

    Zinc_expect_no_errors(pr.errors);

    Zinc_expect_ptr(pr.root, "ptr root");
    Zinc_expect_int_equal(pr.root->kind, LAVA_DOM_HEADER, "kind root");
    Zinc_expect_int_equal(pr.root->data.LAVA_DOM_HEADER.level, 1, "level root");
    Zinc_expect_string(&pr.root->data.LAVA_DOM_HEADER.title, " Test Suite", "string root");
    Zinc_expect_size_t_equal(Lava_dom_count(pr.root), 3, "count root");

    Lava_dom* header_text = Lava_dom_get(pr.root, 0);
    Zinc_expect_ptr(header_text, "ptr header_text");
    Zinc_expect_int_equal(header_text->kind, LAVA_DOM_TEXT, "kind header_text");
    Zinc_expect_string(
        &header_text->data.LAVA_DOM_TEXT,
        "Define constant and use",
        "text header_text");

    Lava_dom* header_backquote = Lava_dom_get(pr.root, 1);
    Zinc_expect_ptr(header_backquote, "ptr header_backquote");
    Zinc_expect_int_equal(header_backquote->kind, LAVA_DOM_BACKQUOTE, "kind header_backquote");
    Zinc_expect_string(&header_backquote->data.LAVA_DOM_BACKQUOTE.format,
        "cent",
        "format header_backquote");
    Zinc_expect_string(&header_backquote->data.LAVA_DOM_BACKQUOTE.text,
        "use lib::base::*\n"
        "TestSuite {\n"
        "  .solo = false\n"
        "  .mute = false\n"
        "}\n",
        "text header_backquote");

    Lava_dom* test = Lava_dom_get(pr.root, 2);
    Zinc_expect_ptr(test, "ptr test");
    Zinc_expect_int_equal(test->kind, LAVA_DOM_HEADER, "kind test");
    Zinc_expect_int_equal(test->data.LAVA_DOM_HEADER.level, 2, "level test");
    Zinc_expect_string(&test->data.LAVA_DOM_HEADER.title, " Test", "string test");
    Zinc_expect_size_t_equal(Lava_dom_count(test), 4, "count test");

    Lava_dom* test_text = Lava_dom_get(test, 0);
    Zinc_expect_ptr(test_text, "ptr test");
    Zinc_expect_int_equal(test_text->kind, LAVA_DOM_TEXT, "kind test");
    Zinc_expect_string(
        &test_text->data.LAVA_DOM_TEXT,
        "Declare and use an integer",
        "text test");

    Lava_dom* test_test = Lava_dom_get(test, 1);
    Zinc_expect_ptr(test_test, "ptr test_test");
    Zinc_expect_int_equal(test_test->kind, LAVA_DOM_BACKQUOTE, "kind test_test");
    Zinc_expect_string(&test_test->data.LAVA_DOM_BACKQUOTE.format, "cent", "format test_test");
    Zinc_expect_string(&test_test->data.LAVA_DOM_BACKQUOTE.text,
        "use lib::base::*\n"
        "Test {\n"
        "  .solo = false\n"
        "  .mute = false\n"
        "  .has_error = false\n"
        "}\n",
        "");

    Lava_dom* test_source = Lava_dom_get(test, 2);
    Zinc_expect_ptr(test_source, "ptr test_source");
    Zinc_expect_int_equal(test_source->kind, LAVA_DOM_BACKQUOTE, "kind test_source");
    Zinc_expect_string(
        &test_source->data.LAVA_DOM_BACKQUOTE.format,
        "akela",
        "format test_source");
    Zinc_expect_string(
        &test_source->data.LAVA_DOM_BACKQUOTE.text,
        "const a: Int32 = 105\n"
        "a\n",
        "text test_source");

    Lava_dom* test_ast = Lava_dom_get(test, 3);
    Zinc_expect_ptr(test_ast, "ptr test_ast");
    Zinc_expect_int_equal(test_ast->kind, LAVA_DOM_BACKQUOTE, "kind test_ast");
    Zinc_expect_string(&test_ast->data.LAVA_DOM_BACKQUOTE.format, "cent", "format test_ast");
    Zinc_expect_string(
        &test_ast->data.LAVA_DOM_BACKQUOTE.text,
        "use lib::base::*\n"
        "Ast::Stmts {\n"
        "  Ast::Const {\n"
        "    Ast::Id {\n"
        "      .value = \"a\"\n"
        "    }\n"
        "    Ast::Type {\n"
        "      .tu = Type_use {\n"
        "        .td = Int32\n"
        "      }\n"
        "    }\n"
        "    Ast::Number {\n"
        "      .value = \"105\"\n"
        "    }\n"
        "  }\n"
        "  Ast::Id {\n"
        "    .value = \"a\"\n"
        "  }\n"
        "}\n",
        "text test_ast");

    Lava_result_destroy(&pr);
}

void unit_parse()
{
    unit_parse_header();
    unit_parse_header_text();
    unit_parse_header_text_backquote();
    unit_parse_nested_header();
}
