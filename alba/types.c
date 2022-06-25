#include "buffer.h"
#include "result.h"
#include "token.h"
#include "scan.h"
#include "parse_tools.h"
#include "dag.h"

int is_valid_type(struct buffer* b)
{
	if (buffer_str_compare(b, "Int32")) return 1;
	if (buffer_str_compare(b, "Int64")) return 1;
	if (buffer_str_compare(b, "Float32")) return 1;
	if (buffer_str_compare(b, "Float64")) return 1;
	if (buffer_str_compare(b, "String")) return 1;
	return 0;
}

/*
* declaration -> id :: type | id
*/
enum result declaration(struct allocator* al, struct token_state* ts, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	int num;

	r = get_lookahead(al, ts, 3, &num);
	if (r == result_error) {
		return r;
	}

	struct token* t0 = get_token(&ts->lookahead, 0);
	struct token* t1 = get_token(&ts->lookahead, 1);
	struct token* t2 = get_token(&ts->lookahead, 2);

	if (t0 && t0->type == token_id && t1 && t1->type == token_double_colon && t2 && t2->type == token_id) {
		/* id::type */
		r = match(al, ts, token_id, "expected id");
		if (r == result_error) {
			return r;
		}

		r = match(al, ts, token_double_colon, "expected double colon");
		if (r == result_error) {
			return r;
		}

		r = match(al, ts, token_id, "expected type");
		if (r == result_error) {
			return r;
		}

		if (!is_valid_type(&t2->value)) {
			return set_error("unknown type");
		}

		r = dag_create_node(al, &n);
		if (r == result_error) {
			return r;
		}
		n->type = dag_type_declaration;

		struct dag_node* a = NULL;
		r = dag_create_node(al, &a);
		if (r == result_error) {
			return r;
		}
		a->type = dag_type_id;
		buffer_copy(al, &t0->value, &a->value);
		dag_add_child(n, a);

		struct dag_node* b = NULL;
		dag_create_node(al, &b);
		b->type = dag_type_type;
		buffer_copy(al, &t2->value, &b->value);
		dag_add_child(n, b);

	} else if (t0 && t0->type == token_id) {
		/* id */
		r = match(al, ts, token_id, "expected id");
		if (r == result_error) {
			return r;
		}

		r = dag_create_node(al, &n);
		if (r == result_error) {
			return r;
		}
		n->type = dag_type_declaration;

		struct dag_node* a = NULL;
		r = dag_create_node(al, &a);
		if (r == result_error) {
			return r;
		}
		a->type = dag_type_id;
		buffer_copy(al, &t0->value, &a->value);
		dag_add_child(n, a);
	}

	*root = n;
	return r;
}
