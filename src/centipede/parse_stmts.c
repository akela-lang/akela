#include "ast.h"
#include "parse_data.h"
#include "parse_tools.h"
#include "parse_expr.h"
#include <assert.h>
#include "comp_unit.h"
#include "parse_const.h"
#include "zinc/list.h"
#include "parse_object.h"
#include "parse_transform.h"

Cent_ast* Cent_parse_stmt(Cent_parse_data* pd);
Cent_ast* Cent_parse_element_type(Cent_parse_data* pd);
Cent_ast* Cent_parse_variant_type(Cent_parse_data* pd);
Cent_ast* Cent_parse_element_properties(Cent_parse_data* pd);
Cent_ast* Cent_parse_property_dec(Cent_parse_data* pd);
Cent_ast* Cent_parse_children(Cent_parse_data* pd);
Cent_ast* Cent_parse_enumerate(Cent_parse_data* pd);
Cent_ast* Cent_parse_use(Cent_parse_data* pd);
void Cent_parse_include_stmts(Cent_parse_data* pd, Cent_ast* n, Cent_comp_unit* cu);
void Cent_parse_module_seq(Cent_parse_data* pd, Cent_ast* n);
void Cent_parse_import_module(Cent_parse_data* pd, Cent_ast* n);
Cent_ast* Cent_parse_struct(Cent_parse_data* pd);
Cent_ast* Cent_parse_param(Cent_parse_data* pd);

/* Stmts -> Stmt? Stmts' */
/* Stmts' -> Sep Stmt? Stmts' | e */
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

    Cent_ast* a = Cent_parse_stmt(pd);
    if (a) {
        Cent_ast_add(n, a);
    }

    while (Cent_parse_sep(pd, n)) {
        Cent_ast* b = Cent_parse_stmt(pd);
        if (b) {
            Cent_ast_add(n, b);
        }
    }

    pd->top = pd->top->prev;

    return n;
}

Cent_ast* Cent_parse_stmt(Cent_parse_data* pd)
{
    Cent_lookahead(pd);

    if (pd->lookahead->type == Cent_token_element) {
        return Cent_parse_element_type(pd);
    }

    if (pd->lookahead->type == Cent_token_variant) {
        return Cent_parse_variant_type(pd);
    }

    if (pd->lookahead->type == Cent_token_enum) {
        return Cent_parse_enumerate(pd);
    }

    if (pd->lookahead->type == Cent_token_struct) {
        return Cent_parse_struct(pd);
    }

    if (pd->lookahead->type == Cent_token_use) {
        return Cent_parse_use(pd);
    }

    if (pd->lookahead->type == Cent_token_const) {
        return Cent_parse_const(pd);
    }

    return Cent_parse_expr(pd);
}

// Element -> element id { Element_stmt? Element_stmts }
// Element_stmt? -> Element_stmt | e
// Element_stmt -> Properties | Children
// Element_stmts -> Sep Element_stmt? Element_stmts | e
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

    // name
    Cent_token* id = NULL;
    Cent_match(pd, Cent_token_id, "expected id", &id, n);
    /* test case: test_parse_element_error_expected_id */

    if (id) {
        Zinc_string_copy(&id->value, &n->text);
        Cent_token_destroy(id);
        free(id);
    }

    // tag
    Cent_lookahead(pd);
    if (pd->lookahead->type == Cent_token_left_paren) {
        Cent_token* lp = NULL;
        if (!Cent_match(pd, Cent_token_left_paren, "expected left parenthesis", &lp, n)) {
            assert(false && "not possible");
        }
        Cent_token_destroy(lp);
        free(lp);

        Cent_token* id2 = NULL;
        Cent_match(pd, Cent_token_id, "expected id", &id2, n);

        Cent_ast* tag = NULL;
        Cent_ast_create(&tag);
        tag->type = Cent_ast_type_element_tag;

        Cent_ast* tag_id = NULL;
        Cent_ast_create(&tag_id);
        tag_id->type = Cent_ast_type_id;
        Zinc_string_add_str(&tag_id->text, "@tag");
        Cent_ast_add(tag, tag_id);

        Cent_ast* tag_type = NULL;
        Cent_ast_create(&tag_type);
        tag_type->type = Cent_ast_type_id;
        if (id2) {
            Zinc_string_add_string(&tag_type->text, &id2->value);
        }
        Cent_ast_add(tag, tag_type);
        Cent_token_destroy(id2);
        free(id2);

        Cent_ast_add(n, tag);

        Cent_token* rp = NULL;
        Cent_match(pd, Cent_token_right_paren, "expected right parenthesis", &rp, n);
        Cent_token_destroy(rp);
        free(rp);
    }

    Cent_token* lcb = NULL;
    Cent_match(pd, Cent_token_left_curly_brace, "expected left-curly-brace", &lcb, n);
    Cent_token_destroy(lcb);
    free(lcb);

    size_t properties_count = 0;
    size_t children_count = 0;

    Cent_lookahead(pd);
    if (pd->lookahead->type == Cent_token_properties) {
        Cent_ast* a = Cent_parse_element_properties(pd);
        Cent_ast_add(n, a);
        properties_count++;
    } else if (pd->lookahead->type == Cent_token_children) {
        Cent_ast* b = Cent_parse_children(pd);
        Cent_ast_add(n, b);
        children_count++;
    }

    while (Cent_parse_sep(pd, n)) {
        Cent_lookahead(pd);
        if (pd->lookahead->type == Cent_token_properties) {
            Cent_ast* a = Cent_parse_element_properties(pd);
            Cent_ast_add(n, a);
            properties_count++;
            if (properties_count == 2) {
                Zinc_error_list_set(pd->errors, &a->loc, "more than one properties");
                n->has_error = true;
            }
        } else if (pd->lookahead->type == Cent_token_children) {
            Cent_ast* b = Cent_parse_children(pd);
            Cent_ast_add(n, b);
            children_count++;
            if (children_count == 2) {
                Zinc_error_list_set(pd->errors, &b->loc, "more than one children");
                n->has_error = true;
            }
        }
    }

    Cent_token* rcb = NULL;
    Cent_match(pd, Cent_token_right_curly_brace, "expected right-curly-brace", &rcb, n);
    Cent_token_destroy(rcb);
    free(rcb);
    /* test case: test_parse_element_error_expected_end */

    if (!n->has_error) {
        Cent_symbol* sym = NULL;
        sym = Cent_environment_get(pd->top, &n->text);
        if (sym) {
            Zinc_string_finish(&n->text);
            Zinc_error_list_set(pd->errors, &n->loc, "name already exists: %s", n->text.buf);
            n->has_error = true;
            return n;
            /* test case: test_parse_element_error_name_already_exits */
        }

        Cent_element_type* element = NULL;
        Cent_element_create(&element);
        Zinc_string_copy(&n->text, &element->name);
        element->loc = n->loc;

        Cent_symbol_create(&sym);
        Cent_symbol_set_type(sym, Cent_symbol_type_element);
        sym->data.element = element;
        Cent_environment_add_symbol(pd->top, &element->name, sym);

        if (!n->has_error) {
            Zinc_priority_task* task = NULL;
            Zinc_priority_task_create(&task);
            task->priority = Cent_task_type_update_element_type;
            task->data = n;
            Zinc_priority_queue_add(&pd->pq, task);
        }
    }


    return n;
}

Cent_ast* Cent_parse_variant_type(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_variant;

    Cent_token* vnt = NULL;
    if (!Cent_match(pd, Cent_token_variant, "expected variant", &vnt, n)) {
        assert(false && "not possible");
    }
    Cent_token_destroy(vnt);
    free(vnt);

    Cent_token* id1 = NULL;
    Cent_match(pd, Cent_token_id, "expected id", &id1, n);

    Cent_token* dc = NULL;
    Cent_match(pd, Cent_token_double_colon, "expected double colon", &dc, n);
    Cent_token_destroy(dc);
    free(dc);

    Cent_token* id2 = NULL;
    Cent_match(pd, Cent_token_id, "expected id", &id2, n);

    Cent_ast* ns = NULL;
    Cent_ast_create(&ns);
    ns->type = Cent_ast_type_namespace;
    Cent_ast_add(n, ns);

    Cent_ast* el = NULL;
    Cent_ast_create(&el);
    el->type = Cent_ast_type_id;
    if (id1) {
        Zinc_string_add_string(&el->text, &id1->value);
        Cent_token_destroy(id1);
        free(id1);
    }
    Cent_ast_add(ns, el);

    Cent_ast* kind = NULL;
    Cent_ast_create(&kind);
    kind->type = Cent_ast_type_id;
    if (id2) {
        Zinc_string_add_string(&kind->text, &id2->value);
        Cent_token_destroy(id2);
        free(id2);
    }
    Cent_ast_add(ns, kind);

    Cent_token* lcb = NULL;
    Cent_match(pd, Cent_token_left_curly_brace, "expected left-curly-brace", &lcb, n);
    Cent_token_destroy(lcb);
    free(lcb);

    size_t properties_count = 0;
    size_t children_count = 0;

    Cent_lookahead(pd);
    if (pd->lookahead->type == Cent_token_properties) {
        Cent_ast* a = Cent_parse_element_properties(pd);
        Cent_ast_add(n, a);
        properties_count++;
    } else if (pd->lookahead->type == Cent_token_children) {
        Cent_ast* b = Cent_parse_children(pd);
        Cent_ast_add(n, b);
        children_count++;
    }

    while (Cent_parse_sep(pd, n)) {
        Cent_lookahead(pd);
        if (pd->lookahead->type == Cent_token_properties) {
            Cent_ast* a = Cent_parse_element_properties(pd);
            Cent_ast_add(n, a);
            properties_count++;
            if (properties_count == 2) {
                Zinc_error_list_set(pd->errors, &a->loc, "more than one properties");
                n->has_error = true;
            }
        } else if (pd->lookahead->type == Cent_token_children) {
            Cent_ast* b = Cent_parse_children(pd);
            Cent_ast_add(n, b);
            children_count++;
            if (children_count == 2) {
                Zinc_error_list_set(pd->errors, &b->loc, "more than one children");
                n->has_error = true;
            }
        }
    }

    Cent_token* rcb = NULL;
    Cent_match(pd, Cent_token_right_curly_brace, "expected right-curly-brace", &rcb, n);
    Cent_token_destroy(rcb);
    free(rcb);

    if (!n->has_error) {
        Zinc_priority_task* task = NULL;
        Zinc_priority_task_create(&task);
        task->priority = Cent_task_type_transform_variant_type;
        task->data = n;
        Zinc_priority_queue_add(&pd->pq, task);
    }
    return n;
}

// Properties -> properties { Property_dec? Properties' }
// Property_dec? -> Property_dec | e
// Properties' -> Sep Property_dec? Properties' | e
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

    Cent_token* lcb = NULL;
    Cent_match(pd, Cent_token_left_curly_brace, "expected left-curly-brace", &lcb, n);
    Cent_token_destroy(lcb);
    free(lcb);

    Cent_lookahead(pd);
    if (pd->lookahead->type == Cent_token_id) {
        Cent_ast* a = Cent_parse_property_dec(pd);
        Cent_ast_add(n, a);
    }

    while (Cent_parse_sep(pd, n)) {
        Cent_lookahead(pd);
        if (pd->lookahead->type == Cent_token_id) {
            Cent_ast* b = Cent_parse_property_dec(pd);
            Cent_ast_add(n, b);
        }
    }

    Cent_token* rcb = NULL;
    Cent_match(pd, Cent_token_right_curly_brace, "expected right-curly-brace", &rcb, n);
    Cent_token_destroy(rcb);
    free(rcb);
    /* test case: test_parse_element_error_properties_expected_end */

    return n;
}

// Property_dec -> id colon id modifier?
// modifier? -> modifier | e
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
    Zinc_string_copy(&id->value, &a->text);
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
        Zinc_string_copy(&type->value, &b->text);
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
        Zinc_string_copy(&mod->value, &c->text);
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

    Cent_token* lcb = NULL;
    Cent_match(pd, Cent_token_left_curly_brace, "expected left-curly-brace", &lcb, n);
    Cent_token_destroy(lcb);
    free(lcb);

    Cent_lookahead(pd);
    if (pd->lookahead->type == Cent_token_id) {
        Cent_ast* a = NULL;
        Cent_ast_create(&a);
        Cent_token* id = NULL;
        if (!Cent_match(pd, Cent_token_id, "expected id", &id, a)) {
            assert(false && "not possible");
        }
        a->type = Cent_ast_type_id;
        Zinc_string_copy(&id->value, &a->text);
        Cent_ast_add(n, a);
        Cent_token_destroy(id);
        free(id);
   }

    while (Cent_parse_sep(pd, n)) {
        Cent_lookahead(pd);
        if (pd->lookahead->type == Cent_token_id) {
            Cent_ast* b = NULL;
            Cent_ast_create(&b);
            Cent_token* id2 = NULL;
            if (!Cent_match(pd, Cent_token_id, "expected id", &id2, b)) {
                assert(false && "not possible");
            }
            b->type = Cent_ast_type_id;
            Zinc_string_copy(&id2->value, &b->text);
            Cent_ast_add(n, b);
            Cent_token_destroy(id2);
            free(id2);
        }
    }

    Cent_token* rcb = NULL;
    Cent_match(pd, Cent_token_right_curly_brace, "expected right-curly-brace", &rcb, n);
    Cent_token_destroy(rcb);
    free(rcb);
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
        Zinc_string_copy(&id->value, &n->text);
        Cent_token_destroy(id);
        free(id);
    }
    /* test case: test_parse_enumerate_error_expected_id */

    Cent_token* lcb = NULL;
    Cent_match(pd, Cent_token_left_curly_brace, "expected left-curly-brace", &lcb, n);
    Cent_token_destroy(lcb);
    free(lcb);

    Cent_lookahead(pd);
    if (pd->lookahead->type == Cent_token_id) {
        Cent_ast* a = NULL;
        Cent_ast_create(&a);
        a->type = Cent_ast_type_id;
        id = NULL;
        if (!Cent_match(pd, Cent_token_id, "expected id", &id, a)) {
            assert(false && "not possible");
        }

        Zinc_string_copy(&id->value, &a->text);
        Cent_token_destroy(id);
        free(id);
        Cent_ast_add(n, a);
   }

    while (Cent_parse_sep(pd, n)) {
        Cent_lookahead(pd);
        if (pd->lookahead->type == Cent_token_id) {
            Cent_ast* b = NULL;
            Cent_ast_create(&b);
            b->type = Cent_ast_type_id;
    
            Cent_token* id2 = NULL;
            if (!Cent_match(pd, Cent_token_id, "expected id", &id2, b)) {
                assert(false && "not possible");
            }
    
            Zinc_string_copy(&id2->value, &b->text);
            Cent_token_destroy(id2);
            free(id2);
            Cent_ast_add(n, b);
        }
    }

    Cent_token* rcb = NULL;
    Cent_match(pd, Cent_token_right_curly_brace, "expected right-curly-brace", &rcb, n);
    Cent_token_destroy(rcb);
    free(rcb);
    /* test case: test_parse_enumerate_error_expected_end */

    if (!n->has_error) {
        Cent_enum_type* enumerate = NULL;
        Cent_enumerate_create(&enumerate);
        Zinc_string_copy(&n->text, &enumerate->name);


        Cent_symbol* sym = NULL;
        Cent_symbol_create(&sym);
        Cent_symbol_set_type(sym, Cent_symbol_type_enumerate);
        sym->data.enumerate = enumerate;
        Cent_environment_add_symbol(pd->top, &enumerate->name, sym);

        Zinc_priority_task* task = NULL;
        Zinc_priority_task_create(&task);
        task->priority = Cent_task_type_update_enum_type;
        task->data = n;
        Zinc_priority_queue_add(&pd->pq, task);
    }

    return n;
}

Cent_ast* Cent_parse_struct(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_struct;

    Cent_token* st = NULL;
    if (!Cent_match(pd, Cent_token_struct, "expected struct", &st, n)) {
        assert(false && "not possible");
    }
    Cent_token_destroy(st);
    free(st);

    Cent_token* lcb = NULL;
    Cent_match(pd, Cent_token_left_curly_brace, "expected left-curly-brace", &lcb, n);
    Cent_token_destroy(lcb);
    free(lcb);

    Cent_ast* a = Cent_parse_param(pd);
    if (a) {
        Cent_ast_add(n, a);
    }

    while (Cent_parse_sep(pd, n)) {
        Cent_ast* b = Cent_parse_param(pd);
        if (b) {
            Cent_ast_add(n, b);
        }
    }

    Cent_token* rcb = NULL;
    Cent_match(pd, Cent_token_right_curly_brace, "expected right-curly-brace", &rcb, n);
    Cent_token_destroy(rcb);
    free(rcb);

    return n;
}

Cent_ast* Cent_parse_param(Cent_parse_data* pd)
{
    Cent_lookahead(pd);
    if (pd->lookahead->type != Cent_token_id) {
        return NULL;
    }

    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_param;

    Cent_token* id = NULL;
    if (!Cent_match(pd, Cent_token_id, "expected id", &id, n)) {
        assert(false && "not possible");
    }

    Cent_ast* a = NULL;
    Cent_ast_create(&a);
    a->type = Cent_ast_type_id;
    Zinc_string_add_string(&a->text, &id->value);
    Cent_ast_add(n, a);

    Cent_token* co = NULL;
    Cent_match(pd, Cent_token_colon, "expected colon", &co, n);
    Cent_token_destroy(co);
    free(co);

    Cent_token* id2 = NULL;
    Cent_match(pd, Cent_token_id, "expected id", &id2, n);
    if (id2) {
        Cent_ast* b = NULL;
        Cent_ast_create(&b); 
        b->type = Cent_ast_type_id;
        Zinc_string_add_string(&b->text, &id2->value);
        Cent_ast_add(n, b);
        Cent_token_destroy(id2);
        free(id2);
    }

    Cent_token_destroy(id);
    free(id);

    return n;
}

Cent_ast* Cent_parse_use(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_use;

    Cent_token* inc = NULL;
    if (!Cent_match(pd, Cent_token_use, "expected use", &inc, n)) {
        assert(false && "not possible");
    }
    Cent_token_destroy(inc);
    free(inc);

    Cent_parse_module_seq(pd, n);

    if (!n->has_error) {
        Cent_parse_import_module(pd, n);
    }

    return n;
}

void Cent_parse_module_seq(Cent_parse_data* pd, Cent_ast* n)
{
    Cent_ast* a = NULL;
    Cent_ast_create(&a);
    a->type = Cent_ast_type_id;
    Cent_token* id = NULL;
    Cent_match(pd, Cent_token_id, "expected id", &id, a);
    /* test case: test_parse_include_error_expected_id */
    if (id) {
        Zinc_string_copy(&id->value, &a->text);
        Cent_token_destroy(id);
        free(id);
    }
    Cent_ast_add(n, a);

    Cent_lookahead(pd);
    while (pd->lookahead->type == Cent_token_double_colon) {
        Cent_token* dc = NULL;
        if (!Cent_match(pd, Cent_token_double_colon, "expected double colon", &dc, n)) {
            assert(false && "not possible");
        }
        Cent_token_destroy(dc);
        free(dc);

        Cent_lookahead(pd);
        if (pd->lookahead->type == Cent_token_id) {
            Cent_ast* b = NULL;
            Cent_lookahead(pd);
            Cent_ast_create(&b);
            b->type = Cent_ast_type_id;
            Cent_token* id2 = NULL;
            if (!Cent_match(pd, Cent_token_id, "expected id", &id2, b)) {
                assert(false && "not possible");
            }
            if (id2) {
                Zinc_string_copy(&id2->value, &b->text);
                Cent_token_destroy(id2);
                free(id2);
            }
            Cent_ast_add(n, b);
        } else if (pd->lookahead->type == Cent_token_asterisk) {
            Cent_ast* b = NULL;
            Cent_ast_create(&b);
            b->type = Cent_ast_type_glob;
            Cent_token* ast = NULL;
            if (!Cent_match(pd, Cent_token_asterisk, "expected asterisk", &ast, b)) {
                assert(false && "not possible");
            }
            Cent_token_destroy(ast);
            free(ast);
            Cent_ast_add(n, b);
        } else {
            break;
        }

        Cent_lookahead(pd);
    }
}
