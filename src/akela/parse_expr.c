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
Ake_Ast* Ake_parse_boolean(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_comparison(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_add(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_mult(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_power(struct Ake_parse_state* ps);
void Ake_parse_subscript(struct Ake_parse_state* ps, Ake_Ast* left, Ake_Ast* n);
void Ake_parse_call(struct Ake_parse_state* ps, Ake_Ast* left, Ake_Ast* n);
void Ake_parse_args(struct Ake_parse_state* ps, Ake_Ast* n);
Ake_Ast* Ake_parse_dot(struct Ake_parse_state* ps);

/* expr -> assignment */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_expr(struct Ake_parse_state* ps)
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

        Ake_AstCreate(&n);
		n->kind = type;

		struct Ake_token* op = NULL;
		if (!Ake_match(ps, t0->type, "expecting && or ||", &op, n)) {
            /* test case: no test case needed */
            assert(false);
        }

        Ake_consume_newline(ps, n);

		/* comparison */
		Ake_Ast* b = NULL;
		b = Ake_parse_comparison(ps);

		if (!b) {
            struct Zinc_location b_loc = Ake_get_location(ps);
			Zinc_error_list_set(ps->el, &b_loc, "expected term after && or ||");
			/* test case: test_parse_boolean_error_expected_term */
            n->has_error = true;
		}

        if (left) {
            Ake_AstAdd(n, left);
        }
        if (b) {
            Ake_AstAdd(n, b);
        }

		if (!n->has_error) {
			assert(b);
            assert(left);
			if (!left->type) {
				Zinc_error_list_set(ps->el, &left->loc, "left-side operand of boolean operator has no type");
				/* test case: test_parse_boolean_error_left_no_value */
                n->has_error = true;
			} else if (!b->type) {
				Zinc_error_list_set(ps->el, &b->loc, "operand of boolean operator has no type");
				/* test case: test_parse_boolean_error_right_no_value */
                n->has_error = true;
			} else if (left->type->kind != AKE_TYPE_BOOLEAN) {
				Zinc_error_list_set(ps->el, &left->loc, "left-side expression of boolean operator is not boolean");
                n->has_error = true;
			} else if (b->type->kind != AKE_TYPE_BOOLEAN) {
				Zinc_error_list_set(ps->el, &b->loc, "expression of boolean operator is not boolean");
                n->has_error = true;
			} else {
				n->type = Ake_TypeClone(left->type);
			}

			left = n;
		}
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
			type = AKE_AST_EQUALITY;
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
            n->has_error = true;
        }

        if (!Ake_consume_newline(ps, n)) {
            n->has_error = true;
        }

		Ake_Ast* b = NULL;
		b = Ake_parse_add(ps);

		if (!b) {
            struct Zinc_location b_loc = Ake_get_location(ps);
			Zinc_error_list_set(ps->el, &b_loc, "expected term after comparison operator");
			/* case case: test_parse_comparison_error_no_term */
            n->has_error = true;
		}

        Ake_AstCreate(&n);
		Ake_AstSet(n, type);

        if (left) {
        	if (n->kind == AKE_AST_EQUALITY) {
        		n->data.equality.left = left;
        		Ake_AstAdd2(n, left);
        	} else {
        		Ake_AstAdd(n, left);
        	}
        }

        if (b) {
        	if (n->kind == AKE_AST_EQUALITY) {
        		n->data.equality.right = b;
        		Ake_AstAdd2(n, b);
        	} else {
        		Ake_AstAdd(n, b);
        	}
        }

		if (!n->has_error) {
			assert(a);
			assert(b);
			assert(op);
            assert(left);
			if (!left->type) {
				Zinc_error_list_set(ps->el, &left->loc, "operand has no value");
				/* test case: test_parse_comparison_error_left_not_numeric */
                n->has_error = true;
			} else if (!b->type) {
				Zinc_error_list_set(ps->el, &b->loc, "operand has no value");
				/* test case: test_parse_comparison_error_right_no_value */
                n->has_error = true;
			} else {
				if (!Ake_is_identity_comparison(type) && !Ake_is_numeric(left->type)) {
					Zinc_error_list_set(ps->el, &left->loc, "comparison operand is not numeric");
					/* test case: test_parse_comparison_error_left_not_numeric */
                    n->has_error = true;
				} else if (!Ake_is_identity_comparison(type) && !Ake_is_numeric(b->type)) {
					Zinc_error_list_set(ps->el, &b->loc, "comparison operand is not numeric");
					/* test case: test_parse_comparison_error_right_not_numeric */
                    n->has_error = true;
				} else {
					Ake_Type* type = Ake_TypeClone(left->type);
					n->type = type;
				}
			}
			left = n;
		}
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
			type = AKE_AST_PLUS;
			op_name = "addition";
		} else if (t0->type == Ake_token_minus) {
			type = AKE_AST_MINUS;
			op_name = "subtraction";
		} else {
			break;
		}

        Ake_AstCreate(&n);
        Ake_AstSet(n, type);

        struct Ake_token* op = NULL;
		if (!Ake_match(ps, t0->type, "expecting + or -", &op, n)) {
            /* test case: no test case needed */
            n->has_error = true;
        }

        if (!Ake_consume_newline(ps, n)) {
            n->has_error = true;
        }

		b = Ake_parse_mult(ps);

		if (!b) {
            struct Zinc_location b_loc = Ake_get_location(ps);
			Zinc_error_list_set(ps->el, &b_loc, "expected term after additive operator");
			/* test case: test_parse_add_error_expected_term */
            n->has_error = true;
		}

        if (left) {
        	if (type == AKE_AST_PLUS) {
        		n->data.plus.left = left;
        	} else if (type == AKE_AST_MINUS) {
        		n->data.minus.left = left;
        	} else {
        		assert(false && "not possible");
        	}
            Ake_AstAdd2(n, left);
        }

        if (b) {
        	if (type == AKE_AST_PLUS) {
        		n->data.plus.right = b;
        	} else if (type == AKE_AST_MINUS) {
        		n->data.minus.right = b;
        	} else {
        		assert(false && "not possible");
        	}
            Ake_AstAdd2(n, b);
        }

		if (!n->has_error) {
			Ake_Type* a_type = a->type;
			Ake_Type* b_type = b->type;

			if (!a_type) {
				Zinc_error_list_set(ps->el, &a->loc, "%s operand has no value", op_name);
				/* test case: test_parse_add_error_left_no_value */
                n->has_error = true;
			} else if (!Ake_is_numeric(a_type)) {
				Zinc_error_list_set(ps->el, &a->loc, "%s on non-numeric operand", op_name);
                n->has_error = true;
			}

			if (!b_type) {
				Zinc_error_list_set(ps->el, &b->loc, "%s operand has no value", op_name);
				/* test case: test_parse_add_error_right_no_value */
                n->has_error = true;
			} else if (!Ake_is_numeric(b_type)) {
				Zinc_error_list_set(ps->el, &b->loc, "%s on non-numeric operand", op_name);
				/* test case: test_parse_add_error_right_not_numeric */
                n->has_error = true;
			}

			if (!n->has_error) {
				Ake_Type* a_type2 = Ake_TypeClone(a_type);
				bool cast = false;
				if (!Ake_TypeMatch(a_type2, b_type, &cast)) {
					Zinc_error_list_set(ps->el, &op->loc, "invalid types for %s", op_name);
					/* test case: no test case needed */
					n->has_error = true;
                    Ake_TypeDestroy(a_type2);
				} else {
					n->type = a_type2;
				}
			}

			left = n;
		}
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
			type = AKE_AST_MULT;
			op_name = "multiplication";
		} else if (t0->type == Ake_token_divide) {
			type = AKE_AST_DIVIDE;
			op_name = "division";
		} else {
			break;
		}

        Ake_AstCreate(&n);
		Ake_AstSet(n, type);

        struct Ake_token* op = NULL;
		if (!Ake_match(ps, t0->type, "expecting * or /", &op, n)) {
            /* test case: test case not needed */
            assert(false);
        }

        if (!Ake_consume_newline(ps, n)) {
            n->has_error = true;
        }

		/* parse_factor */
		b = Ake_parse_power(ps);

		if (!b) {
            struct Zinc_location b_loc = Ake_get_location(ps);
			Zinc_error_list_set(ps->el, &b_loc, "expected term after operator");
			/* test case: test_parse_mult_error_expected_term */
            n->has_error = true;
		}

        if (left) {
        	if (type == AKE_AST_MULT) {
        		n->data.mult.left = left;
        		Ake_AstAdd2(n, left);
        	}
        	if (type == AKE_AST_DIVIDE) {
        		n->data.divide.left = left;
        		Ake_AstAdd2(n, left);
        	}
        }

        if (b) {
        	if (type == AKE_AST_MULT) {
        		n->data.mult.right = b;
				Ake_AstAdd2(n, b);
        	}
        	if (type == AKE_AST_DIVIDE) {
        		n->data.divide.right = b;
        		Ake_AstAdd2(n, b);
        	}
        }

		if (!n->has_error) {
			assert(a);
			assert(b);
			Ake_Type* a_type = a->type;
			Ake_Type* b_type = b->type;

			if (!a_type) {
				Zinc_error_list_set(ps->el, &a->loc, "%s operand has no value", op_name);
				/* test case: test_parse_mult_error_left_no_value */
				n->has_error = true;
			} else if (!Ake_is_numeric(a_type)) {
				Zinc_error_list_set(ps->el, &a->loc, "%s on non-numeric operand", op_name);
				/* test case: test_parse_mult_error_left_not_numeric */
				n->has_error = true;
			}

			if (!b_type) {
				Zinc_error_list_set(ps->el, &b->loc, "%s operand has no value", op_name);
				/* test case: test_parse_mult_error_right_no_value*/
				n->has_error = true;
			} else if (!Ake_is_numeric(b_type)) {
				Zinc_error_list_set(ps->el, &b->loc, "%s on non-numeric operand", op_name);
				/* test case: test_parse_mult_error_right_not_numeric */
				n->has_error = true;
			}

			if (!n->has_error) {
				Ake_Type* a_type2 = Ake_TypeClone(a_type);
				bool cast = false;
				if (!Ake_TypeMatch(a_type2, b_type, &cast)) {
					Zinc_error_list_set(ps->el, &op->loc, "invalid types for %s", op_name);
					/* test case: no test case needed */
					n->has_error = true;
                    Ake_TypeDestroy(a_type2);
				} else {
					n->type = a_type2;
				}
			}

			left = n;
		}
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

        Ake_AstCreate(&n);
        n->kind = Ake_ast_type_power;
        if (left) {
            Ake_AstAdd(n, left);
        }

        struct Ake_token* caret = NULL;
		if (!Ake_match(ps, Ake_token_caret, "expected a caret", &caret, n)) {
            /* test case: no test case needed */
            assert(false);
        }

        Ake_consume_newline(ps, n);

		Ake_Ast* b = NULL;
		b = Ake_parse_complex_operators(ps);

        if (b) {
            Ake_AstAdd(n, b);
        }

        if (!b) {
            struct Zinc_location b_loc = Ake_get_location(ps);
            Zinc_error_list_set(ps->el, &b_loc, "expected term after caret");
            /* test case: test_parse_power_error_expected_term */
			n->has_error = true;
        }

        if (!n->has_error) {
			assert(left);
			assert(b);
			Ake_Type* left_type = left->type;
			Ake_Type* b_type = b->type;

			if (!left_type) {
				Zinc_error_list_set(ps->el, &left->loc, "power operand has no value");
				/* test case: test_parse_power_error_left_no_value */
				n->has_error = true;
			} else if (!Ake_is_numeric(left_type)) {
				Zinc_error_list_set(ps->el, &left->loc, "power on non-numeric operand");
				/* test case: test_parse_power_error_left_not_numeric */
				n->has_error = true;
			}

			if (!b_type) {
				Zinc_error_list_set(ps->el, &b->loc, "power operand has no value");
				/* test case: test_parse_power_error_right_no_value */
				n->has_error = true;
			} else if (!Ake_is_numeric(b_type)) {
				Zinc_error_list_set(ps->el, &b->loc, "power on non-numeric operand");
				/* test case: test_parse_power_error_right_not_numeric */
				n->has_error = true;
			}

			if (!n->has_error) {
				Ake_Type* left_type2 = Ake_TypeClone(left_type);
				bool cast = false;
				if (!Ake_TypeMatch(left_type2, b_type, &cast)) {
					Zinc_error_list_set(ps->el, &b->loc, "invalid power types");
					/* test case: no test case needed */
					n->has_error = true;
                    Ake_TypeDestroy(left_type2);
				} else {
					n->type = left_type2;
				}
			}

			left = n;
		}
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

        Ake_AstCreate(&n);
        if (t0->type == Ake_token_left_square_bracket) {
            Ake_parse_subscript(ps, left, n);
        } else if (t0->type == Ake_token_left_paren) {
            Ake_parse_call(ps, left, n);
        }

        if (left->has_error) {
            n->has_error = true;
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
        left->has_error = true;
    }

	bool is_array_or_slice = false;
    if (!left->has_error) {
        if (left->type->kind == AKE_TYPE_ARRAY) {
	        n->type = Ake_TypeClone(left->type->data.array.type);
        	is_array_or_slice = true;
        } else if (left->type->kind == AKE_TYPE_SLICE) {
            n->type = Ake_TypeClone(left->type->data.slice.type);
        	is_array_or_slice = true;
        }
    }

    if (!n->has_error) {
        if (!is_array_or_slice) {
            Zinc_error_list_set(ps->el,
                           &left->loc,
                           "expression has subscript but is not an array or slice");
            /* test case: test_parse_subscript_error_not_array */
            n->has_error = true;
        }
    }

    struct Ake_token* lsb = NULL;
    if (!Ake_match(ps, Ake_token_left_square_bracket, "expecting array subscript operator", &lsb, n)) {
        /* test case: no test case needed */
        assert(false);
    }

    Ake_consume_newline(ps,  n);

    Ake_Ast* b = NULL;
	ps->context.is_subscript = true;
    b = Ake_parse_expr(ps);
	ps->context.is_subscript = false;

    Ake_consume_newline(ps, n);

    struct Ake_token* rsb = NULL;
    if (!Ake_match(ps, Ake_token_right_square_bracket, "expected right-square-bracket", &rsb, n)) {
        n->has_error = true;
    }

    Ake_AstAdd(n, left);

    if (b) {
        Ake_AstAdd(n, b);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_parse_call(struct Ake_parse_state* ps, Ake_Ast* left, Ake_Ast* n) {
	Ake_AstSet(n, AKE_AST_CALL);

    struct Ake_token *lp = NULL;
    if (!Ake_match(ps, Ake_token_left_paren, "expected left parenthesis", &lp, n)) {
        /* test case: test case not needed */
        n->has_error = true;
    }

    Ake_consume_newline(ps, n);

	n->data.call.func = left;
	Ake_AstAdd2(n, left);

    Ake_parse_args(ps, n);

    Ake_consume_newline(ps, n);

    struct Ake_token *rp = NULL;
    if (!Ake_match(ps, Ake_token_right_paren, "expected right parenthesis", &rp, n)) {
        /* test case: test_parse_call_error_right_paren */
        n->has_error = true;
    }

    if (!n->has_error) {
        Ake_Type *type = left->type;
        assert(type);
        if (type->kind != AKE_TYPE_FUNCTION) {
            Zinc_error_list_set(ps->el, &left->loc, "not a function type");
            /* test case: test_parse_call_error_not_function */
			n->has_error = true;
        } else {
            /* input */
            size_t tcount = 0;
            bool is_variadic = false;
        	Ake_TypeParam* tp = type->data.function.input_head;
            while (tp) {
	            if (tp->kind == AKE_TYPE_PARAM_ELLIPSIS) {
	            	is_variadic = true;
	            } else {
	            	tcount++;
	            }
            	tp = tp->next;
            }
            size_t ccount = 0;
            ccount = Ake_AstListCount(&n->data.call.args);

            if (ccount < tcount) {
                Zinc_error_list_set(ps->el, &rp->loc, "not enough arguments in function call");
                /* test case: test_parse_call_error_not_enough_arguments */
				n->has_error = true;
            } else if (!is_variadic && ccount > tcount) {
                Zinc_error_list_set(ps->el, &rp->loc, "too many arguments in function call");
                /* test case: test_parse_call_error_too_many_arguments */
				n->has_error = true;
            }

            /* output */
            if (type->data.function.output) {
                n->type = Ake_TypeClone(type->data.function.output);
            }
        }

    }
}

/* cseq -> expr cseq' | e */
/* cseq' -> , expr cseq' | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_parse_args(Ake_parse_state* ps, Ake_Ast* n)
{
    if (!n->data.call.func->type || n->data.call.func->type->kind != AKE_TYPE_FUNCTION) {
        Zinc_error_list_set(ps->el, &n->data.call.func->loc, "not a function type");
        /* test case: no test case needed */
		n->has_error = true;
        return;
    }

    Ake_Ast* a = NULL;
    a = Ake_parse_expr(ps);

    if (!a) {
        return;
    }

    int i = 0;
    if (!Ake_check_input_type(ps, n->data.call.func->type, i, a)) {
		n->has_error = true;
    }
    i++;

	Ake_AstListAdd(&n->data.call.args, a);

    while (true) {
        struct Ake_token* t0 = Ake_get_lookahead(ps);

        if (!t0 || t0->type != Ake_token_comma) {
            break;
        }

        struct Ake_token* comma = NULL;
        if (!Ake_match(ps, Ake_token_comma, "expecting comma", &comma, n)) {
            /* test case: no test case needed */
			n->has_error = true;
        }

        Ake_consume_newline(ps, n);

        a = Ake_parse_expr(ps);

        if (!a) {
            struct Zinc_location a_loc = Ake_get_location(ps);
            Zinc_error_list_set(ps->el, &a_loc, "expected expression after comma");
            /* test case: test_parse_call_error_expected_expression */
			n->has_error = true;
        } else {
            Ake_AstListAdd(&n->data.call.args, a);

            if (!Ake_check_input_type(ps, n->data.call.func->type, i, a)) {
				n->has_error = true;
            }
        }

        i++;
    }
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

        Ake_AstCreate(&n);
        n->kind = Ake_ast_type_dot;

        struct Ake_token* dot = NULL;
        if (!Ake_match(ps, Ake_token_dot, "expected a dot", &dot, n)) {
            /* test case: no test case needed */
			n->has_error = true;
        }

        if (!Ake_consume_newline(ps, n)) {
			n->has_error = true;
        }

        struct Ake_token* id = NULL;
        if (!Ake_match(ps, Ake_token_id, "expected identifier", &id, n)) {
			n->has_error = true;
        }

        Ake_Ast* b = NULL;
        Ake_AstCreate(&b);
        Ake_AstSet(b, AKE_AST_ID);
        if (id) {
            Zinc_string_copy(&id->value, &b->data.id.value);
        }

        Ake_AstAdd(n, left);

        Ake_AstAdd(n, b);

        if (!n->has_error) {
            if (!left->type) {
                Zinc_error_list_set(ps->el, &left->loc, "dot operand has no value");
                /* test case: no test case necessary */
				n->has_error = true;
            } else if (left->type->kind != AKE_TYPE_STRUCT) {
                Zinc_error_list_set(ps->el, &left->loc, "dot operand is not a struct");
                /* test case: test_parse_dot_error_left_non_module */
				n->has_error = true;
            } else {
                Ake_Type* type = left->type;
                assert(type);
            	size_t seq = Ake_get_current_seq(ps);
                struct Ake_Symbol* sym = Ake_EnvironmentGet(ps->st->top, &type->name);
                assert(sym);
                //left->sym = sym;
                assert(sym->td);
                assert(sym->td->data.fields.head);
                bool found = false;
                Ake_TypeField* tf = sym->td->data.fields.head;
            	Ake_Type* found_type = NULL;
                while (tf) {
                    if (Zinc_string_compare(&tf->name, &b->data.id.value)) {
                    	found_type = tf->type;
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
					n->has_error = true;
                } else {
                    n->type = Ake_TypeClone(found_type);
                }
            }

            left = n;
        }
    }

    return n;
}