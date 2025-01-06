#include <stdbool.h>
#include "parse_data.h"
#include "ast.h"
#include "lex.h"
#include <assert.h>

void Cent_lookahead(Cent_parse_data* pd)
{
    if (!pd->lookahead) {
        pd->lookahead = lex(pd->ld);
    }
}

bool Cent_match(Cent_parse_data* pd, Cent_token_type type, char* message, Cent_token** t, Cent_ast* n)
{
    Cent_lookahead(pd);

    if (pd->lookahead->type == type) {
        location_combine(&n->loc, &pd->lookahead->loc);
        *t = pd->lookahead;
        pd->lookahead = NULL;
        return true;
    }

    error_list_set(pd->errors, &pd->lookahead->loc, message);
    n->has_error = true;
    return false;
}

void Cent_ignore_newlines(Cent_parse_data* pd, Cent_ast* n)
{
    Cent_lookahead(pd);
    while (pd->lookahead->type == Cent_token_newline) {
        Cent_token* nl = NULL;
        if (!Cent_match(pd, Cent_token_newline, "expected newline", &nl, n)) {
            assert(false && "not possible");
        }
        Cent_token_destroy(nl);
        free(nl);
        Cent_lookahead(pd);
    }
}

/* sep -> \n | ; | e */
bool Cent_has_separator(Cent_parse_data* pd, Cent_ast* n)
{
    Cent_token_type type;

    Cent_lookahead(pd);
    if (pd->lookahead->type == Cent_token_newline || pd->lookahead->type == Cent_token_semicolon) {
        type = pd->lookahead->type;
    } else {
        return false;
    }

    Cent_token* sep = NULL;
    if (!Cent_match(pd, type, "expected separator", &sep, n)) {
        assert(false && "not possible");
    }
    Cent_token_destroy(sep);
    free(sep);

    return true;
}

Cent_environment* Cent_get_environment(Cent_ast* n)
{
    Cent_ast* p = n;
    while (p) {
        if (p->env) {
            return p->env;
        }
        p = p->parent;
    }

    return NULL;
}

Cent_namespace_result Cent_namespace_lookup(Cent_ast* n)
{
    Cent_namespace_result nr = {NULL, NULL};

    Cent_ast* p = n->head;
    assert(p);
    assert(p != n->tail);

    Cent_environment* top = Cent_get_environment(n);
    Cent_symbol* sym = Cent_environment_get(top, &p->text);
    assert(sym);

    if (sym->type != Cent_symbol_type_module) {
        nr.sym = sym;
        nr.node = p;
        return nr;
    }

    Cent_module* mod = sym->data.module;
    assert(mod);

    p = p->next;
    while (p && p != n->tail) {
        Cent_module* temp = Cent_module_get(mod, &p->text);
        if (!temp) break;
        mod = temp;
        p = p->next;
    }

    sym = Cent_environment_get(mod->env, &p->text);
    if (sym) {
        nr.sym = sym;
        nr.node = p;
        return nr;
    }

    return nr;
}