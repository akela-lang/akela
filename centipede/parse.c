#include "parse_data.h"
#include "parse_tools.h"
#include <assert.h>
#include <akela/parse_tools.h>
#include "base.h"
#include "token.h"

Cent_ast* Cent_parse_stmts(Cent_parse_data* pd);
bool Cent_has_separator(Cent_parse_data* pd, Cent_ast* n);
Cent_ast* Cent_parse_stmt(Cent_parse_data* pd);
Cent_ast* Cent_parse_element(Cent_parse_data* pd);
void Cent_ignore_newlines(Cent_parse_data* pd, Cent_ast* n);
Cent_ast* Cent_parse_element_properties(Cent_parse_data* pd);
Cent_ast* Cent_parse_property_dec(Cent_parse_data* pd);
Cent_ast* Cent_parse_children(Cent_parse_data* pd);
Cent_ast* Cent_parse_enumerate(Cent_parse_data* pd);
Cent_ast* Cent_parse_value(Cent_parse_data* pd);
void Cent_parse_number(Cent_parse_data* pd, Cent_ast* n);
void Cent_parse_string(Cent_parse_data* pd, Cent_ast* n);
void Cent_parse_boolean(Cent_parse_data* pd, Cent_ast* n);
void Cent_parse_assign(Cent_parse_data* pd, Cent_ast* n);
void Cent_parse_object(Cent_parse_data* pd, Cent_ast* n);
Cent_ast* Cent_parse_object_stmts(Cent_parse_data* pd);
Cent_ast* Cent_parse_object_stmt(Cent_parse_data* pd);
Cent_ast* Cent_parse_property_set(Cent_parse_data* pd);

Cent_parse_result Cent_parse(Cent_parse_data* pd)
{
    pd->top = Cent_base_create();
    Cent_ast* root = Cent_parse_stmts(pd);
    if (pd->lookahead->type != Cent_token_eof) {
        error_list_set(
            pd->errors,
            &pd->lookahead->loc,
            "Unhandled token: %s",
            Cent_token_name(pd->lookahead->type));
    }

    Cent_parse_result result;
    Cent_parse_result_init(&result);
    result.errors = pd->errors;
    result.base = pd->top;
    result.root = root;

    return result;
}

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

    Cent_ignore_newlines(pd, n);

    Cent_ast* a = Cent_parse_stmt(pd);
    if (a) {
        Cent_ast_add(n, a);
    }

    while (true) {
        if (!Cent_has_separator(pd, n)) {
            break;
        }

        a = Cent_parse_stmt(pd);
        if (a) {
            Cent_ast_add(n, a);
        }
    }

    pd->top = pd->top->prev;

    return n;
}

/* sep -> \n | ; | e */
bool Cent_has_separator(Cent_parse_data* pd, Cent_ast* n)
{
    Cent_token_type type;

    Cent_lookahead(pd);
    if (pd->lookahead->type == Cent_token_newline || pd->lookahead->type == Cent_token_semicolon) {
        type = pd->lookahead->type;
    } else {
        return false;
    }

    Cent_token* sep = NULL;
    if (!Cent_match(pd, type, "expected separator", &sep, n)) {
        assert(false && "not possible");
    }

    return true;
}

/* stmt -> parse_element | e */
Cent_ast* Cent_parse_stmt(Cent_parse_data* pd)
{
    Cent_lookahead(pd);

    if (pd->lookahead->type == Cent_token_eof) {
        return NULL;
    }

    if (pd->lookahead->type == Cent_token_element) {
        return Cent_parse_element(pd);
    }

    if (pd->lookahead->type == Cent_token_enum) {
        return Cent_parse_enumerate(pd);
    }

    return Cent_parse_value(pd);
}

/* parse_element -> element name { element_seq } */
Cent_ast* Cent_parse_element(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_element;

    Cent_token* e = NULL;
    if (!Cent_match(pd, Cent_token_element, "expected element", &e, n)) {
        assert(false && "not possible");
    }

    Cent_token* id = NULL;
    Cent_match(pd, Cent_token_id, "expected id", &id, n);

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

    if (!n->has_error) {
        Cent_element* element = NULL;
        Cent_element_create(&element);
        buffer_copy(&n->text, &element->name);

        Cent_symbol* sym = NULL;
        Cent_symbol_create(&sym);
        sym->type = Cent_symbol_type_element;
        sym->data.element = element;
        sym->n = n;
        Cent_environment_add_symbol(pd->top, &element->name, sym);
    }

    return n;
}

void Cent_ignore_newlines(Cent_parse_data* pd, Cent_ast* n)
{
    Cent_lookahead(pd);
    while (pd->lookahead->type == Cent_token_newline) {
        Cent_token* nl = NULL;
        if (!Cent_match(pd, Cent_token_newline, "expected newline", &nl, n)) {
            assert(false && "not possible");
        }
        Cent_token_destroy(nl);
        free(nl);
        Cent_lookahead(pd);
    }
}

/* element_seq -> id : element element_seq' | e */
/* element_seq' -> sep id : element element_seq | e */
Cent_ast* Cent_parse_element_properties(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_prop;

    Cent_token* prop = NULL;
    Cent_match(pd, Cent_token_properties, "expected properties", &prop, n);
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

    return n;
}

Cent_ast* Cent_parse_property_dec(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_prop_dec;

    Cent_token* id = NULL;
    Cent_match(pd, Cent_token_id, "expected id", &id, n);

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

    Cent_match(pd, Cent_token_id, "expected id", &id, n);

    Cent_ast* b = NULL;
    Cent_ast_create(&b);
    b->type = Cent_ast_type_id;
    buffer_copy(&id->value, &b->text);
    Cent_ast_add(n, b);

    Cent_token_destroy(id);
    free(id);

    Cent_lookahead(pd);
    if (pd->lookahead->type == Cent_token_modifier) {
        Cent_token* mod = NULL;
        Cent_match(pd, Cent_token_modifier, "expected modifier", &mod, n);
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
    Cent_match(pd, Cent_token_children, "expected children", &ch, n);

    Cent_ignore_newlines(pd, n);

    Cent_lookahead(pd);
    while (pd->lookahead->type == Cent_token_id) {
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

        if (!Cent_has_separator(pd, n)) {
            break;
        }

        Cent_lookahead(pd);
    }

    Cent_token* end = NULL;
    Cent_match(pd, Cent_token_end, "expected end", &end, n);
    Cent_token_destroy(end);
    free(end);

    return n;
}

Cent_ast* Cent_parse_enumerate(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_enumerate;

    Cent_token* en = NULL;
    Cent_match(pd, Cent_token_enum, "expected enum", &en, n);
    Cent_token_destroy(en);
    free(en);

    Cent_token* id = NULL;
    Cent_match(pd, Cent_token_id, "expected id", &id, n);
    if (id) {
        buffer_copy(&id->value, &n->text);
        Cent_token_destroy(id);
        free(id);
    }

    Cent_ignore_newlines(pd, n);

    Cent_lookahead(pd);
    while (pd->lookahead->type == Cent_token_id) {
        id = NULL;
        Cent_match(pd, Cent_token_id, "expected id", &id, n);

        if (id) {
            Cent_ast* a = NULL;
            Cent_ast_create(&a);
            a->type = Cent_ast_type_id;
            buffer_copy(&id->value, &a->text);
            Cent_token_destroy(id);
            free(id);
            Cent_ast_add(n, a);
        }

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

    if (!n->has_error) {
        Cent_enumerate* enumerate = NULL;
        Cent_enumerate_create(&enumerate);
        buffer_copy(&n->text, &enumerate->name);


        Cent_symbol* sym = NULL;
        Cent_symbol_create(&sym);
        sym->type = Cent_symbol_type_enumerate;
        sym->data.enumerate = enumerate;
        sym->n = n;
        Cent_environment_add_symbol(pd->top, &enumerate->name, sym);
    }

    return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_ast* Cent_parse_value(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_value;

    Cent_lookahead(pd);

    if (pd->lookahead->type == Cent_token_number) {
        Cent_parse_number(pd, n);
        return n;
    }

    if (pd->lookahead->type == Cent_token_string) {
        Cent_parse_string(pd, n);
        return n;
    }

    if (pd->lookahead->type == Cent_token_true || pd->lookahead->type == Cent_token_false) {
        Cent_parse_boolean(pd, n);
        return n;
    }

    Cent_token* id = NULL;
    if (!Cent_match(pd, Cent_token_id, "expected id", &id, n)) {
        Cent_token_destroy(id);
        free(id);
        return n;
    }

    Cent_lookahead(pd);
    if (pd->lookahead->type == Cent_token_double_colon) {
        Cent_value* v = NULL;
        Cent_value_create(&v);
        v->type = Cent_value_type_enum;
        if (id) {
            buffer_copy(&id->value, &v->display);
            Cent_token_destroy(id);
            free(id);
        } else {
            v->has_error = true;
        }

        Cent_token* dc = NULL;
        Cent_match(pd, Cent_token_double_colon, "expected double-colon", &dc, n);
        if (dc) {
            buffer_copy_str(&v->display, "::");
            Cent_token_destroy(dc);
            free(dc);
        } else {
            v->has_error = true;
        }

        Cent_token* id2 = NULL;
        Cent_match(pd, Cent_token_id, "expected id", &id2, n);

        if (id2) {
            buffer_copy(&id2->value, &v->display);
            Cent_token_destroy(id2);
            free(id2);
        } else {
            v->has_error = true;
        }

        n->value = v;
        return n;
    }

    if (pd->lookahead->type == Cent_token_equal) {
        n->type = Cent_ast_type_assign;

        Cent_ast* a = NULL;
        Cent_ast_create(&a);
        a->type = Cent_ast_type_id;
        buffer_copy(&id->value, &a->text);
        Cent_ast_add(n, a);

        Cent_token* eq = NULL;
        Cent_match(pd, Cent_token_equal, "expected equal", &eq, n);
        Cent_token_destroy(eq);
        free(eq);

        Cent_ast* b = Cent_parse_value(pd);
        if (b->type == Cent_ast_type_assign) {
            error_list_set(pd->errors, &b->loc, "nested assignments are not allowed");
            b->has_error = true;
        }
        Cent_ast_add(n, b);

        Cent_token_destroy(id);
        free(id);
        return n;
    }

    if (pd->lookahead->type == Cent_token_left_curly_brace) {
        buffer_copy(&id->value, &n->text);
        Cent_parse_object(pd, n);

        Cent_token_destroy(id);
        free(id);
        return n;
    }

    error_list_set(pd->errors, &pd->lookahead->loc, "expected assignment or object");
    Cent_token_destroy(id);
    free(id);

    return n;
}

void Cent_parse_number(Cent_parse_data* pd, Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    value->type = Cent_value_type_number;

    Cent_token* num = NULL;
    Cent_match(pd, Cent_token_number, "expected number", &num, n);
    if (num) {
        if (num->number_type == Cent_number_type_integer) {
            value->number_type = Cent_number_type_integer;
            value->data.integer = num->number_value.integer;
        } else if (num->number_type == Cent_number_type_fp) {
            value->number_type = Cent_number_type_fp;
            value->data.fp = num->number_value.fp;
        } else {
            assert(false && "not possible");
        }
    }

    n->value = value;
}

void Cent_parse_string(Cent_parse_data* pd, Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);

    Cent_token* str = NULL;
    Cent_match(pd, Cent_token_string, "expected string", &str, n);

    value->type = Cent_value_type_string;
    buffer_copy(&str->value, &value->data.string);

    n->value = value;
}

void Cent_parse_boolean(Cent_parse_data* pd, Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    value->type = Cent_value_type_boolean;
    Cent_token* bln = NULL;
    Cent_match(pd, pd->lookahead->type, "expected boolean", &bln, n);

    value->type = Cent_value_type_boolean;
    if (bln->type == Cent_token_true) {
        value->data.boolean = true;
    } else if (bln->type == Cent_token_false) {
        value->data.boolean = false;
    } else {
        assert(false && "not possible");
    }

    n->value = value;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_parse_object(Cent_parse_data* pd, Cent_ast* n)
{
    Cent_token* lcb = NULL;
    Cent_match(pd, Cent_token_left_curly_brace, "expected left curly brace", &lcb, n);

    Cent_ast* a = Cent_parse_object_stmts(pd);
    Cent_ast_add(n, a);

    Cent_token* rcb = NULL;
    Cent_match(pd, Cent_token_right_curly_brace, "expected right curly brace", &rcb, n);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_ast* Cent_parse_object_stmts(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_object_stmts;

    Cent_ignore_newlines(pd, n);
    while (true) {
        Cent_lookahead(pd);
        if (pd->lookahead->type == Cent_token_right_curly_brace) {
            break;
        }

        Cent_ast* a = Cent_parse_object_stmt(pd);
        if (a) {
            Cent_ast_add(n, a);
        }
        if (!Cent_has_separator(pd, n)) {
            break;
        }
    }

    return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_ast* Cent_parse_object_stmt(Cent_parse_data* pd)
{
    Cent_lookahead(pd);

    if (pd->lookahead->type == Cent_token_dot) {
        return Cent_parse_property_set(pd);
    }

    return Cent_parse_value(pd);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_ast* Cent_parse_property_set(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_prop_set;

    Cent_token* dot = NULL;
    Cent_match(pd, Cent_token_dot, "expected dot", &dot, n);
    Cent_token_destroy(dot);
    free(dot);

    Cent_token* id = NULL;
    Cent_match(pd, Cent_token_id, "expected id", &id, n);

    Cent_ast* a = NULL;
    Cent_ast_create(&a);
    a->type = Cent_ast_type_id;
    if (id) {
        buffer_copy(&id->value, &a->text);
        Cent_token_destroy(id);
        free(id);
        Cent_ast_add(n, a);
    }

    Cent_token* eq = NULL;
    Cent_match(pd, Cent_token_equal, "expected equal", &eq, n);
    Cent_token_destroy(eq);
    free(eq);

    Cent_ast* b = Cent_parse_value(pd);
    if (!b) {
        error_list_set(pd->errors, &pd->lookahead->loc, "expected a value");
        n->has_error = true;
    } else {
        Cent_ast_add(n, b);
    }

    return n;
}
