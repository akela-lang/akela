#include "field.h"
#include <stdbool.h>
#include "zinc/error.h"
#include "field_lex.h"
#include <assert.h>
#include <ctype.h>
#include "zinc/memory.h"
#include "zinc/input_char.h"

void FieldLexStart(struct FieldLexData* lex_data, enum FieldType* type);
void FieldLexSign(struct FieldLexData* lex_data, enum FieldType* type);
void FieldLexInteger(struct FieldLexData* lex_data, enum FieldType* type);
void FieldLexPoint(struct FieldLexData* lex_data, enum FieldType* type);
void FieldLexFraction(struct FieldLexData* lex_data, enum FieldType* type);
void FieldLexExponentE(struct FieldLexData* lex_data, enum FieldType* type);
void FieldLexExponentSign(struct FieldLexData* lex_data, enum FieldType* type);
void FieldLexExponent(struct FieldLexData* lex_data, enum FieldType* type);
void FieldLexString(struct FieldLexData* lex_data, enum FieldType* type);

void FieldLexDataInit(struct FieldLexData* lex_data)
{
    lex_data->has_digit = false;
    lex_data->has_sign = false;
    lex_data->input_data = NULL;
    lex_data->input_vtable = NULL;
}

void FieldLexDataCreate(struct FieldLexData** lex_data)
{
    Zinc_malloc_safe((void**)lex_data, sizeof(struct FieldLexData));
    FieldLexDataInit(*lex_data);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void FieldLexStart(struct FieldLexData* lex_data, enum FieldType* type)
{
    char c;
    struct Zinc_location loc;
    bool done;

    done = Zinc_input_char_next(lex_data->input_data, lex_data->input_vtable, &c, &loc);
    if (done) {
        *type = FieldTypeEmpty;
    } else if (c == '+' || c == '-') {
        lex_data->has_sign = true;
        FieldLexSign(lex_data, type);
    } else if (isdigit(c)) {
        lex_data->has_digit = true;
        FieldLexInteger(lex_data, type);
    } else if (c == '.') {
        FieldLexPoint(lex_data, type);
    } else {
        FieldLexString(lex_data, type);
    }
}

void FieldLexSign(struct FieldLexData* lex_data, enum FieldType* type)
{
    char c;
    struct Zinc_location loc;
    bool done;

    done = Zinc_input_char_next(lex_data->input_data, lex_data->input_vtable, &c, &loc);
    if (done) {
        *type = FieldTypeString;
    } else if (isdigit(c)) {
        lex_data->has_digit = true;
        FieldLexInteger(lex_data, type);
    } else if (c == '.') {
        FieldLexPoint(lex_data, type);
    } else {
        FieldLexString(lex_data, type);
    }
}

void FieldLexInteger(struct FieldLexData* lex_data, enum FieldType* type)
{
    char c;
    struct Zinc_location loc;
    bool done;

    while (true) {
        done = Zinc_input_char_next(lex_data->input_data, lex_data->input_vtable, &c, &loc);
        if (done) {
            if (lex_data->has_sign) {
                *type = FieldTypeInt;
            } else {
                *type = FieldTypeIntU;
            }
            break;
        } else if (isdigit(c)) {
            lex_data->has_digit = true;
        } else if (c == '.') {
            FieldLexPoint(lex_data, type);
            break;
        } else if (c == 'e' || c == 'E') {
            FieldLexExponentE(lex_data, type);
            break;
        } else {
            *type = FieldTypeString;
            break;
        }
    }
}

void FieldLexPoint(struct FieldLexData* lex_data, enum FieldType* type)
{
    char c;
    struct Zinc_location loc;
    bool done;

    done = Zinc_input_char_next(lex_data->input_data, lex_data->input_vtable, &c, &loc);

    if (done) {
        if (lex_data->has_digit) {
            *type = FieldTypeFloat;
        } else {
            *type = FieldTypeString;
        }
    } else if (isdigit(c)) {
        lex_data->has_digit = true;
        FieldLexFraction(lex_data, type);
    } else if (c == 'e' || c == 'E') {
        FieldLexExponentE(lex_data, type);
    } else {
        *type = FieldTypeString;
    }
}

void FieldLexFraction(struct FieldLexData* lex_data, enum FieldType* type)
{
    char c;
    struct Zinc_location loc;
    bool done;

    while (true) {
        done = Zinc_input_char_next(lex_data->input_data, lex_data->input_vtable, &c, &loc);

        if (done) {
            *type = FieldTypeFloat;
            break;
        } else if (isdigit(c)) {
            lex_data->has_digit = true;
            continue;
        } else if (c == 'e' || c == 'E') {
            FieldLexExponentE(lex_data, type);
            break;
        } else {
            *type = FieldTypeString;
            break;
        }
    }
}

void FieldLexExponentE(struct FieldLexData* lex_data, enum FieldType* type)
{
    char c;
    struct Zinc_location loc;
    bool done;

    done = Zinc_input_char_next(lex_data->input_data, lex_data->input_vtable, &c, &loc);

    if (done) {
        if (lex_data->has_digit) {
            *type = FieldTypeFloat;
        } else {
            *type = FieldTypeString;
        }
    } else if (c == '+' || c == '-') {
        FieldLexExponentSign(lex_data, type);
    } else if (isdigit(c)) {
        FieldLexExponent(lex_data, type);
    } else {
        FieldLexString(lex_data, type);
    }
}

void FieldLexExponentSign(struct FieldLexData* lex_data, enum FieldType* type)
{
    char c;
    struct Zinc_location loc;
    bool done;

    done = Zinc_input_char_next(lex_data->input_data, lex_data->input_vtable, &c, &loc);

    if (done) {
        if (lex_data->has_digit) {
            *type = FieldTypeFloat;
        } else {
            *type = FieldTypeString;
        }
    } else if (isdigit(c)) {
        FieldLexExponent(lex_data, type);
    } else {
        FieldLexString(lex_data, type);
    }
}

void FieldLexExponent(struct FieldLexData* lex_data, enum FieldType* type)
{
    char c;
    struct Zinc_location loc;
    bool done;

    while (true) {
        done = Zinc_input_char_next(lex_data->input_data, lex_data->input_vtable, &c, &loc);

        if (done) {
            if (lex_data->has_digit) {
                *type = FieldTypeFloat;
            } else {
                *type = FieldTypeString;
            }
            break;
        } else if (isdigit(c)) {
            continue;
        } else {
            FieldLexString(lex_data, type);
            break;
        }
    }
}

void FieldLexString(struct FieldLexData* lex_data, enum FieldType* type)
{
    Zinc_vector* text = NULL;
    Zinc_input_char_get_all(lex_data->input_data, lex_data->input_vtable, &text);
    if (Zinc_vector_match_str(text, "True") || Zinc_vector_match_str(text, "False")) {
        *type = FieldTypeBool;
    } else {
        *type = FieldTypeString;
    }
}

void FieldLex(struct FieldLexData* lex_data, enum FieldType* type)
{
    *type = FieldTypeNone;
    FieldLexStart(lex_data, type);
    assert(*type != FieldTypeNone);
}

void FieldGetType(Zinc_vector* text, enum FieldType* type)
{
    Zinc_input_char_string input_string;
    Zinc_input_char_string_init(&input_string, text);

    struct FieldLexData lex_data;
    FieldLexDataInit(&lex_data);
    lex_data.input_data = &input_string;
    lex_data.input_vtable = input_string.input_vtable;

    FieldLex(&lex_data, type);
}