#include <stdbool.h>
#include "token.h"
#include "ast.h"
#include "parse_tools.h"
#include "parse_factor.h"
#include "lex.h"
#include "parse_types.h"
#include "parse_expr.h"
#include "type.h"
#include <assert.h>
#include "symbol.h"

Ake_Ast* Ake_parse_assignment(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_eseq(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_boolean(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_comparison(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_add(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_mult(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_power(struct Ake_parse_state* ps);
void Ake_parse_subscript(struct Ake_parse_state* ps, Ake_Ast* left, Ake_Ast* n);
void Ake_parse_call(struct Ake_parse_state* ps, Ake_Ast* left, Ake_Ast* n);
Ake_Ast* Ake_parse_cseq(struct Ake_parse_state* ps, Ake_Ast* left);
Ake_Ast* Ake_parse_dot(struct Ake_parse_state* ps);

/* expr -> assignment */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_expr(struct Ake_parse_state* ps)
{
    return Ake_parse_assignment(ps);
}

/* assignment -> eseq = assignment | eseq */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_assignment(struct Ake_parse_state* ps)
{
	Ake_Ast* n = NULL;

	Ake_Ast* a = NULL;
    Ake_Ast* a_last;

	while (true) {
        a_last = a;
		a = Ake_parse_eseq(ps);

        if (!Ake_check_assignment_value_count(a, a_last)) {
            Zinc_error_list_set(ps->el, &a->loc, "assignment sequence counts do not match");
            /* test case: test_parse_expr_assignment_eseq_error_eseq_count */
            n->kind = Ake_ast_type_error;
        }

        if (!a) {
            if (n) {
                Zinc_error_list_set(ps->el, &a->loc, "missing rvalue in assignment");
                n->kind = Ake_ast_type_error;
            }
            break;
        }

        /* rvalue */
        if (a_last) {
            if (a->kind == Ake_ast_type_eseq) {
                Ake_Ast* p = a->head;
                while (p) {
                    if (!p->type) {
                        Zinc_error_list_set(ps->el, &p->loc, "rvalue does not have a type");
                    }
                    p = p->next;
                }
            } else {
                if (!a->type) {
                    Zinc_error_list_set(ps->el, &a->loc, "rvalue does not have a type");
                }
            }
        }

		struct Ake_token* t0 = Ake_get_lookahead(ps);
        if (t0->type != Ake_token_equal) {
            if (n) {
                /* last assignment */
                Ake_ast_add(n, a);
                if (a->kind == Ake_ast_type_error) {
                    n->kind = Ake_ast_type_error;
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
                n->kind = Ake_ast_type_assign;
                n->type = Ake_TypeClone(a->type);
            }

            Ake_ast_add(n, a);

            struct Ake_token *equal = NULL;
            if (!Ake_match(ps, Ake_token_equal, "expecting assign operator", &equal, n)) {
                /* test case: no test case needed */
                assert(false);
            }

            if (a->kind == Ake_ast_type_eseq) {
                Ake_Ast* p = a->head;
                while (p) {
                    if (!p->type) {
                        Zinc_error_list_set(ps->el, &p->loc, "lvalue does not have a type");
                        n->kind = Ake_ast_type_error;
                    }
                    p = p->next;
                }
            } else {
                if (!a->type) {
                    Zinc_error_list_set(ps->el, &a->loc, "lvalue does not have a type");
                    n->kind = Ake_ast_type_error;
                }
            }

            Ake_token_destroy(equal);
            free(equal);

            Ake_consume_newline(ps, n);
        }
	}

    if (n && n->kind == Ake_ast_type_assign) {
        Ake_Ast* rhs = n->tail;
        Ake_Ast* lhs = n->head;
        Ake_Ast* prev_lhs = NULL;
        while (lhs && lhs != rhs) {
            if (lhs->kind == Ake_ast_type_eseq) {
                Ake_Ast* lhs2 = lhs->head;
                Ake_Ast* rhs2 = rhs->head;
                while (lhs2) {
                    if (!Ake_check_lvalue(ps, lhs2, &n->loc)) {
                        n->kind = Ake_ast_type_error;
                    }
                    Ake_Override_rhs(lhs2->type, rhs2);
                    if (!Ake_TypeMatch(lhs2->type, rhs2->type, NULL)) {
                        Zinc_error_list_set(ps->el, &rhs2->loc, "values in assignment not compatible");
                    }
                    lhs2 = lhs2->next;
                    rhs2 = rhs2->next;
                }
            } else {
                if (!Ake_check_lvalue(ps, lhs, &n->loc)) {
                    n->kind = Ake_ast_type_error;
                }

            	bool cast = false;
                if (!Ake_TypeMatch(lhs->type, rhs->type, &cast)) {
                    Zinc_error_list_set(ps->el, &rhs->loc, "values in assignment not compatible");
                    n->kind = Ake_ast_type_error;
                }

                if (prev_lhs) {
                    if (!Ake_TypeMatch(lhs->type, prev_lhs->type, NULL)) {
                        Zinc_error_list_set(ps->el, &lhs->loc, "lvalues do not match type in assignment");
                        n->kind = Ake_ast_type_error;
                    }
                }
            }
            prev_lhs = lhs;
            lhs = lhs->next;
        }

        if (prev_lhs && prev_lhs->kind != Ake_ast_type_eseq) {
            Ake_Override_rhs(prev_lhs->type, rhs);
        }
    }

	return n;
}

/* eseq = boolean eseq' */
/* eseq' = , boolean | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_eseq(struct Ake_parse_state* ps)
{
    Ake_Ast* a = NULL;
    a = Ake_parse_boolean(ps);

    if (!a) {
        return NULL;
    }

    Ake_Ast* parent = NULL;
    while (true) {
        struct Ake_token* t0 = Ake_get_lookahead(ps);

        if (!t0 || t0->type != Ake_token_comma) {
            break;
        }

        if (!parent) {
            Ake_ast_create(&parent);
            parent->kind = Ake_ast_type_eseq;
            Ake_ast_add(parent, a);
            if (a->kind == Ake_ast_type_error) {
                parent->kind = Ake_ast_type_error;
            }

            if (parent->kind != Ake_ast_type_error) {
                if (!a->type) {
                    Zinc_error_list_set(ps->el, &a->loc, "operand of eseq has no type");
                    parent->kind = Ake_ast_type_error;
                }
            }
        }

        struct Ake_token* comma = NULL;
        if (!Ake_match(ps, Ake_token_comma, "expected a comma", &comma, parent)) {
            /* test case: no test case needed */
            assert(false);
        }
        Ake_token_destroy(comma);
        free(comma);

        Ake_Ast* b = NULL;
        b = Ake_parse_boolean(ps);
        if (b && b->kind == Ake_ast_type_error) {
            parent->kind = Ake_ast_type_error;
        }

        /* parent checks */
        if (!b) {
            Zinc_error_list_set(ps->el, &b->loc, "expected term after comma");
            parent->kind = Ake_ast_type_error;
        }

        if (b) {
            if (!b->type) {
                Zinc_error_list_set(ps->el, &b->loc, "operand of eseq has no type");
                b->kind = Ake_ast_type_error;
            }
        }

        if (b) {
            Ake_ast_add(parent, b);
            if (b->kind == Ake_ast_type_error) {
                parent->kind = Ake_ast_type_error;
            }
        }
    }

    if (parent == NULL) {
        parent = a;
    }
    return parent;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_simple_expr(struct Ake_parse_state* ps)
{
    return Ake_parse_boolean(ps);
}

/* boolean -> comparison boolean' */
/* boolean' -> && comparison boolean' | || comparison boolean' | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_boolean(struct Ake_parse_state* ps)
{
	Ake_Ast* n = NULL;
	Ake_Ast* left;

	Ake_Ast* a = NULL;
	a = Ake_parse_comparison(ps);

	if (!a) {
		return NULL;
	}

	left = n = a;
	while (true) {
		struct Ake_token* t0 = Ake_get_lookahead(ps);

		/* operator */
		enum Ake_AstKind type;
		if (t0 && t0->type == Ake_token_and) {
			type = Ake_ast_type_and;
		} else if (t0 && t0->type == Ake_token_or) {
			type = Ake_ast_type_or;
		} else {
			break;
		}

        Ake_ast_create(&n);
		n->kind = type;

		struct Ake_token* op = NULL;
		if (!Ake_match(ps, t0->type, "expecting && or ||", &op, n)) {
            /* test case: no test case needed */
            assert(false);
        }

        if (!Ake_consume_newline(ps, n)) {
            n->kind = Ake_ast_type_error;
        }

		/* comparison */
		Ake_Ast* b = NULL;
		b = Ake_parse_comparison(ps);

		if (!b) {
            struct Zinc_location b_loc = Ake_get_location(ps);
			Zinc_error_list_set(ps->el, &b_loc, "expected term after && or ||");
			/* test case: test_parse_boolean_error_expected_term */
            n->kind = Ake_ast_type_error;
		}

        if (left) {
            Ake_ast_add(n, left);
            if (left->kind == Ake_ast_type_error) {
                n->kind = Ake_ast_type_error;
            }
        }
        if (b) {
            Ake_ast_add(n, b);
            if (b->kind == Ake_ast_type_error) {
                n->kind = Ake_ast_type_error;
            }
        }

		if (n->kind != Ake_ast_type_error) {
			assert(b);
            assert(left);
			if (!left->type) {
				Zinc_error_list_set(ps->el, &left->loc, "left-side operand of boolean operator has no type");
				/* test case: test_parse_boolean_error_left_no_value */
                n->kind = Ake_ast_type_error;
			} else if (!b->type) {
				Zinc_error_list_set(ps->el, &b->loc, "operand of boolean operator has no type");
				/* test case: test_parse_boolean_error_right_no_value */
                n->kind = Ake_ast_type_error;
			} else if (left->type->kind != AKE_TYPE_BOOLEAN) {
				Zinc_error_list_set(ps->el, &left->loc, "left-side expression of boolean operator is not boolean");
                n->kind = Ake_ast_type_error;
			} else if (b->type->kind != AKE_TYPE_BOOLEAN) {
				Zinc_error_list_set(ps->el, &b->loc, "expression of boolean operator is not boolean");
                n->kind = Ake_ast_type_error;
			} else {
				n->type = Ake_TypeClone(left->type);
			}

			left = n;
		}

		Ake_token_destroy(op);
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
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_comparison(struct Ake_parse_state* ps)
{
	Ake_Ast* n = NULL;
	Ake_Ast* a = NULL;
	Ake_Ast* left = NULL;

	a = Ake_parse_add(ps);

	if (!a) {
		return NULL;
	}

    left = n = a;

	while (true) {
		struct Ake_token* t0 = Ake_get_lookahead(ps);
		enum Ake_AstKind type;

		if (!t0) {
			break;
		}

		if (t0->type == Ake_token_double_equal) {
			type = Ake_ast_type_equality;
		} else if (t0->type == Ake_token_not_equal) {
			type = Ake_ast_type_not_equal;
		} else if (t0->type == Ake_token_less_than) {
			type = Ake_ast_type_less_than;
		} else if (t0->type == Ake_token_less_than_or_equal) {
			type = Ake_ast_type_less_than_or_equal;
		} else if (t0->type == Ake_token_greater_than) {
			type = Ake_ast_type_greater_than;
		} else if (t0->type == Ake_token_greater_than_or_equal) {
			type = Ake_ast_type_greater_than_or_equal;
		} else {
			break;
		}

		struct Ake_token* op = NULL;
		if (!Ake_match(ps, t0->type, "expecting comparator", &op, n)) {
            /* test case: no test case needed */
            n->kind = Ake_ast_type_error;
        }

        if (!Ake_consume_newline(ps, n)) {
            n->kind = Ake_ast_type_error;
        }

		Ake_Ast* b = NULL;
		b = Ake_parse_add(ps);

		if (!b) {
            struct Zinc_location b_loc = Ake_get_location(ps);
			Zinc_error_list_set(ps->el, &b_loc, "expected term after comparison operator");
			/* case case: test_parse_comparison_error_no_term */
            n->kind = Ake_ast_type_error;
		}

        Ake_ast_create(&n);
        n->kind = type;

        if (left) {
            Ake_ast_add(n, left);
            if (left->kind == Ake_ast_type_error) {
                n->kind = Ake_ast_type_error;
            }
        }

        if (b) {
            Ake_ast_add(n, b);
            if (b->kind == Ake_ast_type_error) {
                n->kind = Ake_ast_type_error;
            }
        }

		if (n->kind != Ake_ast_type_error) {
			assert(a);
			assert(b);
			assert(op);
            assert(left);
			if (!left->type) {
				Zinc_error_list_set(ps->el, &left->loc, "operand has no value");
				/* test case: test_parse_comparison_error_left_not_numeric */
                n->kind = Ake_ast_type_error;
			} else if (!b->type) {
				Zinc_error_list_set(ps->el, &b->loc, "operand has no value");
				/* test case: test_parse_comparison_error_right_no_value */
                n->kind = Ake_ast_type_error;
			} else {
				if (!Ake_is_identity_comparison(type) && !Ake_is_numeric(left->type)) {
					Zinc_error_list_set(ps->el, &left->loc, "comparison operand is not numeric");
					/* test case: test_parse_comparison_error_left_not_numeric */
                    n->kind = Ake_ast_type_error;
				} else if (!Ake_is_identity_comparison(type) && !Ake_is_numeric(b->type)) {
					Zinc_error_list_set(ps->el, &b->loc, "comparison operand is not numeric");
					/* test case: test_parse_comparison_error_right_not_numeric */
                    n->kind = Ake_ast_type_error;
				} else {
					Ake_Type* type = Ake_TypeClone(left->type);
					n->type = type;
				}
			}
			left = n;
		}

		Ake_token_destroy(op);
		free(op);
	}

	return n;
}

/* add -> mult add' */
/* add' -> + mult add' | - mult add' | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_add(struct Ake_parse_state* ps)
{
	Ake_Ast* a = NULL;
	Ake_Ast* b = NULL;
	Ake_Ast* n = NULL;
	Ake_Ast* left = NULL;
	char* op_name;

	a = Ake_parse_mult(ps);

	if (!a) {
		return NULL;
	}

	left = n = a;

	while (true) {
		struct Ake_token* t0 = Ake_get_lookahead(ps);

		/* e */
		if (t0 == NULL) {
			break;
		}

		/* operator */
		enum Ake_AstKind type;
		if (t0->type == Ake_token_plus) {
			type = Ake_ast_type_plus;
			op_name = "addition";
		} else if (t0->type == Ake_token_minus) {
			type = Ake_ast_type_minus;
			op_name = "subtraction";
		} else {
			break;
		}

        Ake_ast_create(&n);
        n->kind = type;

        struct Ake_token* op = NULL;
		if (!Ake_match(ps, t0->type, "expecting + or -", &op, n)) {
            /* test case: no test case needed */
            n->kind = Ake_ast_type_error;
        }

        if (!Ake_consume_newline(ps, n)) {
            n->kind = Ake_ast_type_error;
        }

		b = Ake_parse_mult(ps);

		if (!b) {
            struct Zinc_location b_loc = Ake_get_location(ps);
			Zinc_error_list_set(ps->el, &b_loc, "expected term after additive operator");
			/* test case: test_parse_add_error_expected_term */
            n->kind = Ake_ast_type_error;
		}

        if (left) {
            Ake_ast_add(n, left);
            if (left->kind == Ake_ast_type_error) {
                n->kind = Ake_ast_type_error;
            }
        }

        if (b) {
            Ake_ast_add(n, b);
            if (b->kind == Ake_ast_type_error) {
                n->kind = Ake_ast_type_error;
            }
        }

		if (n->kind != Ake_ast_type_error) {
			Ake_Type* a_type = a->type;
			Ake_Type* b_type = b->type;

			if (!a_type) {
				Zinc_error_list_set(ps->el, &a->loc, "%s operand has no value", op_name);
				/* test case: test_parse_add_error_left_no_value */
                n->kind = Ake_ast_type_error;
			} else if (!Ake_is_numeric(a_type)) {
				Zinc_error_list_set(ps->el, &a->loc, "%s on non-numeric operand", op_name);
                n->kind = Ake_ast_type_error;
			}

			if (!b_type) {
				Zinc_error_list_set(ps->el, &b->loc, "%s operand has no value", op_name);
				/* test case: test_parse_add_error_right_no_value */
                n->kind = Ake_ast_type_error;
			} else if (!Ake_is_numeric(b_type)) {
				Zinc_error_list_set(ps->el, &b->loc, "%s on non-numeric operand", op_name);
				/* test case: test_parse_add_error_right_not_numeric */
                n->kind = Ake_ast_type_error;
			}

			if (n->kind != Ake_ast_type_error) {
				Ake_Type* a_type2 = Ake_TypeClone(a_type);
				bool cast = false;
				if (!Ake_TypeMatch(a_type2, b_type, &cast)) {
					Zinc_error_list_set(ps->el, &op->loc, "invalid types for %s", op_name);
					/* test case: no test case needed */
                    n->kind = Ake_ast_type_error;
                    Ake_TypeDestroy(a_type2);
				} else {
					n->type = a_type2;
				}
			}

			left = n;
		}

		/* destroy op op{} */
		Ake_token_destroy(op);
		free(op);
	}

	return n;
}

/* mult -> power mult' */
/* mult' -> * power mult' | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_mult(struct Ake_parse_state* ps)
{
	Ake_Ast* a = NULL;
	Ake_Ast* b = NULL;
	Ake_Ast* left = NULL;
	Ake_Ast* n = NULL;
	char* op_name;

	a = Ake_parse_power(ps);

	if (!a) {
		return NULL;
	}

    left = n = a;

	while (true) {
		struct Ake_token* t0 = Ake_get_lookahead(ps);

		/* e */
		if (t0 == NULL) {
			break;
		}

		/* operator */
		enum Ake_AstKind type;
		if (t0->type == Ake_token_mult) {
			type = Ake_ast_type_mult;
			op_name = "multiplication";
		} else if (t0->type == Ake_token_divide) {
			type = Ake_ast_type_divide;
			op_name = "division";
		} else {
			break;
		}

        Ake_ast_create(&n);
        n->kind = type;

        struct Ake_token* op = NULL;
		if (!Ake_match(ps, t0->type, "expecting * or /", &op, n)) {
            /* test case: test case not needed */
            assert(false);
        }

        if (!Ake_consume_newline(ps, n)) {
            n->kind = Ake_ast_type_error;
        }

		/* parse_factor */
		b = Ake_parse_power(ps);

		if (!b) {
            struct Zinc_location b_loc = Ake_get_location(ps);
			Zinc_error_list_set(ps->el, &b_loc, "expected term after operator");
			/* test case: test_parse_mult_error_expected_term */
            n->kind = Ake_ast_type_error;
		}

        if (left) {
            Ake_ast_add(n, left);
            if (left->kind == Ake_ast_type_error) {
                n->kind = Ake_ast_type_error;
            }
        }

        if (b) {
            Ake_ast_add(n, b);
            if (b->kind == Ake_ast_type_error) {
                n->kind = Ake_ast_type_error;
            }
        }

		if (n->kind != Ake_ast_type_error) {
			assert(a);
			assert(b);
			Ake_Type* a_type = a->type;
			Ake_Type* b_type = b->type;

			if (!a_type) {
				Zinc_error_list_set(ps->el, &a->loc, "%s operand has no value", op_name);
				/* test case: test_parse_mult_error_left_no_value */
                n->kind = Ake_ast_type_error;
			} else if (!Ake_is_numeric(a_type)) {
				Zinc_error_list_set(ps->el, &a->loc, "%s on non-numeric operand", op_name);
				/* test case: test_parse_mult_error_left_not_numeric */
                n->kind = Ake_ast_type_error;
			}

			if (!b_type) {
				Zinc_error_list_set(ps->el, &b->loc, "%s operand has no value", op_name);
				/* test case: test_parse_mult_error_right_no_value*/
                n->kind = Ake_ast_type_error;
			} else if (!Ake_is_numeric(b_type)) {
				Zinc_error_list_set(ps->el, &b->loc, "%s on non-numeric operand", op_name);
				/* test case: test_parse_mult_error_right_not_numeric */
                n->kind = Ake_ast_type_error;
			}

			if (n->kind != Ake_ast_type_error) {
				Ake_Type* a_type2 = Ake_TypeClone(a_type);
				bool cast = false;
				if (!Ake_TypeMatch(a_type2, b_type, &cast)) {
					Zinc_error_list_set(ps->el, &op->loc, "invalid types for %s", op_name);
					/* test case: no test case needed */
                    n->kind = Ake_ast_type_error;
                    Ake_TypeDestroy(a_type2);
				} else {
					n->type = a_type2;
				}
			}

			left = n;
		}

		Ake_token_destroy(op);
		free(op);

	}

	return n;
}

/* power -> dot power' | e */
/* power' -> ^ dot power' | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_power(struct Ake_parse_state* ps)
{
	Ake_Ast* n = NULL;
	Ake_Ast* a = NULL;

	a = Ake_parse_complex_operators(ps);

	if (!a) {
		return a;
	}

	Ake_Ast* left = n = a ;

	while (true) {
		struct Ake_token* t0 = NULL;
		t0 = Ake_get_lookahead(ps);
		if (!t0 || t0->type != Ake_token_caret) {
			break;
		}

        Ake_ast_create(&n);
        n->kind = Ake_ast_type_power;
        if (left) {
            Ake_ast_add(n, left);
            if (left->kind == Ake_ast_type_error) {
                n->kind = Ake_ast_type_error;
            }
        }

        struct Ake_token* caret = NULL;
		if (!Ake_match(ps, Ake_token_caret, "expected a caret", &caret, n)) {
            /* test case: no test case needed */
            assert(false);
        }

        if (!Ake_consume_newline(ps, n)) {
            n->kind = Ake_ast_type_error;
        }

		Ake_Ast* b = NULL;
		b = Ake_parse_complex_operators(ps);
        if (b && b->kind == Ake_ast_type_error) {
            n->kind = Ake_ast_type_error;
        }

        if (b) {
            Ake_ast_add(n, b);
            if (b->kind == Ake_ast_type_error) {
                n->kind = Ake_ast_type_error;
            }
        }

        if (!b) {
            struct Zinc_location b_loc = Ake_get_location(ps);
            Zinc_error_list_set(ps->el, &b_loc, "expected term after caret");
            /* test case: test_parse_power_error_expected_term */
            n->kind = Ake_ast_type_error;
        }

        if (n->kind != Ake_ast_type_error) {
			assert(left);
			assert(b);
			Ake_Type* left_type = left->type;
			Ake_Type* b_type = b->type;

			if (!left_type) {
				Zinc_error_list_set(ps->el, &left->loc, "power operand has no value");
				/* test case: test_parse_power_error_left_no_value */
                n->kind = Ake_ast_type_error;
			} else if (!Ake_is_numeric(left_type)) {
				Zinc_error_list_set(ps->el, &left->loc, "power on non-numeric operand");
				/* test case: test_parse_power_error_left_not_numeric */
                n->kind = Ake_ast_type_error;
			}

			if (!b_type) {
				Zinc_error_list_set(ps->el, &b->loc, "power operand has no value");
				/* test case: test_parse_power_error_right_no_value */
                n->kind = Ake_ast_type_error;
			} else if (!Ake_is_numeric(b_type)) {
				Zinc_error_list_set(ps->el, &b->loc, "power on non-numeric operand");
				/* test case: test_parse_power_error_right_not_numeric */
                n->kind = Ake_ast_type_error;
			}

			if (n->kind != Ake_ast_type_error) {
				Ake_Type* left_type2 = Ake_TypeClone(left_type);
				bool cast = false;
				if (!Ake_TypeMatch(left_type2, b_type, &cast)) {
					Zinc_error_list_set(ps->el, &b->loc, "invalid power types");
					/* test case: no test case needed */
                    n->kind = Ake_ast_type_error;
                    Ake_TypeDestroy(left_type2);
				} else {
					n->type = left_type2;
				}
			}

			left = n;
		}

		Ake_token_destroy(caret);
		free(caret);
	}

	return n;
}

/* complex_operators -> factor complex_operators' */
/* complex_operators' -> [expr] complex_operators' | (call_seq) complex_operators' | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_complex_operators(struct Ake_parse_state* ps)
{
    Ake_Ast* n = NULL;
    Ake_Ast* a = NULL;

    a = Ake_parse_dot(ps);

    if (!a) {
        return a;
    }

    Ake_Ast* left = n = a;
    while (true) {
        struct Ake_token* t0 = Ake_get_lookahead(ps);

        if (t0->type != Ake_token_left_square_bracket && t0->type != Ake_token_left_paren) {
            break;
        }

        Ake_ast_create(&n);
        if (t0->type == Ake_token_left_square_bracket) {
            Ake_parse_subscript(ps, left, n);
        } else if (t0->type == Ake_token_left_paren) {
            Ake_parse_call(ps, left, n);
        }

        if (left->kind == Ake_ast_type_error) {
            n->kind = Ake_ast_type_error;
        }

        left = n;
    }

    return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_parse_subscript(struct Ake_parse_state* ps, Ake_Ast* left, Ake_Ast* n)
{
    n->kind = Ake_ast_type_array_subscript;

    if (!left->type) {
        Zinc_error_list_set(ps->el, &left->loc, "expression has subscript but has no value");
        left->kind = Ake_ast_type_error;
    }

	bool is_array_or_slice = false;
    if (left->kind != Ake_ast_type_error) {
        if (left->type->kind == AKE_TYPE_ARRAY) {
	        n->type = Ake_TypeClone(left->type->data.array.type);
        	is_array_or_slice = true;
        } else if (left->type->kind == AKE_TYPE_ARRAY_CONST) {
	        n->type = Ake_TypeClone(left->type->data.array_const.type);
        	is_array_or_slice = true;
        } else if (left->type->kind == AKE_TYPE_SLICE) {
            n->type = Ake_TypeClone(left->type->data.slice.type);
        	is_array_or_slice = true;
        }
    }

    if (n->kind != Ake_ast_type_error) {
        if (!is_array_or_slice) {
            Zinc_error_list_set(ps->el,
                           &left->loc,
                           "expression has subscript but is not an array or slice");
            /* test case: test_parse_subscript_error_not_array */
            n->kind = Ake_ast_type_error;
        }
    }

    struct Ake_token* lsb = NULL;
    if (!Ake_match(ps, Ake_token_left_square_bracket, "expecting array subscript operator", &lsb, n)) {
        /* test case: no test case needed */
        assert(false);
    }

    Ake_token_destroy(lsb);
    free(lsb);

    if (!Ake_consume_newline(ps,  n)) {
        n->kind = Ake_ast_type_error;
    }

    Ake_Ast* b = NULL;
	ps->context.is_subscript = true;
    b = Ake_parse_expr(ps);
	ps->context.is_subscript = false;
    if (b && b->kind == Ake_ast_type_error) {
        n->kind = Ake_ast_type_error;
    }

    if (!Ake_consume_newline(ps, n)) {
        n->kind = Ake_ast_type_error;
    }

    struct Ake_token* rsb = NULL;
    if (!Ake_match(ps, Ake_token_right_square_bracket, "expected right-square-bracket", &rsb, n)) {
        n->kind = Ake_ast_type_error;
    }

    Ake_token_destroy(rsb);
    free(rsb);

    Ake_ast_add(n, left);

    if (b) {
        Ake_ast_add(n, b);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_parse_call(struct Ake_parse_state* ps, Ake_Ast* left, Ake_Ast* n) {
    n->kind = Ake_ast_type_call;

    struct Ake_token *lp = NULL;
    if (!Ake_match(ps, Ake_token_left_paren, "expected left parenthesis", &lp, n)) {
        /* test case: test case not needed */
        n->kind = Ake_ast_type_error;
    }

    if (!Ake_consume_newline(ps, n)) {
        n->kind = Ake_ast_type_error;
    }

    Ake_Ast *cseq_node = NULL;
    cseq_node = Ake_parse_cseq(ps, left);

    Ake_ast_add(n, left);

    if (cseq_node) {
        Ake_ast_add(n, cseq_node);
        if (cseq_node->kind == Ake_ast_type_error) {
            n->kind = Ake_ast_type_error;
        }
    }

    if (!Ake_consume_newline(ps, n)) {
        n->kind = Ake_ast_type_error;
    }

    struct Ake_token *rp = NULL;
    if (!Ake_match(ps, Ake_token_right_paren, "expected right parenthesis", &rp, n)) {
        /* test case: test_parse_call_error_right_paren */
        n->kind = Ake_ast_type_error;
    }

    if (n->kind != Ake_ast_type_error) {
        Ake_Type *tu = left->type;
        assert(tu);
        if (tu->kind != AKE_TYPE_FUNCTION) {
            Zinc_error_list_set(ps->el, &left->loc, "not a function type");
            /* test case: test_parse_call_error_not_function */
            n->kind = Ake_ast_type_error;
        } else {
            /* input */
            size_t tcount = 0;
            bool is_variadic = false;
        	Ake_TypeParam* tp = tu->data.function.input_head;
            while (tp) {
	            if (tp->kind == AKE_TYPE_PARAM_ELLIPSIS) {
	            	is_variadic = true;
	            } else {
	            	tcount++;
	            }
            	tp = tp->next;
            }
            size_t ccount = 0;
            if (cseq_node) {
                ccount = Ake_ast_count_children(cseq_node);
            }

            if (ccount < tcount) {
                Zinc_error_list_set(ps->el, &rp->loc, "not enough arguments in function call");
                /* test case: test_parse_call_error_not_enough_arguments */
                n->kind = Ake_ast_type_error;
            } else if (!is_variadic && ccount > tcount) {
                Zinc_error_list_set(ps->el, &rp->loc, "too many arguments in function call");
                /* test case: test_parse_call_error_too_many_arguments */
                n->kind = Ake_ast_type_error;
            }

            /* output */
            if (tu->data.function.output) {
                n->type = Ake_TypeClone(tu->data.function.output);
            }
        }

    }

    Ake_token_destroy(lp);
    free(lp);
    Ake_token_destroy(rp);
    free(rp);
}

/* cseq -> expr cseq' | e */
/* cseq' -> , expr cseq' | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_cseq(Ake_parse_state* ps, Ake_Ast* left)
{
    Ake_Ast* n = NULL;
    Ake_ast_create(&n);
    n->kind = Ake_ast_type_cseq;

    if (!left->type || left->type->kind != AKE_TYPE_FUNCTION) {
        Zinc_error_list_set(ps->el, &left->loc, "not a function type");
        /* test case: no test case needed */
        n->kind = Ake_ast_type_error;
        return n;
    }

    Ake_Ast* a = NULL;
    a = Ake_parse_simple_expr(ps);
    if (a && a->kind == Ake_ast_type_error) {
        n->kind = Ake_ast_type_error;
    }

    if (!a) {
        return n;
    }
    int i = 0;
    if (!Ake_check_input_type(ps, left->type, i, a)) {
        n->kind = Ake_ast_type_error;
    }
    i++;

    Ake_ast_add(n, a);

    while (true) {
        struct Ake_token* t0 = Ake_get_lookahead(ps);

        if (!t0 || t0->type != Ake_token_comma) {
            break;
        }

        struct Ake_token* comma = NULL;
        if (!Ake_match(ps, Ake_token_comma, "expecting comma", &comma, n)) {
            /* test case: no test case needed */
            n->kind = Ake_ast_type_error;
        }

        Ake_token_destroy(comma);
        free(comma);

        if (!Ake_consume_newline(ps, n)) {
            n->kind = Ake_ast_type_error;
        }

        a = Ake_parse_simple_expr(ps);
        if (a && a->kind == Ake_ast_type_error) {
            n->kind = Ake_ast_type_error;
        }

        if (!a) {
            struct Zinc_location a_loc = Ake_get_location(ps);
            Zinc_error_list_set(ps->el, &a_loc, "expected expression after comma");
            /* test case: test_parse_call_error_expected_expression */
            n->kind = Ake_ast_type_error;
        } else {
            /* transfer a -> parent */
            Ake_ast_add(n, a);

            if (!Ake_check_input_type(ps, left->type, i, a)) {
                n->kind = Ake_ast_type_error;
            }
        }

        i++;
    }

    return n;
}

/* dot -> factor dot' */
/* dot' -> . factor dot' | e */
Ake_Ast* Ake_parse_dot(struct Ake_parse_state* ps)
{
    Ake_Ast* n = NULL;
    Ake_Ast* a = NULL;

    a = Ake_parse_factor(ps);

    if (!a) {
        return a;
    }

    Ake_Ast* left = n = a;
    while (true) {
        struct Ake_token* t0 = NULL;
        t0 = Ake_get_lookahead(ps);
        if (t0->type != Ake_token_dot) {
            break;
        }

        Ake_ast_create(&n);
        n->kind = Ake_ast_type_dot;

        struct Ake_token* dot = NULL;
        if (!Ake_match(ps, Ake_token_dot, "expected a dot", &dot, n)) {
            /* test case: no test case needed */
            n->kind = Ake_ast_type_error;
        }

        if (!Ake_consume_newline(ps, n)) {
            n->kind = Ake_ast_type_error;
        }

        struct Ake_token* id = NULL;
        if (!Ake_match(ps, Ake_token_id, "expected identifier", &id, n)) {
            n->kind = Ake_ast_type_error;
        }

        Ake_Ast* b = NULL;
        Ake_ast_create(&b);
        b->kind = Ake_ast_type_id;
        if (id) {
            Zinc_string_copy(&id->value, &b->value);
        }

        Ake_ast_add(n, left);
        if (left->kind == Ake_ast_type_error) {
            n->kind = Ake_ast_type_error;
        }

        Ake_ast_add(n, b);

        if (n->kind != Ake_ast_type_error) {
            if (!left->type) {
                Zinc_error_list_set(ps->el, &left->loc, "dot operand has no value");
                /* test case: no test case necessary */
                n->kind = Ake_ast_type_error;
            } else if (left->type->kind != AKE_TYPE_STRUCT) {
                Zinc_error_list_set(ps->el, &left->loc, "dot operand is not a struct");
                /* test case: test_parse_dot_error_left_non_module */
                n->kind = Ake_ast_type_error;
            } else {
                Ake_Type* type = left->type;
                assert(type);
            	size_t seq = Ake_get_current_seq(ps);
                struct Ake_Symbol* sym = Ake_EnvironmentGet(ps->st->top, &type->name, seq);
                assert(sym);
                //left->sym = sym;
                assert(sym->td);
                assert(sym->td->data.fields.head);
                bool found = false;
                Ake_TypeField* tf = sym->td->data.fields.head;
            	Ake_Type* found_tu = NULL;
                while (tf) {
                    if (Zinc_string_compare(&tf->name, &b->value)) {
                    	found_tu = tf->td;
                        found = true;
                        break;
                    }
                    tf = tf->next;
                }
                if (!found) {
                    Zinc_error_list_set(
                            ps->el,
                            &id->loc,
                            "identifier not a field of struct: %bf", &id->value);
                    n->kind = Ake_ast_type_error;
                } else {
                    n->type = Ake_TypeClone(found_tu);
                }
            }

            left = n;
#pragma warning(suppress:6001)
            Ake_token_destroy(dot);
            free(dot);
            Ake_token_destroy(id);
            free(id);
        }
    }

    return n;
}