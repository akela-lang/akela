#include <stdbool.h>
#include "token.h"
#include "ast.h"
#include "parse_tools.h"
#include "parse_factor.h"
#include "lex.h"
#include "parse_types.h"
#include "parse_expr.h"
#include "type_def.h"
#include <assert.h>
#include "symbol.h"

Ake_ast* parse_assignment(struct parse_state* ps);
Ake_ast* parse_eseq(struct parse_state* ps);
Ake_ast* parse_boolean(struct parse_state* ps);
Ake_ast* parse_comparison(struct parse_state* ps);
Ake_ast* parse_add(struct parse_state* ps);
Ake_ast* parse_mult(struct parse_state* ps);
Ake_ast* parse_power(struct parse_state* ps);
Ake_ast* parse_complex_operators(struct parse_state* ps);
void parse_subscript(struct parse_state* ps, Ake_ast* left, Ake_ast* n);
void parse_call(struct parse_state* ps, Ake_ast* left, Ake_ast* n);
Ake_ast* parse_cseq(struct parse_state* ps, Ake_ast* left);
Ake_ast* parse_dot(struct parse_state* ps);

/* expr -> assignment */
Ake_ast* parse_expr(struct parse_state* ps)
{
    return parse_assignment(ps);
}

/* assignment -> eseq = assignment | eseq */
Ake_ast* parse_assignment(struct parse_state* ps)
{
	Ake_ast* n = NULL;

	Ake_ast* a = NULL;
    Ake_ast* a_last;

	while (true) {
        a_last = a;
		a = parse_eseq(ps);

        if (!check_assignment_value_count(a, a_last)) {
            error_list_set(ps->el, &a->loc, "assignment sequence counts do not match");
            /* test case: test_parse_expr_assignment_eseq_error_eseq_count */
            n->type = Ake_ast_type_error;
        }

        if (!a) {
            if (n) {
                error_list_set(ps->el, &a->loc, "missing rvalue in assignment");
                n->type = Ake_ast_type_error;
            }
            break;
        }

        /* rvalue */
        if (a_last) {
            if (a->type == Ake_ast_type_eseq) {
                Ake_ast* p = a->head;
                while (p) {
                    if (!p->tu) {
                        error_list_set(ps->el, &p->loc, "rvalue does not have a type");
                    }
                    p = p->next;
                }
            } else {
                if (!a->tu) {
                    error_list_set(ps->el, &a->loc, "rvalue does not have a type");
                }
            }
        }

		struct token* t0 = get_lookahead(ps);
        if (t0->type != token_equal) {
            if (n) {
                /* last assignment */
                Ake_ast_add(n, a);
                if (a->type == Ake_ast_type_error) {
                    n->type = Ake_ast_type_error;
                }
            } else {
                /* no assignment */
                n = a;
            }
            break;

        } else {
            if (!n) {
                /* start assign tree */
                Ake_ast_create(&n);
                n->type = Ake_ast_type_assign;
                n->tu = Type_use_clone(a->tu);
            }

            Ake_ast_add(n, a);

            struct token *equal = NULL;
            if (!match(ps, token_equal, "expecting assign operator", &equal, n)) {
                /* test case: no test case needed */
                assert(false);
            }

            if (a->type == Ake_ast_type_eseq) {
                Ake_ast* p = a->head;
                while (p) {
                    if (!p->tu) {
                        error_list_set(ps->el, &p->loc, "lvalue does not have a type");
                        n->type = Ake_ast_type_error;
                    }
                    p = p->next;
                }
            } else {
                if (!a->tu) {
                    error_list_set(ps->el, &a->loc, "lvalue does not have a type");
                    n->type = Ake_ast_type_error;
                }
            }

            token_destroy(equal);
            free(equal);

            consume_newline(ps, n);
        }
	}

    if (n && n->type == Ake_ast_type_assign) {
        Ake_ast* rhs = n->tail;
        Ake_ast* lhs = n->head;
        Ake_ast* prev_lhs = NULL;
        while (lhs && lhs != rhs) {
            if (lhs->type == Ake_ast_type_eseq) {
                Ake_ast* lhs2 = lhs->head;
                Ake_ast* rhs2 = rhs->head;
                while (lhs2) {
                    if (!check_lvalue(ps, lhs2, &n->loc)) {
                        n->type = Ake_ast_type_error;
                    }
                    Override_rhs(lhs2->tu, rhs2);
                    if (!type_use_can_cast(lhs2->tu, rhs2->tu)) {
                        error_list_set(ps->el, &rhs2->loc, "values in assignment not compatible");
                    }
                    lhs2 = lhs2->next;
                    rhs2 = rhs2->next;
                }
            } else {
                if (!check_lvalue(ps, lhs, &n->loc)) {
                    n->type = Ake_ast_type_error;
                }

                if (!type_use_can_cast(lhs->tu, rhs->tu)) {
                    error_list_set(ps->el, &rhs->loc, "values in assignment not compatible");
                    n->type = Ake_ast_type_error;
                }

                if (prev_lhs) {
                    if (lhs->tu->td != prev_lhs->tu->td) {
                        error_list_set(ps->el, &lhs->loc, "lvalues do not match type in assignment");
                        n->type = Ake_ast_type_error;
                    }
                }
            }
            prev_lhs = lhs;
            lhs = lhs->next;
        }

        if (prev_lhs && prev_lhs->type != Ake_ast_type_eseq) {
            Override_rhs(prev_lhs->tu, rhs);
        }
    }

	return n;
}

/* eseq = boolean eseq' */
/* eseq' = , boolean | e */
Ake_ast* parse_eseq(struct parse_state* ps)
{
    Ake_ast* a = NULL;
    a = parse_boolean(ps);

    if (!a) {
        return NULL;
    }

    Ake_ast* parent = NULL;
    while (true) {
        struct token* t0 = get_lookahead(ps);

        if (!t0 || t0->type != token_comma) {
            break;
        }

        if (!parent) {
            Ake_ast_create(&parent);
            parent->type = Ake_ast_type_eseq;
            Ake_ast_add(parent, a);
            if (a->type == Ake_ast_type_error) {
                parent->type = Ake_ast_type_error;
            }

            if (parent->type != Ake_ast_type_error) {
                if (!a->tu) {
                    error_list_set(ps->el, &a->loc, "operand of eseq has no type");
                    parent->type = Ake_ast_type_error;
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

        Ake_ast* b = NULL;
        b = parse_boolean(ps);
        if (b && b->type == Ake_ast_type_error) {
            parent->type = Ake_ast_type_error;
        }

        /* parent checks */
        if (!b) {
            error_list_set(ps->el, &b->loc, "expected term after comma");
            parent->type = Ake_ast_type_error;
        }

        if (b) {
            if (!b->tu) {
                error_list_set(ps->el, &b->loc, "operand of eseq has no type");
                b->type = Ake_ast_type_error;
            }
        }

        if (b) {
            Ake_ast_add(parent, b);
            if (b->type == Ake_ast_type_error) {
                parent->type = Ake_ast_type_error;
            }
        }
    }

    if (parent == NULL) {
        parent = a;
    }
    return parent;
}

Ake_ast* parse_simple_expr(struct parse_state* ps)
{
    return parse_boolean(ps);
}

/* boolean -> comparison boolean' */
/* boolean' -> && comparison boolean' | || comparison boolean' | e */
Ake_ast* parse_boolean(struct parse_state* ps)
{
	Ake_ast* n = NULL;
	Ake_ast* left;

	Ake_ast* a = NULL;
	a = parse_comparison(ps);

	if (!a) {
		return NULL;
	}

	left = n = a;
	while (true) {
		struct token* t0 = get_lookahead(ps);

		/* operator */
		enum Ake_ast_type type;
		if (t0 && t0->type == token_and) {
			type = Ake_ast_type_and;
		} else if (t0 && t0->type == token_or) {
			type = Ake_ast_type_or;
		} else {
			break;
		}

        Ake_ast_create(&n);
		n->type = type;

		struct token* op = NULL;
		if (!match(ps, t0->type, "expecting && or ||", &op, n)) {
            /* test case: no test case needed */
            assert(false);
        }

        if (!consume_newline(ps, n)) {
            n->type = Ake_ast_type_error;
        }

		/* comparison */
		Ake_ast* b = NULL;
		b = parse_comparison(ps);

		if (!b) {
            struct location b_loc = get_location(ps);
			error_list_set(ps->el, &b_loc, "expected term after && or ||");
			/* test case: test_parse_boolean_error_expected_term */
            n->type = Ake_ast_type_error;
		}

        if (left) {
            Ake_ast_add(n, left);
            if (left->type == Ake_ast_type_error) {
                n->type = Ake_ast_type_error;
            }
        }
        if (b) {
            Ake_ast_add(n, b);
            if (b->type == Ake_ast_type_error) {
                n->type = Ake_ast_type_error;
            }
        }

		if (n->type != Ake_ast_type_error) {
			assert(b);
            assert(left);
			if (!left->tu) {
				error_list_set(ps->el, &left->loc, "left-side operand of boolean operator has no type");
				/* test case: test_parse_boolean_error_left_no_value */
                n->type = Ake_ast_type_error;
			} else if (!b->tu) {
				error_list_set(ps->el, &b->loc, "operand of boolean operator has no type");
				/* test case: test_parse_boolean_error_right_no_value */
                n->type = Ake_ast_type_error;
			} else if (left->tu->td->type != type_boolean) {
				error_list_set(ps->el, &left->loc, "left-side expression of boolean operator is not boolean");
                n->type = Ake_ast_type_error;
			} else if (b->tu->td->type != type_boolean) {
				error_list_set(ps->el, &b->loc, "expression of boolean operator is not boolean");
                n->type = Ake_ast_type_error;
			} else {
				n->tu = Type_use_clone(left->tu);
			}

			left = n;
		}

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
Ake_ast* parse_comparison(struct parse_state* ps)
{
	Ake_ast* n = NULL;
	Ake_ast* a = NULL;
	Ake_ast* left = NULL;

	a = parse_add(ps);

	if (!a) {
		return NULL;
	}

    left = n = a;

	while (true) {
		struct token* t0 = get_lookahead(ps);
		enum Ake_ast_type type;

		if (!t0) {
			break;
		}

		if (t0->type == token_double_equal) {
			type = Ake_ast_type_equality;
		} else if (t0->type == token_not_equal) {
			type = Ake_ast_type_not_equal;
		} else if (t0->type == token_less_than) {
			type = Ake_ast_type_less_than;
		} else if (t0->type == token_less_than_or_equal) {
			type = Ake_ast_type_less_than_or_equal;
		} else if (t0->type == token_greater_than) {
			type = Ake_ast_type_greater_than;
		} else if (t0->type == token_greater_than_or_equal) {
			type = Ake_ast_type_greater_than_or_equal;
		} else {
			break;
		}

		struct token* op = NULL;
		if (!match(ps, t0->type, "expecting comparator", &op, n)) {
            /* test case: no test case needed */
            n->type = Ake_ast_type_error;
        }

        if (!consume_newline(ps, n)) {
            n->type = Ake_ast_type_error;
        }

		Ake_ast* b = NULL;
		b = parse_add(ps);

		if (!b) {
            struct location b_loc = get_location(ps);
			error_list_set(ps->el, &b_loc, "expected term after comparison operator");
			/* case case: test_parse_comparison_error_no_term */
            n->type = Ake_ast_type_error;
		}

        Ake_ast_create(&n);
        n->type = type;

        if (left) {
            Ake_ast_add(n, left);
            if (left->type == Ake_ast_type_error) {
                n->type = Ake_ast_type_error;
            }
        }

        if (b) {
            Ake_ast_add(n, b);
            if (b->type == Ake_ast_type_error) {
                n->type = Ake_ast_type_error;
            }
        }

		if (n->type != Ake_ast_type_error) {
			assert(a);
			assert(b);
			assert(op);
            assert(left);
			if (!left->tu) {
				error_list_set(ps->el, &left->loc, "operand has no value");
				/* test case: test_parse_comparison_error_left_not_numeric */
                n->type = Ake_ast_type_error;
			} else if (!b->tu) {
				error_list_set(ps->el, &b->loc, "operand has no value");
				/* test case: test_parse_comparison_error_right_no_value */
                n->type = Ake_ast_type_error;
			} else {
				if (!is_identity_comparison(type) && !is_numeric(left->tu->td)) {
					error_list_set(ps->el, &left->loc, "comparison operand is not numeric");
					/* test case: test_parse_comparison_error_left_not_numeric */
                    n->type = Ake_ast_type_error;
				} else if (!is_identity_comparison(type) && !is_numeric(b->tu->td)) {
					error_list_set(ps->el, &b->loc, "comparison operand is not numeric");
					/* test case: test_parse_comparison_error_right_not_numeric */
                    n->type = Ake_ast_type_error;
				} else {
					Type_use* tu = Type_use_clone(left->tu);
					type_find_whole(ps->st, tu, b->tu);
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

/* add -> mult add' */
/* add' -> + mult add' | - mult add' | e */
Ake_ast* parse_add(struct parse_state* ps)
{
	Ake_ast* a = NULL;
	Ake_ast* b = NULL;
	Ake_ast* n = NULL;
	Ake_ast* left = NULL;
	char* op_name;

	a = parse_mult(ps);

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
		enum Ake_ast_type type;
		if (t0->type == token_plus) {
			type = Ake_ast_type_plus;
			op_name = "addition";
		} else if (t0->type == token_minus) {
			type = Ake_ast_type_minus;
			op_name = "subtraction";
		} else {
			break;
		}

        Ake_ast_create(&n);
        n->type = type;

        struct token* op = NULL;
		if (!match(ps, t0->type, "expecting + or -", &op, n)) {
            /* test case: no test case needed */
            n->type = Ake_ast_type_error;
        }

        if (!consume_newline(ps, n)) {
            n->type = Ake_ast_type_error;
        }

		b = parse_mult(ps);

		if (!b) {
            struct location b_loc = get_location(ps);
			error_list_set(ps->el, &b_loc, "expected term after additive operator");
			/* test case: test_parse_add_error_expected_term */
            n->type = Ake_ast_type_error;
		}

        if (left) {
            Ake_ast_add(n, left);
            if (left->type == Ake_ast_type_error) {
                n->type = Ake_ast_type_error;
            }
        }

        if (b) {
            Ake_ast_add(n, b);
            if (b->type == Ake_ast_type_error) {
                n->type = Ake_ast_type_error;
            }
        }

		if (n->type != Ake_ast_type_error) {
			Type_use* tu_a = a->tu;
			Type_use* tu_b = b->tu;

			if (!tu_a) {
				error_list_set(ps->el, &a->loc, "%s operand has no value", op_name);
				/* test case: test_parse_add_error_left_no_value */
                n->type = Ake_ast_type_error;
			} else if (!is_numeric(tu_a->td)) {
				error_list_set(ps->el, &a->loc, "%s on non-numeric operand", op_name);
                n->type = Ake_ast_type_error;
			}

			if (!tu_b) {
				error_list_set(ps->el, &b->loc, "%s operand has no value", op_name);
				/* test case: test_parse_add_error_right_no_value */
                n->type = Ake_ast_type_error;
			} else if (!is_numeric(tu_b->td)) {
				error_list_set(ps->el, &b->loc, "%s on non-numeric operand", op_name);
				/* test case: test_parse_add_error_right_not_numeric */
                n->type = Ake_ast_type_error;
			}

			if (n->type != Ake_ast_type_error) {
				Type_use* tu = Type_use_clone(tu_a);
				if (!type_find_whole(ps->st, tu, tu_b)) {
					error_list_set(ps->el, &op->loc, "invalid types for %s", op_name);
					/* test case: no test case needed */
                    n->type = Ake_ast_type_error;
                    Type_use_destroy(tu);
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
Ake_ast* parse_mult(struct parse_state* ps)
{
	Ake_ast* a = NULL;
	Ake_ast* b = NULL;
	Ake_ast* left = NULL;
	Ake_ast* n = NULL;
	char* op_name;

	a = parse_power(ps);

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
		enum Ake_ast_type type;
		if (t0->type == token_mult) {
			type = Ake_ast_type_mult;
			op_name = "multiplication";
		} else if (t0->type == token_divide) {
			type = Ake_ast_type_divide;
			op_name = "division";
		} else {
			break;
		}

        Ake_ast_create(&n);
        n->type = type;

        struct token* op = NULL;
		if (!match(ps, t0->type, "expecting * or /", &op, n)) {
            /* test case: test case not needed */
            assert(false);
        }

        if (!consume_newline(ps, n)) {
            n->type = Ake_ast_type_error;
        }

		/* parse_factor */
		b = parse_power(ps);

		if (!b) {
            struct location b_loc = get_location(ps);
			error_list_set(ps->el, &b_loc, "expected term after operator");
			/* test case: test_parse_mult_error_expected_term */
            n->type = Ake_ast_type_error;
		}

        if (left) {
            Ake_ast_add(n, left);
            if (left->type == Ake_ast_type_error) {
                n->type = Ake_ast_type_error;
            }
        }

        if (b) {
            Ake_ast_add(n, b);
            if (b->type == Ake_ast_type_error) {
                n->type = Ake_ast_type_error;
            }
        }

		if (n->type != Ake_ast_type_error) {
			assert(a);
			assert(b);
			Type_use* tu_a = a->tu;
			Type_use* tu_b = b->tu;

			if (!tu_a) {
				error_list_set(ps->el, &a->loc, "%s operand has no value", op_name);
				/* test case: test_parse_mult_error_left_no_value */
                n->type = Ake_ast_type_error;
			} else if (!is_numeric(tu_a->td)) {
				error_list_set(ps->el, &a->loc, "%s on non-numeric operand", op_name);
				/* test case: test_parse_mult_error_left_not_numeric */
                n->type = Ake_ast_type_error;
			}

			if (!tu_b) {
				error_list_set(ps->el, &b->loc, "%s operand has no value", op_name);
				/* test case: test_parse_mult_error_right_no_value*/
                n->type = Ake_ast_type_error;
			} else if (!is_numeric(tu_b->td)) {
				error_list_set(ps->el, &b->loc, "%s on non-numeric operand", op_name);
				/* test case: test_parse_mult_error_right_not_numeric */
                n->type = Ake_ast_type_error;
			}

			if (n->type != Ake_ast_type_error) {
				Type_use* tu = Type_use_clone(tu_a);
				if (!type_find_whole(ps->st, tu, tu_b)) {
					error_list_set(ps->el, &op->loc, "invalid types for %s", op_name);
					/* test case: no test case needed */
                    n->type = Ake_ast_type_error;
                    Type_use_destroy(tu);
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

/* power -> dot power' | e */
/* power' -> ^ dot power' | e */
Ake_ast* parse_power(struct parse_state* ps)
{
	Ake_ast* n = NULL;
	Ake_ast* a = NULL;

	a = parse_complex_operators(ps);

	if (!a) {
		return a;
	}

	Ake_ast* left = n = a ;

	while (true) {
		struct token* t0 = NULL;
		t0 = get_lookahead(ps);
		if (!t0 || t0->type != token_caret) {
			break;
		}

        Ake_ast_create(&n);
        n->type = Ake_ast_type_power;
        if (left) {
            Ake_ast_add(n, left);
            if (left->type == Ake_ast_type_error) {
                n->type = Ake_ast_type_error;
            }
        }

        struct token* caret = NULL;
		if (!match(ps, token_caret, "expected a caret", &caret, n)) {
            /* test case: no test case needed */
            assert(false);
        }

        if (!consume_newline(ps, n)) {
            n->type = Ake_ast_type_error;
        }

		Ake_ast* b = NULL;
		b = parse_complex_operators(ps);
        if (b && b->type == Ake_ast_type_error) {
            n->type = Ake_ast_type_error;
        }

        if (b) {
            Ake_ast_add(n, b);
            if (b->type == Ake_ast_type_error) {
                n->type = Ake_ast_type_error;
            }
        }

        if (!b) {
            struct location b_loc = get_location(ps);
            error_list_set(ps->el, &b_loc, "expected term after caret");
            /* test case: test_parse_power_error_expected_term */
            n->type = Ake_ast_type_error;
        }

        if (n->type != Ake_ast_type_error) {
			assert(left);
			assert(b);
			Type_use* tu_left = left->tu;
			Type_use* tu_b = b->tu;

			if (!tu_left) {
				error_list_set(ps->el, &left->loc, "power operand has no value");
				/* test case: test_parse_power_error_left_no_value */
                n->type = Ake_ast_type_error;
			} else if (!is_numeric(tu_left->td)) {
				error_list_set(ps->el, &left->loc, "power on non-numeric operand");
				/* test case: test_parse_power_error_left_not_numeric */
                n->type = Ake_ast_type_error;
			}

			if (!tu_b) {
				error_list_set(ps->el, &b->loc, "power operand has no value");
				/* test case: test_parse_power_error_right_no_value */
                n->type = Ake_ast_type_error;
			} else if (!is_numeric(tu_b->td)) {
				error_list_set(ps->el, &b->loc, "power on non-numeric operand");
				/* test case: test_parse_power_error_right_not_numeric */
                n->type = Ake_ast_type_error;
			}

			if (n->type != Ake_ast_type_error) {
				Type_use* tu = Type_use_clone(tu_left);
				if (!type_find_whole(ps->st, tu, tu_b)) {
					error_list_set(ps->el, &b->loc, "invalid power types");
					/* test case: no test case needed */
                    n->type = Ake_ast_type_error;
                    Type_use_destroy(tu);
				} else {
					n->tu = tu;
				}
			}

			left = n;
		}

		token_destroy(caret);
		free(caret);
	}

	return n;
}

/* complex_operators -> factor complex_operators' */
/* complex_operators' -> [expr] complex_operators' | (call_seq) complex_operators' | e */
Ake_ast* parse_complex_operators(struct parse_state* ps)
{
    Ake_ast* n = NULL;
    Ake_ast* a = NULL;

    a = parse_dot(ps);

    if (!a) {
        return a;
    }

    Ake_ast* left = n = a;
    while (true) {
        struct token* t0 = get_lookahead(ps);

        if (t0->type != token_left_square_bracket && t0->type != token_left_paren) {
            break;
        }

        Ake_ast_create(&n);
        if (t0->type == token_left_square_bracket) {
            parse_subscript(ps, left, n);
        } else if (t0->type == token_left_paren) {
            parse_call(ps, left, n);
        }

        if (left->type == Ake_ast_type_error) {
            n->type = Ake_ast_type_error;
        }

        left = n;
    }

    return n;
}

void parse_subscript(struct parse_state* ps, Ake_ast* left, Ake_ast* n)
{
    n->type = Ake_ast_type_array_subscript;

    if (!left->tu) {
        error_list_set(ps->el, &left->loc, "expression has subscript but has no value");
        left->type = Ake_ast_type_error;
    }

    if (left->type != Ake_ast_type_error) {
        if (left->tu->is_array || left->tu->is_slice) {
            n->tu = Type_use_clone(left->tu);
            Type_use_reduce_dimension(n->tu);
        }
    }

    if (left->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

    if (n->type != Ake_ast_type_error) {
        if (!left->tu->is_array && !left->tu->is_slice) {
            error_list_set(ps->el,
                           &left->loc,
                           "expression has subscript but is not an array or slice");
            /* test case: test_parse_subscript_error_not_array */
            n->type = Ake_ast_type_error;
        }
    }

    struct token* lsb = NULL;
    if (!match(ps, token_left_square_bracket, "expecting array subscript operator", &lsb, n)) {
        /* test case: no test case needed */
        assert(false);
    }

    token_destroy(lsb);
    free(lsb);

    if (!consume_newline(ps,  n)) {
        n->type = Ake_ast_type_error;
    }

    Ake_ast* b = NULL;
    b = parse_expr(ps);
    if (b && b->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

    if (!consume_newline(ps, n)) {
        n->type = Ake_ast_type_error;
    }

    struct token* rsb = NULL;
    if (!match(ps, token_right_square_bracket, "expected right-square-bracket", &rsb, n)) {
        n->type = Ake_ast_type_error;
    }

    token_destroy(rsb);
    free(rsb);

    Ake_ast_add(n, left);

    if (b) {
        Ake_ast_add(n, b);
    }
}

void parse_call(struct parse_state* ps, Ake_ast* left, Ake_ast* n) {
    n->type = Ake_ast_type_call;

    struct token *lp = NULL;
    if (!match(ps, token_left_paren, "expected left parenthesis", &lp, n)) {
        /* test case: test case not needed */
        n->type = Ake_ast_type_error;
    }

    if (!consume_newline(ps, n)) {
        n->type = Ake_ast_type_error;
    }

    Ake_ast *cseq_node = NULL;
    cseq_node = parse_cseq(ps, left);

    Ake_ast_add(n, left);

    if (cseq_node) {
        Ake_ast_add(n, cseq_node);
        if (cseq_node->type == Ake_ast_type_error) {
            n->type = Ake_ast_type_error;
        }
    }

    if (!consume_newline(ps, n)) {
        n->type = Ake_ast_type_error;
    }

    struct token *rp = NULL;
    if (!match(ps, token_right_paren, "expected right parenthesis", &rp, n)) {
        /* test case: test_parse_call_error_right_paren */
        n->type = Ake_ast_type_error;
    }

    if (n->type != Ake_ast_type_error) {
        Type_use *tu = left->tu;
        assert(tu);
        assert(tu->td);
        struct type_def *td = tu->td;
        if (td->type != type_function) {
            error_list_set(ps->el, &left->loc, "not a function type");
            /* test case: test_parse_call_error_not_function */
            n->type = Ake_ast_type_error;
        } else {
            Type_use *inputs = NULL;
            Type_use *outputs = NULL;
            get_function_children(tu, &inputs, &outputs);

            /* input */
            size_t tcount = 0;
            bool is_variadic = false;
            if (inputs) {
                Type_use *input_tu = inputs->head;
                while (input_tu) {
                    if (input_tu->type == Type_use_function_ellipsis) {
                        is_variadic = true;
                    } else {
                        tcount++;
                    }
                    input_tu = input_tu->next;
                }
            }
            size_t ccount = 0;
            if (cseq_node) {
                ccount = Ast_node_count_children(cseq_node);
            }

            if (ccount < tcount) {
                error_list_set(ps->el, &rp->loc, "not enough arguments in function call");
                /* test case: test_parse_call_error_not_enough_arguments */
                n->type = Ake_ast_type_error;
            } else if (!is_variadic && ccount > tcount) {
                error_list_set(ps->el, &rp->loc, "too many arguments in function call");
                /* test case: test_parse_call_error_too_many_arguments */
                n->type = Ake_ast_type_error;
            }

            /* output */
            if (outputs) {
                n->tu = Type_use_clone(outputs->head);
            }
        }

    }

    token_destroy(lp);
    free(lp);
    token_destroy(rp);
    free(rp);
}

/* cseq -> expr cseq' | e */
/* cseq' -> , expr cseq' | e */
Ake_ast* parse_cseq(struct parse_state* ps, Ake_ast* left)
{
    Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_cseq;

    if (!left->tu || !left->tu->td || left->tu->td->type != type_function) {
        error_list_set(ps->el, &left->loc, "not a function type");
        /* test case: no test case needed */
        n->type = Ake_ast_type_error;
        return n;
    }

    Ake_ast* a = NULL;
    a = parse_simple_expr(ps);
    if (a && a->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

    if (!a) {
        return n;
    }
    int i = 0;
    if (!check_input_type(ps, left->tu, i, a)) {
        n->type = Ake_ast_type_error;
    }
    i++;

    Ake_ast_add(n, a);

    while (true) {
        struct token* t0 = get_lookahead(ps);

        if (!t0 || t0->type != token_comma) {
            break;
        }

        struct token* comma = NULL;
        if (!match(ps, token_comma, "expecting comma", &comma, n)) {
            /* test case: no test case needed */
            n->type = Ake_ast_type_error;
        }

        token_destroy(comma);
        free(comma);

        if (!consume_newline(ps, n)) {
            n->type = Ake_ast_type_error;
        }

        a = parse_simple_expr(ps);
        if (a && a->type == Ake_ast_type_error) {
            n->type = Ake_ast_type_error;
        }

        if (!a) {
            struct location a_loc = get_location(ps);
            error_list_set(ps->el, &a_loc, "expected expression after comma");
            /* test case: test_parse_call_error_expected_expression */
            n->type = Ake_ast_type_error;
        } else {
            /* transfer a -> parent */
            Ake_ast_add(n, a);

            if (!check_input_type(ps, left->tu, i, a)) {
                n->type = Ake_ast_type_error;
            }
        }

        i++;
    }

    return n;
}

/* dot -> factor dot' */
/* dot' -> . factor dot' | e */
Ake_ast* parse_dot(struct parse_state* ps)
{
    Ake_ast* n = NULL;
    Ake_ast* a = NULL;

    a = parse_factor(ps);

    if (!a) {
        return a;
    }

    Ake_ast* left = n = a;
    while (true) {
        struct token* t0 = NULL;
        t0 = get_lookahead(ps);
        if (t0->type != token_dot) {
            break;
        }

        Ake_ast_create(&n);
        n->type = Ake_ast_type_dot;

        struct token* dot = NULL;
        if (!match(ps, token_dot, "expected a dot", &dot, n)) {
            /* test case: no test case needed */
            n->type = Ake_ast_type_error;
        }

        if (!consume_newline(ps, n)) {
            n->type = Ake_ast_type_error;
        }

        struct token* id = NULL;
        if (!match(ps, token_id, "expected identifier", &id, n)) {
            n->type = Ake_ast_type_error;
        }

        Ake_ast* b = NULL;
        Ake_ast_create(&b);
        b->type = Ake_ast_type_id;
        if (id) {
            buffer_copy(&id->value, &b->value);
        }

        Ake_ast_add(n, left);
        if (left->type == Ake_ast_type_error) {
            n->type = Ake_ast_type_error;
        }

        Ake_ast_add(n, b);

        if (n->type != Ake_ast_type_error) {
            if (!left->tu) {
                error_list_set(ps->el, &left->loc, "dot operand has no value");
                /* test case: no test case necessary */
                n->type = Ake_ast_type_error;
            } else if (left->tu->td->type != type_struct) {
                error_list_set(ps->el, &left->loc, "dot operand is not a struct");
                /* test case: test_parse_dot_error_left_non_module */
                n->type = Ake_ast_type_error;
            } else {
                struct type_def* td = left->tu->td;
                assert(td);
                struct symbol* sym = environment_get(ps->st->top, &td->name);
                assert(sym);
                //left->sym = sym;
                assert(sym->td);
                assert(sym->td->composite);
                bool found = false;
                struct Ake_ast* dec = sym->td->composite->head;
                struct Ake_ast* dec_id = NULL;
                struct Ake_ast* dec_type = NULL;
                while (dec) {
                    dec_id = Ast_node_get(dec, 0);
                    dec_type = Ast_node_get(dec, 1);
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
                    n->type = Ake_ast_type_error;
                } else {
                    n->tu = Type_use_clone(dec_type->tu);
                }
            }

            left = n;
#pragma warning(suppress:6001)
            token_destroy(dot);
            free(dot);
            token_destroy(id);
            free(id);
        }
    }

    return n;
}