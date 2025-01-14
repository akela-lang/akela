#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include <stdbool.h>
#include "dataframe/csv_lex.h"
#include "zinc/error.h"
#include "zinc/vector.h"
#include "zinc/input_char_string.h"

void CSVLexSetup(struct CSVLexData** lex_data, const char* text)
{
    Vector* input = NULL;
    VectorCreate(&input, sizeof(char));
    for (int i = 0; text[i]; i++) {
        VectorAdd(input, (char*)(text+i), 1);
    }
    VectorAddNull(input);
    InputCharString* input_data;
    InputCharStringCreate(&input_data, input);
    CSVLexDataCreate(lex_data);
    struct Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);
    (*lex_data)->el = el;
    (*lex_data)->input_data = input_data;
    (*lex_data)->input_vtable = input_data->input_vtable;
}

void CSVLexTeardown(struct CSVLexData* lex_data)
{
    Zinc_error_list_destroy(lex_data->el);
    free(lex_data->el);
    InputCharString* input_data = lex_data->input_data;
    VectorDestroy(input_data->text);
    free(input_data->text);
    free(input_data);
    free(lex_data);
}

void TestCSVLexEmptyField()
{
    test_name(__func__);

    struct CSVLexData* lex_data = NULL;
    CSVLexSetup(&lex_data, "\n");
    struct CSVToken* token = NULL;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 0");
    expect_int_equal(token->type, CSVTokenTypeField, "type 0");
    expect_vector_str(&token->value, "", "value 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 1");
    expect_int_equal(token->type, CSVTokenTypeNewline, "type 1");
    expect_vector_str(&token->value, "", "value 1");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 2");
    expect_int_equal(token->type, CSVTokenTypeEOF, "type 2");
    expect_vector_str(&token->value, "", "value 2");
    CSVTokenDestroy(token);
    free(token);

    CSVLexTeardown(lex_data);
}

void TestCSVLexEmptyFieldQuoted()
{
    test_name(__func__);

    struct CSVLexData* lex_data = NULL;
    CSVLexSetup(&lex_data, "\"\"\n");
    struct CSVToken* token = NULL;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 0");
    expect_int_equal(token->type, CSVTokenTypeField, "type 0");
    expect_vector_str(&token->value, "", "value 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 1");
    expect_int_equal(token->type, CSVTokenTypeNewline, "type 1");
    expect_vector_str(&token->value, "", "value 1");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 2");
    expect_int_equal(token->type, CSVTokenTypeEOF, "type 2");
    expect_vector_str(&token->value, "", "value 2");
    CSVTokenDestroy(token);
    free(token);

    CSVLexTeardown(lex_data);
}

void TestCSVLexOneField()
{
    test_name(__func__);

    struct CSVLexData* lex_data = NULL;
    CSVLexSetup(&lex_data, "25\n");
    struct CSVToken* token = NULL;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 0");
    expect_int_equal(token->type, CSVTokenTypeField, "type 0");
    expect_vector_str(&token->value, "25", "value 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 1");
    expect_int_equal(token->type, CSVTokenTypeNewline, "type 1");
    expect_vector_str(&token->value, "", "value 1");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 2");
    expect_int_equal(token->type, CSVTokenTypeEOF, "type 2");
    expect_vector_str(&token->value, "", "value 2");
    CSVTokenDestroy(token);
    free(token);

    CSVLexTeardown(lex_data);
}

void TestCSVLexOneFieldQuoted()
{
    test_name(__func__);

    struct CSVLexData* lex_data = NULL;
    CSVLexSetup(&lex_data, "\"25\"\n");
    struct CSVToken* token = NULL;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 0");
    expect_int_equal(token->type, CSVTokenTypeField, "type 0");
    expect_vector_str(&token->value, "25", "value 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 1");
    expect_int_equal(token->type, CSVTokenTypeNewline, "type 1");
    expect_vector_str(&token->value, "", "value 1");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 2");
    expect_int_equal(token->type, CSVTokenTypeEOF, "type 2");
    expect_vector_str(&token->value, "", "value 2");
    CSVTokenDestroy(token);
    free(token);

    CSVLexTeardown(lex_data);
}

void TestCSVLexTwoFields()
{
    test_name(__func__);

    struct CSVLexData* lex_data = NULL;
    CSVLexSetup(&lex_data, "24,11\n");
    struct CSVToken* token = NULL;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 0");
    expect_int_equal(token->type, CSVTokenTypeField, "type 0");
    expect_vector_str(&token->value, "24", "value 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 1");
    expect_int_equal(token->type, CSVTokenTypeField, "type 1");
    expect_vector_str(&token->value, "11", "value 1");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 2");
    expect_int_equal(token->type, CSVTokenTypeNewline, "type 2");
    expect_vector_str(&token->value, "", "value 2");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 3");
    expect_int_equal(token->type, CSVTokenTypeEOF, "type 3");
    expect_vector_str(&token->value, "", "value 3");
    CSVTokenDestroy(token);
    free(token);

    CSVLexTeardown(lex_data);
}

void TestCSVLexTwoFieldsQuoted()
{
    test_name(__func__);

    struct CSVLexData* lex_data = NULL;
    CSVLexSetup(&lex_data, "\"24\",\"11\"\n");
    struct CSVToken* token = NULL;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 0");
    expect_int_equal(token->type, CSVTokenTypeField, "type 0");
    expect_vector_str(&token->value, "24", "value 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 1");
    expect_int_equal(token->type, CSVTokenTypeField, "type 1");
    expect_vector_str(&token->value, "11", "value 1");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 2");
    expect_int_equal(token->type, CSVTokenTypeNewline, "type 2");
    expect_vector_str(&token->value, "", "value 2");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 3");
    expect_int_equal(token->type, CSVTokenTypeEOF, "type 3");
    expect_vector_str(&token->value, "", "value 3");
    CSVTokenDestroy(token);
    free(token);

    CSVLexTeardown(lex_data);
}

void TestCSVLexTwoRows()
{
    test_name(__func__);

    struct CSVLexData* lex_data = NULL;
    CSVLexSetup(&lex_data,
                "24,11\n"
                "44,100\n"
    );
    struct CSVToken* token = NULL;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token");
    expect_int_equal(token->type, CSVTokenTypeField, "type 0");
    expect_vector_str(&token->value, "24", "value 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token");
    expect_int_equal(token->type, CSVTokenTypeField, "type 1");
    expect_vector_str(&token->value, "11", "value 1");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token");
    expect_int_equal(token->type, CSVTokenTypeNewline, "type 2");
    expect_vector_str(&token->value, "", "value 2");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token");
    expect_int_equal(token->type, CSVTokenTypeField, "type 3");
    expect_vector_str(&token->value, "44", "value 3");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token");
    expect_int_equal(token->type, CSVTokenTypeField, "type 4");
    expect_vector_str(&token->value, "100", "value 4");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token");
    expect_int_equal(token->type, CSVTokenTypeNewline, "type 5");
    expect_vector_str(&token->value, "", "value 5");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token");
    expect_int_equal(token->type, CSVTokenTypeEOF, "type 6");
    expect_vector_str(&token->value, "", "value 6");
    CSVTokenDestroy(token);
    free(token);

    CSVLexTeardown(lex_data);
}

void TestCSVLexTwoRowsQuoted()
{
    test_name(__func__);

    struct CSVLexData* lex_data = NULL;
    CSVLexSetup(&lex_data,
                "\"24\",\"11\"\n"
                "\"44\",\"100\"\n"
    );
    struct CSVToken* token = NULL;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token");
    expect_int_equal(token->type, CSVTokenTypeField, "type 0");
    expect_vector_str(&token->value, "24", "value 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token");
    expect_int_equal(token->type, CSVTokenTypeField, "type 1");
    expect_vector_str(&token->value, "11", "value 1");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token");
    expect_int_equal(token->type, CSVTokenTypeNewline, "type 2");
    expect_vector_str(&token->value, "", "value 2");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token");
    expect_int_equal(token->type, CSVTokenTypeField, "type 3");
    expect_vector_str(&token->value, "44", "value 3");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token");
    expect_int_equal(token->type, CSVTokenTypeField, "type 4");
    expect_vector_str(&token->value, "100", "value 4");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token");
    expect_int_equal(token->type, CSVTokenTypeNewline, "type 5");
    expect_vector_str(&token->value, "", "value 5");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token");
    expect_int_equal(token->type, CSVTokenTypeEOF, "type 6");
    expect_vector_str(&token->value, "", "value 6");
    CSVTokenDestroy(token);
    free(token);

    CSVLexTeardown(lex_data);
}

void TestCSVLexErrorQuote()
{
    test_name(__func__);

    struct CSVLexData* lex_data = NULL;
    CSVLexSetup(&lex_data, "one\"two\n");
    struct CSVToken* token;

    CSVLex(lex_data, &token);
    expect_int_equal(token->type, CSVTokenTypeField, "type 0");

    CSVTokenDestroy(token);
    free(token);

    Zinc_expect_has_errors(lex_data->el);
    struct Zinc_error* e = Zinc_expect_source_error(lex_data->el, "quote found in unquoted field");
    if (e) {
        expect_size_t_equal(e->loc.start_pos, 3, "byte_pos");
        expect_size_t_equal(e->loc.end_pos, 4, "end_pos");
        expect_size_t_equal(e->loc.line, 1, "line");
        expect_size_t_equal(e->loc.col, 4, "col");
    }

    CSVLexTeardown(lex_data);
}

void TestCSVLexErrorExtraCharactersAfterQuote()
{
    test_name(__func__);

    struct CSVLexData* lex_data = NULL;
    CSVLexSetup(&lex_data, "\"one\"hello\n");
    struct CSVToken* token;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 0");
    expect_int_equal(token->type, CSVTokenTypeField, "type 0");
    expect_vector_str(&token->value, "one", "vector 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_has_errors(lex_data->el);
    assert_ptr(token, "ptr token 1");
    expect_int_equal(token->type, CSVTokenTypeNewline, "type 1");
    expect_vector_str(&token->value, "", "vector 1");
    CSVTokenDestroy(token);
    free(token);
    struct Zinc_error* e = Zinc_expect_source_error(lex_data->el,
                                          "extra characters after field ending quote");
    if (e) {
        expect_size_t_equal(e->loc.start_pos, 5, "byte_pos");
        expect_size_t_equal(e->loc.end_pos, 6, "size");
        expect_size_t_equal(e->loc.line, 1, "line");
        expect_size_t_equal(e->loc.col, 6, "col");
    }

    CSVLexTeardown(lex_data);
}

void TestCSVLexErrorEOFBeforeQuote()
{
    test_name(__func__);

    struct CSVLexData* lex_data = NULL;
    CSVLexSetup(&lex_data, "\"one");
    struct CSVToken* token;

    CSVLex(lex_data, &token);
    Zinc_expect_has_errors(lex_data->el);
    assert_ptr(token, "ptr token 0");
    expect_int_equal(token->type, CSVTokenTypeField, "type 0");
    expect_vector_str(&token->value, "one", "vector 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_has_errors(lex_data->el);
    assert_ptr(token, "ptr token 1");
    expect_int_equal(token->type, CSVTokenTypeEOF, "type 1");
    expect_vector_str(&token->value, "", "vector 1");
    CSVTokenDestroy(token);
    free(token);

    struct Zinc_error* e = Zinc_expect_source_error(lex_data->el,
                                          "End of file found before end of quoted field");
    if (e) {
        expect_size_t_equal(e->loc.start_pos, 4, "byte_pos");
        expect_size_t_equal(e->loc.end_pos, 5, "size");
        expect_size_t_equal(e->loc.line, 1, "line");
        expect_size_t_equal(e->loc.col, 5, "col");
    }

    CSVLexTeardown(lex_data);
}

void TestCSVLexQuoteNewline()
{
    test_name(__func__);

    struct CSVLexData* lex_data = NULL;
    CSVLexSetup(&lex_data, "\"one\ntwo\"\n");
    struct CSVToken* token;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 0");
    expect_int_equal(token->type, CSVTokenTypeField, "type 0");
    expect_vector_str(&token->value, "one\ntwo", "vector 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 1");
    expect_int_equal(token->type, CSVTokenTypeNewline, "type 1");
    expect_vector_str(&token->value, "", "vector 1");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 2");
    expect_int_equal(token->type, CSVTokenTypeEOF, "type 2");
    expect_vector_str(&token->value, "", "vector 2");
    CSVTokenDestroy(token);
    free(token);

    CSVLexTeardown(lex_data);
}

void TestCSVLexQuoteComma()
{
    test_name(__func__);

    struct CSVLexData* lex_data = NULL;
    CSVLexSetup(&lex_data, "\"one,two\"\n");
    struct CSVToken* token;

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 0");
    expect_int_equal(token->type, CSVTokenTypeField, "type 0");
    expect_vector_str(&token->value, "one,two", "vector 0");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 1");
    expect_int_equal(token->type, CSVTokenTypeNewline, "type 1");
    expect_vector_str(&token->value, "", "vector 1");
    CSVTokenDestroy(token);
    free(token);

    CSVLex(lex_data, &token);
    Zinc_expect_no_errors(lex_data->el);
    assert_ptr(token, "ptr token 2");
    expect_int_equal(token->type, CSVTokenTypeEOF, "type 2");
    expect_vector_str(&token->value, "", "vector 2");
    CSVTokenDestroy(token);
    free(token);

    CSVLexTeardown(lex_data);
}

void TestCSVLex()
{
    TestCSVLexEmptyField();
    TestCSVLexEmptyFieldQuoted();
    TestCSVLexOneField();
    TestCSVLexOneFieldQuoted();
    TestCSVLexTwoFields();
    TestCSVLexTwoFieldsQuoted();
    TestCSVLexTwoRows();
    TestCSVLexTwoRowsQuoted();
    TestCSVLexErrorQuote();
    TestCSVLexErrorExtraCharactersAfterQuote();
    TestCSVLexErrorEOFBeforeQuote();
    TestCSVLexQuoteNewline();
    TestCSVLexQuoteComma();
}