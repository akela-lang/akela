#include <string.h>
#include <zinc/error_unit_test.h>
#include <zinc/input_unicode_string.h>

#include "zinc/vector.h"
#include "zinc/unit_test.h"
#include "lava/lex_data.h"
#include "lava/lex.h"

Lava_lex_data* unit_setup(char* s)
{
    Zinc_error_list* errors = NULL;
    Zinc_error_list_create(&errors);

    Zinc_vector* text = NULL;
    Zinc_vector_create(&text, sizeof(char));
    Zinc_vector_add(text, s, strlen(s));

    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, text);

    Lava_lex_data* data = NULL;
    Lava_lex_data_create(&data, errors, input, input->vtable);

    return data;
}

void unit_teardown(Lava_lex_data* ld)
{
    Zinc_error_list* errors = ld->errors;
    Zinc_error_list_destroy(errors);
    free(errors);
    Zinc_input_unicode_string* input = ld->input;
    Zinc_vector* text = input->text;
    Zinc_vector_destroy(text);
    free(text);
    free(input);
    free(ld);
}

void unit_lex1()
{
    Zinc_test_name(__func__);

    Lava_lex_data* ld = unit_setup(
        "# Test Suite\n"
        "Declare constant and use\n"
        "```\n"
        "use lib::base::*\n"
        "TestSuite {\n"
        "  .solo = false\n"
        "  .mute = false\n"
        "  .headers = headers\n"
        "}\n"
        "```\n"
    );

    // line 1
    Lava_token* t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_header, "kind line 1 token 1");
    Zinc_expect_string(&t->text, "#", "string line 1 token 1");
    Lava_token_destroy(t);
    free(t);

    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_text, "kind line 1 token 2");
    Zinc_expect_string(&t->text, " Test Suite", "string line 1 token 2");
    Lava_token_destroy(t);
    free(t);

    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_newline, "kind line 1 token 3");
    Lava_token_destroy(t);
    free(t);

    // line 2
    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_text, "kind line 2 token 1");
    Zinc_expect_string(&t->text, "Declare constant and use", "string line 2 token 1");
    Lava_token_destroy(t);
    free(t);

    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_newline, "kind line 2 token 2");
    Lava_token_destroy(t);
    free(t);

    // line 3
    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_backquote, "kind line 3 token 1");
    Lava_token_destroy(t);
    free(t);

    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_newline, "kind line 3 token 2");
    Lava_token_destroy(t);
    free(t);

    // line 4
    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_text, "kind line 4 token 1");
    Zinc_expect_string(&t->text, "use lib::base::*", "string line 4 token 1");

    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_newline, "kind line 4 token 2");
    Lava_token_destroy(t);
    free(t);

    // line 5
    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_text, "kind line 5 token 1");
    Zinc_expect_string(&t->text, "TestSuite {", "string line 5 token 1");

    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_newline, "kind line 5 token 2");
    Lava_token_destroy(t);
    free(t);

    // line 6
    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_text, "kind line 6 token 1");
    Zinc_expect_string(&t->text, "  .solo = false", "string line 6 token 1");

    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_newline, "kind line 6 token 2");
    Lava_token_destroy(t);
    free(t);

    // line 7
    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_text, "kind line 7 token 1");
    Zinc_expect_string(&t->text, "  .mute = false", "string line 7 token 1");

    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_newline, "kind line 7 token 2");
    Lava_token_destroy(t);
    free(t);

    // line 8
    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_text, "kind line 8 token 1");
    Zinc_expect_string(&t->text, "  .headers = headers", "string line 8 token 1");

    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_newline, "kind line 8 token 2");
    Lava_token_destroy(t);
    free(t);

    // line 9
    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_text, "kind line 9 token 1");
    Zinc_expect_string(&t->text, "}", "string line 9 token 1");

    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_newline, "kind line 9 token 2");
    Lava_token_destroy(t);
    free(t);

    // line 10
    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_backquote, "kind line 10 token 1");
    Lava_token_destroy(t);
    free(t);

    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_newline, "kind line 10 token 2");
    Lava_token_destroy(t);
    free(t);

    // eof
    t = Lava_lex(ld);
    Zinc_expect_int_equal(t->kind, Lava_token_kind_eof, "kind eof");
    Lava_token_destroy(t);
    free(t);

    Zinc_expect_no_errors(ld->errors);
    unit_teardown(ld);
}

void unit_lex()
{
    unit_lex1();
}