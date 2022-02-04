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
			if (r = error_result) {
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
			r = expression2(head->next, &second);
			if (r == error_result) {
				return r;
			}
			if (second) {
				dag_add_child(p, second);
			}
		}
	}
	*root = p;

	return ok_result;
}

enum result_enum expression2(struct token_node* head, struct dag_node** root)
{
	*root = NULL;
	return ok_result;
}

enum result_enum parse(struct token_list* tl, struct dag_node** root)
{
	*root = NULL;
	expression(tl->head, root);
	return ok_result;
}
