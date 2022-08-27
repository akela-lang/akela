#include <stdbool.h>
#include "zinc/buffer.h"
#include "zinc/result.h"
#include "token.h"
#include "scan.h"
#include "parse_tools.h"
#include "ast.h"
#include "source.h"
#include "symbol_table.h"
#include "zinc/memory.h"
#include "parse_types.h"
#include "type_use.h"
#include <assert.h>

bool declaration(struct parse_state* ps, struct ast_node** root);
bool type(struct parse_state* ps, struct token* id, struct ast_node** root);
bool tseq(struct parse_state* ps, struct type_use* parent, struct location* loc);

/* dseq -> declaration dseq' | e */
/* dseq' -> , declaration dseq' | e */
/* dynamic-output ps{} root root{} */
bool dseq(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;

	/* allocate n */
	ast_node_create(&n);
	n->type = ast_type_dseq;

	/* transfer n -> root */
	*root = n;

	/* allocate a */
	struct ast_node* dec = NULL;
	valid = declaration(ps, &dec) && valid;

	if (!dec) {
		return valid;
	}

	if (dec && valid) {
		/* transfer dec -> n */
		ast_node_add(n, dec);
	}

	while (true)
	{
		/* allocate ps{} */
		int num;
		valid = get_lookahead(ps, 1, &num) && valid;
		struct token* t0 = get_token(&ps->lookahead, 0);
		if (!t0 || t0->type != token_comma) {
			break;
		}

		/* allocate ps{} comma comma{} */
		struct token* comma = NULL;
		valid = match(ps, token_comma, "expecting comma", &comma) && valid;

		token_destroy(comma);
		free(comma);

		struct location loc;
		valid = get_parse_location(ps, &loc) && valid;

		/* allocate a */
		struct ast_node* dec = NULL;
		valid = declaration(ps, &dec) && valid;
		
		if (!dec || !valid) {
			/* allocate ps{} */
			set_source_error(ps->el, &loc, "expecting declaration after comma");
			valid = false;
			break;
		}

		if (dec && valid) {
			/* transfer dec -> n */
			ast_node_add(n, dec);
		}
	}

	return valid;
}

/* declaration -> id :: type | id */
/* dynamic-output ps{} root root{} */
bool declaration(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;
	int num;

	/* allocate ps{} */
	valid = get_lookahead(ps, 1, &num) && valid;

	struct token* t0 = get_token(&ps->lookahead, 0);

	if (t0 && t0->type == token_id) {
		/* id::type */

		/* allocate ps{} id id{} */
		struct token* id = NULL;
		valid = match(ps, token_id, "expected id", &id) && valid;

		/* allocate ps{} dc dc{} */
		struct token* dc = NULL;
		valid = match(ps, token_double_colon, "expected double colon", &dc) && valid;

		struct location loc;
		valid = get_parse_location(ps, &loc) && valid;

		struct ast_node* type_use = NULL;
		valid = type(ps, id, &type_use) && valid;

		if (!type_use) {
			valid = set_source_error(ps->el, &loc, "expected a type");
		}

		if (valid) {
			/* allocate n */
			ast_node_create(&n);
			n->type = ast_type_declaration;

			/* allocate a */
			struct ast_node* a = NULL;
			ast_node_create(&a);
			a->type = ast_type_id;

			/* allocate a{} */
			buffer_copy(&id->value, &a->value);

			/* transfer a a{} -> n */
			ast_node_add(n, a);

			/* transfer b b{} -> n */
			ast_node_add(n, type_use);

			/* transfer n -> root */
			*root = n;
		} else {
			ast_node_destroy(type_use);
		}

		token_destroy(id);
		free(id);

		/* destroy dc dc{} */
		token_destroy(dc);
		free(dc);
	}

	return valid;
}

/* type -> id | id { tseq } */
bool type(struct parse_state* ps, struct token* id, struct ast_node** n)
{
	bool valid = true;
	struct type_use* tu = NULL;
	bool is_generic = false;

	int num;
	valid = get_lookahead(ps, 1, &num) && valid;
	struct token* t0 = get_token(&ps->lookahead, 0);

	ast_node_create(n);

	if (t0 && t0->type == token_id) {
		(*n)->type = ast_type_type;

		type_use_create(&tu);
		(*n)->tu = tu;

		struct token* name = NULL;
		valid = match(ps, token_id, "expected type name", &name) && valid;

		valid = get_lookahead(ps, 1, &num) && valid;
		struct token* t0 = get_token(&ps->lookahead, 0);
		if (t0 && t0->type == token_left_curly_brace) {
			is_generic = true;
			struct token* lcb = NULL;
			valid = match(ps, token_left_curly_brace, "expected left curly brace", &lcb) && valid;

			struct location loc_tseq;
			location_init(&loc_tseq);
			valid = tseq(ps, tu, &loc_tseq) && valid;

			struct token* rcb = NULL;
			valid = match(ps, token_right_curly_brace, "expected right curly brace", &rcb) && valid;
		}

		struct symbol* sym = NULL;
		if (valid) {
			sym = environment_get(ps->st->top, &name->value);
			if (!sym) {
				char* a;
				buffer2array(&name->value, &a);
				valid = set_source_error(ps->el, &name->loc, "type not defined: %s", a);
				free(a);
			} else if (!sym->td) {
				char* a;
				buffer2array(&name->value, &a);
				valid = set_source_error(ps->el, &name->loc, "identifier is not a type: %s", a);
				free(a);
			} else if (is_generic && !sym->td->is_generic) {
				char* a;
				buffer2array(&name->value, &a);
				valid = set_source_error(ps->el, &name->loc, "subtype was specified for non-generic type: %s", a);
				free(a);
			} else {
				if (is_generic) {
					int count = type_use_count_children(tu);
					if (sym->td->generic_count > 0 && count != sym->td->generic_count) {
						char* a;
						buffer2array(&name->value, &a);
						valid = set_source_error(
							ps->el, &name->loc, "generic type (%s) should have %d subtype%s but has %d subtype%s",
							a,
							sym->td->generic_count, plural(sym->td->generic_count),
							count, plural(count)
						);
						free(a);
					}
				}

				if (valid) {
					tu->td = sym->td;
				}
			}
		}

		if (valid) {
			if (id) {
				struct symbol* dup = environment_get_local(ps->st->top, &id->value);
				if (dup) {
					char* a;
					buffer2array(&id->value, &a);
					valid = set_source_error(ps->el, &id->loc, "duplicate declaration in same scope: %s", a);
					free(a);
				} else {
					struct symbol* sym = environment_get(ps->st->top, &id->value);
					if (sym && sym->td) {
						char* a;
						buffer2array(&id->value, &a);
						valid = set_source_error(ps->el, &id->loc, "identifier reserved as a type: %s", a);
						free(a);
					} else {
						struct symbol* new_sym = NULL;
						malloc_safe(&new_sym, sizeof(struct symbol));
						symbol_init(new_sym);
						new_sym->tk_type = token_id;
						new_sym->tu = tu;
						environment_put(ps->st->top, &id->value, new_sym);
					}
				}
			}
		}

		/* destroy name name{} */
		token_destroy(name);
		free(name);
	}

	valid = parse_ast_node_finish(ps, *n) && valid;

	return valid;
}

/* tseq -> type tseq' */
/* tseq' -> , tseq' */
bool tseq(struct parse_state* ps, struct type_use* parent, struct location* loc)
{
	bool valid = true;
	struct ast_node* a = NULL;
	struct token* t0 = NULL;

	valid = type(ps, NULL, &a) && valid;
	update_location(loc, &a->loc);

	if (a->empty) {
		valid = set_source_error(ps->el, &a->loc, "expected a type name");
	}

	if (valid) {
		assert(a);
		assert(a->tu);
		type_use_add(parent, a->tu);
		a->tu = NULL;
		ast_node_destroy(a);
		a = NULL;
	}

	while (true) {
		int num;
		valid = get_lookahead(ps, 1, &num) && valid;
		t0 = get_token(&ps->lookahead, 0);

		if (!t0 || t0->type != token_comma) {
			break;
		}

		struct token* comma = NULL;
		valid = match(ps, token_comma, "expected comma", &comma) && valid;

		token_destroy(comma);
		free(comma);

		struct ast_node* b = NULL;
		valid = type(ps, NULL, &b) && valid;

		if (b->empty) {
			valid = set_source_error(ps->el, &b->loc, "expected a type name after comma");
			break;
		}

		if (valid) {
			assert(b);
			assert(b->tu);
			type_use_add(parent, b->tu);
			b->tu = NULL;
			ast_node_destroy(b);
			b = NULL;
		}

	}

	return valid;
}

struct type_use* function2type(struct symbol_table* st, struct ast_node* n)
{
	struct buffer bf;
	int current_node = 0;

	if (n->type == ast_type_function) {
		current_node++;
	}

	/* function */
	struct type_use* tu = NULL;
	type_use_create(&tu);

	buffer_init(&bf);
	buffer_copy_str(&bf, "Function");
	struct symbol* sym = environment_get(st->top, &bf);
	assert(sym);
	assert(sym->td);
	tu->td = sym->td;

	/* input */
	struct type_use* input_tu = NULL;
	type_use_create(&input_tu);

	buffer_clear(&bf);
	buffer_copy_str(&bf, "Input");
	struct symbol* input_sym = environment_get(st->top, &bf);
	assert(input_sym);
	assert(input_sym->td);
	input_tu->td = input_sym->td;

	struct ast_node* dseq = ast_node_get(n, current_node++);
	struct ast_node* dec = dseq->head;
	while (dec) {
		struct ast_node* type_node = ast_node_get(dec, 1);
		struct type_use* element_tu = type_use_copy(type_node->tu);
		type_use_add(input_tu, element_tu);
		dec = dec->next;
	}

	if (input_tu->head) {
		type_use_add(tu, input_tu);
	} else {
		type_use_destroy(input_tu);
	}

	/* output */
	struct ast_node* dret = ast_node_get(n, current_node++);
	struct ast_node* dret_type = ast_node_get(dret, 0);

	if (dret_type) {
		buffer_clear(&bf);
		buffer_copy_str(&bf, "Output");
		struct symbol* output_sym = environment_get(st->top, &bf);
		assert(output_sym);
		assert(output_sym->td);
		struct type_use* output_tu = NULL;
		type_use_create(&output_tu);
		output_tu->td = output_sym->td;

		struct type_use* element_tu = type_use_copy(dret_type->tu);
		type_use_add(output_tu, element_tu);
		type_use_add(tu, output_tu);
	}

	buffer_destroy(&bf);

	return tu;
}

void check_return_type(struct parse_state* ps, struct ast_node* fd, struct ast_node* stmts_node, struct location* loc, bool *valid)
{
	assert(fd);
	assert(fd->tu);
	struct type_use* tu = fd->tu;
	struct type_use* p = tu->head;
	while (p) {
		struct type_def* p_td = p->td;
		if (p_td->type == type_function_output) {
			struct type_use* ret = type_use_get(p, 0);
			if (ret) {
				struct type_def* ret_td = ret->td;
				if (!type_use_can_cast(ret, stmts_node->tu)) {
					*valid = set_source_error(ps->el, loc, "returned type does not match function return type");
				}
			}
		}
		p = p->next;
	}
}

void get_function_children(struct type_use* tu, struct type_use** input, struct type_use** output)
{
	struct type_use* p = tu->head;
	while (p) {
		if (p->td) {
			if (p->td->type == type_function_input) {
				*input = p;
			} else if (p->td->type == type_function_output) {
				*output = p;
			}
		}
		p = p->next;
	}
}
