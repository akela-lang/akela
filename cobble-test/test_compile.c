#include "zinc/unit_test.h"
#include "cobble/compile_data.h"
#include "cobble/compile.h"
#include "cobble/Ast_node.h"
#include "zinc/memory.h"
#include "test_compile_tools.h"
#include "zinc/error_unit_test.h"

void test_compile_empty()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "");

    bool valid = true;
    Ast_node* root = NULL;

    valid = compile(cd, &root);

    expect_true(valid, "valid");
    assert_null(root, "root");

    teardown_compile(cd, root);
}

void test_compile_literal()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "a");

    bool valid = true;
    Ast_node* root = NULL;

    valid = compile(cd, &root);

    expect_true(valid, "valid");
    assert_ptr(root, "ptr root");
    expect_true(root->is_root, "is_group root");
    expect_false(root->is_group, "is_group root");
    expect_int_equal(root->type, Ast_type_literal, "literal root");
    expect_int_equal(root->num, 1, "num");
    expect_char_equal(root->c[0], 'a', "c root");

    teardown_compile(cd, root);
}

void test_compile_union_single()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "a|b");

    bool valid = true;
    Ast_node* root = NULL;

    valid = compile(cd, &root);

    expect_true(valid, "valid");
    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_union, "union root");

    Ast_node* a = Ast_node_get(root, 0);
    assert_ptr(a, "ptr a");
    expect_int_equal(a->type, Ast_type_literal, "literal a");
    expect_int_equal(a->num, 1, "num");
    expect_char_equal(a->c[0], 'a', "c[0] a");

    Ast_node* b = Ast_node_get(root, 1);
    assert_ptr(b, "ptr b");
    expect_int_equal(b->type, Ast_type_literal, "literal b");
    expect_int_equal(b->num, 1, "num");
    expect_char_equal(b->c[0], 'b', "c[0] b");

    teardown_compile(cd, root);
}

void test_compile_union_single_error()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "a|");

    bool valid = true;
    Ast_node* root = NULL;

    valid = compile(cd, &root);

    expect_false(valid, "valid");
    expect_ptr(root, "ptr root");
    expect_has_errors(cd->el);

    struct error* e = NULL;
    e = assert_source_error(cd->el, "expected term after union");
    expect_size_t_equal(e->loc.start_pos, 2, "start pos");
    expect_size_t_equal(e->loc.line, 1, "line");
    expect_size_t_equal(e->loc.col, 3, "col");
    expect_size_t_equal(e->loc.end_pos, 3, "end_pos");

    teardown_compile(cd, root);
}

void test_compile_union_double()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "a|b|c");

    bool valid = true;
    Ast_node* root = NULL;

    valid = compile(cd, &root);

    expect_true(valid, "valid");
    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_union, "union root");

    Ast_node* a = Ast_node_get(root, 0);
    assert_ptr(a, "ptr a");
    expect_int_equal(a->type, Ast_type_literal, "literal a");
    expect_int_equal(a->num, 1, "num a");
    expect_char_equal(a->c[0], 'a', "c a");

    Ast_node* b = Ast_node_get(root, 1);
    assert_ptr(b, "ptr b");
    expect_int_equal(b->type, Ast_type_literal, "literal b");
    expect_int_equal(b->num, 1, "num b");
    expect_char_equal(b->c[0], 'b', "c b");

    Ast_node* c = Ast_node_get(root, 2);
    assert_ptr(c, "ptr c");
    expect_int_equal(c->type, Ast_type_literal, "literal c");
    expect_int_equal(c->num, 1, "num c");
    expect_char_equal(c->c[0], 'c', "c c");

    teardown_compile(cd, root);
}

void test_compile_concat_single()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "ab");

    bool valid = true;
    Ast_node* root = NULL;

    valid = compile(cd, &root);

    expect_true(valid, "valid");
    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_concat, "concat root");

    Ast_node* a = Ast_node_get(root, 0);
    assert_ptr(a, "ptr a");
    expect_int_equal(a->type, Ast_type_literal, "literal a");
    expect_int_equal(a->num, 1, "num a");
    expect_char_equal(a->c[0], 'a', "c a");

    Ast_node* b = Ast_node_get(root, 1);
    assert_ptr(b, "ptr b");
    expect_int_equal(b->type, Ast_type_literal, "literal b");
    expect_int_equal(b->num, 1, "num b");
    expect_char_equal(b->c[0], 'b', "c b");

    teardown_compile(cd, root);
}

void test_compile_concat_double()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "abc");

    bool valid = true;
    Ast_node* root = NULL;

    valid = compile(cd, &root);

    expect_true(valid, "valid");
    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_concat, "concat root");

    Ast_node* a = Ast_node_get(root, 0);
    assert_ptr(a, "ptr a");
    expect_int_equal(a->type, Ast_type_literal, "literal a");
    expect_int_equal(a->num, 1, "num a");
    expect_char_equal(a->c[0], 'a', "c a");

    Ast_node* b = Ast_node_get(root, 1);
    assert_ptr(b, "ptr b");
    expect_int_equal(b->type, Ast_type_literal, "literal b");
    expect_int_equal(b->num, 1, "num b");
    expect_char_equal(b->c[0], 'b', "c b");

    Ast_node* c = Ast_node_get(root, 2);
    assert_ptr(c, "ptr c");
    expect_int_equal(c->type, Ast_type_literal, "literal c");
    expect_int_equal(c->num, 1, "num c");
    expect_char_equal(c->c[0], 'c', "c c");

    teardown_compile(cd, root);
}

void test_compile_union_concat()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "ab|cd");

    bool valid = true;
    Ast_node* root = NULL;

    valid = compile(cd, &root);

    expect_true(valid, "valid");
    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_union, "union root");

    Ast_node* ab = Ast_node_get(root, 0);
    assert_ptr(ab, "ptr ab");
    expect_int_equal(ab->type, Ast_type_concat, "concat ab");

    Ast_node* a = Ast_node_get(ab, 0);
    assert_ptr(a, "ptr a");
    expect_int_equal(a->type, Ast_type_literal, "literal a");
    expect_int_equal(a->num, 1, "num a");
    expect_char_equal(a->c[0], 'a', "c a");

    Ast_node* b = Ast_node_get(ab, 1);
    assert_ptr(b, "ptr b");
    expect_int_equal(b->type, Ast_type_literal, "literal b");
    expect_int_equal(b->num, 1, "num b");
    expect_char_equal(b->c[0], 'b', "c b");

    Ast_node* _cd = Ast_node_get(root, 1);
    assert_ptr(_cd, "ptr _cd");
    expect_int_equal(_cd->type, Ast_type_concat, "concat _cd");

    Ast_node* c = Ast_node_get(_cd, 0);
    assert_ptr(c, "ptr c");
    expect_int_equal(c->type, Ast_type_literal, "literal c");
    expect_int_equal(c->num, 1, "num c");
    expect_char_equal(c->c[0], 'c', "c c");

    Ast_node* d = Ast_node_get(_cd, 1);
    assert_ptr(d, "ptr d");
    expect_int_equal(d->type, Ast_type_literal, "literal d");
    expect_int_equal(d->num, 1, "num d");
    expect_char_equal(d->c[0], 'd', "c d");

    teardown_compile(cd, root);
}

void test_compile_closure()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "a*");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_closure, "closure root");

    Ast_node* lit = Ast_node_get(root, 0);
    assert_ptr(lit, "ptr lit");
    expect_int_equal(lit->type, Ast_type_literal, "literal lit");
    expect_int_equal(lit->num, 1, "num lit");
    expect_char_equal(lit->c[0], 'a', "c a");

    teardown_compile(cd, root);
}

void test_compile_concat_closure()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "a*b*");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_concat, "concat root");

    Ast_node* c0 = Ast_node_get(root, 0);
    assert_ptr(c0, "ptr c0");
    expect_int_equal(c0->type, Ast_type_closure, "closure c0");

    Ast_node* a = Ast_node_get(c0, 0);
    assert_ptr(a, "ptr a");
    expect_int_equal(a->type, Ast_type_literal, "literal a");
    expect_int_equal(a->num, 1, "num a");
    expect_char_equal(a->c[0], 'a', "c a");

    Ast_node* c1 = Ast_node_get(root, 1);
    assert_ptr(c1, "ptr c1");
    expect_int_equal(c1->type, Ast_type_closure, "closure c1");

    Ast_node* b = Ast_node_get(c1, 0);
    assert_ptr(b, "ptr b");
    expect_int_equal(b->type, Ast_type_literal, "literal b");
    expect_int_equal(b->num, 1, "num b");
    expect_char_equal(b->c[0], 'b', "c b");

    teardown_compile(cd, root);
}

void test_compile_union_closure()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "a*|b*");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_union, "concat root");

    Ast_node* c0 = Ast_node_get(root, 0);
    assert_ptr(c0, "ptr c0");
    expect_int_equal(c0->type, Ast_type_closure, "closure c0");

    Ast_node* a = Ast_node_get(c0, 0);
    assert_ptr(a, "ptr a");
    expect_int_equal(a->type, Ast_type_literal, "literal a");
    expect_int_equal(a->num, 1, "num a");
    expect_char_equal(a->c[0], 'a', "c a");

    Ast_node* c1 = Ast_node_get(root, 1);
    assert_ptr(c1, "ptr c1");
    expect_int_equal(c1->type, Ast_type_closure, "closure c1");

    Ast_node* b = Ast_node_get(c1, 0);
    assert_ptr(b, "ptr b");
    expect_int_equal(b->type, Ast_type_literal, "literal b");
    expect_int_equal(b->num, 1, "num b");
    expect_char_equal(b->c[0], 'b', "c b");

    teardown_compile(cd, root);
}

void test_compile_positive_closure()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "a+");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_positive_closure, "closure root");

    Ast_node* lit = Ast_node_get(root, 0);
    assert_ptr(lit, "ptr lit");
    expect_int_equal(lit->type, Ast_type_literal, "literal lit");
    expect_int_equal(lit->num, 1, "num lit");
    expect_char_equal(lit->c[0], 'a', "c a");

    teardown_compile(cd, root);
}

void test_compile_repeat()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "a{5}");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_repeat, "repeat root");

    Ast_node* lit = Ast_node_get(root, 0);
    assert_ptr(lit, "ptr lit");
    expect_int_equal(lit->type, Ast_type_literal, "literal lit");
    expect_int_equal(lit->num, 1, "num lit");
    expect_char_equal(lit->c[0], 'a', "c a");

    Ast_node* num = Ast_node_get(root, 1);
    assert_ptr(num, "ptr num");
    expect_u_long_equal(num->num_value, 5, "num1 5 root");

    teardown_compile(cd, root);
}

void test_compile_repeat_num_error()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "a{x}");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_false(valid, "valid");
    expect_has_errors(cd->el);

    struct error* e = assert_source_error(cd->el, "expected digit");
    expect_size_t_equal(e->loc.start_pos, 2, "start_pos");
    expect_size_t_equal(e->loc.line, 1, "line");
    expect_size_t_equal(e->loc.col, 3, "col");
    expect_size_t_equal(e->loc.end_pos, 3, "end_pos");

    teardown_compile(cd, root);
}

void test_compile_repeat_range()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "a{5,10}");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_repeat_range, "repeat root");

    Ast_node* lit = Ast_node_get(root, 0);
    assert_ptr(lit, "ptr lit");
    expect_int_equal(lit->type, Ast_type_literal, "literal lit");
    expect_char_equal(lit->c[0], 'a', "c a");

    Ast_node* num = Ast_node_get(root, 1);
    assert_ptr(num, "ptr num");
    expect_int_equal(num->type, Ast_type_number, "type num");
    expect_u_long_equal(num->num_value, 5, "num num");

    Ast_node* num2 = Ast_node_get(root, 2);
    assert_ptr(num2, "ptr num2");
    expect_int_equal(num2->type, Ast_type_number, "type num2");
    expect_u_long_equal(num2->num_value, 10, "num num2");

    teardown_compile(cd, root);
}

void test_compile_repeat_range_num_error()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "a{1,x}");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_false(valid, "valid");
    expect_has_errors(cd->el);

    struct error* e = assert_source_error(cd->el, "expected digit");
    expect_size_t_equal(e->loc.start_pos, 4, "byte_pos");
    expect_size_t_equal(e->loc.line, 1, "line");
    expect_size_t_equal(e->loc.col, 5, "col");
    expect_size_t_equal(e->loc.end_pos, 5, "end_pos");

    teardown_compile(cd, root);
}

void test_compile_group_concat()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "a(b|c)d");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_concat, "concat root");

    Ast_node* lit = Ast_node_get(root, 0);
    assert_ptr(lit, "ptr lit");
    expect_int_equal(lit->type, Ast_type_literal, "literal lit");
    expect_int_equal(lit->num, 1, "num lit");
    expect_char_equal(lit->c[0], 'a', "c lit");

    Ast_node* gr = Ast_node_get(root, 1);
    assert_ptr(gr, "ptr gr");
    expect_int_equal(gr->type, Ast_type_group, "group gr");
    expect_true(gr->is_group, "is group gr");
    expect_int_equal(gr->group, 1, "group gr");

    Ast_node* un = Ast_node_get(gr, 0);
    assert_ptr(un, "ptr group");
    expect_int_equal(un->type, Ast_type_union, "union un");

    Ast_node* b = Ast_node_get(un, 0);
    assert_ptr(b, "ptr b");
    expect_int_equal(b->type, Ast_type_literal, "literal b");
    expect_int_equal(b->num, 1, "num b");
    expect_char_equal(b->c[0], 'b', "c b");

    Ast_node* c = Ast_node_get(un, 1);
    assert_ptr(c, "ptr c");
    expect_int_equal(c->type, Ast_type_literal, "literal c");
    expect_int_equal(c->num, 1, "num c");
    expect_char_equal(c->c[0], 'c', "c c");

    Ast_node* d = Ast_node_get(root, 2);
    assert_ptr(d, "ptr d");
    expect_int_equal(d->type, Ast_type_literal, "literal d");
    expect_char_equal(d->c[0], 'd', "c d");

    teardown_compile(cd, root);
}

void test_compile_group_empty()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "a()b");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_concat, "concat root");

    Ast_node* lit = Ast_node_get(root, 0);
    assert_ptr(lit, "ptr lit");
    expect_int_equal(lit->type, Ast_type_literal, "literal lit");
    expect_int_equal(lit->num, 1, "num lit");
    expect_char_equal(lit->c[0], 'a', "c a");
    expect_false(lit->is_group, "is group lit");
    expect_int_equal(lit->group, 0, "group lit");

    Ast_node* gr = Ast_node_get(root, 1);
    assert_ptr(gr, "ptr gr");
    expect_int_equal(gr->type, Ast_type_group, "group gr");
    expect_true(gr->is_group, "is group gr");
    expect_int_equal(gr->group, 1, "group gr");

    Ast_node* b = Ast_node_get(root, 2);
    assert_ptr(b, "ptr b");
    expect_int_equal(b->type, Ast_type_literal, "literal b");
    expect_int_equal(b->num, 1, "num b");
    expect_char_equal(b->c[0], 'b', "c b");

    teardown_compile(cd, root);
}

void test_compile_option()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "a?");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_option, "option root");

    Ast_node* lit = Ast_node_get(root, 0);
    assert_ptr(lit, "ptr lit");
    expect_int_equal(lit->type, Ast_type_literal, "literal lit");
    expect_int_equal(lit->num, 1, "num lit");
    expect_char_equal(lit->c[0], 'a', "c a");

    teardown_compile(cd, root);
}

void test_compile_wildcard()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, ".");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_wildcard, "type root");

    teardown_compile(cd, root);
}

void test_compile_begin()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "^");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_begin, "type root");

    teardown_compile(cd, root);
}

void test_compile_end()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "$");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_end, "type root");

    teardown_compile(cd, root);
}

void test_compile_escape_backslash()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "\\\\");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_escape, "type root");
    expect_int_equal(root->num, 1, "num root");
    expect_char_equal(root->c[0], '\\', "c root");

    teardown_compile(cd, root);
}

void test_compile_escape_asterisk()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "\\*");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_escape, "type root");
    expect_int_equal(root->num, 1, "num root");
    expect_char_equal(root->c[0], '*', "c root");

    teardown_compile(cd, root);
}

void test_compile_character_class()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "[abc]");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_character_class, "type root");

    Ast_node* c0 = Ast_node_get(root, 0);
    assert_ptr(c0, "ptr c0");
    expect_int_equal(c0->type, Ast_type_literal, "type c0");
    expect_int_equal(c0->num, 1, "num c0");
    expect_char_equal(c0->c[0], 'a', "c c0");

    Ast_node* c1 = Ast_node_get(root, 1);
    assert_ptr(c1, "ptr c1");
    expect_int_equal(c1->type, Ast_type_literal, "type c1");
    expect_int_equal(c1->num, 1, "num c1");
    expect_char_equal(c1->c[0], 'b', "c c1");

    Ast_node* c2 = Ast_node_get(root, 2);
    assert_ptr(c2, "ptr c2");
    expect_int_equal(c2->type, Ast_type_literal, "type c2");
    expect_int_equal(c2->num, 1, "num c2");
    expect_char_equal(c2->c[0], 'c', "c c2");

    teardown_compile(cd, root);
}

void test_compile_character_class_wildcard_error()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "[.]");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_false(valid, "valid");
    expect_has_errors(cd->el);
    expect_source_error(cd->el, "unexpected wildcard");

    teardown_compile(cd, root);
}

void test_compile_character_class_begin_error()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "[a^]");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_false(valid, "valid");
    expect_has_errors(cd->el);
    expect_source_error(cd->el, "unexpected begin");

    teardown_compile(cd, root);
}

void test_compile_character_class_end_error()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "[$]");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_false(valid, "valid");
    expect_has_errors(cd->el);
    expect_source_error(cd->el, "unexpected end");

    teardown_compile(cd, root);
}

void test_compile_character_range()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "[a-z]");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_character_class, "type root");

    Ast_node* c0 = Ast_node_get(root, 0);
    assert_ptr(c0, "ptr c0");
    expect_int_equal(c0->type, Ast_type_character_range, "type c0");
    expect_int_equal(c0->num, 0, "num c0");

    Ast_node* c00 = Ast_node_get(c0, 0);
    assert_ptr(c00, "ptr c00");
    expect_int_equal(c00->type, Ast_type_literal, "type c00");
    expect_int_equal(c00->num, 1, "num c00");
    expect_char_equal(c00->c[0], 'a', "c c00");

    Ast_node* c01 = Ast_node_get(c0, 1);
    assert_ptr(c01, "ptr c01");
    expect_int_equal(c01->type, Ast_type_literal, "type c01");
    expect_int_equal(c01->num, 1, "num c01");
    expect_char_equal(c01->c[0], 'z', "c c01");

    teardown_compile(cd, root);
}

void test_compile_character_range_error()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "[a-]");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_false(valid, "valid");
    expect_has_errors(cd->el);
    expect_source_error(cd->el, "expected end character in character range");

    teardown_compile(cd, root);
}

void test_compile_character_range_non_ascii_error_left()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "[á-z]");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_false(valid, "valid");
    expect_has_errors(cd->el);
    expect_source_error(cd->el, "character range must use ascii characters");

    teardown_compile(cd, root);
}

void test_compile_character_range_non_ascii_error_right()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "[a-ź]");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_false(valid, "valid");
    expect_has_errors(cd->el);
    expect_source_error(cd->el, "character range must use ascii characters");

    teardown_compile(cd, root);
}

void test_compile_character_type_word()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "\\w");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_character_type_word, "type root");

    teardown_compile(cd, root);
}

void test_compile_character_type_word_opposite()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "\\W");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_character_type_word_opposite, "type root");

    teardown_compile(cd, root);
}

void test_compile_character_type_digit()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "\\d");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_character_type_digit, "type root");

    teardown_compile(cd, root);
}

void test_compile_character_type_digit_opposite()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "\\D");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_character_type_digit_opposite, "type root");

    teardown_compile(cd, root);
}

void test_compile_character_type_space()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "\\s");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_character_type_space, "type root");

    teardown_compile(cd, root);
}

void test_compile_character_type_space_opposite()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "\\S");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_character_type_space_opposite, "type root");

    teardown_compile(cd, root);
}

void test_compile_character_type_newline_opposite()
{
    test_name(__func__);

    struct compile_data* cd = NULL;
    setup_compile(&cd, "\\N");

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);

    expect_true(valid, "valid");
    expect_no_errors(cd->el);

    assert_ptr(root, "ptr root");
    expect_int_equal(root->type, Ast_type_character_type_newline_opposite, "type root");

    teardown_compile(cd, root);
}

void test_compile()
{
    test_compile_empty();
    test_compile_literal();
    test_compile_union_single();
    test_compile_union_single_error();
    test_compile_union_double();
    test_compile_concat_single();
    test_compile_concat_double();
    test_compile_union_concat();
    test_compile_closure();
    test_compile_concat_closure();
    test_compile_union_closure();
    test_compile_positive_closure();
    test_compile_repeat();
    test_compile_repeat_num_error();
    test_compile_repeat_range();
    test_compile_repeat_range_num_error();
    test_compile_group_concat();
    test_compile_group_empty();
    test_compile_option();
    test_compile_wildcard();
    test_compile_begin();
    test_compile_end();
    test_compile_escape_backslash();
    test_compile_escape_asterisk();
    test_compile_character_class();
    test_compile_character_class_wildcard_error();
    test_compile_character_class_begin_error();
    test_compile_character_class_end_error();
    test_compile_character_range();
    test_compile_character_range_error();
    test_compile_character_range_non_ascii_error_left();
    test_compile_character_range_non_ascii_error_right();
    test_compile_character_type_word();
    test_compile_character_type_word_opposite();
    test_compile_character_type_digit();
    test_compile_character_type_digit_opposite();
    test_compile_character_type_space();
    test_compile_character_type_space_opposite();
    test_compile_character_type_newline_opposite();
}