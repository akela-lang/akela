#include <stdbool.h>
#include "zinc/buffer.h"
#include "zinc/result.h"
#include "token.h"
#include "scan.h"
#include "parse_tools.h"
#include "dag.h"
#include "source.h"
#include "symbol_table.h"
#include "zinc/memory.h"

bool declaration(struct parse_state* ps, struct dag_node** root);
bool type(struct parse_state* ps, struct token* id, struct dag_node** root);

/* dseq -> declaration dseq' | e */
/* dseq' -> , declaration dseq' | e */
/* dynamic-output ps{} root root{} */
bool dseq(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	/* allocate n */
	dag_create_node(&n);
	n->type = dag_type_dseq;

	/* transfer n -> root */
	*root = n;

	/* allocate a */
	struct dag_node* dec = NULL;
	valid = declaration(ps, &dec) && valid;

	if (!dec) {
		return valid;
	}

	if (dec && valid) {
		/* transfer dec -> n */
		dag_add_child(n, dec);
	}

	while (true)
	{
		/* allocate ps{} */
		int num;
		valid = get_lookahead(ps, 2, &num) && valid;
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
		struct dag_node* dec = NULL;
		valid = declaration(ps, &dec) && valid;
		
		if (!dec || !valid) {
			/* allocate ps{} */
			set_source_error(ps->el, &loc, "expecting declaration after comma");
			valid = false;
			break;
		}

		if (dec && valid) {
			/* transfer dec -> n */
			dag_add_child(n, dec);
		}
	}

	return valid;
}

/* declaration -> id :: type | id */
/* dynamic-output ps{} root root{} */
bool declaration(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;
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

		struct dag_node* type_node = NULL;
		valid = type(ps, id, &type_node) && valid;

		if (!type_node) {
			valid = set_source_error(ps->el, &loc, "expected a type");
		}

		if (valid) {
			/* allocate n */
			dag_create_node(&n);
			n->type = dag_type_declaration;

			/* allocate a */
			struct dag_node* a = NULL;
			dag_create_node(&a);
			a->type = dag_type_id;

			/* allocate a{} */
			buffer_copy(&id->value, &a->value);

			/* transfer a a{} -> n */
			dag_add_child(n, a);

			/* transfer b b{} -> n */
			dag_add_child(n, type_node);

			/* transfer n -> root */
			*root = n;
		} else {
			dag_destroy(type_node);
		}

		token_destroy(id);
		free(id);

		/* destroy dc dc{} */
		token_destroy(dc);
		free(dc);
	}

	return valid;
}

bool type(struct parse_state* ps, struct token* id, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	int num;
	valid = get_lookahead(ps, 2, &num) && valid;
	struct token* t0 = get_token(&ps->lookahead, 0);
	struct token* t1 = get_token(&ps->lookahead, 1);

	if (t0 && t0->type == token_type_name && t1 && t1->type == token_left_curly_brace) {
		struct location loc;
		get_token_location(t0, &loc);
		char* a;
		buffer2array(&t0->value, &a);
		valid = set_source_error(ps->el, &loc, "type is not an array type: %s", a);
		free(a);
	} else if (t0 && t0->type == token_array_type_name && t1 && t1->type == token_left_curly_brace) {
		struct token* name = NULL;
		valid = match(ps, token_array_type_name, "expected array type name", &name) && valid;

		struct token* lcb = NULL;
		valid = match(ps, token_left_curly_brace, "expected left curly brace", &lcb) && valid;
		token_destroy(lcb);
		free(lcb);

		struct token* name2 = NULL;
		valid = match(ps, token_type_name, "expected type name", &name2) && valid;

		struct token* rcb = NULL;
		valid = match(ps, token_right_curly_brace, "expected right curly brace", &rcb) && valid;
		token_destroy(rcb);
		free(rcb);

		if (valid) {
			dag_create_node(&n);
			n->type = dag_type_array;

			struct dag_node* a = NULL;
			dag_create_node(&a);
			a->type = dag_type_array_type_name;
			buffer_copy(&name->value, &a->value);
			dag_add_child(n, a);

			struct dag_node* b = NULL;
			dag_create_node(&b);
			b->type = dag_type_type_name;
			buffer_copy(&name2->value, &b->value);
			dag_add_child(n, b);

			if (id) {
				struct symbol* search = environment_get_local(ps->st->top, &id->value);
				if (search) {
					struct location loc;
					get_token_location(id, &loc);
					char* a;
					buffer2array(&id->value, &a);
					valid = set_source_error(ps->el, &loc, "duplicate declaration in same scope: %s", a);
					free(a);
				} else {
					struct symbol* sym = NULL;
					malloc_safe((void**)&sym, sizeof(struct symbol));
					symbol_init(sym);
					sym->tk_type = id->type;
					sym->dec = n;
					environment_put(ps->st->top, &id->value, sym);
				}
			}

			*root = n;
		}

		/* destroy name name{} name2 name2{} */
		token_destroy(name);
		free(name);
		token_destroy(name2);
		free(name2);

	} else if (t0 && t0->type == token_type_name) {
		struct token* name = NULL;
		valid = match(ps, token_type_name, "expected type name", &name) && valid;

		if (valid) {
			dag_create_node(&n);
			n->type = dag_type_type_name;
			buffer_copy(&name->value, &n->value);

			if (id) {
				struct symbol* search = environment_get_local(ps->st->top, &id->value);
				if (search) {
					struct location loc;
					get_token_location(id, &loc);
					char* a;
					buffer2array(&id->value, &a);
					valid = set_source_error(ps->el, &loc, "duplicate declaration in same scope: %s", a);
					free(a);
				} else {
					struct symbol* sym = NULL;
					malloc_safe((void**)&sym, sizeof(struct symbol));
					symbol_init(sym);
					sym->tk_type = id->type;
					sym->dec = n;
					environment_put(ps->st->top, &id->value, sym);
				}
			}

			*root = n;
		}

		/* destroy name name{} */
		token_destroy(name);
		free(name);
	}

	return valid;
}

struct dag_node* af2etype(struct dag_node* n)
{
	struct dag_node* etype = NULL;
	dag_create_node(&etype);
	etype->type = dag_type_type_function;

	/* dseq */
	struct dag_node* dseq = dag_get_child(n, 0);
	struct dag_node* type_dseq = NULL;
	dag_create_node(&type_dseq);
	type_dseq->type = dag_type_type_dseq;
	struct dag_node* dec = dseq->head;
	while (dec) {
		struct dag_node* t = dag_get_child(dec, 1);
		dag_add_child(type_dseq, dag_copy(t));
		dec = dec->next;
	}
	dag_add_child(etype, type_dseq);

	/* dret */
	struct dag_node* dret = dag_get_child(n, 1);
	struct dag_node* type_dret = dag_copy(dret);
	type_dret->type = dag_type_type_dret;
	dag_add_child(etype, type_dret);

	return etype;
}
