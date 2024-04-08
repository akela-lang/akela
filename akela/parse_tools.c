#include <string.h>
#include <stdbool.h>
#include "input.h"
#include "parse_tools.h"
#include "token.h"
#include "zinc/result.h"
#include "scan.h"
#include "source.h"
#include "ast.h"
#include <assert.h>

/* dynamic-output-none */
/* initialize-output ps ps{}*/
void parse_state_init(struct parse_state* ps, struct scan_state* sns, struct error_list* el, struct symbol_table* st)
{
	ps->sns = sns;
	ps->lookahead = NULL;
	ps->el = el;
	ps->st = st;
	buffer_init(&ps->qualifier);
}

void parse_state_destroy(struct parse_state* ps)
{
	buffer_destroy(&ps->qualifier);
}

bool get_lookahead_one(struct parse_state* ps)
{
    bool valid = true;

    if (ps->lookahead) {
        return valid;
    }

    struct token* t = NULL;
    while (true) {
        valid = scan_get_token(ps->sns, &t) && valid;
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
	set_source_error(ps->el, &loc, "%s", reason);
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

bool is_identity_comparison(enum ast_type type)
{
	return type == ast_type_equality || type == ast_type_not_equal;
}

struct token* get_lookahead(struct parse_state* ps)
{
    get_lookahead_one(ps);
    struct token* t = ps->lookahead;
    return t;
}
