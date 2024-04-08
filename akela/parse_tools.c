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
	token_list_init(&ps->lookahead);
	ps->el = el;
	ps->st = st;
	buffer_init(&ps->qualifier);
}

void parse_state_destroy(struct parse_state* ps)
{
	token_list_destroy(&ps->lookahead);
	buffer_destroy(&ps->qualifier);
}

bool get_lookahead_one(struct parse_state* ps)
{
    bool valid = true;

    if (ps->lookahead.head) {
        return valid;
    }

    struct token* t = NULL;
    while (true) {
        valid = scan_get_token(ps->sns, &t) && valid;
        if (t) {
            break;
        }
    }

    token_list_add(&ps->lookahead, t);

    return valid;
}

/* expecting specific token */
/* dynamic-output ps{} t t{} */
bool match(struct parse_state* ps, enum token_enum type, const char* reason, struct token** t)
{
	bool valid = true;
	struct location loc;

	int num = token_list_count(&ps->lookahead);
	int got_token;
	if (num <= 0) {
		/* allocate ps{sns{wt{} el{}}} t t{}*/
		valid = scan_get_token(ps->sns, t) && valid;
		if (!valid) {
			return valid;
		}
		/* transfer t t{} -> ps{lookahead{}} */
		token_list_add(&ps->lookahead, *t);
	} else {
		*t = ps->lookahead.head;
	}

	if ((*t)->type == type) {
		token_list_pop(&ps->lookahead);
		return valid;
	}

	valid = false;
	get_token_location(*t, &loc);
	/* alocate ps{el{}} */
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
    *loc = ps->lookahead.head->loc;
    return valid;
}

bool is_identity_comparison(enum ast_type type)
{
	return type == ast_type_equality || type == ast_type_not_equal;
}

struct token* get_lookahead(struct parse_state* ps)
{
    get_lookahead_one(ps);
    return ps->lookahead.head;
}
