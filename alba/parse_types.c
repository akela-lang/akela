#include "zinc/buffer.h"
#include "zinc/result.h"
#include "token.h"
#include "scan.h"
#include "parse_tools.h"
#include "dag.h"
#include "source.h"

/* dseq->declaration dseq' | e */
/* dynamic-output ps{} root root{} */
enum result dseq(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;

	/* allocate n */
	dag_create_node(&n);

	n->type = dag_type_dseq;

	/* allocate a */
	struct dag_node* a = NULL;
	r = declaration(ps, &a);
	if (r == result_error) {
		dag_destroy(n);
		return r;
	}

	if (!a) {
		/* transfer n -> root */
		*root = n;
		return r;
	}

	/* transfer a -> n */
	dag_add_child(n, a);

	/* allocate ps{} n{} */
	r = dseq_prime(ps, n);
	if (r == result_error) {
		/* destroy n n{} */
		dag_destroy(n);
		return r;
	}

	/* transfer n -> root */
	*root = n;
	return r;
}


/* dseq' -> , declaration dseq' | e */
/* dynamic-output ps{} parent{} */
enum result dseq_prime(struct parse_state* ps, struct dag_node* parent)
{
	struct dag_node* n = NULL;
	enum result r = result_ok;
	int num;

	/* allocate ps{} */
	r = get_lookahead(ps, 2, &num);

	struct token* t0 = get_token(&ps->lookahead, 0);

	if (!t0 || t0->type != token_comma)
	{
		return r;
	}

	/* allocate ps{} comma comma{} */
	struct token* comma = NULL;
	r = match(ps, token_comma, "expecting comma", &comma);
	if (r == result_error) {
		return r;
	}

	/* destroy comma comma{} */
	token_destroy(comma);
	free(comma);

	struct location loc;
	r = get_parse_location(ps, &loc);
	if (r == result_error) {
		return r;
	}

	/* allocate a */
	struct dag_node* a = NULL;
	r = declaration(ps, &a);
	if (r == result_error) {
		/* destroy n n{} */
		dag_destroy(n);
		return r;
	}

	if (!a) {
		/* allocate ps{} */
		r = set_source_error(ps->el, &loc, "expecting declaration after comma");
		return r;
	}

	/* transfer a -> parent */
	dag_add_child(parent, a);

	/* allocate ps{} parent{} */
	r = dseq_prime(ps, parent);
	if (r == result_error) {
		return r;
	}

	return r;
}

/* dynamic-output-none */
int is_valid_type(struct buffer* b)
{
	if (buffer_str_compare(b, "Int32")) return 1;
	if (buffer_str_compare(b, "Int64")) return 1;
	if (buffer_str_compare(b, "Float32")) return 1;
	if (buffer_str_compare(b, "Float64")) return 1;
	if (buffer_str_compare(b, "String")) return 1;
	return 0;
}

/* declaration -> id :: type | id */
/* dynamic-output ps{} root root{} */
enum result declaration(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	int num;

	/* allocate ps{} */
	r = get_lookahead(ps, 3, &num);
	if (r == result_error) {
		return r;
	}

	struct token* t0 = get_token(&ps->lookahead, 0);
	struct token* t1 = get_token(&ps->lookahead, 1);
	struct token* t2 = get_token(&ps->lookahead, 2);

	if (t0 && t0->type == token_id && t1 && t1->type == token_double_colon && t2 && t2->type == token_id) {
		/* id::type */

		/* allocate ps{} id id{} */
		struct token* id = NULL;
		r = match(ps, token_id, "expected id", &id);
		if (r == result_error) {
			return r;
		}

		/* allocate ps{} dc dc{} */
		struct token* dc = NULL;
		r = match(ps, token_double_colon, "expected double colon", &dc);
		if (r == result_error) {
			return r;
		}

		/* destroy dc dc{} */
		token_destroy(dc);
		free(dc);

		struct token* type_id = NULL;
		r = match(ps, token_id, "expected type", &type_id);
		if (r == result_error) {
			return r;
		}

		if (!is_valid_type(&type_id->value)) {
			/* allocate a */
			char* a;
			buffer2array(&type_id->value, &a);
			struct location loc;
			get_token_location(type_id, &loc);
			r = set_source_error(ps->el, &loc, "unknown type: %s", a);

			/* destroy a */
			free(a);

			/* destroy id id{} type_id type_id{} */
			token_destroy(id);
			free(id);
			token_destroy(type_id);
			free(type_id);
			return r;
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

		/* allocate b */
		struct dag_node* b = NULL;
		dag_create_node(&b);
		b->type = dag_type_type;

		/* allocate b{} */
		buffer_copy(&type_id->value, &b->value);

		/* transfer b b{} -> n */
		dag_add_child(n, b);

		/* destroy id id{} type_id type_id{} */
		token_destroy(id);
		free(id);
		token_destroy(type_id);
		free(type_id);

	} else if (t0 && t0->type == token_id) {
		/* id */

		struct token* id = NULL;
		r = match(ps, token_id, "expected id", &id);
		if (r == result_error) {
			return r;
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

		/* destroy id id{} */
		token_destroy(id);
		free(id);
	}

	/* transfer n -> root */
	*root = n;
	return r;
}
