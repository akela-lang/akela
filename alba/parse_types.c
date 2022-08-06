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

bool dseq_prime(struct parse_state* ps, struct dag_node* parent);
bool declaration(struct parse_state* ps, struct dag_node** root);
bool type(struct parse_state* ps, struct token* id, struct dag_node** root);

/* dseq -> declaration dseq' | e */
/* dynamic-output ps{} root root{} */
bool dseq(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	/* allocate n */
	dag_create_node(&n);

	n->type = dag_type_dseq;

	/* allocate a */
	struct dag_node* a = NULL;
	valid = valid && declaration(ps, &a);

	if (a) {
		/* transfer a -> n */
		dag_add_child(n, a);

		/* allocate ps{} n{} */
		valid = valid && dseq_prime(ps, n);
	}

	if (valid) {
		/* transfer n -> root */
		*root = n;
	}

	return valid;
}


/* dseq' -> , declaration dseq' | e */
/* dynamic-output ps{} parent{} */
bool dseq_prime(struct parse_state* ps, struct dag_node* parent)
{
	bool valid = true;
	struct dag_node* n = NULL;
	int num;

	/* allocate ps{} */
	valid = valid && get_lookahead(ps, 2, &num);

	struct token* t0 = get_token(&ps->lookahead, 0);

	if (!t0 || t0->type != token_comma)
	{
		return valid;
	}

	/* allocate ps{} comma comma{} */
	struct token* comma = NULL;
	valid = valid && match(ps, token_comma, "expecting comma", &comma);

	/* destroy comma comma{} */
	token_destroy(comma);
	free(comma);

	struct location loc;
	valid = valid && get_parse_location(ps, &loc);

	/* allocate a */
	struct dag_node* a = NULL;
	valid = valid && declaration(ps, &a);

	if (!a) {
		/* allocate ps{} */
		set_source_error(ps->el, &loc, "expecting declaration after comma");
		valid = false;
		return valid;
	}

	/* transfer a -> parent */
	dag_add_child(parent, a);

	/* allocate ps{} parent{} */
	valid = valid && dseq_prime(ps, parent);

	return valid;
}

/* dynamic-output-none */
bool is_valid_type(struct buffer* b)
{
	if (buffer_str_compare(b, "Int32")) return true;
	if (buffer_str_compare(b, "Int64")) return true;
	if (buffer_str_compare(b, "Float32")) return true;
	if (buffer_str_compare(b, "Float64")) return true;
	if (buffer_str_compare(b, "String")) return true;
	return false;
}

/* declaration -> id :: type | id */
/* dynamic-output ps{} root root{} */
bool declaration(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;
	int num;

	/* allocate ps{} */
	valid = valid && get_lookahead(ps, 1, &num);

	struct token* t0 = get_token(&ps->lookahead, 0);

	if (t0 && t0->type == token_id) {
		/* id::type */

		/* allocate ps{} id id{} */
		struct token* id = NULL;
		valid = valid && match(ps, token_id, "expected id", &id);
		if (!valid) {
			return valid;
		}

		/* allocate ps{} dc dc{} */
		struct token* dc = NULL;
		valid = valid && match(ps, token_double_colon, "expected double colon", &dc);

		/* destroy dc dc{} */
		token_destroy(dc);
		free(dc);

		struct location loc;
		valid = valid && get_parse_location(ps, &loc);

		struct dag_node* b = NULL;
		valid = valid && type(ps, id, &b);

		if (!b) {
			set_source_error(ps->el, &loc, "expected a type");
			valid = false;
			token_destroy(id);
			free(id);
			return valid;
		}

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
		dag_add_child(n, b);

		/* destroy id id{} type_id type_id{} */
		token_destroy(id);
		free(id);

		/* transfer n -> root */
		*root = n;
	}

	return valid;
}

bool type(struct parse_state* ps, struct token* id, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	int num;
	valid = valid && get_lookahead(ps, 2, &num);
	struct token* t0 = get_token(&ps->lookahead, 0);
	struct token* t1 = get_token(&ps->lookahead, 1);

	if (t0 && t0->type == token_id && t1 && t1->type == token_left_curly_brace) {
		struct token* name = NULL;
		valid = valid && match(ps, token_id, "expected id", &name);

		struct token* lcb = NULL;
		valid = valid && match(ps, token_left_curly_brace, "expected left curly brace", &lcb);
		token_destroy(lcb);
		free(lcb);

		struct token* name2 = NULL;
		valid = valid && match(ps, token_id, "expected id", &name2);

		struct token* rcb = NULL;
		valid = valid && match(ps, token_right_curly_brace, "expected right curly brace", &rcb);
		token_destroy(rcb);
		free(rcb);

		if (valid) {
			dag_create_node(&n);
			n->type = dag_type_id;
			buffer_copy(&name->value, &n->value);

			struct dag_node* a = NULL;
			dag_create_node(&a);
			a->type = dag_type_id;
			buffer_copy(&name2->value, &a->value);

			dag_add_child(n, a);

			if (id) {
				struct symbol* search = environment_get_local(ps->top, &id->value);
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
					buffer_copy(&name->value, &sym->type);
					sym->dec = n;
					environment_put(ps->top, &id->value, sym);
				}
			}

			*root = n;
		}

		/* destroy name name{} name2 name2{} */
		token_destroy(name);
		free(name);
		token_destroy(name2);
		free(name2);

	} else if (t0 && t0->type == token_id) {
		struct token* name = NULL;
		valid = valid && match(ps, token_id, "expected type id", &name);

		if (valid) {
			dag_create_node(&n);
			n->type = dag_type_id;
			buffer_copy(&name->value, &n->value);

			if (id) {
				struct symbol* search = environment_get_local(ps->top, &id->value);
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
					buffer_copy(&name->value, &sym->type);
					sym->dec = n;
					environment_put(ps->top, &id->value, sym);
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
