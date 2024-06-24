#include <stdbool.h>
#include "zinc/result.h"
#include "token.h"
#include "ast.h"
#include "parse_tools.h"
#include "parse_factor.h"
#include "lex.h"
#include "parse_types.h"
#include "parse_expr.h"
#include "type_def.h"
#include <assert.h>

Ast_node* parse_assignment(struct parse_state* ps, struct location* loc);
Ast_node* parse_eseq(struct parse_state* ps, size_t assign_index, struct location* loc);
Ast_node* parse_boolean(struct parse_state* ps, struct location* loc);
Ast_node* parse_comparison(struct parse_state* ps, struct location* loc);
Ast_node* parse_add(struct parse_state* ps, struct location* loc);
Ast_node* parse_mult(struct parse_state* ps, struct location* loc);
Ast_node* parse_power(struct parse_state* ps, struct location* loc);
Ast_node* parse_subscript(struct parse_state* ps, struct location* loc);
Ast_node* parse_call(struct parse_state* ps, struct location* loc);
Ast_node* parse_cseq(struct parse_state* ps, Ast_node* tu, struct location* loc);
Ast_node* parse_dot(struct parse_state* ps, struct location* loc);

/* expr -> assignment */
Ast_node* parse_expr(struct parse_state* ps, struct location* loc)
{
    return parse_assignment(ps, loc);
}

/* assignment -> eseq = assignment | eseq */
Ast_node* parse_assignment(struct parse_state* ps, struct location* loc)
{
	Ast_node* n = NULL;
    size_t assign_index = 0;

    get_location(ps, loc);

	Ast_node* a = NULL;
	struct location a_loc;
    Ast_node* a_last;

	while (true) {
        a_last = a;
		a = parse_eseq(ps, assign_index, &a_loc);

        if (!check_assignment_value_count(a, a_last)) {
            error_list_set(ps->el, &a_loc, "assignment sequence counts do not match");
            /* test case: test_parse_expr_assignment_eseq_error_eseq_count */
            n->type = Ast_type_error;
        }

        if (a) {
            a->loc = a_loc;
        }

        if (!a) {
            if (n) {
                error_list_set(ps->el, &a_loc, "missing rvalue in assignment");
                n->type = Ast_type_error;
            }
            break;
        }

        /* rvalue */
        if (a_last && a) {
            if (a->type == Ast_type_eseq) {
                Ast_node* p = a->head;
                while (p) {
                    if (!p->tu) {
                        error_list_set(ps->el, &p->loc, "rvalue does not have a type");
                    }
                    p = p->next;
                }
            } else {
                if (!a->tu) {
                    error_list_set(ps->el, &a_loc, "rvalue does not have a type");
                }
            }
        }

		struct token* t0 = get_lookahead(ps);
        if (t0->type != token_equal) {
            if (n) {
                /* last assignment */
                Ast_node_add(n, a);
                if (a->type == Ast_type_error) {
                    n->type = Ast_type_error;
                }
            } else {
                /* no assignment */
                n = a;
            }
            break;

        } else {
            if (!n) {
                /* start assign tree */
                Ast_node_create(&n);
                n->type = Ast_type_assign;
                n->tu = Ast_node_copy(a->tu);
            }

            Ast_node_add(n, a);

            struct token *equal = NULL;
            if (!match(ps, token_equal, "expecting assign operator", &equal, n)) {
                /* test case: no test case needed */
                assert(false);
            }

            if (a->type == Ast_type_eseq) {
                Ast_node* p = a->head;
                while (p) {
                    if (!p->tu) {
                        error_list_set(ps->el, &p->loc, "lvalue does not have a type");
                        n->type = Ast_type_error;
                    }
                    p = p->next;
                }
            } else {
                if (!a->tu) {
                    error_list_set(ps->el, &a_loc, "lvalue does not have a type");
                    n->type = Ast_type_error;
                }
            }

            token_destroy(equal);
            free(equal);

            consume_newline(ps, n);
        }
        assign_index++;
	}

    if (n && n->type == Ast_type_assign) {
        Ast_node* rhs = n->tail;
        Ast_node* lhs = n->head;
        Ast_node* prev_lhs = NULL;
        while (lhs && lhs != rhs) {
            if (lhs->type == Ast_type_eseq) {
                Ast_node* lhs2 = lhs->head;
                Ast_node* rhs2 = rhs->head;
                while (lhs2) {
                    if (!check_lvalue(ps, lhs2, loc)) {
                        n->type = Ast_type_error;
                    }
                    Override_rhs(lhs2->tu, rhs2);
                    if (!type_use_can_cast(lhs2->tu, rhs2->tu)) {
                        error_list_set(ps->el, &rhs2->loc, "values in assignment not compatible");
                    }
                    lhs2 = lhs2->next;
                    rhs2 = rhs2->next;
                }
            } else {
                if (!check_lvalue(ps, lhs, loc)) {
                    n->type = Ast_type_error;
                }

                if (!type_use_can_cast(lhs->tu, rhs->tu)) {
                    error_list_set(ps->el, &rhs->loc, "values in assignment not compatible");
                    n->type = Ast_type_error;
                }

                if (prev_lhs) {
                    if (lhs->tu->td != prev_lhs->tu->td) {
                        error_list_set(ps->el, &lhs->loc, "lvalues do not match type in assignment");
                        n->type = Ast_type_error;
                    }
                }
            }
            prev_lhs = lhs;
            lhs = lhs->next;
        }

        if (prev_lhs && prev_lhs->type != Ast_type_eseq) {
            Override_rhs(prev_lhs->tu, rhs);
        }
    }

	return n;
}

/* eseq = boolean eseq' */
/* eseq' = , boolean | e */
Ast_node* parse_eseq(struct parse_state* ps, size_t assign_index, struct location* loc)
{
    get_location(ps, loc);

    Ast_node* a = NULL;
    struct location a_loc;
    a = parse_boolean(ps, &a_loc);

    if (!a) {
        return NULL;
    }

    Ast_node* parent = NULL;
    while (true) {
        struct token* t0 = get_lookahead(ps);

        if (!t0 || t0->type != token_comma) {
            break;
        }

        if (!parent) {
            Ast_node_create(&parent);
            parent->type = Ast_type_eseq;
            Ast_node_add(parent, a);
            if (a->type == Ast_type_error) {
                parent->type = Ast_type_error;
            }

            if (parent->type != Ast_type_error) {
                if (!a->tu) {
                    error_list_set(ps->el, &a_loc, "operand of eseq has no type");
                    parent->type = Ast_type_error;
                }
            }
        }

        struct token* comma = NULL;
        if (!match(ps, token_comma, "expected a comma", &comma, parent)) {
            /* test case: no test case needed */
            assert(false);
        }
        token_destroy(comma);
        free(comma);

        Ast_node* b = NULL;
        struct location b_loc;
        b = parse_boolean(ps, &b_loc);
        if (b && b->type == Ast_type_error) {
            parent->type = Ast_type_error;
        }

        /* parent checks */
        if (!b) {
            error_list_set(ps->el, &b_loc, "expected term after comma");
            parent->type = Ast_type_error;
        }

        if (b) {
            if (!b->tu) {
                error_list_set(ps->el, &b_loc, "operand of eseq has no type");
                b->type = Ast_type_error;
            }
        }

        if (b) {
            Ast_node_add(parent, b);
            if (b->type == Ast_type_error) {
                parent->type = Ast_type_error;
            }
        }
    }

    if (parent == NULL) {
        parent = a;
    }
    return parent;
}

Ast_node* parse_simple_expr(struct parse_state* ps, struct location* loc)
{
    return parse_boolean(ps, loc);
}

/* boolean -> comparison boolean' */
/* boolean' -> && comparison boolean' | || comparison boolean' | e */
Ast_node* parse_boolean(struct parse_state* ps, struct location* loc)
{
    get_location(ps, loc);

	Ast_node* n = NULL;
	Ast_node* left;
    struct location left_loc;

	Ast_node* a = NULL;
	struct location a_loc;
	a = parse_comparison(ps, &a_loc);

	if (!a) {
		return NULL;
	}

	left = n = a;
    left_loc = a_loc;
	while (true) {
		struct token* t0 = get_lookahead(ps);

		/* operator */
		enum Ast_type type = Ast_type_none;
		if (t0 && t0->type == token_and) {
			type = Ast_type_and;
		} else if (t0 && t0->type == token_or) {
			type = Ast_type_or;
		} else {
			break;
		}

        Ast_node_create(&n);
		n->type = type;

		struct token* op = NULL;
		if (!match(ps, t0->type, "expecting && or ||", &op, n)) {
            /* test case: no test case needed */
            assert(false);
        }

        if (!consume_newline(ps, n)) {
            n->type = Ast_type_error;
        }

		/* comparison */
		Ast_node* b = NULL;
		struct location b_loc;
		b = parse_comparison(ps, &b_loc);

		if (!b) {
			error_list_set(ps->el, &b_loc, "expected term after && or ||");
			/* test case: test_parse_boolean_error_expected_term */
            n->type = Ast_type_error;
		}

        if (left) {
            Ast_node_add(n, left);
            if (left->type == Ast_type_error) {
                n->type = Ast_type_error;
            }
        }
        if (b) {
            Ast_node_add(n, b);
            if (b->type == Ast_type_error) {
                n->type = Ast_type_error;
            }
        }

		if (n->type != Ast_type_error) {
			assert(b);
			if (!left->tu) {
				error_list_set(ps->el, &left_loc, "left-side operand of boolean operator has no type");
				/* test case: test_parse_boolean_error_left_no_value */
                n->type = Ast_type_error;
			} else if (!b->tu) {
				error_list_set(ps->el, &b_loc, "operand of boolean operator has no type");
				/* test case: test_parse_boolean_error_right_no_value */
                n->type = Ast_type_error;
			} else if (left->tu->td->type != type_boolean) {
				error_list_set(ps->el, &left_loc, "left-side expression of boolean operator is not boolean");
                n->type = Ast_type_error;
			} else if (b->tu->td->type != type_boolean) {
				error_list_set(ps->el, &b_loc, "expression of boolean operator is not boolean");
                n->type = Ast_type_error;
			} else {
				n->tu = Ast_node_copy(left->tu);
			}

			left = n;
		}

		#pragma warning(suppress:6001)
		left_loc = op->loc;

		token_destroy(op);
		free(op);
	}

	return n;
}

/* comparison -> add comparison' */
/*
* comparison' -> == add comparison'
*			   | < add comparison'
*	           | <= add comparison'
*			   | > add comparison'
*	           | >= add comparison'
*	           | e
*/
Ast_node* parse_comparison(struct parse_state* ps, struct location* loc)
{
	Ast_node* n = NULL;
	Ast_node* a = NULL;
	Ast_node* left = NULL;
	int num;

	struct location a_loc;
	a = parse_add(ps, &a_loc);
    *loc = a_loc;

	if (!a) {
		return NULL;
	}

    left = n = a;

	struct location left_loc = a_loc;

	while (true) {
		struct token* t0 = get_lookahead(ps);
		enum Ast_type type = Ast_type_none;

		if (!t0) {
			break;
		}

		if (t0->type == token_double_equal) {
			type = Ast_type_equality;
		} else if (t0->type == token_not_equal) {
			type = Ast_type_not_equal;
		} else if (t0->type == token_less_than) {
			type = Ast_type_less_than;
		} else if (t0->type == token_less_than_or_equal) {
			type = Ast_type_less_than_or_equal;
		} else if (t0->type == token_greater_than) {
			type = Ast_type_greater_than;
		} else if (t0->type == token_greater_than_or_equal) {
			type = Ast_type_greater_than_or_equal;
		} else {
			break;
		}

		struct token* op = NULL;
		if (!match(ps, t0->type, "expecting comparator", &op, n)) {
            /* test case: no test case needed */
            n->type = Ast_type_error;
        }

        if (!consume_newline(ps, n)) {
            n->type = Ast_type_error;
        }

		Ast_node* b = NULL;
		struct location loc_b;
		b = parse_add(ps, &loc_b);

		if (!b) {
			error_list_set(ps->el, &loc_b, "expected term after comparison operator");
			/* case case: test_parse_comparison_error_no_term */
            n->type = Ast_type_error;
		}

        Ast_node_create(&n);
        n->type = type;

        if (left) {
            Ast_node_add(n, left);
            if (left->type == Ast_type_error) {
                n->type = Ast_type_error;
            }
        }

        if (b) {
            Ast_node_add(n, b);
            if (b->type == Ast_type_error) {
                n->type = Ast_type_error;
            }
        }

		if (n->type != Ast_type_error) {
			assert(a);
			assert(b);
			assert(op);
			if (!left->tu) {
				error_list_set(ps->el, &left_loc, "operand has no value");
				/* test case: test_parse_comparison_error_left_not_numeric */
                n->type = Ast_type_error;
			} else if (!b->tu) {
				error_list_set(ps->el, &loc_b, "operand has no value");
				/* test case: test_parse_comparison_error_right_no_value */
                n->type = Ast_type_error;
			} else {
				if (!is_identity_comparison(type) && !is_numeric(left->tu->td)) {
					error_list_set(ps->el, &left_loc, "comparison operand is not numeric");
					/* test case: test_parse_comparison_error_left_not_numeric */
                    n->type = Ast_type_error;
				} else if (!is_identity_comparison(type) && !is_numeric(b->tu->td)) {
					error_list_set(ps->el, &loc_b, "comparison operand is not numeric");
					/* test case: test_parse_comparison_error_right_not_numeric */
                    n->type = Ast_type_error;
				} else {
					Ast_node* tu = Ast_node_copy(left->tu);
					type_find_whole(ps->st, tu, b->tu);
					n->tu = tu;
				}
			}
			left = n;
			#pragma warning(suppress:6001)
			left_loc = op->loc;
		}

		token_destroy(op);
		free(op);
	}

	return n;
}

/* add -> mult add' */
/* add' -> + mult add' | - mult add' | e */
Ast_node* parse_add(struct parse_state* ps, struct location* loc)
{
	Ast_node* a = NULL;
	Ast_node* b = NULL;
	Ast_node* n = NULL;
	Ast_node* left = NULL;
	char* op_name;
	int num;

	struct location a_loc;
	a = parse_mult(ps, &a_loc);
    *loc = a_loc;

	if (!a) {
		return NULL;
	}

	left = n = a;

	while (true) {
		struct token* t0 = get_lookahead(ps);

		/* e */
		if (t0 == NULL) {
			break;
		}

		/* operator */
		enum Ast_type type;
		if (t0->type == token_plus) {
			type = Ast_type_plus;
			op_name = "addition";
		} else if (t0->type == token_minus) {
			type = Ast_type_minus;
			op_name = "subtraction";
		} else {
			break;
		}

        Ast_node_create(&n);
        n->type = type;

        struct token* op = NULL;
		if (!match(ps, t0->type, "expecting + or -", &op, n)) {
            /* test case: no test case needed */
            n->type = Ast_type_error;
        }

        if (!consume_newline(ps, n)) {
            n->type = Ast_type_error;
        }

		struct location b_loc;
		b = parse_mult(ps, &b_loc);

		if (!b) {
			error_list_set(ps->el, &b_loc, "expected term after additive operator");
			/* test case: test_parse_add_error_expected_term */
            n->type = Ast_type_error;
		}

        if (left) {
            Ast_node_add(n, left);
            if (left->type == Ast_type_error) {
                n->type = Ast_type_error;
            }
        }

        if (b) {
            Ast_node_add(n, b);
            if (b->type == Ast_type_error) {
                n->type = Ast_type_error;
            }
        }

		if (n->type != Ast_type_error) {
			Ast_node* tu_a = a->tu;
			Ast_node* tu_b = b->tu;

			if (!tu_a) {
				error_list_set(ps->el, &a_loc, "%s operand has no value", op_name);
				/* test case: test_parse_add_error_left_no_value */
                n->type = Ast_type_error;
			} else if (!is_numeric(tu_a->td)) {
				error_list_set(ps->el, &a_loc, "%s on non-numeric operand", op_name);
                n->type = Ast_type_error;
			}

			if (!tu_b) {
				error_list_set(ps->el, &b_loc, "%s operand has no value", op_name);
				/* test case: test_parse_add_error_right_no_value */
                n->type = Ast_type_error;
			} else if (!is_numeric(tu_b->td)) {
				error_list_set(ps->el, &b_loc, "%s on non-numeric operand", op_name);
				/* test case: test_parse_add_error_right_not_numeric */
                n->type = Ast_type_error;
			}

			if (n->type != Ast_type_error) {
				Ast_node* tu = Ast_node_copy(tu_a);
				if (!type_find_whole(ps->st, tu, tu_b)) {
					error_list_set(ps->el, &op->loc, "invalid types for %s", op_name);
					/* test case: no test case needed */
                    n->type = Ast_type_error;
				} else {
					n->tu = tu;
				}
			}

			left = n;
		}

		/* destroy op op{} */
		token_destroy(op);
		free(op);
	}

	return n;
}

/* mult -> power mult' */
/* mult' -> * power mult' | e */
Ast_node* parse_mult(struct parse_state* ps, struct location* loc)
{
	Ast_node* a = NULL;
	Ast_node* b = NULL;
	Ast_node* left = NULL;
	Ast_node* n = NULL;
	char* op_name;

	struct location a_loc;
	a = parse_power(ps, &a_loc);
    *loc = a_loc;

	if (!a) {
		return NULL;
	}

    left = n = a;

	while (true) {
		struct token* t0 = get_lookahead(ps);

		/* e */
		if (t0 == NULL) {
			break;
		}

		/* operator */
		enum Ast_type type;
		if (t0->type == token_mult) {
			type = Ast_type_mult;
			op_name = "multiplication";
		} else if (t0->type == token_divide) {
			type = Ast_type_divide;
			op_name = "division";
		} else {
			break;
		}

        Ast_node_create(&n);
        n->type = type;

        struct token* op = NULL;
		if (!match(ps, t0->type, "expecting * or /", &op, n)) {
            /* test case: test case not needed */
            assert(false);
        }

        if (!consume_newline(ps, n)) {
            n->type = Ast_type_error;
        }

		/* parse_factor */
		struct location b_loc;
		b = parse_power(ps, &b_loc);

		if (!b) {
			error_list_set(ps->el, &b_loc, "expected term after operator");
			/* test case: test_parse_mult_error_expected_term */
            n->type = Ast_type_error;
		}

        if (left) {
            Ast_node_add(n, left);
            if (left->type == Ast_type_error) {
                n->type = Ast_type_error;
            }
        }

        if (b) {
            Ast_node_add(n, b);
            if (b->type == Ast_type_error) {
                n->type = Ast_type_error;
            }
        }

		if (n->type != Ast_type_error) {
			assert(a);
			assert(b);
			Ast_node* tu_a = a->tu;
			Ast_node* tu_b = b->tu;

			if (!tu_a) {
				error_list_set(ps->el, &a_loc, "%s operand has no value", op_name);
				/* test case: test_parse_mult_error_left_no_value */
                n->type = Ast_type_error;
			} else if (!is_numeric(tu_a->td)) {
				error_list_set(ps->el, &a_loc, "%s on non-numeric operand", op_name);
				/* test case: test_parse_mult_error_left_not_numeric */
                n->type = Ast_type_error;
			}

			if (!tu_b) {
				error_list_set(ps->el, &b_loc, "%s operand has no value", op_name);
				/* test case: test_parse_mult_error_right_no_value*/
                n->type = Ast_type_error;
			} else if (!is_numeric(tu_b->td)) {
				error_list_set(ps->el, &b_loc, "%s on non-numeric operand", op_name);
				/* test case: test_parse_mult_error_right_not_numeric */
                n->type = Ast_type_error;
			}

			if (n->type != Ast_type_error) {
				Ast_node* tu = Ast_node_copy(tu_a);
				if (!type_find_whole(ps->st, tu, tu_b)) {
					error_list_set(ps->el, &op->loc, "invalid types for %s", op_name);
					/* test case: no test case needed */
                    n->type = Ast_type_error;
				} else {
					n->tu = tu;
				}
			}

			left = n;
		}

		token_destroy(op);
		free(op);

	}

	return n;
}

/* power -> subscript power' | e */
/* power' -> ^ subscript power' | e */
Ast_node* parse_power(struct parse_state* ps, struct location* loc)
{
	Ast_node* n = NULL;
	Ast_node* a = NULL;

	struct location a_loc;
	a = parse_subscript(ps, &a_loc);
    *loc = a_loc;

	if (!a) {
		return a;
	}

	Ast_node* left = n = a ;
	struct location loc_left = a_loc;

	while (true) {
		struct token* t0 = NULL;
		int num;
		t0 = get_lookahead(ps);
		if (!t0 || t0->type != token_caret) {
			break;
		}

        Ast_node_create(&n);
        n->type = Ast_type_power;
        if (left) {
            Ast_node_add(n, left);
            if (left->type == Ast_type_error) {
                n->type = Ast_type_error;
            }
        }

        struct token* caret = NULL;
		if (!match(ps, token_caret, "expected a caret", &caret, n)) {
            /* test case: no test case needed */
            assert(false);
        }

        if (!consume_newline(ps, n)) {
            n->type = Ast_type_error;
        }

		Ast_node* b = NULL;
		struct location b_loc;
		b = parse_subscript(ps, &b_loc);
        if (b && b->type == Ast_type_error) {
            n->type = Ast_type_error;
        }

        if (b) {
            Ast_node_add(n, b);
            if (b->type == Ast_type_error) {
                n->type = Ast_type_error;
            }
        }

        if (!b) {
            error_list_set(ps->el, &b_loc, "expected term after caret");
            /* test case: test_parse_power_error_expected_term */
            n->type = Ast_type_error;
        }

        if (n->type != Ast_type_error) {
			assert(left);
			assert(b);
			Ast_node* tu_left = left->tu;
			Ast_node* tu_b = b->tu;

			if (!tu_left) {
				error_list_set(ps->el, &loc_left, "power operand has no value");
				/* test case: test_parse_power_error_left_no_value */
                n->type = Ast_type_error;
			} else if (!is_numeric(tu_left->td)) {
				error_list_set(ps->el, &loc_left, "power on non-numeric operand");
				/* test case: test_parse_power_error_left_not_numeric */
                n->type = Ast_type_error;
			}

			if (!tu_b) {
				error_list_set(ps->el, &b_loc, "power operand has no value");
				/* test case: test_parse_power_error_right_no_value */
                n->type = Ast_type_error;
			} else if (!is_numeric(tu_b->td)) {
				error_list_set(ps->el, &b_loc, "power on non-numeric operand");
				/* test case: test_parse_power_error_right_not_numeric */
                n->type = Ast_type_error;
			}

			if (n->type != Ast_type_error) {
				Ast_node* tu = Ast_node_copy(tu_left);
				if (!type_find_whole(ps->st, tu, tu_b)) {
					error_list_set(ps->el, &b_loc, "invalid power types");
					/* test case: no test case needed */
                    n->type = Ast_type_error;
				} else {
					n->tu = tu;
				}
			}

			left = n;
			#pragma warning(suppress:6001)
			loc_left = caret->loc;
		}

		token_destroy(caret);
		free(caret);
	}

	return n;
}

/* parse_subscript -> call subscript' */
/* subscript' -> [expr] subscript' | e */
Ast_node* parse_subscript(struct parse_state* ps, struct location* loc)
{
	int num;
	Ast_node* n = NULL;
	Ast_node* a = NULL;

	struct location a_loc;
	a = parse_call(ps, &a_loc);
    *loc = a_loc;

    if (!a) {
        return a;
    }

	Ast_node* element_tu = NULL;
    struct location loc_last;
    Ast_node* left = a;
    struct location left_loc = a->loc;
    n = a;
	while (true) {
		struct token* t0 = get_lookahead(ps);

		if (!t0 || t0->type != token_left_square_bracket) {
			break;
		}

        if (!left->tu) {
            error_list_set(ps->el, &left_loc, "expression has subscript but has no value");
            left->type = Ast_type_error;
        }

        Ast_node_create(&n);
        n->type = Ast_type_array_subscript;
        Ast_node_add(n, left);
        if (left->type != Ast_type_error) {
            if (left->tu->to.is_array || left->tu->to.is_slice) {
                n->tu = Ast_node_copy(left->tu);
                Type_options_reduce_dimension(&n->tu->to);
            }
        }

        if (left->type == Ast_type_error) {
            n->type = Ast_type_error;
        }

        if (n->type != Ast_type_error) {
            if (!left->tu->to.is_array && !left->tu->to.is_slice) {
				error_list_set(ps->el,
                               &a_loc,
                               "expression has subscript but is not an array or slice");
				/* test case: test_parse_subscript_error_not_array */
                n->type = Ast_type_error;
			}
		}

		struct token* lsb = NULL;
		if (!match(ps, token_left_square_bracket, "expecting array subscript operator", &lsb, n)) {
            /* test case: no test case needed */
            assert(false);
        }
		if (lsb) {
            left_loc = lsb->loc;
		}

		token_destroy(lsb);
		free(lsb);

        if (!consume_newline(ps,  n)) {
            n->type = Ast_type_error;
        }

		Ast_node* b = NULL;
		struct location loc_expr;
        b = parse_expr(ps, &loc_expr);
		if (b && b->type == Ast_type_error) {
            n->type = Ast_type_error;
        }

        if (b) {
            left_loc = loc_expr;
        }

        if (!consume_newline(ps, n)) {
            n->type = Ast_type_error;
        }

		struct token* rsb = NULL;
		if (!match(ps, token_right_square_bracket, "expected right-square-bracket", &rsb, n)) {
            n->type = Ast_type_error;
        }

        if (rsb) {
            left_loc = rsb->loc;
        }

		token_destroy(rsb);
		free(rsb);

		if (b) {
            Ast_node_add(n, b);
		}

        left = n;
	}

	return n;
}

/* call -> dot call' */
/* call' -> ( cseq ) call' */
Ast_node* parse_call(struct parse_state* ps, struct location* loc)
{
	Ast_node* n = NULL;

	Ast_node* dot_node = NULL;
	struct location dot_loc;
	dot_node = parse_dot(ps, &dot_loc);
    *loc = dot_loc;

	if (!dot_node) {
		return dot_node;
	}

	Ast_node* left = n = dot_node;
	struct location left_loc = dot_loc;

	while (true) {
		struct token* t0 = get_lookahead(ps);

		if (!t0 || t0->type != token_left_paren) {
			break;
		}

        Ast_node_create(&n);
        n->type = Ast_type_call;

        struct token* lp = NULL;
		if (!match(ps, token_left_paren, "expected left parenthesis", &lp, n)) {
            /* test case: test case not needed */
            n->type = Ast_type_error;
        }

        if (!consume_newline(ps, n)) {
            n->type = Ast_type_error;
        }

		Ast_node* cseq_node = NULL;
		struct location loc_cseq;
		cseq_node = parse_cseq(ps, left->tu, &loc_cseq);
        if (cseq_node && cseq_node->type == Ast_type_error) {
            n->type = Ast_type_error;
        }

        if (!consume_newline(ps, n)) {
            n->type = Ast_type_error;
        }

		struct token* rp = NULL;
		if (!match(ps, token_right_paren, "expected right parenthesis", &rp, n)) {
            /* test case: test_parse_call_error_right_paren */
            n->type = Ast_type_error;
        }

        if (left) {
            Ast_node_add(n, left);
            if (left->type == Ast_type_error) {
                n->type = Ast_type_error;
            }
        }

        if (cseq_node) {
            Ast_node_add(n, cseq_node);
            if (cseq_node->type == Ast_type_error) {
                n->type = Ast_type_error;
            }
        }

		if (n->type != Ast_type_error) {
			Ast_node* tu = left->tu;
			assert(tu);
			assert(tu->td);
			struct type_def* td = tu->td;
			if (td->type != type_function) {
				error_list_set(ps->el, &left_loc, "not a function type");
				/* test case: test_parse_call_error_not_function */
                n->type = Ast_type_error;
			} else {
				Ast_node* dseq = NULL;
				Ast_node* dret = NULL;
				get_function_children(tu, &dseq, &dret);

				/* input */
				size_t tcount = 0;
                bool is_variadic = false;
				if (dseq) {
                    Ast_node* dec = dseq->head;
                    while (dec) {
                        if (dec->type == Ast_type_ellipsis) {
                            is_variadic = true;
                        } else {
                            tcount++;
                        }
                        dec = dec->next;
                    }
				}
				size_t ccount = 0;
				if (cseq_node) {
					ccount = Ast_node_count_children(cseq_node);
				}

				if (ccount < tcount) {
					error_list_set(ps->el, &rp->loc, "not enough arguments in function call");
					/* test case: test_parse_call_error_not_enough_arguments */
                    n->type = Ast_type_error;
				} else if (!is_variadic && ccount > tcount) {
					error_list_set(ps->el, &rp->loc, "too many arguments in function call");
					/* test case: test_parse_call_error_too_many_arguments */
                    n->type = Ast_type_error;
				}

				/* output */
				if (dret && dret->head) {
					n->tu = Ast_node_copy(dret->head);
				}
			}

			left = n;
			#pragma warning(suppress:6001)
			left_loc = rp->loc;
		}

		/* destroy id id{} lp lp{} rp rp{} */
		token_destroy(lp);
		free(lp);
		token_destroy(rp);
		free(rp);

	}

	return n;
}

/* cseq -> expr cseq' | e */
/* cseq' -> , expr cseq' | e */
Ast_node* parse_cseq(struct parse_state* ps, Ast_node* tu, struct location* loc)
{
	Ast_node* n = NULL;
    Ast_node_create(&n);
    n->type = Ast_type_cseq;

	if (!get_location(ps, loc)) {
        n->type = Ast_type_error;
    }

	if (!tu || !tu->td || tu->td->type != type_function) {
		error_list_set(ps->el, loc, "not a function type");
		/* test case: no test case needed */
        n->type = Ast_type_error;
		return n;
	}

	Ast_node* a = NULL;
	struct location loc_expr;
	a = parse_simple_expr(ps, &loc_expr);
    if (a && a->type == Ast_type_error) {
        n->type = Ast_type_error;
    }

	if (!a) {
		return n;
	}
	int i = 0;
	if (!check_input_type(ps, tu, i, a, &loc_expr)) {
        n->type = Ast_type_error;
    }
	i++;

    Ast_node_add(n, a);

	while (true) {
		struct token* t0 = get_lookahead(ps);

		if (!t0 || t0->type != token_comma) {
			break;
		}

		struct token* comma = NULL;
		if (!match(ps, token_comma, "expecting comma", &comma, n)) {
            /* test case: no test case needed */
            n->type = Ast_type_error;
        }

		token_destroy(comma);
		free(comma);

        if (!consume_newline(ps, n)) {
            n->type = Ast_type_error;
        }

		a = parse_simple_expr(ps, &loc_expr);
		if (a && a->type == Ast_type_error) {
            n->type = Ast_type_error;
        }

		if (!a) {
			error_list_set(ps->el, &loc_expr, "expected expression after comma");
			/* test case: test_parse_call_error_expected_expression */
            n->type = Ast_type_error;
		} else {
			/* transfer a -> parent */
            Ast_node_add(n, a);

			if (!check_input_type(ps, tu, i, a, &loc_expr)) {
                n->type = Ast_type_error;
            }
		}

		i++;
	}

	return n;
}

/* dot -> factor dot' */
/* dot' -> . factor dot' | e */
Ast_node* parse_dot(struct parse_state* ps, struct location* loc)
{
	Ast_node* n = NULL;
	Ast_node* a = NULL;

    get_location(ps, loc);

    struct location a_loc;
	a = parse_factor(ps, &a_loc);

	if (!a) {
		return a;
	}

	Ast_node* left = n = a;
	struct location left_loc = *loc;
	while (true) {
		struct token* t0 = NULL;
		t0 = get_lookahead(ps);
		if (t0->type != token_dot) {
			break;
		}

        Ast_node_create(&n);
        n->type = Ast_type_dot;

        struct token* dot = NULL;
		if (!match(ps, token_dot, "expected a dot", &dot, n)) {
            /* test case: no test case needed */
            n->type = Ast_type_error;
        }

        if (!consume_newline(ps, n)) {
            n->type = Ast_type_error;
        }

        struct token* id = NULL;
        if (!match(ps, token_id, "expected identifier", &id, n)) {
            n->type = Ast_type_error;
        }

        Ast_node* b = NULL;
        Ast_node_create(&b);
        b->type = Ast_type_id;
        if (id) {
            buffer_copy(&id->value, &b->value);
        }

        Ast_node_add(n, left);
        if (left->type == Ast_type_error) {
            n->type = Ast_type_error;
        }

        Ast_node_add(n, b);

		if (n->type != Ast_type_error) {
			if (!left->tu) {
				error_list_set(ps->el, &left_loc, "dot operand has no value");
				/* test case: no test case necessary */
                n->type = Ast_type_error;
			} else if (left->tu->td->type != type_struct) {
				error_list_set(ps->el, &left_loc, "dot operand is not a struct");
				/* test case: test_parse_dot_error_left_non_module */
                n->type = Ast_type_error;
			} else {
                struct type_def* td = left->tu->td;
                assert(td);
                struct symbol* sym = environment_get(ps->st->top, &td->name);
                assert(sym);
                //left->sym = sym;
                assert(sym->td);
                assert(sym->td->composite);
                bool found = false;
                struct Ast_node* dec = sym->td->composite->head;
                struct Ast_node* dec_id = NULL;
                struct Ast_node* dec_tu = NULL;
                while (dec) {
                    dec_id = Ast_node_get(dec, 0);
                    dec_tu = Ast_node_get(dec, 1);
                    if (buffer_compare(&dec_id->value, &b->value)) {
                        found = true;
                        break;
                    }
                    dec = dec->next;
                }
                if (!found) {
                    error_list_set(
                            ps->el,
                            &id->loc,
                            "identifier not a field of struct: %b", &id->value);
                    n->type = Ast_type_error;
                } else {
                    n->tu = Ast_node_copy(dec_tu);
                }
            }

			left = n;
			#pragma warning(suppress:6001)
            left_loc = dot->loc;
            token_destroy(dot);
            free(dot);
            token_destroy(id);
            free(id);
		}
	}

	return n;
}
