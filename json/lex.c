#include "token.h"
#include "lex_tools.h"
#include "zinc/input_unicode.h"
#include "zinc/error.h"
#include <string.h>

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
            error_list_set(jld->el, &loc, "missing escape character");
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
            char temp[10];
            struct buffer bf;
            buffer_init(&bf);
            buffer_copy_str(&bf, "\\u");
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
                        buffer_add(&t->value, c, num);
                    } else {
                        error_list_set(jld->el, &loc, "invalid hex number: %c", c[0]);
                    }
                }
            }
            buffer_finish(&bf);
            snprintf(temp, 10, bf.buf);
            buffer_add(&t->value, temp, strlen(temp));
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