#include <stdio.h>
#include <stdbool.h>
#include "zinc/zstring.h"
#include "zinc/vector.h"
#include "csv_lex.h"
#include <assert.h>
#include "zinc/memory.h"
#include "zinc/input_char.h"

#define CSV_CHUNK_SIZE 1024

void CSVLexDispatch(struct CSVLexData* lex_data, struct CSVToken* token);

void CSVTokenInit(struct CSVToken* token)
{
    token->type = CSVTokenTypeNone;
    Zinc_vector_init(&token->value, sizeof(char));
    Zinc_location_init(&token->loc);
}

void CSVTokenCreate(struct CSVToken** token)
{
    Zinc_malloc_safe((void**)token, sizeof(struct CSVToken));
    CSVTokenInit(*token);
}

void CSVTokenDestroy(struct CSVToken* token)
{
    Zinc_vector_destroy(&token->value);
}

void CSVLexDataInit(struct CSVLexData* lex_data)
{
    lex_data->el = NULL;
    lex_data->state = CSVStateTypeStart;
    lex_data->input_data = NULL;
    lex_data->input_vtable = NULL;
}

void CSVLexDataCreate(struct CSVLexData** lex_data)
{
    Zinc_malloc_safe((void**)lex_data, sizeof(struct CSVLexData));
    CSVLexDataInit(*lex_data);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void CSVLexStart(struct CSVLexData* lex_data, struct CSVToken* token)
{
    char c;
    struct Zinc_location loc;
    Zinc_input_char_next(lex_data->input_data, lex_data->input_vtable, &c, &loc);
    if (c == '"') {
        lex_data->state = CSVStateTypeFieldQuoted;
        token->loc = *Zinc_input_char_location(lex_data->input_data, lex_data->input_vtable);
    } else {
        lex_data->state = CSVStateTypeField;
        Zinc_input_char_repeat(lex_data->input_data, lex_data->input_vtable);
    }
    token->type = CSVTokenTypeField;
    CSVLexDispatch(lex_data, token);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void CSVLexField(struct CSVLexData* lex_data, struct CSVToken* token) {
    bool done = false;
    char c;

    token->loc = *Zinc_input_char_location(lex_data->input_data, lex_data->input_vtable);

    while (true) {
        struct Zinc_location loc;
        done = Zinc_input_char_next(lex_data->input_data, lex_data->input_vtable, &c, &loc);
        if (done) {
            lex_data->state = CSVStateTypeEOF;
            break;
        } else if (c == ',') {
            lex_data->state = CSVStateTypeStart;
            break;
        } else if (c == '\n') {
            lex_data->state = CSVStateTypeEndOfRow;
            Zinc_input_char_repeat(lex_data->input_data, lex_data->input_vtable);
            break;
        } else if (c == '"') {
            Zinc_error_list_set(lex_data->el, &loc, "quote found in unquoted field");
            /* test case: CSVLexErrorQuote */
        } else {
            Zinc_vector_add(&token->value, &c, 1);
        }
    }

    struct Zinc_location end = *Zinc_input_char_location(lex_data->input_data, lex_data->input_vtable);
    token->loc.end = end.start;
}

void CSVLexFieldQuoted(struct CSVLexData* lex_data, struct CSVToken* token)
{
    bool done = false;
    char c;
    struct Zinc_location loc;

    while (true) {
        done = Zinc_input_char_next(lex_data->input_data, lex_data->input_vtable, &c, &loc);
        if (done) {
            Zinc_error_list_set(lex_data->el, &loc,
                           "End of file found before end of quoted field");
            /* test case: CSVLexErrorExtraEOFBeforeQuote */
            lex_data->state = CSVStateTypeEOF;
            break;
        } else if (c == '"') {
            done = Zinc_input_char_next(lex_data->input_data, lex_data->input_vtable, &c, &loc);
            if (done) {
                lex_data->state = CSVStateTypeEOF;
                break;
            }
            if (c == '"') {
                Zinc_vector_add(&token->value, &c, 1);
            } else {
                lex_data->state = CSVStateTypeFieldEndOfQuote;
                Zinc_input_char_repeat(lex_data->input_data, lex_data->input_vtable);
                break;
            }
        } else {
            Zinc_vector_add(&token->value, &c, 1);
        }
    }

    struct Zinc_location end = *Zinc_input_char_location(lex_data->input_data, lex_data->input_vtable);
    token->loc.end = end.start;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void CSVLexFieldEndOfQuote(struct CSVLexData* lex_data, struct CSVToken* token)
{
    bool done = false;
    char c;
    struct Zinc_location loc;

    while (true) {
        done = Zinc_input_char_next(lex_data->input_data, lex_data->input_vtable, &c, &loc);
        if (done) {
            token->type = CSVTokenTypeEOF;
            lex_data->state = CSVStateTypeEOF;
            break;
        } else if (c == ',') {
            lex_data->state = CSVStateTypeStart;
            CSVLexDispatch(lex_data, token);
            break;
        } else if (c == '\n') {
            lex_data->state = CSVStateTypeEndOfRow;
            Zinc_input_char_repeat(lex_data->input_data, lex_data->input_vtable);
            CSVLexDispatch(lex_data, token);
            break;
        } else {
            Zinc_error_list_set(lex_data->el, &loc, "extra characters after field ending quote");
            /* test case: CSVLexErrorExtraCharactersAfterQuote */
        }
    }

    struct Zinc_location end = *Zinc_input_char_location(lex_data->input_data, lex_data->input_vtable);
    token->loc.end = end.start;
}

void CSVLexEndOfRow(struct CSVLexData* lex_data, struct CSVToken* token)
{
    bool done = false;
    char c;
    struct Zinc_location loc;

    Zinc_input_char_next(lex_data->input_data, lex_data->input_vtable, &c, &loc);
    if (c == '\n') {
        token->type = CSVTokenTypeNewline;
        done = Zinc_input_char_next(lex_data->input_data, lex_data->input_vtable, &c, &loc);
        Zinc_input_char_repeat(lex_data->input_data, lex_data->input_vtable);
        if (done) {
            lex_data->state = CSVStateTypeEOF;
        } else {
            lex_data->state = CSVStateTypeStart;
        }
    } else {
        assert(false);
    }

    struct Zinc_location end = *Zinc_input_char_location(lex_data->input_data, lex_data->input_vtable);
    token->loc.end = end.start;
}

void CSVLexEOF(struct CSVLexData* lex_data, struct CSVToken* token)
{
    token->type = CSVTokenTypeEOF;
    struct Zinc_location end = *Zinc_input_char_location(lex_data->input_data, lex_data->input_vtable);
    token->loc.end = end.start;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void CSVLexDispatch(struct CSVLexData* lex_data, struct CSVToken* token)
{
    switch (lex_data->state) {
        case CSVStateTypeStart:
            CSVLexStart(lex_data, token);
            break;
        case CSVStateTypeField:
            CSVLexField(lex_data, token);
            break;
        case CSVStateTypeFieldQuoted:
            CSVLexFieldQuoted(lex_data, token);
            break;
        case CSVStateTypeFieldEndOfQuote:
            CSVLexFieldEndOfQuote(lex_data, token);
            break;
        case CSVStateTypeEndOfRow:
            CSVLexEndOfRow(lex_data, token);
            break;
        case CSVStateTypeEOF:
            CSVLexEOF(lex_data, token);
            break;
        default:
            assert(false);
    }
}

void CSVLex(struct CSVLexData* lex_data, struct CSVToken** token)
{
    CSVTokenCreate(token);
    CSVLexDispatch(lex_data, *token);
    struct Zinc_location end = *Zinc_input_char_location(lex_data->input_data, lex_data->input_vtable);
    (*token)->loc.end = end.start;
}

enum Zinc_result CSVLoad(const char* filename, Zinc_vector* text)
{
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if (!fp) {
        return Zinc_set_error("Could not open file: %v", filename);
    }

    while (true) {
        Zinc_vector_expand(text, CSV_CHUNK_SIZE);
        size_t n = fread(ZINC_VECTOR_PTR(text, text->count),
                         text->value_size, CSV_CHUNK_SIZE,
                         fp);
        text->count += n;
        if (n < CSV_CHUNK_SIZE) {
            break;
        }
    }
    Zinc_vector_add_null(text);
    return Zinc_result_ok;
}
