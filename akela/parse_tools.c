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

/* get lookahead token */
/* dynamic ps{} */
/* dynamic-temp t t{} */
bool get_lookahead(struct parse_state* ps, int count, int* num)
{
	bool valid = true;
	int gt;
	struct token* t;

	*num = token_list_count(&ps->lookahead);
	if (*num >= count) {
		return valid;
	}

	for (; *num < count; (*num)++) {
		/* allocate sns{wt{} el{}} t t{}*/
		valid = scan_get_token(ps->sns, &gt, &t);
		if (!valid) {
			return valid;
		}
		if (!gt) {
			break;
		}

		/* transfer t t{} -> ps{lookahead{}} */
		token_list_add(&ps->lookahead, t);
	}

	return valid;
}

bool get_lookahead_one(struct parse_state* ps)
{
    bool valid = true;

    if (ps->lookahead.head) {
        return valid;
    }

    int gt;
    struct token* t = NULL;
    do {
        valid = scan_get_token(ps->sns, &gt, &t) && valid;
    } while (!valid || !gt || !t);

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
		valid = scan_get_token(ps->sns, &got_token, t) && valid;
		if (!valid) {
			return valid;
		}
		if (!got_token) {
			get_scan_location(ps->sns, &loc);
			/* dynamic-output ps{el{}} */
			set_source_error(ps->el, &loc, "%s", reason);
			valid = false;
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
        int num;
        valid = get_lookahead(ps, 1, &num) && valid;
        struct token* t0 = get_token(&ps->lookahead, 0);
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

/* dynamic-output ps{} */
bool get_parse_location(struct parse_state* ps, struct location* loc)
{
	bool valid = true;
	if (!ps->lookahead.head) {
		/* allocate ps{} */
		int num;
		valid = get_lookahead(ps, 1, &num) && valid;
	}

	struct token* t = get_token(&ps->lookahead, 0);
    //assert(t);

	if (!valid || !t) {
		loc->line = ps->sns->lc->line;
		loc->col = ps->sns->lc->col;
		loc->byte_pos = ps->sns->lc->byte_pos;
		return valid;
	}

	loc->line = t->loc.line;
	loc->col = t->loc.col;
	loc->byte_pos = t->loc.byte_pos;
	loc->size = t->loc.size;

	return valid;
}

bool is_identity_comparison(enum ast_type type)
{
	return type == ast_type_equality || type == ast_type_not_equal;
}

void location_update_token(struct location* loc, struct token* t)
{
	if (t && !loc->line) {
		*loc = t->loc;
	}
}

void location_update(struct location* loc, struct location* loc2)
{
	*loc = *loc2;
}

bool location_default(struct parse_state* ps, struct location* loc)
{
	bool valid = true;
	if (!loc->line) {
		valid = get_parse_location(ps, loc);
	}
	return valid;
}

bool ast_node_location_default(struct parse_state* ps, struct ast_node* n)
{
    bool valid = true;
    if (!n->loc.line) {
        valid = get_parse_location(ps, &n->loc);
    }
    return valid;
}