#include "token.h"
#include "lex_tools.h"
#include "zinc/input_unicode.h"
#include "zinc/error.h"
#include <string.h>
#include "zinc/utf8.h"
#include <assert.h>

void Json_lex_start(Json_lex_data* jld, Json_token* t);
void Json_lex_string(Json_lex_data* jld, Json_token* t);

Json_token* Json_lex(Json_lex_data* jld)
{
    Json_token* t = NULL;
    Json_token_create(&t);
    Json_lex_start(jld, t);
    return t;
}

void Json_lex_start(Json_lex_data* jld, Json_token* t)
{
    char c[4];
    int num;
    struct location loc;
    bool done;

    while (true) {
        enum result r = InputUnicodeNext(jld->input_obj, jld->input_vtable, c, &num, &loc, &done);
        if (r == result_error) {
            error_list_set(jld->el, &loc, error_message);
            continue;
        }

        if (done) {
            t->type = Json_token_type_eof;
            return;
        }

        if (num != 1) {
            struct buffer bf;
            buffer_init(&bf);
            buffer_add(&bf, c, num);
            error_list_set(jld->el, &loc, "invalid character: %b", &bf);
            buffer_destroy(&bf);
            continue;
        }

        if (c[0] == '"') {
            t->type = Json_token_type_string;
            Json_lex_string(jld, t);
            return;
        }

        if (c[0] == ' ') {
            continue;
        }

        if (c[0] == '\n') {
            continue;
        }

        if (c[0] == '\r') {
            continue;
        }

        if (c[0] == '\t') {
            continue;
        }

        struct buffer bf;
        buffer_init(&bf);
        buffer_add(&bf, c, num);
        error_list_set(jld->el, &loc, "invalid character: %b", &bf);
        buffer_destroy(&bf);
    }
}

void Json_lex_string(Json_lex_data* jld, Json_token* t)
{
    char c[4];
    int num;
    struct location loc;
    bool done;

    while (true) {
        enum result r = InputUnicodeNext(jld->input_obj, jld->input_vtable, c, &num, &loc, &done);
        if (r == result_error) {
            error_list_set(jld->el, &loc, error_message);
            continue;
        }

        if (done) {
            t->type = Json_token_type_eof;
            return;
        }

        if (num != 1) {
            buffer_add(&t->value, c, num);
            continue;
        }

        if (c[0] == '"') {
            return;
        }

        if (c[0] != '\\') {
            buffer_add(&t->value, c, num);
            continue;
        }

        /* escape character */
        r = InputUnicodeNext(jld->input_obj, jld->input_vtable, c, &num, &loc, &done);
        if (r == result_error) {
            error_list_set(jld->el, &loc, error_message);
            continue;
        }

        if (done) {
            error_list_set(jld->el, &loc, "missing escape character");
            InputUnicodeRepeat(jld->input_obj, jld->input_vtable);
            return;
        }

        if (num != 1) {
            struct buffer bf;
            buffer_init(&bf);
            buffer_add(&bf, c, num);
            error_list_set(jld->el, &loc, "invalid escape character: %b", &bf);
            buffer_destroy(&bf);
            continue;
        }

        if (c[0] == '"') {
            buffer_add(&t->value, c, num);
            continue;
        }

        if (c[0] == '\\') {
            buffer_add_char(&t->value, '\\');
            continue;
        }

        if (c[0] == '/') {
            buffer_add_char(&t->value, '/');
            continue;
        }

        if (c[0] == 'b') {
            buffer_add_char(&t->value, '\b');
            continue;
        }

        if (c[0] == 'f') {
            buffer_add_char(&t->value, '\f');
            continue;
        }

        if (c[0] == 'n') {
            buffer_add_char(&t->value, '\n');
            continue;
        }

        if (c[0] == 'r') {
            buffer_add_char(&t->value, '\r');
            continue;
        }

        if (c[0] == 't') {
            buffer_add_char(&t->value, '\t');
            continue;
        }

        if (c[0] == 'u') {
            struct buffer bf;
            buffer_init(&bf);
            buffer_copy_str(&bf, "\\u");
            bool valid = true;
            for (int i = 0; i < 4; i++) {
                r = InputUnicodeNext(jld->input_obj, jld->input_vtable, c, &num, &loc, &done);

                if (r == result_error) {
                    error_list_set(jld->el, &loc, error_message);
                } else {
                    if (done) {
                        error_list_set(jld->el, &loc, "unicode escape not finished");
                        return;
                    }
                    if (Json_is_hex_digit(c, num)) {
                        buffer_add(&bf, c, num);
                    } else {
                        error_list_set(jld->el, &loc, "invalid hex digit: %c", c[0]);
                        valid = false;
                    }
                }
            }
            buffer_finish(&bf);
            if (valid) {
                unsigned int cp = char4_to_hex(bf.buf + 2);
                if (cp < 0x20) {
                    error_list_set(jld->el, &loc, "code point is less than \\u0020: %s", bf.buf);
                } else if (cp > 0x10FFFF) {
                    error_list_set(jld->el, &loc, "code point greater than \\u10FFFF: %s", bf.buf);
                    assert(false && "not possible");
                } else {
                    char dest[4];
                    num = code_to_utf8(dest, cp);
                    buffer_add(&t->value, dest, num);
                }
            }
            buffer_destroy(&bf);
            continue;
        }

        struct buffer bf;
        buffer_init(&bf);
        buffer_add(&bf, c, num);
        error_list_set(jld->el, &loc, "invalid escape character: %b", &bf);
        buffer_destroy(&bf);
    }
}