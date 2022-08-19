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
bool tseq(struct parse_state* ps, struct type_use* parent);

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
bool type(struct parse_state* ps, struct token* id, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;
	struct type_use* tu = NULL;

	int num;
	valid = get_lookahead(ps, 1, &num) && valid;
	struct token* t0 = get_token(&ps->lookahead, 0);

	if (t0 && t0->type == token_id) {
		ast_node_create(&n);
		n->type = ast_type_type;

		type_use_create(&tu);
		n->tu = tu;

		struct location loc;
		valid = get_parse_location(ps, &loc) && valid;

		struct token* name = NULL;
		valid = match(ps, token_id, "expected type name", &name) && valid;

		valid = get_lookahead(ps, 1, &num) && valid;
		struct token* t0 = get_token(&ps->lookahead, 0);
		if (t0 && t0->type == token_left_curly_brace) {
			struct token* lcb = NULL;
			valid = match(ps, token_left_curly_brace, "expected left curly brace", &lcb) && valid;

			valid = tseq(ps, tu) && valid;

			struct token* rcb = NULL;
			valid = match(ps, token_right_curly_brace, "expected right curly brace", &rcb) && valid;
		}

		struct symbol* sym = NULL;
		if (valid) {
			sym = environment_get(ps->st->top, &name->value);
			if (!sym) {
				char* a;
				buffer2array(&name->value, &a);
				valid = set_source_error(ps->el, &loc, "type not defined", a);
				free(a);
			} else if (!sym->td) {
				char* a;
				buffer2array(&name->value, &a);
				valid = set_source_error(ps->el, &loc, "identifier is not a type", a);
				free(a);
			} else {
				ast_node_create(&n);
				n->type = ast_type_type;

				type_use_create(&tu);
				tu->td = sym->td;

				n->tu = tu;
			}
		}

		if (valid) {
			if (id) {
				struct symbol* dup = environment_get_local(ps->st->top, &id->value);
				if (dup) {
					char* a;
					buffer2array(&id->value, &a);
					valid = set_source_error(ps->el, &loc, "duplicate declaration in same scope: %s", a);
					free(a);
				} else {
					struct symbol* sym = environment_get(ps->st->top, &id->value);
					if (sym && sym->td) {
						char* a;
						buffer2array(&id->value, &a);
						valid = set_source_error(ps->el, &loc, "identifier reserved as a type: %s", a);
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

	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	return valid;
}

/* tseq -> type tseq' */
/* tseq' -> , tseq' */
bool tseq(struct parse_state* ps, struct type_use* parent)
{
	bool valid = true;
	struct ast_node* a = NULL;
	struct token* t0 = NULL;

	struct location loc;
	valid = get_parse_location(ps, &loc) && valid;

	valid = type(ps, NULL, &a) && valid;

	if (!a) {
		valid = set_source_error(ps->el, &loc, "expected a type name");
	}

	while (true) {
		int num;
		valid = get_lookahead(ps, 1, &num) && valid;
		t0 = get_token(&ps->lookahead, 0);

		if (!t0 || t0->type != token_comma) {
			break;
		}

		struct location loc;
		valid = get_parse_location(ps, &loc) && valid;

		valid = type(ps, NULL, &a) && valid;

		if (!a) {
			valid = set_source_error(ps->el, &loc, "expected a type name after comma");
		}

		if (valid) {
			assert(a);
			assert(a->tu);
			type_use_add(parent, a->tu);
			a->tu = NULL;
			ast_node_destroy(a);
		}

	}

	return valid;
}

struct ast_node* af2etype(struct ast_node* n)
{
	struct ast_node* etype = NULL;
	ast_node_create(&etype);
	etype->type = ast_type_type_function;

	/* dseq */
	struct ast_node* dseq = ast_node_get(n, 0);
	struct ast_node* type_dseq = NULL;
	ast_node_create(&type_dseq);
	type_dseq->type = ast_type_type_dseq;
	struct ast_node* dec = dseq->head;
	while (dec) {
		struct ast_node* t = ast_node_get(dec, 1);
		ast_node_add(type_dseq, ast_node_copy(t));
		dec = dec->next;
	}
	ast_node_add(etype, type_dseq);

	/* dret */
	struct ast_node* dret = ast_node_get(n, 1);
	struct ast_node* type_dret = ast_node_copy(dret);
	type_dret->type = ast_type_type_dret;
	ast_node_add(etype, type_dret);

	return etype;
}
