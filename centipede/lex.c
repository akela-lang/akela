#include "lex_data.h"
#include "token.h"

void lex_start(Cent_lex_data* ld, Cent_token* t);
void lex_id(Cent_lex_data* ld, Cent_token* t);

Cent_token* lex(Cent_lex_data* ld)
{
    Cent_token* t = NULL;
    Cent_token_create(&t);

    lex_start(ld, t);
    if (t->type == Cent_token_id) {
        Cent_token_type* p = Cent_lex_get_reserved_word(ld, &t->value);
        if (p) {
            t->type = *p;
        }
    }

    return t;
}

void lex_start(Cent_lex_data* ld, Cent_token* t)
{
    char c[4];
    int num;
    struct location loc;
    bool done;

    while (true) {
        enum result r = InputUnicodeNext(ld->input, ld->input_vtable, c, &num, &loc, &done);

        if (r == result_error) {
            fprintf(stderr, "%s\n", error_message);
            exit(1);
        }

        if (done) {
            t->type = Cent_token_eof;
            t->loc = loc;
            return;
        }

        if (num == 1) {
            if (c[0] == ' ') {
                continue;
            }

            if (c[0] == '\n') {
                t->type = Cent_token_newline;
                t->loc = loc;
                return;
            }

            if (c[0] == ':') {
                t->type = Cent_token_colon;
                t->loc = loc;
                return;
            }

            if (c[0] == '#') {
                while (true) {
                    r = InputUnicodeNext(ld->input, ld->input_vtable, c, &num, &loc, &done);
                    if (r == result_error) {
                        fprintf(stderr, "%s\n", error_message);
                        exit(1);
                    }
                    if (done) {
                        t->type = Cent_token_eof;
                        t->loc = loc;
                        return;
                    }
                    if (num == 1 && c[0] == '\n') {
                        break;
                    }
                }

                continue;
            }

            if (isalpha(c[0]) || isdigit(c[0]) || c[0] == '_') {
                t->type = Cent_token_id;
                t->loc = loc;
                buffer_add(&t->value, c, num);
                lex_id(ld, t);
                return;
            }
        }

        struct buffer bf;
        buffer_init(&bf);
        buffer_add(&bf, c, num);
        error_list_set(ld->errors, &loc, "invalid character: %b", &bf);
        buffer_destroy(&bf);
    }
}

void lex_id(Cent_lex_data* ld, Cent_token* t)
{
    char c[4];
    int num;
    struct location loc;
    bool done;

    while (true) {
        enum result r = InputUnicodeNext(ld->input, ld->input_vtable, c, &num, &loc, &done);

        if (r == result_error) {
            fprintf(stderr, "%s\n", error_message);
            exit(1);
        }

        if (done) {
            InputUnicodeRepeat(ld->input, ld->input_vtable);
            return;
        }

        if (num == 1) {
            if (isalpha(c[0]) || isdigit(c[0]) || c[0] == '_' || c[0] == '.') {
                t->type = Cent_token_id;
                t->loc = loc;
                buffer_add(&t->value, c, num);
                continue;
            }
        }

        loc = InputUnicodeGetLocation(ld->input, ld->input_vtable);
        t->loc.end_pos = loc.start_pos;
        InputUnicodeRepeat(ld->input, ld->input_vtable);
        return;
    }
}