#ifndef ZINC_CSV_LEX_H
#define ZINC_CSV_LEX_H

#include "zinc/zstring.h"
#include "zinc/vector.h"
#include <stdlib.h>
#include <stdbool.h>
#include "zinc/error.h"
#include "zinc/input_char.h"

enum CSVTokenType {
    CSVTokenTypeNone,
    CSVTokenTypeField,
    CSVTokenTypeNewline,
    CSVTokenTypeEOF,
};

enum CSVStateType {
    CSVStateTypeStart,
    CSVStateTypeField,
    CSVStateTypeFieldQuoted,
    CSVStateTypeFieldEndOfQuote,
    CSVStateTypeEndOfRow,
    CSVStateTypeEOF,
};

struct CSVToken {
    enum CSVTokenType type;
    Vector value;
    struct Zinc_location loc;
};

struct CSVLexData {
    struct Zinc_error_list* el;
    enum CSVStateType state;
    void* input_data;
    Zinc_input_char_vtable* input_vtable;
};

bool CSVLexNextChar(struct CSVLexData* lex_data, char* c, struct Zinc_location* loc);
void CSVLexRepeatChar(struct CSVLexData* lex_data);
void CSVTokenInit(struct CSVToken* token);
void CSVTokenCreate(struct CSVToken** token);
void CSVTokenDestroy(struct CSVToken* token);
void CSVLexDataInit(struct CSVLexData* lex_data);
void CSVLexDataCreate(struct CSVLexData** lex_data);
void CSVLex(struct CSVLexData* lex_data, struct CSVToken** token);
enum result CSVLoad(const char* filename, Vector* text);

#endif