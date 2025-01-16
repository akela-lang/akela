#include "compile_data.h"
#include "zinc/memory.h"
#include "token.h"
#include "ast.h"

Cob_token* Cob_lex(void* input_obj, Zinc_input_unicode_vtable *input_vtable)
{
    char c[4];
    int num;
    struct Zinc_location loc;
    bool done;
    enum Zinc_result r = Zinc_result_error;
    while (r == Zinc_result_error) {
        r = Zinc_input_unicode_next(input_obj, input_vtable, c, &num, &loc, &done);
    }

    Cob_token* t = NULL;
    Zinc_malloc_safe((void**)&t, sizeof(Cob_token));
    Cob_token_init(t);
    t->type = Cob_token_classify(c, num, done);
    for (int i = 0; i < num; i++) {
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

    cd->lookahead = Cob_lex(cd->input_obj, cd->input_vtable);
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
        Zinc_error_list_set(cd->el, &cd->lookahead->loc, "%s", reason);
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