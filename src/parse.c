#include <stdlib.h>
#include "dag.h"
#include "result.h"
#include "defer.h"
#include "scan.h"

enum result_enum expression(struct token_node* head, struct dag_node** root)
{
	enum result_enum r;

	struct token* t0;
	struct token* t1;
	struct dag_node* p = NULL;
	struct dag_node* first;
	struct dag_node* second;

	t0 = get_token(head, 0);
	t1 = get_token(head, 1);

	if (t0 != NULL && (t0->type == token_word || t0->type == token_number)) {
		if (t1 != NULL && (t1->type == token_plus || t1->type == token_minus)) {
			r = dag_create_node(&p);
			if (r == error_result) {
				return r;
			}
			if (t1->type == token_plus) {
				p->type = dag_type_plus;
			} else if (t1->type == token_minus) {
				p->type = dag_type_minus;
			}

			r = dag_create_node(&first);
			if (r == error_result) {
				return r;
			}
			if (t0->type == token_word) {
				first->type = dag_type_word;
			} else if (t0->type == token_number) {
				first->type = dag_type_number;
			}
			string_copy(&t0->value, &first->value);

			dag_add_child(p, first);
			r = expression2(head->next->next, &second);
			if (r == error_result) {
				return r;
			}
			if (second) {
				dag_add_child(p, second);
			}
		}
	}

	if (!p) {
		return set_error("not a valid expression");
	}

	*root = p;

	return ok_result;
}

enum result_enum expression2(struct token_node* head, struct dag_node** root)
{
	enum result_enum r;
	struct dag_node* p = NULL;
	struct token* t0 = get_token(head, 0);
	struct token* t1 = get_token(head, 1);

	if (t0 && t1) {
		if (t0->type == token_word || t0->type == token_number) {
			if (t1->type == token_mult || t1->type == token_divide) {
				r = dag_create_node(&p);
				if (r == error_result) {
					return r;
				}
				if (t1->type == token_mult) {
					p->type = dag_type_mult;
				} else if (t1->type == token_divide) {
					p->type = dag_type_divide;
				}

				struct dag_node* first;
				r = dag_create_node(&first);
				if (r == error_result) {
					return r;
				}
				if (t0->type == token_word) {
					first->type = dag_type_word;
				} else if (t0->type == token_number) {
					first->type = dag_type_number;
				}
				string_copy(&first->value, &t0->value);

				struct dag_node* second;
				r = expression2(head, &second);
				if (r == error_result) {
					return r;
				}

				dag_add_child(p, first);
				dag_add_child(p, second);
			}
		}
	} else if (t0) {
		if (t0->type == token_word || t0->type == token_number) {
			r = dag_create_node(&p);
			if (r == error_result) {
				return r;
			}
			if (t0->type == token_word) {
				p->type = dag_type_word;
			} else if (t0->type == token_number) {
				p->type = dag_type_number;
			}
		}
	}

	if (!p) {
		return set_error("not a valid expression2");
	}

	*root = p;
	return ok_result;
}

enum result_enum parse(struct token_list* tl, struct dag_node** root)
{
	*root = NULL;
	enum result_enum r = expression(tl->head, root);
	if (r == error_result) {
		return r;
	}
	return ok_result;
}
