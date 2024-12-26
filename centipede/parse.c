#include "parse_data.h"
#include "parse_tools.h"
#include <assert.h>
#include <akela/parse_tools.h>
#include "base.h"

Cent_ast* Cent_parse_stmts(Cent_parse_data* pd);
bool Cent_has_separator(Cent_parse_data* pd, Cent_ast* n);
Cent_ast* Cent_parse_stmt(Cent_parse_data* pd);
Cent_ast* Cent_parse_element(Cent_parse_data* pd);
void Cent_ignore_newlines(Cent_parse_data* pd, Cent_ast* n);
Cent_ast* Cent_parse_element_properties(Cent_parse_data* pd);
Cent_ast* Cent_parse_property_dec(Cent_parse_data* pd);

Cent_parse_result Cent_parse(Cent_parse_data* pd)
{
    pd->top = Cent_base_create();
    Cent_ast* root = Cent_parse_stmts(pd);

    Cent_parse_result result;
    Cent_parse_result_init(&result);
    result.errors = pd->errors;
    result.base = pd->top;
    result.root = root;

    return result;
}

/* stmts -> stmt stmts' | e */
/* stmts' -> sep stmt stmts' | e */
Cent_ast* Cent_parse_stmts(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_stmts;

    Cent_environment* env = NULL;
    Cent_environment_create(&env);
    env->prev = pd->top;
    pd->top = env;
    n->env = env;

    Cent_ast* a = Cent_parse_stmt(pd);
    if (a) {
        Cent_ast_add(n, a);
    }

    while (true) {
        if (!Cent_has_separator(pd, n)) {
            break;
        }

        a = Cent_parse_stmt(pd);
        if (a) {
            Cent_ast_add(n, a);
        }
    }

    pd->top = pd->top->prev;

    return n;
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

    return true;
}

/* stmt -> parse_element | e */
Cent_ast* Cent_parse_stmt(Cent_parse_data* pd)
{
    Cent_lookahead(pd);

    if (pd->lookahead->type == Cent_token_element) {
        return Cent_parse_element(pd);
    }

    return NULL;
}

/* parse_element -> element name { element_seq } */
Cent_ast* Cent_parse_element(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_element;

    Cent_token* e = NULL;
    if (!Cent_match(pd, Cent_token_element, "expected element", &e, n)) {
        assert(false && "not possible");
    }

    Cent_token* id = NULL;
    Cent_match(pd, Cent_token_id, "expected id", &id, n);

    Cent_element* element = NULL;
    Cent_element_create(&element);

    if (id) {
        buffer_copy(&id->value, &element->name);
        Cent_token_destroy(id);
        free(id);
    }

    Cent_token* lcb = NULL;
    Cent_match(pd, Cent_token_left_curly_brace, "expected left-curly-brace", &lcb, n);
    Cent_token_destroy(lcb);
    free(lcb);

    Cent_ignore_newlines(pd, n);

    Cent_lookahead(pd);
    if (pd->lookahead->type == Cent_token_properties) {
        Cent_ast* a = Cent_parse_element_properties(pd);
        Cent_ast_add(n, a);
    }

    Cent_ignore_newlines(pd, n);

    Cent_token* rcb = NULL;
    Cent_match(pd, Cent_token_right_curly_brace, "expected right-curly-brace", &rcb, n);
    Cent_token_destroy(rcb);
    free(rcb);

    if (!n->has_error) {
        Cent_symbol* sym = NULL;
        Cent_symbol_create(&sym);
        sym->type = Cent_symbol_type_element;
        sym->element = element;
        Cent_environment_add_symbol(pd->top, &element->name, sym);
    }

    return n;
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

/* element_seq -> id : element element_seq' | e */
/* element_seq' -> sep id : element element_seq | e */
Cent_ast* Cent_parse_element_properties(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_prop;

    Cent_token* prop = NULL;
    Cent_match(pd, Cent_token_properties, "expected properties", &prop, n);
    Cent_token_destroy(prop);
    free(prop);

    Cent_ignore_newlines(pd, n);

    while (true) {
        Cent_lookahead(pd);
        if (pd->lookahead->type != Cent_token_id) {
            break;
        }

        Cent_ast* a = Cent_parse_property_dec(pd);
        Cent_ast_add(n, a);

        if (!Cent_has_separator(pd, n)) {
            break;
        }
    }

    Cent_token* end = NULL;
    Cent_match(pd, Cent_token_end, "expected end", &end, n);
    Cent_token_destroy(end);
    free(end);

    return n;
}

Cent_ast* Cent_parse_property_dec(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_prop_dec;

    Cent_token* id = NULL;
    Cent_match(pd, Cent_token_id, "expected id", &id, n);

    Cent_ast* a = NULL;
    Cent_ast_create(&a);
    a->type = Cent_ast_type_id;
    buffer_copy(&id->value, &a->value);
    Cent_ast_add(n, a);

    Cent_token_destroy(id);
    free(id);

    Cent_token* colon = NULL;
    Cent_match(pd, Cent_token_colon, "expected colon", &colon, n);
    Cent_token_destroy(colon);
    free(colon);

    Cent_match(pd, Cent_token_id, "expected id", &id, n);

    Cent_ast* b = NULL;
    Cent_ast_create(&b);
    b->type = Cent_ast_type_id;
    buffer_copy(&id->value, &b->value);
    Cent_ast_add(n, b);

    Cent_token_destroy(id);
    free(id);

    Cent_lookahead(pd);
    if (pd->lookahead->type == Cent_token_modifier) {
        Cent_token* mod = NULL;
        Cent_match(pd, Cent_token_modifier, "expected modifier", &mod, n);
        Cent_ast* c = NULL;
        Cent_ast_create(&c);
        c->type = Cent_ast_type_modifier;
        buffer_copy(&mod->value, &c->value);
        Cent_ast_add(n, c);
        Cent_token_destroy(mod);
        free(mod);
    }

    return n;
}

Cent_ast* Cent_parse_children(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_children;

    Cent_token* ch = NULL;

    return n;
}