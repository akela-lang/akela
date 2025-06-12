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
    if (t->kind != Lava_token_kind_eof) {
        Zinc_location loc = Zinc_input_unicode_get_location(ld->input, ld->vtable);
        t->loc.end = loc.start;
    }
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

        t->loc = loc;

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
                return;
            }

            if (c[0] == '\n') {
                t->kind = Lava_token_kind_newline;
                return;
            }
        }

        Zinc_input_unicode_repeat(ld->input, ld->vtable);
        t->loc = loc;
        Lava_lex_text(ld, t);
        return;
    }
}

void Lava_lex_header(Lava_lex_data* ld, Lava_token* t)
{
    char c[4];
    int num;
    Zinc_location loc;
    bool done;

    t->kind = Lava_token_kind_header;

    while (true) {
        Zinc_result r = Zinc_input_unicode_next(ld->input, ld->vtable, c, &num, &loc, &done);
        if (r == Zinc_result_error) {
            Zinc_error_list_set(ld->errors, &loc, Zinc_error_message);
            continue;
        }

        if (done) {
            Zinc_input_unicode_repeat(ld->input, ld->vtable);
            return;
        }

        if (num != 1 || c[0] != '#') {
            Zinc_input_unicode_repeat(ld->input, ld->vtable);
            break;
        }

        Zinc_string_add(&t->text, c, num);
    }
}

void Lava_lex_backquote(Lava_lex_data* ld, Lava_token* t)
{
    char c[4];
    int num;
    Zinc_location loc;
    bool done;

    t->kind = Lava_token_kind_backquote;

    while (true) {
        Zinc_result r = Zinc_input_unicode_next(ld->input, ld->vtable, c, &num, &loc, &done);
        if (r == Zinc_result_error) {
            Zinc_error_list_set(ld->errors, &loc, Zinc_error_message);
            continue;
        }

        if (done) {
            Zinc_input_unicode_repeat(ld->input, ld->vtable);
            return;
        }

        if (num != 1 || c[0] != '`') {
            Zinc_input_unicode_repeat(ld->input, ld->vtable);
            break;
        }

        Zinc_string_add(&t->text, c, num);

        if (t->text.size == 3) {
            break;
        }
    }

    if (t->text.size != 3) {
        Zinc_error_list_set(ld->errors, &loc, "expected 3 backquotes");
        Lava_lex_text(ld, t);
    }
}

void Lava_lex_text(Lava_lex_data* ld, Lava_token* t)
{
    char c[4];
    int num;
    Zinc_location loc;
    bool done;

    t->kind = Lava_token_kind_text;
    while (true) {
        Zinc_result r = Zinc_input_unicode_next(ld->input, ld->vtable, c, &num, &loc, &done);
        if (r == Zinc_result_error) {
            Zinc_error_list_set(ld->errors, &loc, Zinc_error_message);
            continue;
        }

        if (done) {
            Zinc_input_unicode_repeat(ld->input, ld->vtable);
            break;
        }

        if (num == 1 && c[0] == '\n') {
            Zinc_input_unicode_repeat(ld->input, ld->vtable);
            break;
        }

        Zinc_string_add(&t->text, c, num);
    }
}