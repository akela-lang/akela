#include "parse_data.h"
#include "parse_tools.h"
#include <assert.h>
#include <akela/parse_tools.h>
#include "base.h"

Cent_ast* Cent_parse_stmts(Cent_parse_data* pd);
bool Cent_has_separator(Cent_parse_data* pd, Cent_ast* n);
Cent_ast* Cent_parse_stmt(Cent_parse_data* pd);
Cent_ast* Cent_parse_element(Cent_parse_data* pd);

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

    Cent_token* e = NULL;
    if (!Cent_match(pd, Cent_token_element, "expected element", &e, n)) {
        assert(false && "expected element");
    }

    Cent_token* id = NULL;
    if (!Cent_match(pd, Cent_token_id, "expected id", &id, n)) {
        error_list_set(pd->errors, &pd->lookahead->loc, "expected id");
        n->has_error = true;
    }

    Cent_element* element = NULL;
    Cent_element_create(&element);

    if (id) {
        buffer_copy(&id->value, &element->name);
        Cent_token_destroy(id);
        free(id);
    }

    if (!n->has_error) {
        Cent_symbol* sym = NULL;
        Cent_symbol_create(&sym);
        sym->type = Cent_symbol_type_element;
        sym->element = element;
        Cent_environment_add_symbol(pd->top, &element->name, sym);
    }

    return n;
}