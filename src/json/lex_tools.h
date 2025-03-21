#ifndef JSON_LEX_TOOLS_H
#define JSON_LEX_TOOLS_H

#include "zinc/error.h"
#include "zinc/input_unicode.h"
#include <unicode/ucnv.h>
#include "zinc/string_slice.h"

typedef enum Json_lex_state {
    Json_lex_state_start,
    Json_lex_state_string,
} Json_lex_state;

typedef struct Json_lex_data {
    struct Zinc_error_list* el;
    void* input_obj;
    Zinc_input_unicode_vtable* input_vtable;
} Json_lex_data;

enum Zinc_result Json_lex_data_init(
    Json_lex_data* ld,
    struct Zinc_error_list* el,
    void* input_obj,
    Zinc_input_unicode_vtable* input_vtable);
void Json_lex_data_create(
    Json_lex_data** ld,
    struct Zinc_error_list* el,
    void* input_obj,
    Zinc_input_unicode_vtable* input_vtable);
enum Zinc_result Json_convert_slice(Zinc_string_slice slice, UChar32* c);
enum Zinc_result Json_convert_char(char c[4], int num, UChar32* cp);

#endif