#include "lex.h"
#include "lex_data.h"
#include "token.h"

void Lava_lex_start(Lava_lex_data* ld, Lava_token* t);
void Lava_lex_header(Lava_lex_data* ld, Lava_token* t);
void Lava_lex_text(Lava_lex_data* ld, Lava_token* t);
void Lava_lex_backquote(Lava_lex_data* ld, Lava_token* t);

Lava_token* Lava_lex(Lava_lex_data* ld)
{
    Lava_token* t = NULL;
    Lava_token_create(&t);

    Lava_lex_start(ld, t);

    return t;
}

void Lava_lex_start(Lava_lex_data* ld, Lava_token* t)
{
    char c[4];
    int num;
    Zinc_location loc;
    bool done;

    while (true) {
        Zinc_result r = Zinc_input_unicode_next(ld->input, ld->vtable, c, &num, &loc, &done);
        if (r == Zinc_result_error) {
            Zinc_error_list_set(ld->errors, &loc, Zinc_error_message);
            continue;
        }

        if (done) {
            t->kind = Lava_token_kind_eof;
            return;
        }

        if (num == 1) {
            if (c[0] == '#' && loc.col == 1) {
                Zinc_input_unicode_repeat(ld->input, ld->vtable);
                Lava_lex_header(ld, t);
                return;
            }

            if (c[0] == '`' && loc.col == 1) {
                Zinc_input_unicode_repeat(ld->input, ld->vtable);
                Lava_lex_backquote(ld, t);
            }

            if ((isalpha(c[0]) || isdigit(c[0]) || c[0] == '_') && loc.col == 1) {
                Zinc_input_unicode_repeat(ld->input, ld->vtable);
                Lava_lex_text(ld, t);
                return;
            }
        }
    }
}

void Lava_lex_header(Lava_lex_data* ld, Lava_token* t)
{
    char c[4];
    int num;
    Zinc_location loc;
    bool done;

    Zinc_string title;
    Zinc_string_init(&title);

    // ###
    for (int i = 0; i < 3; i++) {
        Zinc_result r = Zinc_input_unicode_next(ld->input, ld->vtable, c, &num, &loc, &done);
        if (r == Zinc_result_error) {
            Zinc_error_list_set(ld->errors, &loc, Zinc_error_message);
            continue;
        }

        if (done) {
            Zinc_error_list_set(ld->errors, &loc, "expected beginning of header");
            Zinc_input_unicode_repeat(ld->input, ld->vtable);
            return;
        }

        if (num != 1 || c[0] != '#') {
            Zinc_error_list_set(ld->errors, &loc, "expected beginning of header");
        }
    }

    while (true) {
        Zinc_result r = Zinc_input_unicode_next(ld->input, ld->vtable, c, &num, &loc, &done);
        if (r == Zinc_result_error) {
            Zinc_error_list_set(ld->errors, &loc, Zinc_error_message);
            continue;
        }

        if (done) {
            Zinc_error_list_set(ld->errors, &loc, "expected beginning of header");
            Zinc_input_unicode_repeat(ld->input, ld->vtable);
            return;
        }

        if (num == 1 && c[0] == '\n') {
            break;
        }

        Zinc_string_add(&title, c, num);
    }


}

void Lava_lex_text(Lava_lex_data* ld, Lava_token* t)
{

}

void Lava_lex_backquote(Lava_lex_data* ld, Lava_token* t)
{

}