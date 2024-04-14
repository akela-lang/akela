#include <stdbool.h>
#include "zinc/result.h"
#include "token.h"
#include "ast.h"
#include "parse_tools.h"
#include "parse_factor.h"
#include "source.h"
#include "lex.h"
#include "parse_types.h"
#include "parse_expr.h"
#include "type_def.h"
#include <assert.h>

struct ast_node* parse_assignment(struct parse_state* ps, struct location* loc);
struct ast_node* parse_eseq(struct parse_state* ps, size_t assign_index, struct location* loc);
struct ast_node* parse_boolean(struct parse_state* ps, struct location* loc);
struct ast_node* parse_comparison(struct parse_state* ps, struct location* loc);
struct ast_node* parse_add(struct parse_state* ps, struct location* loc);
struct ast_node* parse_mult(struct parse_state* ps, struct location* loc);
struct ast_node* parse_power(struct parse_state* ps, struct location* loc);
struct ast_node* parse_subscript(struct parse_state* ps, struct location* loc);
struct ast_node* parse_call(struct parse_state* ps, struct location* loc);
struct ast_node* parse_cseq(struct parse_state* ps, struct ast_node* tu, struct location* loc);
struct ast_node* parse_dot(struct parse_state* ps, struct location* loc);

/* expr -> assignment */
struct ast_node* parse_expr(struct parse_state* ps, struct location* loc)
{
    return parse_assignment(ps, loc);
}

bool is_lvalue(enum ast_type type)
{
    if (type == ast_type_id) {
        return true;
    }

    if (type == ast_type_array_subscript) {
        return true;
    }

    if (type == ast_type_var) {
        return true;
    }

    if (type == ast_type_eseq) {
        return true;
    }

    return false;
}

bool check_lvalue(struct parse_state* ps, enum ast_type type, struct location* loc)
{
    bool valid = true;

    if (!is_lvalue(type)) {
        valid = set_source_error(ps->el, loc, "invalid lvalue");
    }

    return valid;
}

/* assignment -> eseq = assignment | eseq */
struct ast_node* parse_assignment(struct parse_state* ps, struct location* loc)
{
	struct ast_node* n = NULL;
    size_t assign_index = 0;
    size_t assign_count = 0;
    bool has_eseq = false;

    get_location(ps, loc);

	struct ast_node* last_p = NULL;
	struct ast_node* last_a = NULL;
	struct location loc_last_a;

	struct ast_node* a = NULL;
	struct location a_loc;

	while (true) {
		bool done = false;

		a = parse_eseq(ps, assign_index, &a_loc);
        if (n == NULL) {
            n = a;
        }
        if (assign_index == 0) {
            *loc = a_loc;
        }
        if (a && a->type == ast_type_eseq) {
            has_eseq = true;
        }

		if (!a) {
			if (last_p) {
				set_source_error(ps->el, &a_loc, "expected an assignment term");
				/* test case: test_parse_assign_error_term */
                n->type = ast_type_error;
				break;
			} else {
				break;
			}
		}

		struct token* t0 = get_lookahead(ps);
		struct ast_node* p = NULL;
		if (t0 && t0->type == token_equal) {
            assign_count++;

            struct token *equal = NULL;
            if (!match(ps, token_equal, "expecting assign operator", &equal)) {
                /* test case: no test case needed */
                assert(false);
            }

            if (has_eseq && assign_count > 1) {
                set_source_error(ps->el, loc, "more than one assignment adjacent to an assignment sequence");
                /* test case: test_parse_expr_assignment_eseq_error_too_many_assignments */
                n->type = ast_type_error;
            }

            token_destroy(equal);
            free(equal);

            if (!consume_newline(ps)) {
                n->type = ast_type_error;
            }

            if (!check_lvalue(ps, a->type, &a_loc)) {
                /* test case: test_parse_expr_error_lvalue */
                n->type = ast_type_error;
            }

            ast_node_create(&p);
            p->type = ast_type_assign;
            ast_node_add(p, a);
            if (a->type == ast_type_error) {
                p->type = ast_type_error;
            }

            if (last_p) {
                ast_node_add(last_p, p);
                if (p->type == ast_type_error) {
                    last_p->type = ast_type_error;
                }
            } else {
                n = p;
            }

        } else {
            if (last_p) {
                ast_node_add(last_p, a);
            }
            done = true;
        }


		if (n && n->type != ast_type_error) {
			assert(a);
			if (last_p && last_a) {
				struct ast_node* last_a_tu = last_a->tu;
				struct ast_node* a_tu = a->tu;

				if (a->type != ast_type_eseq && !a_tu) {
					set_source_error(ps->el, &a_loc, "cannot assign with operand that has no value");
					/* test case: test_parse_assign_error_no_value_right */
                    n->type = ast_type_error;
				}
				if (n->type != ast_type_error) {
					if (!type_use_can_cast(last_a_tu, a_tu)) {
						set_source_error(ps->el, &a_loc, "values in assignment not compatible");
						/* test case: test_parse_assign_error_not_compatible */
                        n->type = ast_type_error;
					}
				}
			}

			if (n->type != ast_type_error) {
				if (p && a->tu) {
					p->tu = ast_node_copy(a->tu);
				}
			}

			last_p = p;
			last_a = a;
			loc_last_a = a_loc;
		}

		if (done) {
			break;
		}

        assign_index++;
	}

    if (n && n->type != ast_type_error) {
        if (n && n->type == ast_type_assign) {
            struct ast_node* n0 = ast_node_get(n, 0);
            struct ast_node* n1 = ast_node_get(n, 1);
            if (n0 && n0->type == ast_type_eseq && n1 && n1->type == ast_type_eseq) {
                int count0 = ast_node_count_children(n0);
                int count1 = ast_node_count_children(n1);
                if (count0 != count1) {
                    set_source_error(ps->el, loc, "assignment sequence counts do not match");
                    /* test case: test_parse_expr_assignment_eseq_error_eseq_count */
                    n->type = ast_type_error;
                }
            }
        }
    }

	return n;
}

/* eseq = boolean | , boolean */
struct ast_node* parse_eseq(struct parse_state* ps, size_t assign_index, struct location* loc)
{
    struct ast_node* a = NULL;
    struct location a_loc;
    a = parse_boolean(ps, &a_loc);
    *loc = a_loc;

    if (!a) {
        return NULL;
    }

    struct ast_node* parent = NULL;
    while (true) {
        struct token* t0 = get_lookahead(ps);

        if (!t0 || t0->type != token_comma) {
            break;
        }

        if (!parent) {
            ast_node_create(&parent);
            parent->type = ast_type_eseq;
            ast_node_add(parent, a);
            if (a->type == ast_type_error) {
                parent->type = ast_type_error;
            }

            if (parent->type != ast_type_error) {
                if (!a->tu) {
                    set_source_error(ps->el, &a_loc, "operand of eseq has no type");
                    parent->type = ast_type_error;
                }
                if (assign_index == 0) {
                    if (!is_lvalue(a->type)) {
                        set_source_error(ps->el, &a_loc, "invalid lvalue");
                        /* test case: test_parse_expr_error_eseq_lvalue */
                        parent->type = ast_type_error;
                    }
                }
            }
        }

        struct token* comma = NULL;
        if (!match(ps, token_comma, "expected a comma", &comma)) {
            /* test case: no test case needed */
            assert(false);
        }
        token_destroy(comma);
        free(comma);

        struct ast_node* b = NULL;
        struct location b_loc;
        b = parse_boolean(ps, &b_loc);
        if (b && b->type == ast_type_error) {
            parent->type = ast_type_error;
        }

        /* parent checks */
        if (!b) {
            set_source_error(ps->el, &b_loc, "expected term after comma");
            parent->type = ast_type_error;
        } else if (assign_index == 0) {
            if (!is_lvalue(b->type)) {
                set_source_error(ps->el, &b_loc, "invalid lvalue");
                parent->type = ast_type_error;
            }
        }

        if (b) {
            if (!b->tu) {
                set_source_error(ps->el, &b_loc, "operand of eseq has no type");
                b->type = ast_type_error;
            }
        }

        if (b) {
            ast_node_add(parent, b);
            if (b->type == ast_type_error) {
                parent->type = ast_type_error;
            }
        }
    }

    if (parent == NULL) {
        parent = a;
    }
    return parent;
}

struct ast_node* parse_simple_expr(struct parse_state* ps, struct location* loc)
{
    return parse_boolean(ps, loc);
}

/* boolean -> comparison boolean' */
/* boolean' -> && comparison boolean' | || comparison boolean' | e */
struct ast_node* parse_boolean(struct parse_state* ps, struct location* loc)
{
    get_location(ps, loc);

	struct ast_node* n = NULL;
	struct ast_node* left;
    struct location left_loc;

	struct ast_node* a = NULL;
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
		enum ast_type type = ast_type_none;
		if (t0 && t0->type == token_and) {
			type = ast_type_and;
		} else if (t0 && t0->type == token_or) {
			type = ast_type_or;
		} else {
			break;
		}

		ast_node_create(&n);
		n->type = type;

		struct token* op = NULL;
		if (!match(ps, t0->type, "expecting && or ||", &op)) {
            /* test case: no test case needed */
            assert(false);
        }

        if (!consume_newline(ps)) {
            n->type = ast_type_error;
        }

		/* comparison */
		struct ast_node* b = NULL;
		struct location b_loc;
		b = parse_comparison(ps, &b_loc);

		if (!b) {
			set_source_error(ps->el, &b_loc, "expected term after && or ||");
			/* test case: test_parse_boolean_error_expected_term */
            n->type = ast_type_error;
		}

        if (left) {
            ast_node_add(n, left);
            if (left->type == ast_type_error) {
                n->type = ast_type_error;
            }
        }
        if (b) {
            ast_node_add(n, b);
            if (b->type == ast_type_error) {
                n->type = ast_type_error;
            }
        }

		if (n->type != ast_type_error) {
			assert(b);
			if (!left->tu) {
				set_source_error(ps->el, &left_loc, "left-side operand of boolean operator has no type");
				/* test case: test_parse_boolean_error_left_no_value */
                n->type = ast_type_error;
			} else if (!b->tu) {
				set_source_error(ps->el, &b_loc, "operand of boolean operator has no type");
				/* test case: test_parse_boolean_error_right_no_value */
                n->type = ast_type_error;
			} else if (left->tu->td->type != type_boolean) {
				set_source_error(ps->el, &left_loc, "left-side expression of boolean operator is not boolean");
                n->type = ast_type_error;
			} else if (b->tu->td->type != type_boolean) {
				set_source_error(ps->el, &b_loc, "expression of boolean operator is not boolean");
                n->type = ast_type_error;
			} else {
				n->tu = ast_node_copy(left->tu);
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
struct ast_node* parse_comparison(struct parse_state* ps, struct location* loc)
{
	struct ast_node* n = NULL;
	struct ast_node* a = NULL;
	struct ast_node* left = NULL;
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
		enum ast_type type = ast_type_none;

		if (!t0) {
			break;
		}

		if (t0->type == token_double_equal) {
			type = ast_type_equality;
		} else if (t0->type == token_not_equal) {
			type = ast_type_not_equal;
		} else if (t0->type == token_less_than) {
			type = ast_type_less_than;
		} else if (t0->type == token_less_than_or_equal) {
			type = ast_type_less_than_or_equal;
		} else if (t0->type == token_greater_than) {
			type = ast_type_greater_than;
		} else if (t0->type == token_greater_than_or_equal) {
			type = ast_type_greater_than_or_equal;
		} else {
			break;
		}

		struct token* op = NULL;
		if (!match(ps, t0->type, "expecting comparator", &op)) {
            /* test case: no test case needed */
            n->type = ast_type_error;
        }

        if (!consume_newline(ps)) {
            n->type = ast_type_error;
        }

		struct ast_node* b = NULL;
		struct location loc_b;
		b = parse_add(ps, &loc_b);

		if (!b) {
			set_source_error(ps->el, &loc_b, "expected term after comparison operator");
			/* case case: test_parse_comparison_error_no_term */
            n->type = ast_type_error;
		}

        ast_node_create(&n);
        n->type = type;

        if (left) {
            ast_node_add(n, left);
            if (left->type == ast_type_error) {
                n->type = ast_type_error;
            }
        }

        if (b) {
            ast_node_add(n, b);
            if (b->type == ast_type_error) {
                n->type = ast_type_error;
            }
        }

		if (n->type != ast_type_error) {
			assert(a);
			assert(b);
			assert(op);
			if (!left->tu) {
				set_source_error(ps->el, &left_loc, "operand has no value");
				/* test case: test_parse_comparison_error_left_not_numeric */
                n->type = ast_type_error;
			} else if (!b->tu) {
				set_source_error(ps->el, &loc_b, "operand has no value");
				/* test case: test_parse_comparison_error_right_no_value */
                n->type = ast_type_error;
			} else {
				if (!is_identity_comparison(type) && !is_numeric(left->tu->td)) {
					set_source_error(ps->el, &left_loc, "comparison operand is not numeric");
					/* test case: test_parse_comparison_error_left_not_numeric */
                    n->type = ast_type_error;
				} else if (!is_identity_comparison(type) && !is_numeric(b->tu->td)) {
					set_source_error(ps->el, &loc_b, "comparison operand is not numeric");
					/* test case: test_parse_comparison_error_right_not_numeric */
                    n->type = ast_type_error;
				} else {
					struct ast_node* tu = ast_node_copy(left->tu);
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
struct ast_node* parse_add(struct parse_state* ps, struct location* loc)
{
	struct ast_node* a = NULL;
	struct ast_node* b = NULL;
	struct ast_node* n = NULL;
	struct ast_node* left = NULL;
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
		enum ast_type type;
		if (t0->type == token_plus) {
			type = ast_type_plus;
			op_name = "addition";
		} else if (t0->type == token_minus) {
			type = ast_type_minus;
			op_name = "subtraction";
		} else {
			break;
		}

		struct token* op = NULL;
		if (!match(ps, t0->type, "expecting + or -", &op)) {
            /* test case: no test case needed */
            n->type = ast_type_error;
        }

        if (!consume_newline(ps)) {
            n->type = ast_type_error;
        }

		struct location b_loc;
		b = parse_mult(ps, &b_loc);

		if (!b) {
			set_source_error(ps->el, &b_loc, "expected term after additive operator");
			/* test case: test_parse_add_error_expected_term */
            n->type = ast_type_error;
		}

        ast_node_create(&n);
        n->type = type;

        if (left) {
            ast_node_add(n, left);
            if (left->type == ast_type_error) {
                n->type = ast_type_error;
            }
        }

        if (b) {
            ast_node_add(n, b);
            if (b->type == ast_type_error) {
                n->type = ast_type_error;
            }
        }

		if (n->type != ast_type_error) {
			struct ast_node* tu_a = a->tu;
			struct ast_node* tu_b = b->tu;

			if (!tu_a) {
				set_source_error(ps->el, &a_loc, "%s operand has no value", op_name);
				/* test case: test_parse_add_error_left_no_value */
                n->type = ast_type_error;
			} else if (!is_numeric(tu_a->td)) {
				set_source_error(ps->el, &a_loc, "%s on non-numeric operand", op_name);
                n->type = ast_type_error;
			}

			if (!tu_b) {
				set_source_error(ps->el, &b_loc, "%s operand has no value", op_name);
				/* test case: test_parse_add_error_right_no_value */
                n->type = ast_type_error;
			} else if (!is_numeric(tu_b->td)) {
				set_source_error(ps->el, &b_loc, "%s on non-numeric operand", op_name);
				/* test case: test_parse_add_error_right_not_numeric */
                n->type = ast_type_error;
			}

			if (n->type != ast_type_error) {
				struct ast_node* tu = ast_node_copy(tu_a);
				if (!type_find_whole(ps->st, tu, tu_b)) {
					set_source_error(ps->el, &op->loc, "invalid types for %s", op_name);
					/* test case: no test case needed */
                    n->type = ast_type_error;
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
struct ast_node* parse_mult(struct parse_state* ps, struct location* loc)
{
	struct ast_node* a = NULL;
	struct ast_node* b = NULL;
	struct ast_node* left = NULL;
	struct ast_node* n = NULL;
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
		enum ast_type type;
		if (t0->type == token_mult) {
			type = ast_type_mult;
			op_name = "multiplication";
		} else if (t0->type == token_divide) {
			type = ast_type_divide;
			op_name = "division";
		} else {
			break;
		}

		struct token* op = NULL;
		if (!match(ps, t0->type, "expecting * or /", &op)) {
            /* test case: test case not needed */
            n->type = ast_type_error;
        }

        if (!consume_newline(ps)) {
            n->type = ast_type_error;
        }

		/* parse_factor */
		struct location b_loc;
		b = parse_power(ps, &b_loc);

		if (!b) {
			set_source_error(ps->el, &b_loc, "expected term after operator");
			/* test case: test_parse_mult_error_expected_term */
            n->type = ast_type_error;
		}

        ast_node_create(&n);
        n->type = type;

        if (left) {
            ast_node_add(n, left);
            if (left->type == ast_type_error) {
                n->type = ast_type_error;
            }
        }

        if (b) {
            ast_node_add(n, b);
            if (b->type == ast_type_error) {
                n->type = ast_type_error;
            }
        }

		if (n->type != ast_type_error) {
			assert(a);
			assert(b);
			struct ast_node* tu_a = a->tu;
			struct ast_node* tu_b = b->tu;

			if (!tu_a) {
				set_source_error(ps->el, &a_loc, "%s operand has no value", op_name);
				/* test case: test_parse_mult_error_left_no_value */
                n->type = ast_type_error;
			} else if (!is_numeric(tu_a->td)) {
				set_source_error(ps->el, &a_loc, "%s on non-numeric operand", op_name);
				/* test case: test_parse_mult_error_left_not_numeric */
                n->type = ast_type_error;
			}

			if (!tu_b) {
				set_source_error(ps->el, &b_loc, "%s operand has no value", op_name);
				/* test case: test_parse_mult_error_right_no_value*/
                n->type = ast_type_error;
			} else if (!is_numeric(tu_b->td)) {
				set_source_error(ps->el, &b_loc, "%s on non-numeric operand", op_name);
				/* test case: test_parse_mult_error_right_not_numeric */
                n->type = ast_type_error;
			}

			if (n->type != ast_type_error) {
				struct ast_node* tu = ast_node_copy(tu_a);
				if (!type_find_whole(ps->st, tu, tu_b)) {
					set_source_error(ps->el, &op->loc, "invalid types for %s", op_name);
					/* test case: no test case needed */
                    n->type = ast_type_error;
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
struct ast_node* parse_power(struct parse_state* ps, struct location* loc)
{
	struct ast_node* n = NULL;
	struct ast_node* a = NULL;

	struct location a_loc;
	a = parse_subscript(ps, &a_loc);
    *loc = a_loc;

	if (!a) {
		return a;
	}

	struct ast_node* left = n = a ;
	struct location loc_left = a_loc;

	while (true) {
		struct token* t0 = NULL;
		int num;
		t0 = get_lookahead(ps);
		if (!t0 || t0->type != token_caret) {
			break;
		}

		struct token* caret = NULL;
		if (!match(ps, token_caret, "expected a caret", &caret)) {
            /* test case: no test case needed */
            assert(false);
        }

        if (!consume_newline(ps)) {
            n->type = ast_type_error;
        }

		struct ast_node* b = NULL;
		struct location b_loc;
		b = parse_subscript(ps, &b_loc);
        if (b && b->type == ast_type_error) {
            n->type = ast_type_error;
        }

        ast_node_create(&n);
        n->type = ast_type_power;
        if (left) {
            ast_node_add(n, left);
            if (left->type == ast_type_error) {
                n->type = ast_type_error;
            }
        }
        if (b) {
            ast_node_add(n, b);
            if (b->type == ast_type_error) {
                n->type = ast_type_error;
            }
        }

        if (!b) {
            set_source_error(ps->el, &b_loc, "expected term after caret");
            /* test case: test_parse_power_error_expected_term */
            n->type = ast_type_error;
        }

        if (n->type != ast_type_error) {
			assert(left);
			assert(b);
			struct ast_node* tu_left = left->tu;
			struct ast_node* tu_b = b->tu;

			if (!tu_left) {
				set_source_error(ps->el, &loc_left, "power operand has no value");
				/* test case: test_parse_power_error_left_no_value */
                n->type = ast_type_error;
			} else if (!is_numeric(tu_left->td)) {
				set_source_error(ps->el, &loc_left, "power on non-numeric operand");
				/* test case: test_parse_power_error_left_not_numeric */
                n->type = ast_type_error;
			}

			if (!tu_b) {
				set_source_error(ps->el, &b_loc, "power operand has no value");
				/* test case: test_parse_power_error_right_no_value */
                n->type = ast_type_error;
			} else if (!is_numeric(tu_b->td)) {
				set_source_error(ps->el, &b_loc, "power on non-numeric operand");
				/* test case: test_parse_power_error_right_not_numeric */
                n->type = ast_type_error;
			}

			if (n->type != ast_type_error) {
				struct ast_node* tu = ast_node_copy(tu_left);
				if (!type_find_whole(ps->st, tu, tu_b)) {
					set_source_error(ps->el, &b_loc, "invalid power types");
					/* test case: no test case needed */
                    n->type = ast_type_error;
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

/* subscript -> call subscript' */
/* subscript' -> [expr] subscript' | e */
struct ast_node* parse_subscript(struct parse_state* ps, struct location* loc)
{
	int num;
	struct ast_node* n = NULL;
	struct ast_node* a = NULL;

	struct location a_loc;
	a = parse_call(ps, &a_loc);
    *loc = a_loc;

    if (!a) {
        return a;
    }

    struct ast_node* tu = NULL;
	struct ast_node* element_tu = NULL;
    struct location loc_last;

	while (true) {
		struct token* t0 = get_lookahead(ps);

		if (!t0 || t0->type != token_left_square_bracket) {
			break;
		}

        if (!n) {
            ast_node_create(&n);
            n->type = ast_type_array_subscript;
            ast_node_add(n, a);
        }

        if (n->type != ast_type_error) {
			if (!tu) {
				tu = a->tu;
			} else {
				tu = ast_node_get(tu, 0);
			}

			if (!tu) {
				set_source_error(ps->el, &a_loc, "subscripting expression with no type");
				/* test case: test_parse_subscript_error_no_type */
                n->type = ast_type_error;
			} else if (tu->td->type != type_array) {
				set_source_error(ps->el, &a_loc, "subscripting expression that is not an array");
				/* test case: test_parse_subscript_error_not_array */
                n->type = ast_type_error;
			}
		}

		struct token* lsb = NULL;
		if (!match(ps, token_left_square_bracket, "expecting array subscript operator", &lsb)) {
            /* test case: no test case needed */
            assert(false);
        }
		if (lsb) {
		#pragma warning(suppress:6001)
			loc_last = lsb->loc;
		}

		token_destroy(lsb);
		free(lsb);

        if (!consume_newline(ps)) {
            n->type = ast_type_error;
        }

		struct ast_node* b = NULL;
		struct location loc_expr;
        b = parse_expr(ps, &loc_expr);
		if (b && b->type == ast_type_error) {
            n->type = ast_type_error;
        }

        if (!consume_newline(ps)) {
            n->type = ast_type_error;
        }

		struct token* rsb = NULL;
		if (!match(ps, token_right_square_bracket, "expected right-square-bracket", &rsb)) {
            n->type = ast_type_error;
        }

		token_destroy(rsb);
		free(rsb);

		if (b) {
			ast_node_add(n, b);
		}
	}

	if ((n && n->type != ast_type_error) || (a && a->type != ast_type_error)) {
		if (tu) {
			element_tu = ast_node_get(tu, 0);
		}

		if (n) {
			if (!element_tu) {
				set_source_error(ps->el, &loc_last, "subscripting expression with no subtype");
				/* test case: test_parse_subscript_error_no_subtype */
                n->type = ast_type_error;
			} else {
				n->tu = ast_node_copy(element_tu);
			}
		}
	}

    if (!n) {
        n = a;
    }

	return n;
}

/* call -> dot call' */
/* call' -> ( cseq ) call' */
struct ast_node* parse_call(struct parse_state* ps, struct location* loc)
{
	struct ast_node* n = NULL;

	struct ast_node* dot_node = NULL;
	struct location dot_loc;
	dot_node = parse_dot(ps, &dot_loc);
    *loc = dot_loc;

	if (!dot_node) {
		return dot_node;
	}

	struct ast_node* left = n = dot_node;
	struct location left_loc = dot_loc;

	while (true) {
		struct token* t0 = get_lookahead(ps);

		if (!t0 || t0->type != token_left_paren) {
			break;
		}

        ast_node_create(&n);
        n->type = ast_type_call;

        struct token* lp = NULL;
		if (!match(ps, token_left_paren, "expected left parenthesis", &lp)) {
            /* test case: test case not needed */
            n->type = ast_type_error;
        }

        if (!consume_newline(ps)) {
            n->type = ast_type_error;
        }

        struct ast_node* cseq_node = NULL;
		struct location loc_cseq;
		cseq_node = parse_cseq(ps, left->tu, &loc_cseq);
        if (cseq_node && cseq_node->type == ast_type_error) {
            n->type = ast_type_error;
        }

        if (!consume_newline(ps)) {
            n->type = ast_type_error;
        }

		struct token* rp = NULL;
		if (!match(ps, token_right_paren, "expected right parenthesis", &rp)) {
            /* test case: test_parse_call_error_right_paren */
            n->type = ast_type_error;
        }


        if (left) {
            ast_node_add(n, left);
            if (left->type == ast_type_error) {
                n->type = ast_type_error;
            }
        }

        if (cseq_node) {
            ast_node_add(n, cseq_node);
            if (cseq_node->type == ast_type_error) {
                n->type = ast_type_error;
            }
        }

		if (n->type != ast_type_error) {
			struct ast_node* tu = left->tu;
			assert(tu);
			assert(tu->td);
			struct type_def* td = tu->td;
			if (td->type != type_function) {
				set_source_error(ps->el, &left_loc, "not a function type");
				/* test case: test_parse_call_error_not_function */
                n->type = ast_type_error;
			} else {
				struct ast_node* input = NULL;
				struct ast_node* output = NULL;
				get_function_children(tu, &input, &output);

				/* input */
				int tcount = 0;
				if (input) {
					tcount = ast_node_count_children(input);
				}
				int ccount = 0;
				if (cseq_node) {
					ccount = ast_node_count_children(cseq_node);
				}

				if (ccount < tcount) {
					set_source_error(ps->el, &rp->loc, "not enough arguments in function call");
					/* test case: test_parse_call_error_not_enough_arguments */
                    n->type = ast_type_error;
				} else if (ccount > tcount) {
					set_source_error(ps->el, &rp->loc, "too many arguments in function call");
					/* test case: test_parse_call_error_too_many_arguments */
                    n->type = ast_type_error;
				}

				/* output */
				if (output) {
					n->tu = ast_node_copy(ast_node_get(output, 0));
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
struct ast_node* parse_cseq(struct parse_state* ps, struct ast_node* tu, struct location* loc)
{
	struct ast_node* n = NULL;
    ast_node_create(&n);
    n->type = ast_type_cseq;

	if (!get_location(ps, loc)) {
        n->type = ast_type_error;
    }

	if (!tu || !tu->td || tu->td->type != type_function) {
		set_source_error(ps->el, loc, "not a function type");
		/* test case: no test case needed */
        n->type = ast_type_error;
		return n;
	}

	struct ast_node* a = NULL;
	struct location loc_expr;
	a = parse_simple_expr(ps, &loc_expr);
    if (a && a->type == ast_type_error) {
        n->type = ast_type_error;
    }

	if (!a) {
		return n;
	}
	int i = 0;
	if (!check_input_type(ps, tu, i, a, &loc_expr)) {
        n->type = ast_type_error;
    }
	i++;

	ast_node_add(n, a);

	while (true) {
		struct token* t0 = get_lookahead(ps);

		if (!t0 || t0->type != token_comma) {
			break;
		}

		struct token* comma = NULL;
		if (!match(ps, token_comma, "expecting comma", &comma)) {
            /* test case: no test case needed */
            n->type = ast_type_error;
        }

		token_destroy(comma);
		free(comma);

        if (!consume_newline(ps)) {
            n->type = ast_type_error;
        }

		a = parse_simple_expr(ps, &loc_expr);
		if (a && a->type == ast_type_error) {
            n->type = ast_type_error;
        }

		if (!a) {
			set_source_error(ps->el, &loc_expr, "expected expression after comma");
			/* test case: test_parse_call_error_expected_expression */
            n->type = ast_type_error;
		} else {
			/* transfer a -> parent */
			ast_node_add(n, a);

			if (!check_input_type(ps, tu, i, a, &loc_expr)) {
                n->type = ast_type_error;
            }
		}

		i++;
	}

	return n;
}

struct ast_node* parse_dot(struct parse_state* ps, struct location* loc)
{
	struct ast_node* n = NULL;
	struct ast_node* a = NULL;

    struct location a_loc;
	a = parse_factor(ps, &a_loc);
    *loc = a_loc;

	if (!a) {
		return a;
	}

	struct ast_node* left = n = a;
	struct location loc_left = *loc;
	if (ps->qualifier.size > 0) buffer_add_char(&ps->qualifier, '.');
	buffer_copy(&a->value, &ps->qualifier);
	while (true) {
		struct token* t0 = NULL;
		t0 = get_lookahead(ps);
		if (!t0 || t0->type != token_dot) {
			break;
		}

        ast_node_create(&n);
        n->type = ast_type_dot;

        struct token* dot = NULL;
		if (!match(ps, token_dot, "expected a dot", &dot)) {
            /* test case: no test case needed */
            n->type = ast_type_error;
        }

        if (!consume_newline(ps)) {
            n->type = ast_type_error;
        }

		struct ast_node* b = NULL;
        struct location b_loc;
		b = parse_factor(ps, &b_loc);
        if (b && b->type == ast_type_error) {
            n->type = ast_type_error;
        }

		if (!b) {
			set_source_error(ps->el, &b_loc, "expected term after dot");
			/* test case: test_parse_dot_error_expected_term */
            n->type = ast_type_error;
		}

        if (left && b) {
            if (left->type == ast_type_dot) {
                buffer_copy(&left->value, &n->value);
                buffer_add_char(&n->value, '.');
                buffer_copy(&b->value, &n->value);
            } else if (left->type == ast_type_id) {
                buffer_copy(&left->value, &n->value);
                buffer_add_char(&n->value, '.');
                buffer_copy(&b->value, &n->value);
            }
        }

        if (left) {
            ast_node_add(n, left);
            if (left->type == ast_type_error) {
                n->type = ast_type_error;
            }
        }

        if (b) {
            ast_node_add(n, b);
            if (b->type == ast_type_error) {
                n->type = ast_type_error;
            }
        }

		if (n->type != ast_type_error) {
			assert(left);
			assert(b);
			struct ast_node* tu_left = left->tu;
			struct ast_node* tu_b = b->tu;

			if (!tu_left) {
				set_source_error(ps->el, &loc_left, "dot operand has no value");
				/* test case: no test case necessary */
                n->type = ast_type_error;
			} else if (tu_left->td->type != type_module && tu_left->td->type != type_struct) {
				set_source_error(ps->el, &loc_left, "dot operand is not a module or struct");
				/* test case: test_parse_dot_error_left_non_module */
                n->type = ast_type_error;
			}

			if (!tu_b) {
				set_source_error(ps->el, &b_loc, "dot operand has no value");
				/* test case: test_parse_dot_error_non_module */
                n->type = ast_type_error;
			}

			if (left == a) {
				if (left->type != ast_type_id) {
					set_source_error(ps->el, &loc_left, "operand of dot operator not an identifier");
					/* test case: no test case necessary */
                    n->type = ast_type_error;
				}
			}

			if (b->type != ast_type_id) {
				set_source_error(ps->el, &b_loc, "operand of dot operator not an identifier");
				/* test case: test_parse_dot_error_right_not_identifier */
                n->type = ast_type_error;
			}

			if (n->type != ast_type_error) {
				n->tu = ast_node_copy(tu_b);
			}

			left = n;
			#pragma warning(suppress:6001)
			loc_left = dot->loc;
			if (ps->qualifier.size > 0) buffer_add_char(&ps->qualifier, '.');
			buffer_copy(&b->value, &ps->qualifier);
		}

		token_destroy(dot);
		free(dot);
	}

	buffer_clear(&ps->qualifier);

	return n;
}
