#include <string.h>
#include <stdbool.h>
#include "parse_tools.h"
#include "token.h"
#include "lex.h"
#include "ast.h"
#include <assert.h>
#include "zinc/buffer_list.h"

void Ake_parse_state_init(
        struct Ake_parse_state* ps,
        struct Ake_lex_state* ls,
        struct error_list* el,
        struct buffer_list* extern_list,
        struct Ake_symbol_table* st)
{
	ps->ls = ls;
	ps->lookahead = NULL;
	ps->el = el;
    ps->extern_list = extern_list;
	ps->st = st;
}

void Ake_parse_state_destroy(struct Ake_parse_state* ps)
{
}

bool Ake_get_lookahead_one(struct Ake_parse_state* ps)
{
    bool valid = true;

    if (ps->lookahead) {
        return valid;
    }

    struct Ake_token* t = NULL;
    while (true) {
        valid = Ake_lex(ps->ls, &t) && valid;
        if (t) {
            break;
        }
    }

    ps->lookahead = t;

    return valid;
}

/* expecting specific token */
bool Ake_match(
    struct Ake_parse_state* ps,
    enum Ake_token_enum type,
    const char* reason,
    struct Ake_token** t,
    Ake_ast* n)
{
	bool valid = true;
	struct location loc;

	if (!ps->lookahead) {
        Ake_get_lookahead_one(ps);
	}

    *t = ps->lookahead;
	if ((*t)->type == type) {
		ps->lookahead = NULL;
        location_combine(&n->loc, &(*t)->loc);
		return valid;
	}

	valid = false;
	Ake_get_token_location(*t, &loc);
	error_list_set(ps->el, &loc, "%s", reason);
	*t = NULL;
	return valid;
}

bool Ake_consume_newline(struct Ake_parse_state* ps, Ake_ast* n)
{
    bool valid = true;
    while (true) {
        valid = Ake_get_lookahead_one(ps) && valid;
        struct Ake_token* t0 = Ake_get_lookahead(ps);
        if (t0 && t0->type == Ake_token_newline) {
            struct Ake_token* t = NULL;
            valid = Ake_match(ps, Ake_token_newline, "expected newline", &t, n) && valid;
            Ake_token_destroy(t);
            free(t);
        } else {
            break;
        }
    }

    return valid;
}

struct location Ake_get_location(struct Ake_parse_state* ps)
{
    Ake_get_lookahead_one(ps);
    return ps->lookahead->loc;
}

bool Ake_is_identity_comparison(enum Ake_ast_type type)
{
	return type == Ake_ast_type_equality || type == Ake_ast_type_not_equal;
}

struct Ake_token* Ake_get_lookahead(struct Ake_parse_state* ps)
{
    Ake_get_lookahead_one(ps);
    struct Ake_token* t = ps->lookahead;
    return t;
}

bool Ake_check_assignment_value_count(Ake_ast* a, Ake_ast* b)
{
    if (a && b) {
        if (a->type != Ake_ast_type_error && b->type != Ake_ast_type_error) {
            if (a->type == Ake_ast_type_eseq && b->type != Ake_ast_type_eseq) {
                return false;
            } else if (a->type != Ake_ast_type_eseq && b->type == Ake_ast_type_eseq) {
                return false;
            } else if (a->type == Ake_ast_type_eseq || b->type == Ake_ast_type_eseq) {
                if (Ake_ast_count_children(a) != Ake_ast_count_children(b)) {
                    return false;
                }
            }
        }
    }
    return true;
}

/* separator -> \n | ; */
void Ake_parse_separator(struct Ake_parse_state* ps, Ake_ast* n, bool* has_separator)
{
    enum Ake_token_enum type;
    *has_separator = false;

    struct Ake_token* t0 = Ake_get_lookahead(ps);
    if (t0 && t0->type == Ake_token_newline) {
        type = Ake_token_newline;
        *has_separator = true;
    } else if (t0 && t0->type == Ake_token_semicolon) {
        type = Ake_token_semicolon;
        *has_separator = true;
    } else {
        return;
    }

    struct Ake_token* sep = NULL;
    if (!Ake_match(ps, type, "expecting newline or semicolon", &sep, n)) {
        assert(false);
        /* test case: no test case necessary */
    }

    Ake_token_destroy(sep);
    free(sep);
}