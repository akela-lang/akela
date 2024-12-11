#include "lex_tools.h"
#include "zinc/input_unicode.h"
#include "zinc/error.h"
#include "zinc/memory.h"

void Json_lex_data_init(
    Json_lex_data* jld,
    struct error_list* el,
    void* input_obj,
    InputUnicodeVTable* input_vtable)
{
    jld->el = el;
    jld->input_obj = input_obj;
    jld->input_vtable = input_vtable;
}

void Json_lex_data_create(
    Json_lex_data** jld,
    struct error_list* el,
    void* input_obj,
    InputUnicodeVTable* input_vtable)
{
    malloc_safe((void**)jld, sizeof(Json_lex_data));
    Json_lex_data_init(*jld, el, input_obj, input_vtable);
}

bool Json_is_hex_digit(char c[4], int num)
{
    if (num != 1)
        return false;
    if (c[0] >= 'A' && c[0] <= 'F') return true;
    if (c[0] >= 'a' && c[0] <= 'f') return true;
    if (c[0] >= '0' && c[0] <= '9') return true;
    return false;
}

int code_to_utf8(unsigned char *const buffer, const unsigned int code)
{
    if (code <= 0x7F) {
        buffer[0] = code;
        return 1;
    }
    if (code <= 0x7FF) {
        buffer[0] = 0xC0 | (code >> 6);            /* 110xxxxx */
        buffer[1] = 0x80 | (code & 0x3F);          /* 10xxxxxx */
        return 2;
    }
    if (code <= 0xFFFF) {
        buffer[0] = 0xE0 | (code >> 12);           /* 1110xxxx */
        buffer[1] = 0x80 | ((code >> 6) & 0x3F);   /* 10xxxxxx */
        buffer[2] = 0x80 | (code & 0x3F);          /* 10xxxxxx */
        return 3;
    }
    if (code <= 0x10FFFF) {
        buffer[0] = 0xF0 | (code >> 18);           /* 11110xxx */
        buffer[1] = 0x80 | ((code >> 12) & 0x3F);  /* 10xxxxxx */
        buffer[2] = 0x80 | ((code >> 6) & 0x3F);   /* 10xxxxxx */
        buffer[3] = 0x80 | (code & 0x3F);          /* 10xxxxxx */
        return 4;
    }
    return 0;
}

unsigned int char_to_hex(char c)
{
    int x = toupper(c);
    if (x == '0') return 0;
    if (x == '1') return 1;
    if (x == '2') return 2;
    if (x == '3') return 3;
    if (x == '4') return 4;
    if (x == '5') return 5;
    if (x == '6') return 6;
    if (x == '7') return 7;
    if (x == '8') return 8;
    if (x == '9') return 9;
    if (x == 'A') return 10;
    if (x == 'B') return 11;
    if (x == 'C') return 12;
    if (x == 'D') return 13;
    if (x == 'E') return 14;
    if (x == 'F') return 15;
    return 0;
}

unsigned int char4_to_hex(char* src, int num)
{
    unsigned int code = 0;
    if (num > 0) {
        code = char_to_hex(src[0]);
    }
    for (int i = 1; i < num; i++) {
        code = (code << 4) + char_to_hex(src[i]);
    }
    return code;
}