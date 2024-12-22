#include "lex_data.h"
#include "token.h"

void lex_start(Cent_lex_data* ld, Cent_token* t);
void lex_id(Cent_lex_data* ld, Cent_token* t);

Cent_token* lex(Cent_lex_data* ld)
{
    Cent_token* t = NULL;
    Cent_token_create(&t);

    lex_start(ld, t);

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

        if (num == 1 && isalpha(c[0]) || isdigit(c[0]) || c[0] == '_' || c[0] == '.') {
            t->type = Cent_token_id;
            t->loc = loc;
            buffer_add(&t->value, c, num);
            lex_id(ld, t);
            return;
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