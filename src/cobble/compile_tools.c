#include "compile_data.h"
#include "zinc/memory.h"
#include "token.h"
#include "ast.h"

Cob_token* Cob_lex(Cob_compile_data* cd)
{
    char c[5];
    int num;
    bool done;
    Zinc_location loc;
    Zinc_result r = Zinc_result_error;
    loc = cd->loc;
    while (r == Zinc_result_error) {
        r = Zinc_string_next(cd->text, &cd->pos, c, &num, &done);
        if (done) break;
        if (r == Zinc_result_ok) {
            cd->loc.start_pos = cd->pos;
            cd->loc.end_pos = cd->pos + 1;
            if (num == 1 && c[0] == '\n') {
                cd->loc.line++;
                cd->loc.col = 0;
            } else {
                cd->loc.col++;
            }
        }
    }

    Cob_token* t = NULL;
    Zinc_malloc_safe((void**)&t, sizeof(Cob_token));
    Cob_token_init(t);
    t->type = Cob_token_classify(c, num, done);
    for (int i = 0; i < 5; i++) {
        t->c[i] = c[i];
    }
    t->num = num;
    t->loc = loc;
    return t;
}

void Cob_lookahead(Cob_compile_data* cd)
{
    if (cd->lookahead) {
        return;
    }

    cd->lookahead = Cob_lex(cd);
}

bool Cob_match_token(
    Cob_compile_data* cd,
    Cob_token_type type,
    const char* reason,
    Cob_token** t,
    Cob_ast* n)
{
    if (!cd->lookahead) {
        Cob_lookahead(cd);
    }

    if (cd->lookahead->type == type) {
        *t = cd->lookahead;
        cd->lookahead = NULL;
        Zinc_location_combine(&n->loc, &(*t)->loc);
        return true;
    } else {
        *t = NULL;
        Zinc_error_list_set(cd->errors, &cd->lookahead->loc, "%s", reason);
        Zinc_location_combine(&n->loc, &n->loc);
        return false;
    }
}

void Cob_location_update(struct Zinc_location* dest, struct Zinc_location* src)
{
    if (dest->line == 0) {
        *dest = *src;
    }
}

void Cob_location_update_token(struct Zinc_location* dest, Cob_token* t)
{
    if (dest->line == 0 && t) {
        *dest = t->loc;
    }
}