#include "parse_data.h"
#include "ast.h"
#include <assert.h>
#include "parse_tools.h"

bool Worm_parse_sep(Worm_parse_data* pd, Worm_ast* n);
Worm_ast* Worm_parse_stmt(Worm_parse_data* pd);

Worm_ast* Worm_parse_element(Worm_parse_data* pd);
Worm_ast* Worm_parse_properties(Worm_parse_data* pd);
Worm_ast* Worm_parse_property_type(Worm_parse_data* pd);
Worm_ast* Worm_parse_children(Worm_parse_data* pd);

Worm_ast* Worm_parse_node(Worm_parse_data* pd);
Worm_ast* Worm_parse_object(Worm_parse_data* pd);
Worm_ast* Worm_parse_property(Worm_parse_data* pd);
Worm_ast* Worm_parse_expr(Worm_parse_data* pd);
Worm_ast* Worm_parse_integer(Worm_parse_data* pd);
Worm_ast* Worm_parse_natural(Worm_parse_data* pd);
Worm_ast* Worm_parse_real(Worm_parse_data* pd);
Worm_ast* Worm_parse_boolean(Worm_parse_data* pd);
Worm_ast* Worm_parse_string(Worm_parse_data* pd);

// stmts -> stmt? stmts'
// stmts' -> sep stmt? stmts' | e
// stmt? -> stmt | e
Worm_ast* Worm_parse_stmts(Worm_parse_data* pd)
{
    Worm_ast* n = NULL;
    Worm_ast_create(&n);
    n->type = Worm_ast_type_stmts;

    Worm_ast* a = Worm_parse_stmt(pd);
    if (a) {
        Worm_ast_add(n, a);
    }

    while (Worm_parse_sep(pd, n)) {
        Worm_ast* b = Worm_parse_stmt(pd);
        if (b) {
            Worm_ast_add(n, b);
        }
    }

    return n;
}

// sep -> '\n' | ';'
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

// stmt -> element | node | e
Worm_ast* Worm_parse_stmt(Worm_parse_data* pd)
{
    Worm_lookahead(pd);

    if (pd->lookahead->type == Worm_token_type_element) {
        return Worm_parse_element(pd);
    }

    if (pd->lookahead->type == Worm_token_type_dot || pd->lookahead->type == Worm_token_type_id) {
        return Worm_parse_node(pd);
    }
    
    return NULL;
}

// element -> 'element' id { element_stmt? element_stmts' }
// element_stmt? -> properties | children | e
// element_stmts' -> sep element_stmt? element_stmts' | e
// note: limited to one properties and one children
Worm_ast* Worm_parse_element(Worm_parse_data* pd)
{
    Worm_ast* n = NULL;
    Worm_ast_create(&n);
    n->type = Worm_ast_type_element;

    Worm_token* el = NULL;
    if (!Worm_match(pd, Worm_token_type_element, "expected element", &el, n)) {
        assert(false && "not possible");
    }
    Worm_token_destroy(el);
    free(el);

    Worm_token* id = NULL;
    Worm_match(pd, Worm_token_type_id, "expected id", &id, n);
    if (id) {
        Worm_ast* a = NULL;
        Worm_ast_create(&a);
        a->type = Worm_ast_type_id;
        Zinc_string_add_string(&a->value, &id->value);
        Worm_ast_add(n, a);
        Worm_token_destroy(id);
        free(id);
    }

    Worm_token* lcb = NULL;
    Worm_match(pd, Worm_token_type_left_curly_brace, "expected left-curly-brace", &lcb, n);
    Worm_token_destroy(lcb);
    free(lcb);

    if (Worm_parse_sep(pd, n)) {
        while (Worm_parse_sep(pd, n));
    }

    int properties_count = 0;
    int children_count = 0;

    Worm_lookahead(pd);
    if (pd->lookahead->type == Worm_token_type_properties) {
        Worm_ast* a = Worm_parse_properties(pd);
        Worm_ast_add(n, a);
        properties_count++;
    } else if (pd->lookahead->type == Worm_token_type_children) {
        Worm_ast* b = Worm_parse_children(pd);
        Worm_ast_add(n, b);
        children_count++;
    }

    while (Worm_parse_sep(pd, n)) {
        Worm_lookahead(pd);
        if (pd->lookahead->type == Worm_token_type_properties) {
            Worm_ast* a = Worm_parse_properties(pd);
            Worm_ast_add(n, a);
            properties_count++;
            if (properties_count == 2) {
                Zinc_error_list_set(pd->errors, &a->loc, "duplicate properties declaration");
            }
        } else if (pd->lookahead->type == Worm_token_type_children) {
            Worm_ast* b = Worm_parse_children(pd);
            Worm_ast_add(n, b);
            children_count++;
            if (children_count == 2) {
                Zinc_error_list_set(pd->errors, &b->loc, "duplicate children declaration");
            }
        }
    }


    Worm_token* rcb = NULL;
    Worm_match(pd, Worm_token_type_right_curly_brace, "expected right-curly-brace", &rcb, n);
    Worm_token_destroy(rcb);
    free(rcb);

    return n;
}

Worm_ast* Worm_parse_properties(Worm_parse_data* pd)
{
    Worm_ast* n = NULL;
    Worm_ast_create(&n);
    n->type = Worm_ast_type_properties;

    Worm_token* pr = NULL;
    if (!Worm_match(pd, Worm_token_type_properties, "expected properties", &pr, n)) {
        assert(false && "not possible");
    }
    Worm_token_destroy(pr);
    free(pr);

    Worm_token* lcb = NULL;
    Worm_match(pd, Worm_token_type_left_curly_brace, "expected left-curly-brace", &lcb, n);
    Worm_token_destroy(lcb);
    free(lcb);

    Worm_lookahead(pd);
    if (pd->lookahead->type == Worm_token_type_id) {
        Worm_ast* a = Worm_parse_property_type(pd);

        while (Worm_parse_sep(pd, n)) {
            Worm_lookahead(pd);
            if (pd->lookahead->type == Worm_token_type_id) {
                Worm_ast* b = Worm_parse_property_type(pd);
            }
        }
    }

    Worm_token* rcb = NULL;
    Worm_match(pd, Worm_token_type_right_curly_brace, "expected right-curly-brace", &rcb, n);
    Worm_token_destroy(rcb);
    free(rcb);


    return n;
}

Worm_ast* Worm_parse_property_type(Worm_parse_data* pd)
{
    Worm_ast* n = NULL;
    Worm_ast_create(&n);
    n->type = Worm_ast_type_property_type;

    Worm_token* id = NULL;
    if (!Worm_match(pd, Worm_token_type_id, "expected id", &id, n)) {
        assert(false && "not possible");
    }
    Worm_ast* a = NULL;
    Worm_ast_create(&a);
    a->type = Worm_ast_type_id;
    if (id) {
        Zinc_string_add_string(&a->value, &id->value);
        Worm_token_destroy(id);
        free(id);
    }
    Worm_ast_add(n, a);

    Worm_token* colon = NULL;
    Worm_match(pd, Worm_token_type_colon, "expected colon", &colon, n);
    Worm_token_destroy(colon);
    free(colon);

    Worm_token* id2 = NULL;
    Worm_match(pd, Worm_token_type_id, "expected id", &id2, n);
    Worm_ast* b = NULL;
    Worm_ast_create(&b);
    a->type = Worm_ast_type_id;
    if (id2) {
        Zinc_string_add_string(&b->value, &id2->value);
        Worm_token_destroy(id2);
        free(id2);
    }
    Worm_ast_add(n, b);
}

// Children -> children { Types }
// Types -> Id? Types' | e
// Types' -> sep Id? Types' | e
// Id? -> id | e
Worm_ast* Worm_parse_children(Worm_parse_data* pd)
{
    Worm_ast* n = NULL;
    Worm_ast_create(&n);
    n->type = Worm_ast_type_children;

    Worm_token* ch = NULL;
    if (!Worm_match(pd, Worm_token_type_children, "expected children", &ch, n)) {
        assert(false && "not possible");
    }
    Worm_token_destroy(ch);
    free(ch);

    Worm_token* lcb = NULL;
    Worm_match(pd, Worm_token_type_left_curly_brace, "expected left-curly-brace", &lcb, n);
    Worm_token_destroy(lcb);
    free(lcb);

    Worm_lookahead(pd);
    if (pd->lookahead->type == Worm_token_type_id) {
        Worm_token* id = NULL;
        if (!Worm_match(pd, Worm_token_type_id, "expected id", &id, n)) {
            assert(false && "not possible");
        }
        Worm_ast* a = NULL;
        Worm_ast_create(&a);
        a->type = Worm_ast_type_id;
        Zinc_string_add_string(&a->value, &id->value);
        Worm_ast_add(n, a);
    }

    Worm_lookahead(pd);
    while (Worm_parse_sep(pd, n)) {
        Worm_lookahead(pd);
        if (pd->lookahead->type == Worm_token_type_id) {
            Worm_token* id = NULL;
            if (!Worm_match(pd, Worm_token_type_id, "expected id", &id, n)) {
                assert(false && "not possible");
            }
            Worm_ast* b = NULL;
            Worm_ast_create(&b);
            b->type = Worm_ast_type_id;
            Zinc_string_add_string(&b->value, &id->value);
            Worm_ast_add(n, b);
        }
    }

    Worm_token* rcb = NULL;
    Worm_match(pd, Worm_token_type_right_curly_brace, "expected right-curly-brace", &rcb, n);
    Worm_token_destroy(rcb);
    free(rcb);

    return n;
}

// node -> dots object
// dots -> '.' dots | e
Worm_ast* Worm_parse_node(Worm_parse_data* pd)
{
    Worm_ast* n = NULL;
    Worm_ast_create(&n);
    n->type = Worm_ast_type_node;

    size_t dot_count = 0;
    Worm_token* dot = NULL;
    while (pd->lookahead->type == Worm_token_type_dot) {
        if (!Worm_match(pd, Worm_token_type_dot, "expected dot", &dot, n)) {
            assert(false && "not possible");
        }
        dot_count++;
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

// object -> id { properties } | id {}
// properties -> properties ; property | property
//
// properties -> property properties'
// properties' -> ; property | e
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
        Worm_lookahead(pd);
        if (pd->lookahead->type == Worm_token_type_semicolon) {
            Worm_token* sc = NULL;
            if (!Worm_match(pd, Worm_token_type_semicolon, "expected semicolon", &sc, n)) {
                assert(false && "not possible");
            }
            Worm_token_destroy(sc);
            free(sc);
        } else {
            break;
        }
        Worm_lookahead(pd);
    }

    Worm_token* rcb = NULL;
    Worm_match(pd, Worm_token_type_right_curly_brace, "expected right-curly-brace", &rcb, n);
    Worm_token_destroy(rcb);
    free(rcb);

    return n;
}

Worm_ast* Worm_parse_property(Worm_parse_data* pd)
{
    Worm_ast* n = NULL;
    Worm_ast_create(&n);
    n->type = Worm_ast_type_property;

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
