#include "parse_data.h"
#include "ast.h"
#include <assert.h>
#include "parse_tools.h"

bool Worm_parse_sep(Worm_parse_data* pd, Worm_ast* n);
Worm_ast* Worm_parse_stmt(Worm_parse_data* pd);
Worm_ast* Worm_parse_level(Worm_parse_data* pd);
Worm_ast* Worm_parse_object(Worm_parse_data* pd);
Worm_ast* Worm_parse_property(Worm_parse_data* pd);
Worm_ast* Worm_parse_expr(Worm_parse_data* pd);
Worm_ast* Worm_parse_integer(Worm_parse_data* pd);
Worm_ast* Worm_parse_natural(Worm_parse_data* pd);
Worm_ast* Worm_parse_real(Worm_parse_data* pd);
Worm_ast* Worm_parse_boolean(Worm_parse_data* pd);
Worm_ast* Worm_parse_string(Worm_parse_data* pd);

/* stmts => opt-seps stmts' */
/* stmts' => stmt seps stmts' | stmt | e */
Worm_ast* Worm_parse_stmts(Worm_parse_data* pd)
{
    Worm_ast* n = NULL;
    Worm_ast_create(&n);
    n->type = Worm_ast_type_stmts;

    if (Worm_parse_sep(pd, n)) {
        while (Worm_parse_sep(pd, n));
    }


    while (true) {
        Worm_ast* a = Worm_parse_stmt(pd);
        if (a) {
            Worm_ast_add(n, a);
        }

        if (Worm_parse_sep(pd, n))  {
            while (Worm_parse_sep(pd, n));
        } else {
            break;
        }
    }

    return n;
}

bool Worm_parse_sep(Worm_parse_data* pd, Worm_ast* n)
{
    Worm_lookahead(pd);
    if (pd->lookahead->type == Worm_token_type_newline || pd->lookahead->type == Worm_token_type_semicolon) {
        Worm_token_type type = pd->lookahead->type;
        Worm_token* t = NULL;
        if (!Worm_match(pd, type, "expected separator", &t, n)) {
            assert(false && "not possible");
        }
        Worm_token_destroy(t);
        free(t);
        return true;
    }

    return false;
}

Worm_ast* Worm_parse_stmt(Worm_parse_data* pd)
{
    Worm_lookahead(pd);

    if (pd->lookahead->type == Worm_token_type_dot || pd->lookahead->type == Worm_token_type_id) {
        return Worm_parse_level(pd);
    }
    
    return NULL;
}

Worm_ast* Worm_parse_level(Worm_parse_data* pd)
{
    Worm_ast* n = NULL;
    Worm_ast_create(&n);
    n->type = Worm_ast_type_level;

    size_t dot_count = 0;
    Worm_token* dot = NULL;
    while (pd->lookahead->type == Worm_token_type_dot) {
        if (!Worm_match(pd, Worm_token_type_dot, "expected dot", &dot, n)) {
            assert(false && "not possible");
        }
        Worm_token_destroy(dot);
        free(dot);
        Worm_lookahead(pd);
    }

    Worm_ast* a = NULL;
    Worm_ast_create(&a);
    a->type = Worm_ast_type_natural;
    a->number.natural = dot_count;
    Worm_ast_add(n, a);

    Worm_ast* b = Worm_parse_object(pd);
    Worm_ast_add(n, b);

    return n;
}

Worm_ast* Worm_parse_object(Worm_parse_data* pd)
{
    Worm_ast* n = NULL;
    Worm_ast_create(&n);
    n->type = Worm_ast_type_object;

    Worm_token* id = NULL;
    Worm_match(pd, Worm_token_type_id, "expected id", &id, n);
    if (id) {
        Zinc_string_add_string(&n->value, &id->value);
        Worm_token_destroy(id);
        free(id);
    }

    Worm_token* lcb = NULL;
    Worm_match(pd, Worm_token_type_left_curly_brace, "expected left-curly-brace", &lcb, n);
    Worm_token_destroy(lcb);
    free(lcb);

    Worm_lookahead(pd);
    while (pd->lookahead->type == Worm_token_type_dot) {
        Worm_ast* a = Worm_parse_property(pd);
        Worm_ast_add(n, a);
    }

    Worm_token* rcb = NULL;
    Worm_match(pd, Worm_token_type_right_curly_brace, "expected right-curly-brace", &rcb, n);
    Worm_token_destroy(rcb);
    free(rcb);
}

Worm_ast* Worm_parse_property(Worm_parse_data* pd)
{
    Worm_ast* n = NULL;
    Worm_ast_create(&n);
    n->type = Worm_token_type_property;

    Worm_token* dot = NULL;
    Worm_match(pd, Worm_token_type_dot, "expected dot", &dot, n);
    Worm_token_destroy(dot);
    free(dot);

    Worm_token* id = NULL;
    Worm_match(pd, Worm_token_type_id, "expected id", &id, n);
    Worm_ast* a = NULL;
    Worm_ast_create(&a);
    a->type = Worm_ast_type_id;
    if (id) {
        Zinc_string_add_string(&a->value, &id->value);
    }
    Worm_token_destroy(id);
    free(id);
    Worm_ast_add(n, a);

    Worm_token* eq = NULL;
    Worm_match(pd, Worm_token_type_equal, "expected equal", &eq, n);
    Worm_token_destroy(eq);
    free(eq);

    Worm_ast* c = Worm_parse_expr(pd);
    Worm_ast_add(n, c);
}

Worm_ast* Worm_parse_expr(Worm_parse_data* pd)
{
    Worm_lookahead(pd);
    if (pd->lookahead->type == Worm_token_type_integer) {
        return Worm_parse_integer(pd);
    } else if (pd->lookahead->type == Worm_token_type_natural) {
        return Worm_parse_natural(pd);
    } else if (pd->lookahead->type == Worm_token_type_real) {
        return Worm_parse_real(pd);
    } else if (pd->lookahead->type == Worm_token_type_true || pd->lookahead->type == Worm_token_type_false) {
        return Worm_parse_boolean(pd);
    } else if (pd->lookahead->type == Worm_token_type_string) {
        return Worm_parse_string(pd);
    } else {
        Zinc_error_list_set(pd->errors, &pd->lookahead->loc, "expected a number, boolean, or string expression");
        Worm_ast* n = NULL;
        Worm_ast_create(&n);
        n->has_error = true;
        return n;
    }

}

Worm_ast* Worm_parse_integer(Worm_parse_data* pd)
{
    Worm_ast* n = NULL;
    Worm_ast_create(&n);
    n->type = Worm_ast_type_integer;

    Worm_token* int_token = NULL;
    if (!Worm_match(pd, Worm_token_type_integer, "expected integer", &int_token, n)) {
        assert(false && "not possible");
    }
    if (int_token) {
        n->number.integer = int_token->number.integer;
        Worm_token_destroy(int_token);
        free(int_token);
    }

    return n;
}


Worm_ast* Worm_parse_natural(Worm_parse_data* pd)
{
    Worm_ast* n = NULL;
    Worm_ast_create(&n);
    n->type = Worm_ast_type_natural;

    Worm_token* nat_token = NULL;
    if (!Worm_match(pd, Worm_token_type_natural, "expected natural", &nat_token, n)) {
        assert(false && "not possible");
    }
    if (nat_token) {
        n->number.natural = nat_token->number.natural;
        Worm_token_destroy(nat_token);
        free(nat_token);
    }

    return n;
}

Worm_ast* Worm_parse_real(Worm_parse_data* pd)
{
    Worm_ast* n = NULL;
    Worm_ast_create(&n);
    n->type = Worm_ast_type_real;

    Worm_token* real_token = NULL;
    if (!Worm_match(pd, Worm_token_type_real, "expected real", &real_token, n)) {
        assert(false && "not possible");
    }
    if (real_token) {
        n->number.real = real_token->number.real;
        Worm_token_destroy(real_token);
        free(real_token);
    }

    return n;
}

Worm_ast* Worm_parse_boolean(Worm_parse_data* pd)
{
    Worm_ast* n = NULL;
    Worm_ast_create(&n);
    n->type = Worm_ast_type_boolean;

    Worm_token* bool_token = NULL;
    Worm_token_type type = pd->lookahead->type;
    if (!Worm_match(pd, type, "expected true or false", &bool_token, n)) {
        assert(false && "not possible");
    }
    if (bool_token) {
        if (type == Worm_token_type_true) {
            n->number.boolean = true;
        } else if (type == Worm_token_type_false) {
            n->number.boolean = false;
        } else {
            assert(false && "not possible");
        }
        Worm_token_destroy(bool_token);
        free(bool_token);
    }

    return n;
}

Worm_ast* Worm_parse_string(Worm_parse_data* pd)
{
    Worm_ast* n = NULL;
    Worm_ast_create(&n);
    n->type = Worm_ast_type_string;

    Worm_token* str = NULL;
    if (!Worm_match(pd, Worm_token_type_string, "expected string", &str, n)) {
        assert(false && "not possible");
    }
    if (str) {
        Zinc_string_add_string(&n->value, &str->value);
        Worm_token_destroy(str);
        free(str);
    }

    return n;
}
