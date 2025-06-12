#include <stdbool.h>
#include "dataframe/csv_lex.h"
#include "zinc/error.h"
#include "zinc/vector.h"
#include "zinc/input_char_string.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void DfUnit_CSVLexSetup(struct CSVLexData** lex_data, const char* text)
{
    Zinc_vector* input = NULL;
    Zinc_vector_create(&input, sizeof(char));
    for (int i = 0; text[i]; i++) {
        Zinc_vector_add(input, (char*)(text+i), 1);
    }
    Zinc_vector_add_null(input);
    Zinc_input_char_string* input_data;
    Zinc_input_char_string_create(&input_data, input);
    CSVLexDataCreate(lex_data);
    struct Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);
    (*lex_data)->el = el;
    (*lex_data)->input_data = input_data;
    (*lex_data)->input_vtable = input_data->input_vtable;
}

void DfUnit_CSVLexTeardown(struct CSVLexData* lex_data)
{
    Zinc_error_list_destroy(lex_data->el);
    free(lex_data->el);
    Zinc_input_char_string* input_data = lex_data->input_data;
    Zinc_vector_destroy(input_data->text);
    free(input_data->text);
    free(input_data);
    free(lex_data);
}

void DfUnit_CSVLexEmptyField(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct CSVLexData* lex_data = NULL;
    DfUnit_CSVLexSetup(&lex_data, "\n");
    struct CSVToken* token = NULL;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 0");
    Zinc_expect_vector_str(test, &token->value, "", "value 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeNewline, "type 1");
    Zinc_expect_vector_str(test, &token->value, "", "value 1");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeEOF, "type 2");
    Zinc_expect_vector_str(test, &token->value, "", "value 2");
    CSVTokenDestroy(token);
    free(token);

    DfUnit_CSVLexTeardown(lex_data);
}

void DfUnit_CSVLexEmptyFieldQuoted(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct CSVLexData* lex_data = NULL;
    DfUnit_CSVLexSetup(&lex_data, "\"\"\n");
    struct CSVToken* token = NULL;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 0");
    Zinc_expect_vector_str(test, &token->value, "", "value 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeNewline, "type 1");
    Zinc_expect_vector_str(test, &token->value, "", "value 1");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeEOF, "type 2");
    Zinc_expect_vector_str(test, &token->value, "", "value 2");
    CSVTokenDestroy(token);
    free(token);

    DfUnit_CSVLexTeardown(lex_data);
}

void DfUnit_CSVLexOneField(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct CSVLexData* lex_data = NULL;
    DfUnit_CSVLexSetup(&lex_data, "25\n");
    struct CSVToken* token = NULL;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 0");
    Zinc_expect_vector_str(test, &token->value, "25", "value 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeNewline, "type 1");
    Zinc_expect_vector_str(test, &token->value, "", "value 1");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeEOF, "type 2");
    Zinc_expect_vector_str(test, &token->value, "", "value 2");
    CSVTokenDestroy(token);
    free(token);

    DfUnit_CSVLexTeardown(lex_data);
}

void DfUnit_CSVLexOneFieldQuoted(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct CSVLexData* lex_data = NULL;
    DfUnit_CSVLexSetup(&lex_data, "\"25\"\n");
    struct CSVToken* token = NULL;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 0");
    Zinc_expect_vector_str(test, &token->value, "25", "value 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeNewline, "type 1");
    Zinc_expect_vector_str(test, &token->value, "", "value 1");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeEOF, "type 2");
    Zinc_expect_vector_str(test, &token->value, "", "value 2");
    CSVTokenDestroy(token);
    free(token);

    DfUnit_CSVLexTeardown(lex_data);
}

void DfUnit_CSVLexTwoFields(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct CSVLexData* lex_data = NULL;
    DfUnit_CSVLexSetup(&lex_data, "24,11\n");
    struct CSVToken* token = NULL;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 0");
    Zinc_expect_vector_str(test, &token->value, "24", "value 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 1");
    Zinc_expect_vector_str(test, &token->value, "11", "value 1");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeNewline, "type 2");
    Zinc_expect_vector_str(test, &token->value, "", "value 2");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 3")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeEOF, "type 3");
    Zinc_expect_vector_str(test, &token->value, "", "value 3");
    CSVTokenDestroy(token);
    free(token);

    DfUnit_CSVLexTeardown(lex_data);
}

void DfUnit_CSVLexTwoFieldsQuoted(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct CSVLexData* lex_data = NULL;
    DfUnit_CSVLexSetup(&lex_data, "\"24\",\"11\"\n");
    struct CSVToken* token = NULL;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 0");
    Zinc_expect_vector_str(test, &token->value, "24", "value 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 1");
    Zinc_expect_vector_str(test, &token->value, "11", "value 1");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeNewline, "type 2");
    Zinc_expect_vector_str(test, &token->value, "", "value 2");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 3")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeEOF, "type 3");
    Zinc_expect_vector_str(test, &token->value, "", "value 3");
    CSVTokenDestroy(token);
    free(token);

    DfUnit_CSVLexTeardown(lex_data);
}

void DfUnit_CSVLexTwoRows(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct CSVLexData* lex_data = NULL;
    DfUnit_CSVLexSetup(&lex_data,
                "24,11\n"
                "44,100\n"
    );
    struct CSVToken* token = NULL;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 0");
    Zinc_expect_vector_str(test, &token->value, "24", "value 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 1");
    Zinc_expect_vector_str(test, &token->value, "11", "value 1");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeNewline, "type 2");
    Zinc_expect_vector_str(test, &token->value, "", "value 2");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 3");
    Zinc_expect_vector_str(test, &token->value, "44", "value 3");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 4");
    Zinc_expect_vector_str(test, &token->value, "100", "value 4");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeNewline, "type 5");
    Zinc_expect_vector_str(test, &token->value, "", "value 5");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeEOF, "type 6");
    Zinc_expect_vector_str(test, &token->value, "", "value 6");
    CSVTokenDestroy(token);
    free(token);

    DfUnit_CSVLexTeardown(lex_data);
}

void DfUnit_CSVLexTwoRowsQuoted(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct CSVLexData* lex_data = NULL;
    DfUnit_CSVLexSetup(&lex_data,
                "\"24\",\"11\"\n"
                "\"44\",\"100\"\n"
    );
    struct CSVToken* token = NULL;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 0");
    Zinc_expect_vector_str(test, &token->value, "24", "value 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 1");
    Zinc_expect_vector_str(test, &token->value, "11", "value 1");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeNewline, "type 2");
    Zinc_expect_vector_str(test, &token->value, "", "value 2");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 3");
    Zinc_expect_vector_str(test, &token->value, "44", "value 3");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 4");
    Zinc_expect_vector_str(test, &token->value, "100", "value 4");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeNewline, "type 5");
    Zinc_expect_vector_str(test, &token->value, "", "value 5");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeEOF, "type 6");
    Zinc_expect_vector_str(test, &token->value, "", "value 6");
    CSVTokenDestroy(token);
    free(token);

    DfUnit_CSVLexTeardown(lex_data);
}

void DfUnit_CSVLexErrorQuote(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct CSVLexData* lex_data = NULL;
    DfUnit_CSVLexSetup(&lex_data, "one\"two\n");
    struct CSVToken* token;

    CSVLex(lex_data, &token);
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 0");

    CSVTokenDestroy(token);
    free(token);

    Zinc_expect_has_errors(test, lex_data->el);
    struct Zinc_error* e = Zinc_expect_source_error(test, lex_data->el, "quote found in unquoted field");
    if (e) {
        Zinc_expect_size_t_equal(test, e->loc.start, 3, "byte_pos");
        Zinc_expect_size_t_equal(test, e->loc.end, 4, "end_pos");
        Zinc_expect_size_t_equal(test, e->loc.line, 1, "line");
        Zinc_expect_size_t_equal(test, e->loc.col, 4, "col");
    }

    DfUnit_CSVLexTeardown(lex_data);
}

void DfUnit_CSVLexErrorExtraCharactersAfterQuote(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct CSVLexData* lex_data = NULL;
    DfUnit_CSVLexSetup(&lex_data, "\"one\"hello\n");
    struct CSVToken* token;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 0");
    Zinc_expect_vector_str(test, &token->value, "one", "vector 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_has_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeNewline, "type 1");
    Zinc_expect_vector_str(test, &token->value, "", "vector 1");
    CSVTokenDestroy(token);
    free(token);
    struct Zinc_error* e = Zinc_expect_source_error(test, lex_data->el,
                                          "extra characters after field ending quote");
    if (e) {
        Zinc_expect_size_t_equal(test, e->loc.start, 5, "byte_pos");
        Zinc_expect_size_t_equal(test, e->loc.end, 6, "size");
        Zinc_expect_size_t_equal(test, e->loc.line, 1, "line");
        Zinc_expect_size_t_equal(test, e->loc.col, 6, "col");
    }

    DfUnit_CSVLexTeardown(lex_data);
}

void DfUnit_CSVLexErrorEOFBeforeQuote(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct CSVLexData* lex_data = NULL;
    DfUnit_CSVLexSetup(&lex_data, "\"one");
    struct CSVToken* token;

    CSVLex(lex_data, &token);
    Zinc_expect_has_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 0");
    Zinc_expect_vector_str(test, &token->value, "one", "vector 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_has_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeEOF, "type 1");
    Zinc_expect_vector_str(test, &token->value, "", "vector 1");
    CSVTokenDestroy(token);
    free(token);

    struct Zinc_error* e = Zinc_expect_source_error(test, lex_data->el,
                                          "End of file found before end of quoted field");
    if (e) {
        Zinc_expect_size_t_equal(test, e->loc.start, 4, "byte_pos");
        Zinc_expect_size_t_equal(test, e->loc.end, 5, "size");
        Zinc_expect_size_t_equal(test, e->loc.line, 1, "line");
        Zinc_expect_size_t_equal(test, e->loc.col, 5, "col");
    }

    DfUnit_CSVLexTeardown(lex_data);
}

void DfUnit_CSVLexQuoteNewline(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct CSVLexData* lex_data = NULL;
    DfUnit_CSVLexSetup(&lex_data, "\"one\ntwo\"\n");
    struct CSVToken* token;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 0");
    Zinc_expect_vector_str(test, &token->value, "one\ntwo", "vector 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeNewline, "type 1");
    Zinc_expect_vector_str(test, &token->value, "", "vector 1");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeEOF, "type 2");
    Zinc_expect_vector_str(test, &token->value, "", "vector 2");
    CSVTokenDestroy(token);
    free(token);

    DfUnit_CSVLexTeardown(lex_data);
}

void DfUnit_CSVLexQuoteComma(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct CSVLexData* lex_data = NULL;
    DfUnit_CSVLexSetup(&lex_data, "\"one,two\"\n");
    struct CSVToken* token;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeField, "type 0");
    Zinc_expect_vector_str(test, &token->value, "one,two", "vector 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeNewline, "type 1");
    Zinc_expect_vector_str(test, &token->value, "", "vector 1");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(test, lex_data->el);
    if (!Zinc_expect_ptr(test, token, "ptr token 2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, token->type, CSVTokenTypeEOF, "type 2");
    Zinc_expect_vector_str(test, &token->value, "", "vector 2");
    CSVTokenDestroy(token);
    free(token);

    DfUnit_CSVLexTeardown(lex_data);
}

void DfUnit_CSVLex(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, DfUnit_CSVLexEmptyField);
        Zinc_test_register(test, DfUnit_CSVLexEmptyFieldQuoted);
        Zinc_test_register(test, DfUnit_CSVLexOneField);
        Zinc_test_register(test, DfUnit_CSVLexOneFieldQuoted);
        Zinc_test_register(test, DfUnit_CSVLexTwoFields);
        Zinc_test_register(test, DfUnit_CSVLexTwoFieldsQuoted);
        Zinc_test_register(test, DfUnit_CSVLexTwoRows);
        Zinc_test_register(test, DfUnit_CSVLexTwoRowsQuoted);
        Zinc_test_register(test, DfUnit_CSVLexErrorQuote);
        Zinc_test_register(test, DfUnit_CSVLexErrorExtraCharactersAfterQuote);
        Zinc_test_register(test, DfUnit_CSVLexErrorEOFBeforeQuote);
        Zinc_test_register(test, DfUnit_CSVLexQuoteNewline);
        Zinc_test_register(test, DfUnit_CSVLexQuoteComma);
        return;
    }

    Zinc_test_perform(test);
}