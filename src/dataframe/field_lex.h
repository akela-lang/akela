#ifndef ZINC_FIELD_LEX_H
#define ZINC_FIELD_LEX_H

#include "error.h"
#include "zinc/input_char_string.h"
#include "zinc/input_char.h"
#include "field.h"

struct FieldLexData {
    bool has_digit;
    bool has_sign;
    void* input_data;
    Zinc_input_char_vtable* input_vtable;
};

void FieldLexDataInit(struct FieldLexData* lex_data);
void FieldLexDataCreate(struct FieldLexData** lex_data);
void FieldLex(struct FieldLexData* lex_data, enum FieldType* type);
void FieldGetType(Vector* text, enum FieldType* type);

#endif