#include "compile_data.h"
#include "zinc/memory.h"
#include "token.h"
#include "Ast_node.h"

struct token* lex(void* input_obj, InputUnicodeVTable *input_vtable)
{
    char c[4];
    int num;
    struct location loc;
    bool done;
    enum result r = result_error;
    while (r == result_error) {
        r = InputUnicodeNext(input_obj, input_vtable, c, &num, &loc, &done);
    }

    struct token* t = NULL;
    malloc_safe((void**)&t, sizeof(struct token));
    token_init(t);
    t->type = token_classify(c, num, done);
    for (int i = 0; i < num; i++) {
        t->c[i] = c[i];
    }
    t->num = num;
    t->loc = loc;
    return t;
}

void get_lookahead(struct Cob_compile_data* cd)
{
    if (cd->lookahead) {
        return;
    }

    cd->lookahead = lex(cd->input_obj, cd->input_vtable);
}

bool match(
    struct Cob_compile_data* cd,
    enum token_type type,
    const char* reason,
    struct token** t,
    struct Ast_node* n)
{
    if (!cd->lookahead) {
        get_lookahead(cd);
    }

    if (cd->lookahead->type == type) {
        *t = cd->lookahead;
        cd->lookahead = NULL;
        location_combine(&n->loc, &(*t)->loc);
        return true;
    } else {
        *t = NULL;
        error_list_set(cd->el, &cd->lookahead->loc, "%s", reason);
        location_combine(&n->loc, &n->loc);
        return false;
    }
}

void location_update(struct location* dest, struct location* src)
{
    if (dest->line == 0) {
        *dest = *src;
    }
}

void location_update_token(struct location* dest, struct token* t)
{
    if (dest->line == 0 && t) {
        *dest = t->loc;
    }
}