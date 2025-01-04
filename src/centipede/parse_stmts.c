#include "ast.h"
#include "parse_data.h"
#include "parse_tools.h"
#include "parse_expr.h"
#include <assert.h>
#include "comp_unit.h"

Cent_ast* Cent_parse_stmt(Cent_parse_data* pd);
Cent_ast* Cent_parse_element_type(Cent_parse_data* pd);
Cent_ast* Cent_parse_element_properties(Cent_parse_data* pd);
Cent_ast* Cent_parse_property_dec(Cent_parse_data* pd);
Cent_ast* Cent_parse_children(Cent_parse_data* pd);
Cent_ast* Cent_parse_enumerate(Cent_parse_data* pd);
Cent_ast* Cent_parse_include(Cent_parse_data* pd);
void Cent_parse_include_stmts(Cent_parse_data* pd, Cent_ast* n, Cent_comp_unit* cu);

/* stmts -> stmt stmts' | e */
/* stmts' -> sep stmt stmts' | e */
Cent_ast* Cent_parse_stmts(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_stmts;

    Cent_environment* env = NULL;
    Cent_environment_create(&env);
    env->prev = pd->top;
    pd->top = env;
    n->env = env;

    while (true) {
        Cent_lookahead(pd);

        if (pd->lookahead->type == Cent_token_eof) {
            if (pd->ld->input != pd->comp_table->primary->input) {
                Cent_comp_unit* cu = pd->comp_table->primary;
                pd->ld->input = cu->input;
                pd->ld->input_vtable = cu->input_vtable;
                pd->ld->errors = cu->errors;
                Cent_token* t = pd->lookahead;
                pd->lookahead = NULL;
                Cent_token_destroy(t);
                free(t);
            }
        }

        Cent_ast* a = Cent_parse_stmt(pd);
        if (a) {
            Cent_ast_add(n, a);

            if (!a->has_error && a->type == Cent_ast_type_include) {
                Cent_ast* b = a->head;
                Cent_comp_unit* cu = Cent_comp_table_get(pd->comp_table, &b->text);
                if (!cu) {
                    error_list_set(pd->errors, &b->loc, "Could not find compile unit: %b", &b->text);
                    continue;
                }

                Cent_lookahead(pd);
                pd->ld->input = cu->input;
                pd->ld->input_vtable = cu->input_vtable;
                pd->ld->errors = cu->errors;
            }
        }

        if (Cent_has_separator(pd, n)) {
            while (Cent_has_separator(pd, n));
        } else {
            break;
        }

    }

    pd->top = pd->top->prev;

    return n;
}

/* stmt -> parse_element | e */
Cent_ast* Cent_parse_stmt(Cent_parse_data* pd)
{
    Cent_lookahead(pd);

    if (pd->lookahead->type == Cent_token_eof) {
        return NULL;
    }

    if (pd->lookahead->type == Cent_token_element) {
        return Cent_parse_element_type(pd);
    }

    if (pd->lookahead->type == Cent_token_enum) {
        return Cent_parse_enumerate(pd);
    }

    if (pd->lookahead->type == Cent_token_include) {
        return Cent_parse_include(pd);
    }

    return Cent_parse_expr(pd);
}

/* parse_element -> element name { element_seq } */
Cent_ast* Cent_parse_element_type(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_element_type;

    Cent_token* e = NULL;
    if (!Cent_match(pd, Cent_token_element, "expected element", &e, n)) {
        assert(false && "not possible");
    }
    Cent_token_destroy(e);
    free(e);

    Cent_token* id = NULL;
    Cent_match(pd, Cent_token_id, "expected id", &id, n);
    /* test case: test_parse_element_error_expected_id */

    if (id) {
        buffer_copy(&id->value, &n->text);
        Cent_token_destroy(id);
        free(id);
    }

    Cent_ignore_newlines(pd, n);

    Cent_lookahead(pd);
    if (pd->lookahead->type == Cent_token_properties) {
        Cent_ast* a = Cent_parse_element_properties(pd);
        Cent_ast_add(n, a);
    }

    Cent_ignore_newlines(pd, n);

    Cent_lookahead(pd);
    if (pd->lookahead->type == Cent_token_children) {
        Cent_ast* b = Cent_parse_children(pd);
        Cent_ast_add(n, b);
    }

    Cent_ignore_newlines(pd, n);

    Cent_token* end = NULL;
    Cent_match(pd, Cent_token_end, "expected end", &end, n);
    Cent_token_destroy(end);
    free(end);
    /* test case: test_parse_element_error_expected_end */

    if (!n->has_error) {
        Cent_symbol* sym = NULL;
        sym = Cent_environment_get(pd->top, &n->text);
        if (sym) {
            buffer_finish(&n->text);
            error_list_set(pd->errors, &n->loc, "name already exists: %s", n->text.buf);
            n->has_error = true;
            return n;
            /* test case: test_parse_element_error_name_already_exits */
        }

        Cent_element_type* element = NULL;
        Cent_element_create(&element);
        buffer_copy(&n->text, &element->name);
        element->loc = n->loc;

        Cent_symbol_create(&sym);
        Cent_symbol_set_type(sym, Cent_symbol_type_element);
        sym->data.element = element;
        Cent_environment_add_symbol(pd->top, &element->name, sym);
    }

    return n;
}

/* element_seq -> id : element element_seq' | e */
/* element_seq' -> sep id : element element_seq | e */
Cent_ast* Cent_parse_element_properties(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_prop;

    Cent_token* prop = NULL;
    if (!Cent_match(pd, Cent_token_properties, "expected properties", &prop, n)) {
        assert(false && "not possible");
    }
    Cent_token_destroy(prop);
    free(prop);

    Cent_ignore_newlines(pd, n);

    while (true) {
        Cent_lookahead(pd);
        if (pd->lookahead->type != Cent_token_id) {
            break;
        }

        Cent_ast* a = Cent_parse_property_dec(pd);
        Cent_ast_add(n, a);

        if (!Cent_has_separator(pd, n)) {
            break;
        }
    }

    Cent_token* end = NULL;
    Cent_match(pd, Cent_token_end, "expected end", &end, n);
    Cent_token_destroy(end);
    free(end);
    /* test case: test_parse_element_error_properties_expected_end */

    return n;
}

Cent_ast* Cent_parse_property_dec(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_prop_dec;

    Cent_token* id = NULL;
    if (!Cent_match(pd, Cent_token_id, "expected id", &id, n)) {
        assert(false && "not possible");
    }

    Cent_ast* a = NULL;
    Cent_ast_create(&a);
    a->type = Cent_ast_type_id;
    buffer_copy(&id->value, &a->text);
    Cent_ast_add(n, a);

    Cent_token_destroy(id);
    free(id);

    Cent_token* colon = NULL;
    Cent_match(pd, Cent_token_colon, "expected colon", &colon, n);
    Cent_token_destroy(colon);
    free(colon);
    /* test case: test_parse_element_error_property_expected_colon */

    Cent_token* type = NULL;
    Cent_match(pd, Cent_token_id, "expected id", &type, n);
    /* test case: test_parse_element_error_property_expected_id */

    Cent_ast* b = NULL;
    Cent_ast_create(&b);
    b->type = Cent_ast_type_id;
    if (type) {
        buffer_copy(&type->value, &b->text);
        Cent_token_destroy(type);
        free(type);
    }
    Cent_ast_add(n, b);

    Cent_lookahead(pd);
    if (pd->lookahead->type == Cent_token_modifier) {
        Cent_token* mod = NULL;
        if (!Cent_match(pd, Cent_token_modifier, "expected modifier", &mod, n)) {
            assert(false && "not possible");
        }
        Cent_ast* c = NULL;
        Cent_ast_create(&c);
        c->type = Cent_ast_type_modifier;
        buffer_copy(&mod->value, &c->text);
        Cent_ast_add(n, c);
        Cent_token_destroy(mod);
        free(mod);
    }

    return n;
}

Cent_ast* Cent_parse_children(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_children;

    Cent_token* ch = NULL;
    if (!Cent_match(pd, Cent_token_children, "expected children", &ch, n)) {
        assert(false && "not possible");
    }
    Cent_token_destroy(ch);
    free(ch);

    Cent_ignore_newlines(pd, n);

    Cent_lookahead(pd);
    while (pd->lookahead->type == Cent_token_id) {
        Cent_ast* a = NULL;
        Cent_ast_create(&a);
        Cent_token* id = NULL;
        if (!Cent_match(pd, Cent_token_id, "expected id", &id, a)) {
            assert(false && "not possible");
        }
        a->type = Cent_ast_type_id;
        buffer_copy(&id->value, &a->text);
        Cent_ast_add(n, a);
        Cent_token_destroy(id);
        free(id);

        if (!Cent_has_separator(pd, n)) {
            break;
        }

        Cent_lookahead(pd);
    }

    Cent_token* end = NULL;
    Cent_match(pd, Cent_token_end, "expected end", &end, n);
    Cent_token_destroy(end);
    free(end);
    /* test case: test_parse_element_error_children_expected_end */

    return n;
}

Cent_ast* Cent_parse_enumerate(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_enum_type;

    Cent_token* en = NULL;
    if (!Cent_match(pd, Cent_token_enum, "expected enum", &en, n)) {
        assert(false && "not possible");
    }
    Cent_token_destroy(en);
    free(en);

    Cent_token* id = NULL;
    Cent_match(pd, Cent_token_id, "expected id", &id, n);
    if (id) {
        buffer_copy(&id->value, &n->text);
        Cent_token_destroy(id);
        free(id);
    }
    /* test case: test_parse_enumerate_error_expected_id */

    Cent_ignore_newlines(pd, n);

    Cent_lookahead(pd);
    while (pd->lookahead->type == Cent_token_id) {
        Cent_ast* a = NULL;
        Cent_ast_create(&a);
        a->type = Cent_ast_type_id;

        id = NULL;
        if (!Cent_match(pd, Cent_token_id, "expected id", &id, a)) {
            assert(false && "not possible");
        }

        buffer_copy(&id->value, &a->text);
        Cent_token_destroy(id);
        free(id);
        Cent_ast_add(n, a);

        if (!Cent_has_separator(pd, n)) {
            break;
        }

        Cent_ignore_newlines(pd, n);
        Cent_lookahead(pd);
    }

    Cent_token* end = NULL;
    Cent_match(pd, Cent_token_end, "expected end", &end, n);
    Cent_token_destroy(end);
    free(end);
    /* test case: test_parse_enumerate_error_expected_end */

    if (!n->has_error) {
        Cent_enum_type* enumerate = NULL;
        Cent_enumerate_create(&enumerate);
        buffer_copy(&n->text, &enumerate->name);


        Cent_symbol* sym = NULL;
        Cent_symbol_create(&sym);
        Cent_symbol_set_type(sym, Cent_symbol_type_enumerate);
        sym->data.enumerate = enumerate;
        Cent_environment_add_symbol(pd->top, &enumerate->name, sym);
    }

    return n;
}

Cent_ast* Cent_parse_include(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_include;

    Cent_token* inc = NULL;
    if (!Cent_match(pd, Cent_token_include, "expected include", &inc, n)) {
        assert(false && "not possible");
    }
    Cent_token_destroy(inc);
    free(inc);

    Cent_ast* a = NULL;
    Cent_ast_create(&a);
    a->type = Cent_ast_type_expr_string;

    Cent_token* path;
    if (!Cent_match(pd, Cent_token_string, "expected string", &path, a)) {
        Cent_ast_add(n, a);
        return n;
    }

    buffer_copy(&path->value, &a->text);
    Cent_ast_add(n, a);
    Cent_token_destroy(path);
    free(path);

    return n;
}