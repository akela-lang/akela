#include "zinc/buffer.h"
#include "zinc/result.h"
#include "token.h"
#include "scan.h"
#include "parse_tools.h"
#include "dag.h"
#include "source.h"

/**
 * @brief dseq -> declaration dseq' | e
 * @param dynamic-output root
 */
enum result dseq(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;

	/* allocate n */
	r = dag_create_node(&n);
	if (r == result_error) {
		goto function_error;
	}

	n->type = dag_type_dseq;

	struct dag_node* a = NULL;
	r = declaration(ps, &a);

	if (!a) {
		goto function_success;
	}

	dag_add_child(n, a);

	struct dag_node* b = NULL;
	r = dseq_prime(ps, &b);
	if (r == result_error) {
		goto function_error;
	}
	if (b && b->type == dag_type_dseq && b->head) {
		a->next = b->head;
		b->head->prev = a;
		n->tail = b->tail;
	}

function_success:
	*root = n;
	return r;

function_error:
	return r;
}

/**
 * @brief dseq' -> , declaration dseq' | e
 * @param dynamic-output root
 */
enum result dseq_prime(struct parse_state* ps, struct dag_node** root)
{
	struct dag_node* n = NULL;
	enum result r = result_ok;
	int num;
	struct token* t;

	r = get_lookahead(ps, 2, &num);
	struct token* t0 = get_token(&ps->lookahead, 0);

	if (!t0 || t0->type != token_comma)
	{
		goto function_success;
	}

	r = match(ps, token_comma, "expecting comma", &t);
	if (r == result_error) {
		goto function_error;
	}

	r = dag_create_node(&n);
	if (r == result_error) {
		goto function_error;
	}
	n->type = dag_type_dseq;

	struct dag_node* a = NULL;
	r = declaration(ps, &a);
	if (r == result_error) {
		goto function_error;
	}
	if (!a) {
		r = set_source_error(ps->el, t0, ps->sns->lc, "expecting declaration after comma");
		goto function_error;
	}
	dag_add_child(n, a);

	struct dag_node* b = NULL;
	r = dseq_prime(ps, &b);
	if (r == result_error) {
		goto function_error;
	}
	if (b && b->type == dag_type_dseq && b->head) {
		a->next = b->head;
		b->head->prev = a;
		n->tail = b->tail;
	}

function_success:
	*root = n;
	return r;

function_error:
	return r;
}

int is_valid_type(struct buffer* b)
{
	if (buffer_str_compare(b, "Int32")) return 1;
	if (buffer_str_compare(b, "Int64")) return 1;
	if (buffer_str_compare(b, "Float32")) return 1;
	if (buffer_str_compare(b, "Float64")) return 1;
	if (buffer_str_compare(b, "String")) return 1;
	return 0;
}

/**
 * @brief declaration -> id :: type | id
 * @param dynamic-output ps{} root
 */
enum result declaration(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	int num;
	struct token* t;

	r = get_lookahead(ps, 3, &num);
	if (r == result_error) {
		return r;
	}

	struct token* t0 = get_token(&ps->lookahead, 0);
	struct token* t1 = get_token(&ps->lookahead, 1);
	struct token* t2 = get_token(&ps->lookahead, 2);

	if (t0 && t0->type == token_id && t1 && t1->type == token_double_colon && t2 && t2->type == token_id) {
		/* id::type */
		r = match(ps, token_id, "expected id", &t);
		if (r == result_error) {
			return r;
		}

		r = match(ps, token_double_colon, "expected double colon", &t);
		if (r == result_error) {
			return r;
		}

		r = match(ps, token_id, "expected type", &t);
		if (r == result_error) {
			return r;
		}

		if (!is_valid_type(&t2->value)) {
			char* a;
			r = buffer2array(&t2->value, &a);
			return set_source_error(ps->el, t2, ps->sns->lc, "unknown type: %s", a);
		}

		r = dag_create_node(&n);
		if (r == result_error) {
			return r;
		}
		n->type = dag_type_declaration;

		struct dag_node* a = NULL;
		r = dag_create_node(&a);
		if (r == result_error) {
			return r;
		}
		a->type = dag_type_id;
		buffer_copy(&t0->value, &a->value);
		dag_add_child(n, a);

		struct dag_node* b = NULL;
		dag_create_node(&b);
		b->type = dag_type_type;
		buffer_copy(&t2->value, &b->value);
		dag_add_child(n, b);

	} else if (t0 && t0->type == token_id) {
		/* id */
		r = match(ps, token_id, "expected id", &t);
		if (r == result_error) {
			return r;
		}

		r = dag_create_node(&n);
		if (r == result_error) {
			return r;
		}
		n->type = dag_type_declaration;

		struct dag_node* a = NULL;
		r = dag_create_node(&a);
		if (r == result_error) {
			return r;
		}
		a->type = dag_type_id;
		buffer_copy(&t0->value, &a->value);
		dag_add_child(n, a);
	}

	*root = n;
	return r;
}
