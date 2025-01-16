#include "lex_tools.h"
#include "zinc/input_unicode.h"
#include "zinc/error.h"
#include "zinc/memory.h"
#include <unicode/locid.h>
#include <unicode/ucnv.h>
#include "zinc/String_slice.h"
#include "token.h"

enum Zinc_result Json_lex_data_init(
    Json_lex_data* ld,
    struct Zinc_error_list* el,
    void* input_obj,
    Zinc_input_unicode_vtable* input_vtable)
{
    ld->el = el;
    ld->input_obj = input_obj;
    ld->input_vtable = input_vtable;
    return Zinc_result_ok;
}

void Json_lex_data_create(
    Json_lex_data** ld,
    struct Zinc_error_list* el,
    void* input_obj,
    Zinc_input_unicode_vtable* input_vtable)
{
    Zinc_malloc_safe((void**)ld, sizeof(Json_lex_data));
    Json_lex_data_init(*ld, el, input_obj, input_vtable);
}

void Json_lex_data_destroy(Json_lex_data* ld)
{
}

enum Zinc_result Json_convert_slice(String_slice slice, UChar32* c)
{
    UErrorCode status = U_ZERO_ERROR;
    UConverter *conv = ucnv_open("utf8", &status);
    if (status != U_ZERO_ERROR) {
        Zinc_set_error("ucnv_open() failed with error code: %s", u_errorName(status));
        return Zinc_result_error;
    }

    UChar dest[4];
    int32_t len = ucnv_toUChars(conv, dest, 4, slice.p, slice.size, &status);
    size_t pos = 0;
    U16_NEXT(dest, pos, len, *c);

    ucnv_close(conv);

    return Zinc_result_ok;
}

enum Zinc_result Json_convert_char(char c[4], int num, UChar32* cp)
{
    String_slice slice;
    slice.p = c;
    slice.size = num;
    return Json_convert_slice(slice, cp);
}