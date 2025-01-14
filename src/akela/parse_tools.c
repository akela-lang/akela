#include <string.h>
#include <stdbool.h>
#include "parse_tools.h"
#include "token.h"
#include "lex.h"
#include "ast.h"
#include <assert.h>
#include "zinc/buffer_list.h"

void parse_state_init(
        struct parse_state* ps,
        struct lex_state* ls,
        struct error_list* el,
        struct buffer_list* extern_list,
        struct symbol_table* st)
{
	ps->ls = ls;
	ps->lookahead = NULL;
	ps->el = el;
    ps->extern_list = extern_list;
	ps->st = st;
}

void parse_state_destroy(struct parse_state* ps)
{
}

bool get_lookahead_one(struct parse_state* ps)
{
    bool valid = true;

    if (ps->lookahead) {
        return valid;
    }

    struct token* t = NULL;
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
bool match(
    struct parse_state* ps,
    enum token_enum type,
    const char* reason,
    struct token** t,
    Ake_ast* n)
{
	bool valid = true;
	struct location loc;

	if (!ps->lookahead) {
        get_lookahead_one(ps);
	}

    *t = ps->lookahead;
	if ((*t)->type == type) {
		ps->lookahead = NULL;
        location_combine(&n->loc, &(*t)->loc);
		return valid;
	}

	valid = false;
	get_token_location(*t, &loc);
	error_list_set(ps->el, &loc, "%s", reason);
	*t = NULL;
	return valid;
}

bool consume_newline(struct parse_state* ps, Ake_ast* n)
{
    bool valid = true;
    while (true) {
        valid = get_lookahead_one(ps) && valid;
        struct token* t0 = get_lookahead(ps);
        if (t0 && t0->type == token_newline) {
            struct token* t = NULL;
            valid = match(ps, token_newline, "expected newline", &t, n) && valid;
            token_destroy(t);
            free(t);
        } else {
            break;
        }
    }

    return valid;
}

struct location get_location(struct parse_state* ps)
{
    get_lookahead_one(ps);
    return ps->lookahead->loc;
}

bool is_identity_comparison(enum Ake_ast_type type)
{
	return type == Ake_ast_type_equality || type == Ake_ast_type_not_equal;
}

struct token* get_lookahead(struct parse_state* ps)
{
    get_lookahead_one(ps);
    struct token* t = ps->lookahead;
    return t;
}

bool check_assignment_value_count(Ake_ast* a, Ake_ast* b)
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
void parse_separator(struct parse_state* ps, Ake_ast* n, bool* has_separator)
{
    enum token_enum type;
    *has_separator = false;

    struct token* t0 = get_lookahead(ps);
    if (t0 && t0->type == token_newline) {
        type = token_newline;
        *has_separator = true;
    } else if (t0 && t0->type == token_semicolon) {
        type = token_semicolon;
        *has_separator = true;
    } else {
        return;
    }

    struct token* sep = NULL;
    if (!match(ps, type, "expecting newline or semicolon", &sep, n)) {
        assert(false);
        /* test case: no test case necessary */
    }

    token_destroy(sep);
    free(sep);
}