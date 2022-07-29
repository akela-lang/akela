#include <stdbool.h>
#include "zinc/buffer.h"
#include "zinc/result.h"
#include "token.h"
#include "scan.h"
#include "parse_tools.h"
#include "dag.h"
#include "source.h"

bool dseq_prime(struct parse_state* ps, struct dag_node* parent);
bool declaration(struct parse_state* ps, bool strict, struct dag_node** root);

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
	valid = valid && declaration(ps, true, &a);

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
	valid = valid && declaration(ps, true, &a);

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
bool declaration(struct parse_state* ps, bool strict, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;
	int num;

	/* allocate ps{} */
	valid = valid && get_lookahead(ps, 3, &num);

	struct token* t0 = get_token(&ps->lookahead, 0);
	struct token* t1 = get_token(&ps->lookahead, 1);
	struct token* t2 = get_token(&ps->lookahead, 2);

	if (t0 && t0->type == token_id && t1 && t1->type == token_double_colon && t2 && t2->type == token_id) {
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

		struct token* type_id = NULL;
		valid = valid && match(ps, token_id, "expected type", &type_id);
		if (!valid) {
			return valid;
		}

		if (!is_valid_type(&type_id->value)) {
			/* allocate a */
			char* a;
			buffer2array(&type_id->value, &a);
			struct location loc;
			get_token_location(type_id, &loc);
			set_source_error(ps->el, &loc, "unknown type: %s", a);
			valid = false;

			/* destroy a */
			free(a);

			/* destroy id id{} type_id type_id{} */
			token_destroy(id);
			free(id);
			token_destroy(type_id);
			free(type_id);
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

	} else if (!strict && t0 && t0->type == token_id) {
		/* id */

		struct token* id = NULL;
		valid = valid && match(ps, token_id, "expected id", &id);
		if (!valid) {
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

		/* destroy id id{} */
		token_destroy(id);
		free(id);
	}

	/* transfer n -> root */
	*root = n;
	return valid;
}
