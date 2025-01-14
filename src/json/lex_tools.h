#ifndef JSON_LEX_TOOLS_H
#define JSON_LEX_TOOLS_H

#include "zinc/error.h"
#include "zinc/input_unicode.h"
#include <unicode/ucnv.h>
#include "zinc/String_slice.h"

typedef enum Json_lex_state {
    Json_lex_state_start,
    Json_lex_state_string,
} Json_lex_state;

typedef struct Json_lex_data {
    struct Zinc_error_list* el;
    void* input_obj;
    InputUnicodeVTable* input_vtable;
} Json_lex_data;

enum result Json_lex_data_init(
    Json_lex_data* ld,
    struct Zinc_error_list* el,
    void* input_obj,
    InputUnicodeVTable* input_vtable);
void Json_lex_data_create(
    Json_lex_data** ld,
    struct Zinc_error_list* el,
    void* input_obj,
    InputUnicodeVTable* input_vtable);
enum result Json_convert_slice(String_slice slice, UChar32* c);
enum result Json_convert_char(char c[4], int num, UChar32* cp);

#endif