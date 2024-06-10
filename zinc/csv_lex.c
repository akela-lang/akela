#include <stdio.h>
#include <stdbool.h>
#include "buffer.h"
#include "vector.h"
#include "csv_lex.h"
#include <assert.h>
#include "memory.h"
#include "input_char.h"

#define CSV_CHUNK_SIZE 1024

void CSVLexDispatch(struct CSVLexData* lex_data, struct CSVToken* token);

void CSVTokenInit(struct CSVToken* token)
{
    token->type = CSVTokenTypeNone;
    VectorInit(&token->value, sizeof(char));
    location_init(&token->loc);
}

void CSVTokenCreate(struct CSVToken** token)
{
    malloc_safe((void**)token, sizeof(struct CSVToken));
    CSVTokenInit(*token);
}

void CSVTokenDestroy(struct CSVToken* token)
{
    VectorDestroy(&token->value);
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
    malloc_safe((void**)lex_data, sizeof(struct CSVLexData));
    CSVLexDataInit(*lex_data);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void CSVLexStart(struct CSVLexData* lex_data, struct CSVToken* token)
{
    char c;
    struct location loc;
    InputCharNext(lex_data->input_data, lex_data->input_vtable, &c, &loc);
    if (c == '"') {
        lex_data->state = CSVStateTypeFieldQuoted;
        token->loc = *InputCharLocation(lex_data->input_data, lex_data->input_vtable);
        token->loc.size++;
    } else {
        lex_data->state = CSVStateTypeField;
        InputCharRepeat(lex_data->input_data, lex_data->input_vtable);
    }
    token->type = CSVTokenTypeField;
    CSVLexDispatch(lex_data, token);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void CSVLexField(struct CSVLexData* lex_data, struct CSVToken* token) {
    bool done = false;
    char c;

    token->loc = *InputCharLocation(lex_data->input_data, lex_data->input_vtable);

    while (true) {
        struct location loc;
        done = InputCharNext(lex_data->input_data, lex_data->input_vtable, &c, &loc);
        if (done) {
            lex_data->state = CSVStateTypeEOF;
            break;
        } else if (c == ',') {
            lex_data->state = CSVStateTypeStart;
            break;
        } else if (c == '\n') {
            lex_data->state = CSVStateTypeEndOfRow;
            InputCharRepeat(lex_data->input_data, lex_data->input_vtable);
            break;
        } else if (c == '"') {
            loc.size = 1;
            error_list_set(lex_data->el, &loc, "quote found in unquoted field");
            /* test case: CSVLexErrorQuote */
        } else {
            VectorAdd(&token->value, &c, 1);
            token->loc.size++;
        }
    }
}

void CSVLexFieldQuoted(struct CSVLexData* lex_data, struct CSVToken* token)
{
    bool done = false;
    char c;
    struct location loc;

    while (true) {
        done = InputCharNext(lex_data->input_data, lex_data->input_vtable, &c, &loc);
        if (done) {
            error_list_set(lex_data->el, &loc,
                           "End of file found before end of quoted field");
            /* test case: CSVLexErrorExtraEOFBeforeQuote */
            lex_data->state = CSVStateTypeEOF;
            break;
        } else if (c == '"') {
            done = InputCharNext(lex_data->input_data, lex_data->input_vtable, &c, &loc);
            if (done) {
                token->loc.size++;
                lex_data->state = CSVStateTypeEOF;
                break;
            }
            if (c == '"') {
                token->loc.size++;
                VectorAdd(&token->value, &c, 1);
            } else {
                token->loc.size++;
                lex_data->state = CSVStateTypeFieldEndOfQuote;
                InputCharRepeat(lex_data->input_data, lex_data->input_vtable);
                break;
            }
        } else {
            token->loc.size++;
            VectorAdd(&token->value, &c, 1);
        }
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void CSVLexFieldEndOfQuote(struct CSVLexData* lex_data, struct CSVToken* token)
{
    bool done = false;
    char c;
    struct location loc;

    while (true) {
        done = InputCharNext(lex_data->input_data, lex_data->input_vtable, &c, &loc);
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
            InputCharRepeat(lex_data->input_data, lex_data->input_vtable);
            CSVLexDispatch(lex_data, token);
            break;
        } else {
            loc.size = 1;
            error_list_set(lex_data->el, &loc, "extra characters after field ending quote");
            /* test case: CSVLexErrorExtraCharactersAfterQuote */
        }
    }
}

void CSVLexEndOfRow(struct CSVLexData* lex_data, struct CSVToken* token)
{
    bool done = false;
    char c;
    struct location loc;

    InputCharNext(lex_data->input_data, lex_data->input_vtable, &c, &loc);
    if (c == '\n') {
        token->type = CSVTokenTypeNewline;
        done = InputCharNext(lex_data->input_data, lex_data->input_vtable, &c, &loc);
        InputCharRepeat(lex_data->input_data, lex_data->input_vtable);
        if (done) {
            lex_data->state = CSVStateTypeEOF;
        } else {
            lex_data->state = CSVStateTypeStart;
        }
    } else {
        assert(false);
    }
}

void CSVLexEOF(struct CSVToken* token)
{
    token->type = CSVTokenTypeEOF;
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
            CSVLexEOF(token);
            break;
        default:
            assert(false);
    }
}

void CSVLex(struct CSVLexData* lex_data, struct CSVToken** token)
{
    CSVTokenCreate(token);
    CSVLexDispatch(lex_data, *token);
}

enum result CSVLoad(const char* filename, Vector* text)
{
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if (!fp) {
        return set_error("Could not open file: %v", filename);
    }

    while (true) {
        VectorExpand(text, CSV_CHUNK_SIZE);
        size_t n = fread(VECTOR_PTR(text, text->count),
                         text->value_size, CSV_CHUNK_SIZE,
                         fp);
        text->count += n;
        if (n < CSV_CHUNK_SIZE) {
            break;
        }
    }
    VectorAddNull(text);
    return result_ok;
}
