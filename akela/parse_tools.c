#include <string.h>
#include <stdbool.h>
#include "parse_tools.h"
#include "token.h"
#include "zinc/result.h"
#include "lex.h"
#include "ast.h"
#include <assert.h>

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
        valid = lex(ps->ls, &t) && valid;
        if (t) {
            break;
        }
    }

    ps->lookahead = t;

    return valid;
}

/* expecting specific token */
bool match(struct parse_state* ps, enum token_enum type, const char* reason, struct token** t)
{
	bool valid = true;
	struct location loc;

	if (!ps->lookahead) {
        get_lookahead_one(ps);
	}

    *t = ps->lookahead;
	if ((*t)->type == type) {
		ps->lookahead = NULL;
		return valid;
	}

	valid = false;
	get_token_location(*t, &loc);
	error_list_set(ps->el, &loc, "%s", reason);
	*t = NULL;
	return valid;
}

bool consume_newline(struct parse_state* ps)
{
    bool valid = true;
    while (true) {
        valid = get_lookahead_one(ps) && valid;
        struct token* t0 = get_lookahead(ps);
        if (t0 && t0->type == token_newline) {
            struct token* t = NULL;
            valid = match(ps, token_newline, "expected newline", &t) && valid;
            token_destroy(t);
            free(t);
        } else {
            break;
        }
    }

    return valid;
}

bool get_location(struct parse_state* ps, struct location* loc)
{
    bool valid = get_lookahead_one(ps);
    *loc = ps->lookahead->loc;
    return valid;
}

bool is_identity_comparison(enum Ast_type type)
{
	return type == Ast_type_equality || type == Ast_type_not_equal;
}

struct token* get_lookahead(struct parse_state* ps)
{
    get_lookahead_one(ps);
    struct token* t = ps->lookahead;
    return t;
}

bool check_assignment_value_count(Ast_node* a, Ast_node* b)
{
    if (a && b) {
        if (a->type != Ast_type_error && b->type != Ast_type_error) {
            if (a->type == Ast_type_eseq && b->type != Ast_type_eseq) {
                return false;
            } else if (a->type != Ast_type_eseq && b->type == Ast_type_eseq) {
                return false;
            } else if (a->type == Ast_type_eseq || b->type == Ast_type_eseq) {
                if (Ast_node_count_children(a) != Ast_node_count_children(b)) {
                    return false;
                }
            }
        }
    }
    return true;
}

/* separator -> \n | ; */
void parse_separator(struct parse_state* ps, bool* has_separator, struct location* loc)
{
    enum token_enum type;
    *has_separator = false;

    get_location(ps, loc);

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
    if (!match(ps, type, "expecting newline or semicolon", &sep)) {
        assert(false);
        /* test case: no test case necessary */
    }

    token_destroy(sep);
    free(sep);
}