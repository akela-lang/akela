#include "result.h"
#include "token.h"
#include "scan.h"
#include "dag.h"
#include "defer.h"
#include "parse_misc.h"

/*
* expr -> term expr'
*/
enum result expr_defer(struct token_list* tl, struct dag_node** root)
{
	enum result r = result_ok;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* n = NULL;

	r = term_defer(tl, &a);
	if (r == result_error) {
		goto function_error;
	}

	r = defer(dag_destroy, a, &stack_error);
	if (r == result_error) {
		dag_destroy(a);
		goto function_error;
	}

	r = expr_prime_defer(tl, &b);
	if (r == result_error) {
		goto function_error;
	}

	r = defer(dag_destroy, b, &stack_error);
	if (r == result_error) {
		dag_destroy(b);
		goto function_error;
	}

	if (a == NULL) {
		set_error("expecting an expression");
		goto function_error;
	}
	if (b == NULL) {
		n = a;
		goto function_success;
	}
	if (b && is_binary_operator(b)) {
		n = b;
		dag_push(n, a);
		goto function_success;
	} else {
		dag_destroy(a);
		dag_destroy(b);
		goto function_error;
	}

function_success:
	*root = n;
	cleanup(stack_temp);
	cleanup_stack(stack_error);
	return r;

function_error:
	cleanup(stack_error);
	cleanup(stack_temp);
	return r;
}

/*
* expr' -> + term expr'
*	     | - term expr'
*	     | e
*/
enum result expr_prime_defer(struct token_list* tl, struct dag_node** root)
{
	enum result r = result_ok;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	enum dag_type type = dag_type_none;

	struct token* t0 = get_token(tl->head, 0);

	/* e */
	if (t0 == NULL) {
		goto function_success;
	}

	/* operator */
	if (t0->type == token_plus) {
		type = dag_type_plus;
	} else if (t0->type == token_minus) {
		type = dag_type_minus;
	}

	if (type == dag_type_none) {
		set_error("expecting expr'");
		goto function_error;
	}

	r = dag_create_node(&n);
	if (r == result_error) {
		goto function_error;
	}

	r = defer(dag_destroy, n, &stack_error);
	if (r == result_error) {
		dag_destroy(n);
		goto function_error;
	}

	n->type = type;
	struct token* tx = token_list_pop(tl);
	token_destroy(tx);

	/* term */
	r = term_defer(tl, &a);
	if (r == result_error) {
		goto function_error;
	}

	if (a) {
		r = defer(dag_destroy, a, &stack_error);
		if (r == result_error) {
			dag_destroy(a);
			goto function_error;
		}
	}

	/* expr' */
	r = expr_prime_defer(tl, &b);
	if (r == result_error) {
		goto function_error;
	}

	if (b) {
		r = defer(dag_destroy, b, &stack_error);
		if (r == result_error) {
			dag_destroy(b);
			goto function_error;
		}
	}

	if (a == NULL && b == NULL) {
		set_error("expecting expr'");
		goto function_error;
	}

	if (b && is_binary_operator(b)) {
		dag_push(b, a);
		dag_add_child(n, b);
		goto function_success;
	}

	if (a) {
		dag_add_child(n, a);
	}

	if (b) {
		dag_add_child(n, b);
	}
	
function_success:
	*root = n;
	cleanup(stack_temp);
	cleanup_stack(stack_error);
	return r;

function_error:
	cleanup(stack_error);
	cleanup(stack_temp);
	return r;
}

/*
* term -> factor term_prime
*/
enum result term_defer(struct token_list* tl, struct dag_node** root)
{
	enum result r = result_ok;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;

	r = factor_term(tl, &a);
	if (r == result_error) {
		goto function_error;
	}

	r = defer(dag_destroy, a, &stack_error);
	if (r == result_error) {
		dag_destroy(a);
		goto function_error;
	}

	r = term_prime_defer(tl, &b);
	if (r == result_error) {
		goto function_error;
	}

	r = defer(dag_destroy, b, &stack_error);
	if (r == result_error) {
		dag_destroy(b);
		goto function_error;
	}

	if (a == NULL && b == NULL) {
		set_error("expected term");
		goto function_error;
	}

	if (b == NULL) {
		n = a;
		goto function_success;
	}

	if (a == NULL) {
		n = b;
		goto function_success;
	}

	if (is_binary_operator(b)) {
		n = b;
		dag_push(n, a);
		goto function_success;
	}

	set_error("expected term");
	goto function_error;

function_success:
	*root = n;
	cleanup(stack_temp);
	cleanup_stack(stack_error);
	return r;

function_error:
	cleanup(stack_error);
	cleanup(stack_temp);
	return r;
}

/*
* term' -> * factor term'
*	     | / factor term'
*	     | e
*/
enum result term_prime_defer(struct token_list* tl, struct dag_node** root)
{
	enum result r = result_ok;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct token* t0 = get_token(tl->head, 0);
	enum dag_type type = dag_type_none;

	/* e */
	if (t0 == NULL) {
		goto function_success;
	}
	
	/* operator */
	if (t0->type == token_mult) {
		type = dag_type_mult;
	} else if (t0->type == token_divide) {
		type = dag_type_divide;
	}

	if (type == dag_type_none) {
		set_error("expecting expr'");
		goto function_error;
	}

	r = dag_create_node(&n);
	if (r == result_error) {
		goto function_error;
	}

	r = defer(dag_destroy, n, &stack_error);
	if (r == result_error) {
		dag_destroy(n);
		goto function_error;
	}

	n->type = type;
	struct token* t_op = token_list_pop(tl);
	token_destroy(t_op);

	/* factor */
	r = factor_term(tl, &a);
	if (r == result_error) {
		goto function_error;
	}

	if (a) {
		r = defer(dag_destroy, a, &stack_error);
		if (r == result_error) {
			dag_destroy(a);
			goto function_error;
		}
	}

	/* term' */
	r = term_prime_defer(tl, &b);
	if (r == result_error) {
		goto function_error;
	}

	if (b) {
		r = defer(dag_destroy, b, &stack_error);
		if (r == result_error) {
			dag_destroy(b);
			goto function_error;
		}
	}

	if (a == NULL && b == NULL) {
		set_error("expecting expr'");
		goto function_error;
	}

	if (b && is_binary_operator(b)) {
		dag_push(b, a);
		dag_add_child(n, b);
		goto function_success;
	}

	if (a) {
		dag_add_child(n, a);
	}

	if (b) {
		dag_add_child(n, b);
	}

function_success:
	*root = n;
	cleanup(stack_temp);
	cleanup_stack(stack_error);
	return r;

function_error:
	cleanup(stack_error);
	cleanup(stack_temp);
	return r;
}

/*
* factor -> number
*	| + number
*	| - number
*	| word
*	| + word
*	| - word
*	| (expr)
*/
enum result factor_term(struct token_list *tl, struct dag_node** root)
{
	enum result r = result_ok;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* n = NULL;

	struct token* t0;
	struct token* t1;
	t0 = get_token(tl->head, 0);
	t1 = get_token(tl->head, 1);

	/* number or word */
	if (t0->type == token_number || t0->type == token_word) {
		r = dag_create_node(&n);
		if (r == result_error) {
			goto function_error;
		}

		r = defer(dag_destroy, n, &stack_error);
		if (r == result_error) {
			dag_destroy(n);
			goto function_error;
		}

		if (t0->type == token_number) {
			n->type = dag_type_number;
		} else if (t0->type == token_word) {
			n->type = dag_type_word;
		}
		r = string_copy(&t0->value, &n->value);
		if (r == result_error) {
			goto function_error;
		}

		struct token* tx = token_list_pop(tl);
		if (tx) {
			token_destroy(tx);
		}
	}

	/* sign and number or word */
	else if ((t0->type == token_plus || t0->type == token_minus) && (t1->type == token_number || t1->type == token_word)) {
		r = dag_create_node(&n);
		if (r == result_error) {
			goto function_error;
		}

		r = defer(dag_destroy, n, &stack_error);
		if (r == result_error) {
			dag_destroy(n);
			goto function_error;
		}

		n->type = dag_type_sign;

		struct dag_node* left;
		r = dag_create_node(&left);
		if (r == result_error) {
			goto function_error;
		}

		r = defer(dag_destroy, left, &stack_error);
		if (r == result_error) {
			dag_destroy(left);
			goto function_error;
		}

		if (t0->type == token_plus) {
			left->type = dag_type_plus;
		} else {
			left->type = dag_type_minus;
		}

		dag_add_child(n, left);

		struct dag_node* right;
		r = dag_create_node(&right);
		if (r == result_error) {
			goto function_error;
		}

		r = defer(dag_destroy, right, &stack_error);
		if (r == result_error) {
			dag_destroy(right);
		}

		if (t1->type == token_number) {
			right->type = dag_type_number;
		} else {
			right->type = dag_type_word;
		}

		r = string_copy(&t1->value, &right->value);
		if (r == result_error) {
			goto function_error;
		}

		dag_add_child(n, right);

		struct token* tx = token_list_pop(tl);
		if (tx) {
			token_destroy(tx);
		}

		tx = token_list_pop(tl);
		if (tx) {
			token_destroy(tx);
		}

	/* parenthesis */
	} else if (t0 && t0->type == token_left_paren) {
		struct token* lp = token_list_pop(tl);
		token_destroy(lp);

		r = expr_defer(tl, &n);
		if (r == result_error) {
			goto function_error;
		}

		r = defer(dag_destroy, n, &stack_error);
		if (r == result_error) {
			dag_destroy(n);
			goto function_error;
		}

		struct token* rp = get_token(tl->head, 0);
		if (!rp || rp->type != token_right_paren) {
			set_error("expecting right parenthesis");
			goto function_error;
		}

		rp = token_list_pop(tl);
		goto function_success;
	}

function_success:
	*root = n;
	cleanup(stack_temp);
	cleanup_stack(stack_error);
	return r;

function_error:
	cleanup(stack_error);
	cleanup(stack_temp);
	return r;
}

enum result parse_defer(struct token_list* tl, struct dag_node** root)
{
	*root = NULL;
	enum result r = expr_defer(tl, root);
	if (r == result_error) {
		return r;
	}
	return result_ok;
}
