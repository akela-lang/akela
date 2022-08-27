#include <string.h>
#include <stdbool.h>
#include "input.h"
#include "parse_tools.h"
#include "token.h"
#include "zinc/result.h"
#include "scan.h"
#include "source.h"
#include "ast.h"

/* dynamic-output-none */
/* initialize-output ps ps{}*/
void parse_state_init(struct parse_state* ps, struct scan_state* sns, struct compile_error_list* el, struct symbol_table* st)
{
	ps->sns = sns;
	token_list_init(&ps->lookahead);
	ps->el = el;
	ps->st = st;
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
		if (lookahead_char_done(ps->sns->lc)) {
			break;
		}

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

/* expecting specific token */
/* dynamic-output ps{} t t{} */
bool match(struct parse_state* ps, enum token_enum type, char* reason, struct token** t)
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

	if (!valid || !t) {
		loc->line = ps->sns->lc->line;
		loc->col = ps->sns->lc->col;
		loc->byte_pos = ps->sns->lc->byte_pos;
		return valid;
	}

	loc->line = t->loc.line;
	loc->col = t->loc.col;
	loc->byte_pos = t->loc.byte_pos;
	loc->byte_count = t->loc.byte_count;

	return valid;
}

bool is_identity_comparison(enum ast_type type)
{
	return type == ast_type_equality || type == ast_type_not_equal;
}

void update_location_token(struct ast_node* n, struct token* t)
{
	if (t && !n->loc.line) {
		n->loc = t->loc;
	}
}

void update_location_ast_node(struct ast_node* n, struct ast_node* a)
{
	if (a && !n->loc.line) {
		n->loc = a->loc;
	}
}

void update_location(struct location* loc, struct location* loc2)
{
	if (loc2 && !loc->line) {
		*loc = *loc2;
	}
}

bool default_location(struct parse_state* ps, struct ast_node* n)
{
	bool valid = true;
	if (!n->loc.line) {
		valid = get_parse_location(ps, &n->loc);
	}
	return valid;
}

bool parse_ast_node_finish(struct parse_state* ps, struct ast_node* n)
{
	bool valid = true;

	valid = default_location(ps, n) && valid;

	if (!valid) {
		n->empty = true;
	}

	return valid;
}
