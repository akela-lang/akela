#include <cobble/match_tools.h>

#include "zinc/unit_test.h"
#include "cobble/compile_data.h"
#include "cobble/compile.h"
#include "cobble/ast.h"
#include "test_compile_tools.h"
#include "zinc/error_unit_test.h"

void test_compile_empty()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("");

    Zinc_expect_no_errors(re.errors);
    Zinc_assert_null(re.root, "root");
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Cob_re_destroy(&re);
}

void test_compile_literal()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("a");

    Zinc_expect_no_errors(re.errors);
    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_true(re.root->is_root, "is_group root");
    Zinc_expect_false(re.root->is_group, "is_group root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_literal, "literal root");
    Zinc_expect_int_equal(re.root->num, 1, "num");
    Zinc_expect_char_equal(re.root->c[0], 'a', "c root");

    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Cob_re_destroy(&re);
}

void test_compile_union_single()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("a|b");

    Zinc_expect_no_errors(re.errors);

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_union, "union root");

    Cob_ast* a = Cob_ast_get(re.root, 0);
    Zinc_assert_ptr(a, "ptr a");
    Zinc_expect_int_equal(a->type, Cob_ast_type_literal, "literal a");
    Zinc_expect_int_equal(a->num, 1, "num");
    Zinc_expect_char_equal(a->c[0], 'a', "c[0] a");

    Cob_ast* b = Cob_ast_get(re.root, 1);
    Zinc_assert_ptr(b, "ptr b");
    Zinc_expect_int_equal(b->type, Cob_ast_type_literal, "literal b");
    Zinc_expect_int_equal(b->num, 1, "num");
    Zinc_expect_char_equal(b->c[0], 'b', "c[0] b");

    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Cob_re_destroy(&re);
}

void test_compile_union_single_error()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("a|");

    Zinc_expect_has_errors(re.errors);
    Zinc_expect_ptr(re.root, "ptr root");
    Zinc_expect_has_errors(re.errors);

    struct Zinc_error* e = NULL;
    e = Zinc_assert_source_error(re.errors, "expected term after union");
    Zinc_expect_size_t_equal(e->loc.start_pos, 2, "start pos");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line");
    Zinc_expect_size_t_equal(e->loc.col, 3, "col");
    Zinc_expect_size_t_equal(e->loc.end_pos, 3, "end_pos");

    Cob_re_destroy(&re);
}

void test_compile_union_double()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("a|b|c");

    Zinc_expect_no_errors(re.errors);
    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_union, "union root");

    Cob_ast* a = Cob_ast_get(re.root, 0);
    Zinc_assert_ptr(a, "ptr a");
    Zinc_expect_int_equal(a->type, Cob_ast_type_literal, "literal a");
    Zinc_expect_int_equal(a->num, 1, "num a");
    Zinc_expect_char_equal(a->c[0], 'a', "c a");

    Cob_ast* b = Cob_ast_get(re.root, 1);
    Zinc_assert_ptr(b, "ptr b");
    Zinc_expect_int_equal(b->type, Cob_ast_type_literal, "literal b");
    Zinc_expect_int_equal(b->num, 1, "num b");
    Zinc_expect_char_equal(b->c[0], 'b', "c b");

    Cob_ast* c = Cob_ast_get(re.root, 2);
    Zinc_assert_ptr(c, "ptr c");
    Zinc_expect_int_equal(c->type, Cob_ast_type_literal, "literal c");
    Zinc_expect_int_equal(c->num, 1, "num c");
    Zinc_expect_char_equal(c->c[0], 'c', "c c");

    Cob_re_destroy(&re);
}

void test_compile_concat_single()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("ab");

    Zinc_expect_no_errors(re.errors);
    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_concat, "concat root");

    Cob_ast* a = Cob_ast_get(re.root, 0);
    Zinc_assert_ptr(a, "ptr a");
    Zinc_expect_int_equal(a->type, Cob_ast_type_literal, "literal a");
    Zinc_expect_int_equal(a->num, 1, "num a");
    Zinc_expect_char_equal(a->c[0], 'a', "c a");

    Cob_ast* b = Cob_ast_get(re.root, 1);
    Zinc_assert_ptr(b, "ptr b");
    Zinc_expect_int_equal(b->type, Cob_ast_type_literal, "literal b");
    Zinc_expect_int_equal(b->num, 1, "num b");
    Zinc_expect_char_equal(b->c[0], 'b', "c b");

    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Cob_re_destroy(&re);
}

void test_compile_concat_double()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("abc");

    Zinc_expect_no_errors(re.errors);
    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_concat, "concat root");

    Cob_ast* a = Cob_ast_get(re.root, 0);
    Zinc_assert_ptr(a, "ptr a");
    Zinc_expect_int_equal(a->type, Cob_ast_type_literal, "literal a");
    Zinc_expect_int_equal(a->num, 1, "num a");
    Zinc_expect_char_equal(a->c[0], 'a', "c a");

    Cob_ast* b = Cob_ast_get(re.root, 1);
    Zinc_assert_ptr(b, "ptr b");
    Zinc_expect_int_equal(b->type, Cob_ast_type_literal, "literal b");
    Zinc_expect_int_equal(b->num, 1, "num b");
    Zinc_expect_char_equal(b->c[0], 'b', "c b");

    Cob_ast* c = Cob_ast_get(re.root, 2);
    Zinc_assert_ptr(c, "ptr c");
    Zinc_expect_int_equal(c->type, Cob_ast_type_literal, "literal c");
    Zinc_expect_int_equal(c->num, 1, "num c");
    Zinc_expect_char_equal(c->c[0], 'c', "c c");

    Cob_re_destroy(&re);
}

void test_compile_union_concat()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("ab|cd");

    Zinc_expect_no_errors(re.errors);
    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_union, "union root");

    Cob_ast* ab = Cob_ast_get(re.root, 0);
    Zinc_assert_ptr(ab, "ptr ab");
    Zinc_expect_int_equal(ab->type, Cob_ast_type_concat, "concat ab");

    Cob_ast* a = Cob_ast_get(ab, 0);
    Zinc_assert_ptr(a, "ptr a");
    Zinc_expect_int_equal(a->type, Cob_ast_type_literal, "literal a");
    Zinc_expect_int_equal(a->num, 1, "num a");
    Zinc_expect_char_equal(a->c[0], 'a', "c a");

    Cob_ast* b = Cob_ast_get(ab, 1);
    Zinc_assert_ptr(b, "ptr b");
    Zinc_expect_int_equal(b->type, Cob_ast_type_literal, "literal b");
    Zinc_expect_int_equal(b->num, 1, "num b");
    Zinc_expect_char_equal(b->c[0], 'b', "c b");

    Cob_ast* _cd = Cob_ast_get(re.root, 1);
    Zinc_assert_ptr(_cd, "ptr _cd");
    Zinc_expect_int_equal(_cd->type, Cob_ast_type_concat, "concat _cd");

    Cob_ast* c = Cob_ast_get(_cd, 0);
    Zinc_assert_ptr(c, "ptr c");
    Zinc_expect_int_equal(c->type, Cob_ast_type_literal, "literal c");
    Zinc_expect_int_equal(c->num, 1, "num c");
    Zinc_expect_char_equal(c->c[0], 'c', "c c");

    Cob_ast* d = Cob_ast_get(_cd, 1);
    Zinc_assert_ptr(d, "ptr d");
    Zinc_expect_int_equal(d->type, Cob_ast_type_literal, "literal d");
    Zinc_expect_int_equal(d->num, 1, "num d");
    Zinc_expect_char_equal(d->c[0], 'd', "c d");

    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Cob_re_destroy(&re);
}

void test_compile_closure()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("a*");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_closure, "closure root");

    Cob_ast* lit = Cob_ast_get(re.root, 0);
    Zinc_assert_ptr(lit, "ptr lit");
    Zinc_expect_int_equal(lit->type, Cob_ast_type_literal, "literal lit");
    Zinc_expect_int_equal(lit->num, 1, "num lit");
    Zinc_expect_char_equal(lit->c[0], 'a', "c a");

    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Cob_re_destroy(&re);
}

void test_compile_concat_closure()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("a*b*");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_concat, "concat root");

    Cob_ast* c0 = Cob_ast_get(re.root, 0);
    Zinc_assert_ptr(c0, "ptr c0");
    Zinc_expect_int_equal(c0->type, Cob_ast_type_closure, "closure c0");

    Cob_ast* a = Cob_ast_get(c0, 0);
    Zinc_assert_ptr(a, "ptr a");
    Zinc_expect_int_equal(a->type, Cob_ast_type_literal, "literal a");
    Zinc_expect_int_equal(a->num, 1, "num a");
    Zinc_expect_char_equal(a->c[0], 'a', "c a");

    Cob_ast* c1 = Cob_ast_get(re.root, 1);
    Zinc_assert_ptr(c1, "ptr c1");
    Zinc_expect_int_equal(c1->type, Cob_ast_type_closure, "closure c1");

    Cob_ast* b = Cob_ast_get(c1, 0);
    Zinc_assert_ptr(b, "ptr b");
    Zinc_expect_int_equal(b->type, Cob_ast_type_literal, "literal b");
    Zinc_expect_int_equal(b->num, 1, "num b");
    Zinc_expect_char_equal(b->c[0], 'b', "c b");


    Cob_re_destroy(&re);
}

void test_compile_union_closure()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("a*|b*");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_union, "concat root");

    Cob_ast* c0 = Cob_ast_get(re.root, 0);
    Zinc_assert_ptr(c0, "ptr c0");
    Zinc_expect_int_equal(c0->type, Cob_ast_type_closure, "closure c0");

    Cob_ast* a = Cob_ast_get(c0, 0);
    Zinc_assert_ptr(a, "ptr a");
    Zinc_expect_int_equal(a->type, Cob_ast_type_literal, "literal a");
    Zinc_expect_int_equal(a->num, 1, "num a");
    Zinc_expect_char_equal(a->c[0], 'a', "c a");

    Cob_ast* c1 = Cob_ast_get(re.root, 1);
    Zinc_assert_ptr(c1, "ptr c1");
    Zinc_expect_int_equal(c1->type, Cob_ast_type_closure, "closure c1");

    Cob_ast* b = Cob_ast_get(c1, 0);
    Zinc_assert_ptr(b, "ptr b");
    Zinc_expect_int_equal(b->type, Cob_ast_type_literal, "literal b");
    Zinc_expect_int_equal(b->num, 1, "num b");
    Zinc_expect_char_equal(b->c[0], 'b', "c b");

    Cob_re_destroy(&re);
}

void test_compile_positive_closure()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("a+");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_positive_closure, "closure root");

    Cob_ast* lit = Cob_ast_get(re.root, 0);
    Zinc_assert_ptr(lit, "ptr lit");
    Zinc_expect_int_equal(lit->type, Cob_ast_type_literal, "literal lit");
    Zinc_expect_int_equal(lit->num, 1, "num lit");
    Zinc_expect_char_equal(lit->c[0], 'a', "c a");

    Cob_re_destroy(&re);
}

void test_compile_repeat()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("a{5}");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_repeat, "repeat root");

    Cob_ast* lit = Cob_ast_get(re.root, 0);
    Zinc_assert_ptr(lit, "ptr lit");
    Zinc_expect_int_equal(lit->type, Cob_ast_type_literal, "literal lit");
    Zinc_expect_int_equal(lit->num, 1, "num lit");
    Zinc_expect_char_equal(lit->c[0], 'a', "c a");

    Cob_ast* num = Cob_ast_get(re.root, 1);
    Zinc_assert_ptr(num, "ptr num");
    Zinc_expect_u_long_equal(num->num_value, 5, "num1 5 root");

    Cob_re_destroy(&re);
}

void test_compile_repeat_num_error()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("a{x}");

    Zinc_expect_has_errors(re.errors);
    Zinc_expect_has_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    struct Zinc_error* e = Zinc_assert_source_error(re.errors, "expected digit");
    Zinc_expect_size_t_equal(e->loc.start_pos, 2, "start_pos");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line");
    Zinc_expect_size_t_equal(e->loc.col, 3, "col");
    Zinc_expect_size_t_equal(e->loc.end_pos, 3, "end_pos");

    Cob_re_destroy(&re);
}

void test_compile_repeat_range()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("a{5,10}");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_repeat_range, "repeat root");

    Cob_ast* lit = Cob_ast_get(re.root, 0);
    Zinc_assert_ptr(lit, "ptr lit");
    Zinc_expect_int_equal(lit->type, Cob_ast_type_literal, "literal lit");
    Zinc_expect_char_equal(lit->c[0], 'a', "c a");

    Cob_ast* num = Cob_ast_get(re.root, 1);
    Zinc_assert_ptr(num, "ptr num");
    Zinc_expect_int_equal(num->type, Cob_ast_type_number, "type num");
    Zinc_expect_u_long_equal(num->num_value, 5, "num num");

    Cob_ast* num2 = Cob_ast_get(re.root, 2);
    Zinc_assert_ptr(num2, "ptr num2");
    Zinc_expect_int_equal(num2->type, Cob_ast_type_number, "type num2");
    Zinc_expect_u_long_equal(num2->num_value, 10, "num num2");

    Cob_re_destroy(&re);
}

void test_compile_repeat_range_num_error()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("a{1,x}");

    Zinc_expect_has_errors(re.errors);
    Zinc_expect_has_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    struct Zinc_error* e = Zinc_assert_source_error(re.errors, "expected digit");
    Zinc_expect_size_t_equal(e->loc.start_pos, 4, "byte_pos");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line");
    Zinc_expect_size_t_equal(e->loc.col, 5, "col");
    Zinc_expect_size_t_equal(e->loc.end_pos, 5, "end_pos");

    Cob_re_destroy(&re);
}

void test_compile_group_concat()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("a(b|c)d");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 2, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_concat, "concat root");

    Cob_ast* lit = Cob_ast_get(re.root, 0);
    Zinc_assert_ptr(lit, "ptr lit");
    Zinc_expect_int_equal(lit->type, Cob_ast_type_literal, "literal lit");
    Zinc_expect_int_equal(lit->num, 1, "num lit");
    Zinc_expect_char_equal(lit->c[0], 'a', "c lit");

    Cob_ast* gr = Cob_ast_get(re.root, 1);
    Zinc_assert_ptr(gr, "ptr gr");
    Zinc_expect_int_equal(gr->type, Cob_ast_type_group, "group gr");
    Zinc_expect_true(gr->is_group, "is group gr");
    Zinc_expect_size_t_equal(gr->group, 1, "group gr");

    Cob_ast* un = Cob_ast_get(gr, 0);
    Zinc_assert_ptr(un, "ptr group");
    Zinc_expect_int_equal(un->type, Cob_ast_type_union, "union un");

    Cob_ast* b = Cob_ast_get(un, 0);
    Zinc_assert_ptr(b, "ptr b");
    Zinc_expect_int_equal(b->type, Cob_ast_type_literal, "literal b");
    Zinc_expect_int_equal(b->num, 1, "num b");
    Zinc_expect_char_equal(b->c[0], 'b', "c b");

    Cob_ast* c = Cob_ast_get(un, 1);
    Zinc_assert_ptr(c, "ptr c");
    Zinc_expect_int_equal(c->type, Cob_ast_type_literal, "literal c");
    Zinc_expect_int_equal(c->num, 1, "num c");
    Zinc_expect_char_equal(c->c[0], 'c', "c c");

    Cob_ast* d = Cob_ast_get(re.root, 2);
    Zinc_assert_ptr(d, "ptr d");
    Zinc_expect_int_equal(d->type, Cob_ast_type_literal, "literal d");
    Zinc_expect_char_equal(d->c[0], 'd', "c d");

    Cob_re_destroy(&re);
}

void test_compile_group_empty()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("a()b");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 2, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_concat, "concat root");

    Cob_ast* lit = Cob_ast_get(re.root, 0);
    Zinc_assert_ptr(lit, "ptr lit");
    Zinc_expect_int_equal(lit->type, Cob_ast_type_literal, "literal lit");
    Zinc_expect_int_equal(lit->num, 1, "num lit");
    Zinc_expect_char_equal(lit->c[0], 'a', "c a");
    Zinc_expect_false(lit->is_group, "is group lit");
    Zinc_expect_size_t_equal(lit->group, 0, "group lit");

    Cob_ast* gr = Cob_ast_get(re.root, 1);
    Zinc_assert_ptr(gr, "ptr gr");
    Zinc_expect_int_equal(gr->type, Cob_ast_type_group, "group gr");
    Zinc_expect_true(gr->is_group, "is group gr");
    Zinc_expect_size_t_equal(gr->group, 1, "group gr");

    Cob_ast* b = Cob_ast_get(re.root, 2);
    Zinc_assert_ptr(b, "ptr b");
    Zinc_expect_int_equal(b->type, Cob_ast_type_literal, "literal b");
    Zinc_expect_int_equal(b->num, 1, "num b");
    Zinc_expect_char_equal(b->c[0], 'b', "c b");

    Cob_re_destroy(&re);
}

void test_compile_option()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("a?");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_option, "option root");

    Cob_ast* lit = Cob_ast_get(re.root, 0);
    Zinc_assert_ptr(lit, "ptr lit");
    Zinc_expect_int_equal(lit->type, Cob_ast_type_literal, "literal lit");
    Zinc_expect_int_equal(lit->num, 1, "num lit");
    Zinc_expect_char_equal(lit->c[0], 'a', "c a");

    Cob_re_destroy(&re);
}

void test_compile_wildcard()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str(".");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_wildcard, "type root");

    Cob_re_destroy(&re);
}

void test_compile_begin()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("^");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_begin, "type root");

    Cob_re_destroy(&re);
}

void test_compile_end()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("$");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_end, "type root");

    Cob_re_destroy(&re);
}

void test_compile_escape_backslash()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("\\\\");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_escape, "type root");
    Zinc_expect_int_equal(re.root->num, 1, "num root");
    Zinc_expect_char_equal(re.root->c[0], '\\', "c root");

    Cob_re_destroy(&re);
}

void test_compile_escape_asterisk()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("\\*");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_escape, "type root");
    Zinc_expect_int_equal(re.root->num, 1, "num root");
    Zinc_expect_char_equal(re.root->c[0], '*', "c root");

    Cob_re_destroy(&re);
}

void test_compile_character_class()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("[abc]");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_character_class, "type root");

    Cob_ast* c0 = Cob_ast_get(re.root, 0);
    Zinc_assert_ptr(c0, "ptr c0");
    Zinc_expect_int_equal(c0->type, Cob_ast_type_literal, "type c0");
    Zinc_expect_int_equal(c0->num, 1, "num c0");
    Zinc_expect_char_equal(c0->c[0], 'a', "c c0");

    Cob_ast* c1 = Cob_ast_get(re.root, 1);
    Zinc_assert_ptr(c1, "ptr c1");
    Zinc_expect_int_equal(c1->type, Cob_ast_type_literal, "type c1");
    Zinc_expect_int_equal(c1->num, 1, "num c1");
    Zinc_expect_char_equal(c1->c[0], 'b', "c c1");

    Cob_ast* c2 = Cob_ast_get(re.root, 2);
    Zinc_assert_ptr(c2, "ptr c2");
    Zinc_expect_int_equal(c2->type, Cob_ast_type_literal, "type c2");
    Zinc_expect_int_equal(c2->num, 1, "num c2");
    Zinc_expect_char_equal(c2->c[0], 'c', "c c2");

    Cob_re_destroy(&re);
}

void test_compile_character_class_wildcard_error()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("[.]");

    Zinc_expect_has_errors(re.errors);
    Zinc_expect_has_errors(re.errors);
    Zinc_expect_source_error(re.errors, "unexpected wildcard");

    Cob_re_destroy(&re);
}

void test_compile_character_class_begin_error()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("[a^]");

    Zinc_expect_has_errors(re.errors);
    Zinc_expect_has_errors(re.errors);
    Zinc_expect_source_error(re.errors, "unexpected begin");

    Cob_re_destroy(&re);
}

void test_compile_character_class_end_error()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("[$]");

    Zinc_expect_has_errors(re.errors);
    Zinc_expect_has_errors(re.errors);
    Zinc_expect_source_error(re.errors, "unexpected end");

    Cob_re_destroy(&re);
}

void test_compile_character_range()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("[a-z]");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_character_class, "type root");

    Cob_ast* c0 = Cob_ast_get(re.root, 0);
    Zinc_assert_ptr(c0, "ptr c0");
    Zinc_expect_int_equal(c0->type, Cob_ast_type_character_range, "type c0");
    Zinc_expect_int_equal(c0->num, 0, "num c0");

    Cob_ast* c00 = Cob_ast_get(c0, 0);
    Zinc_assert_ptr(c00, "ptr c00");
    Zinc_expect_int_equal(c00->type, Cob_ast_type_literal, "type c00");
    Zinc_expect_int_equal(c00->num, 1, "num c00");
    Zinc_expect_char_equal(c00->c[0], 'a', "c c00");

    Cob_ast* c01 = Cob_ast_get(c0, 1);
    Zinc_assert_ptr(c01, "ptr c01");
    Zinc_expect_int_equal(c01->type, Cob_ast_type_literal, "type c01");
    Zinc_expect_int_equal(c01->num, 1, "num c01");
    Zinc_expect_char_equal(c01->c[0], 'z', "c c01");

    Cob_re_destroy(&re);
}

void test_compile_character_range_error()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("[a-]");

    Zinc_expect_has_errors(re.errors);
    Zinc_expect_has_errors(re.errors);
    Zinc_expect_source_error(re.errors, "expected end character in character range");

    Cob_re_destroy(&re);
}

void test_compile_character_range_non_ascii_error_left()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("[á-z]");

    Zinc_expect_has_errors(re.errors);
    Zinc_expect_has_errors(re.errors);
    Zinc_expect_source_error(re.errors, "character range must use ascii characters");

    Cob_re_destroy(&re);
}

void test_compile_character_range_non_ascii_error_right()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("[a-ź]");

    Zinc_expect_has_errors(re.errors);
    Zinc_expect_has_errors(re.errors);
    Zinc_expect_source_error(re.errors, "character range must use ascii characters");

    Cob_re_destroy(&re);
}

void test_compile_character_type_word()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("\\w");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_character_type_word, "type root");

    Cob_re_destroy(&re);
}

void test_compile_character_type_word_opposite()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("\\W");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_character_type_word_opposite, "type root");

    Cob_re_destroy(&re);
}

void test_compile_character_type_digit()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("\\d");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_character_type_digit, "type root");

    Cob_re_destroy(&re);
}

void test_compile_character_type_digit_opposite()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("\\D");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_character_type_digit_opposite, "type root");

    Cob_re_destroy(&re);
}

void test_compile_character_type_space()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("\\s");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_character_type_space, "type root");

    Cob_re_destroy(&re);
}

void test_compile_character_type_space_opposite()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("\\S");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_character_type_space_opposite, "type root");

    Cob_re_destroy(&re);
}

void test_compile_character_type_newline_opposite()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("\\N");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_character_type_newline_opposite, "type root");

    Cob_re_destroy(&re);
}

void test_compile_coverage_line()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("\\s*(\\-):\\s*(\\d+):Source:(.+)");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 4, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_concat, "type root");
    Zinc_expect_true(re.root->is_root, "is_root root");
    Zinc_expect_false(re.root->is_group, "is_group root");

    Cob_ast* space0 = Cob_ast_get(re.root, 0);
    Zinc_assert_ptr(space0, "ptr space0");
    Zinc_expect_int_equal(space0->type, Cob_ast_type_closure, "type space0");
    Zinc_expect_false(space0->is_root, "is_root space0");
    Zinc_expect_false(space0->is_group, "is_group space0");

    Cob_ast* space00 = Cob_ast_get(space0, 0);
    Zinc_assert_ptr(space00, "ptr space00");
    Zinc_expect_int_equal(space00->type, Cob_ast_type_character_type_space, "type space00");
    Zinc_expect_false(space00->is_root, "is_root space00");
    Zinc_expect_false(space00->is_group, "is_group space00");

    Cob_ast* group0 = Cob_ast_get(re.root, 1);
    Zinc_assert_ptr(group0, "ptr group0");
    Zinc_expect_int_equal(group0->type, Cob_ast_type_group, "type group0");
    Zinc_expect_false(group0->is_root, "is_root group0");
    Zinc_expect_true(group0->is_group, "is_group group0");
    Zinc_expect_size_t_equal(group0->group, 1, "group group0");

    Cob_ast* escape0 = Cob_ast_get(group0, 0);
    Zinc_assert_ptr(escape0, "ptr escape0");
    Zinc_expect_int_equal(escape0->type, Cob_ast_type_escape, "type escape0");
    Zinc_expect_utf8_char_str(escape0->c, escape0->num, "-", "char[4],num escape0");
    Zinc_expect_false(escape0->is_root, "is_root escape0");
    Zinc_expect_false(escape0->is_group, "is_group escape0");

    Cob_ast* colon0 = Cob_ast_get(re.root, 2);
    Zinc_assert_ptr(colon0, "ptr colon0");
    Zinc_expect_int_equal(colon0->type, Cob_ast_type_literal, "type colon0");
    Zinc_expect_utf8_char_str(colon0->c, colon0->num, ":", "char[4],num colon0");
    Zinc_expect_false(colon0->is_root, "is_root colon0");
    Zinc_expect_false(colon0->is_group, "is_group colon0");

    Cob_ast* space1 = Cob_ast_get(re.root, 3);
    Zinc_assert_ptr(space1, "ptr space1");
    Zinc_expect_int_equal(space1->type, Cob_ast_type_closure, "type space1");
    Zinc_expect_false(space1->is_root, "is_root space1");
    Zinc_expect_false(space1->is_group, "is_group space1");

    Cob_ast* space10 = Cob_ast_get(space1, 0);
    Zinc_assert_ptr(space10, "ptr space10");
    Zinc_expect_int_equal(space10->type, Cob_ast_type_character_type_space, "type space10");
    Zinc_expect_false(space10->is_root, "is_root space10");
    Zinc_expect_false(space10->is_group, "is_group space10");

    Cob_ast* group1 = Cob_ast_get(re.root, 4);
    Zinc_assert_ptr(group1, "ptr group1");
    Zinc_expect_int_equal(group1->type, Cob_ast_type_group, "type group1");
    Zinc_expect_false(group1->is_root, "is_root group1");
    Zinc_expect_true(group1->is_group, "is_group group1");
    Zinc_expect_size_t_equal(group1->group, 2, "group group1");

    Cob_ast* digit0 = Cob_ast_get(group1, 0);
    Zinc_assert_ptr(digit0, "ptr digit0");
    Zinc_expect_int_equal(digit0->type, Cob_ast_type_positive_closure, "type digit0");
    Zinc_expect_false(digit0->is_root, "is_root digit0");
    Zinc_expect_false(digit0->is_group, "is_group digit0");

    Cob_ast* digit01 = Cob_ast_get(digit0, 0);
    Zinc_assert_ptr(digit01, "ptr digit01");
    Zinc_expect_int_equal(digit01->type, Cob_ast_type_character_type_digit, "type digit01");
    Zinc_expect_false(digit01->is_root, "is_root digit01");
    Zinc_expect_false(digit01->is_group, "is_group digit01");

    Cob_ast* colon1 = Cob_ast_get(re.root, 5);
    Zinc_assert_ptr(colon1, "ptr colon1");
    Zinc_expect_int_equal(colon1->type, Cob_ast_type_literal, "type colon1");
    Zinc_expect_utf8_char_str(colon1->c, colon1->num, ":", "char[4],num colon1");
    Zinc_expect_false(colon1->is_root, "is_root colon1");
    Zinc_expect_false(colon1->is_group, "is_group colon1");

    Cob_ast* lit0 = Cob_ast_get(re.root, 6);
    Zinc_assert_ptr(lit0, "ptr lit0");
    Zinc_expect_int_equal(lit0->type, Cob_ast_type_literal, "type lit0");
    Zinc_expect_utf8_char_str(lit0->c, lit0->num, "S", "char[4],num lit0");
    Zinc_expect_false(lit0->is_root, "is_root lit0");
    Zinc_expect_false(lit0->is_group, "is_group lit0");

    Cob_ast* lit1 = Cob_ast_get(re.root, 7);
    Zinc_assert_ptr(lit1, "ptr lit1");
    Zinc_expect_int_equal(lit1->type, Cob_ast_type_literal, "type lit1");
    Zinc_expect_utf8_char_str(lit1->c, lit1->num, "o", "char[4],num lit1");
    Zinc_expect_false(lit1->is_root, "is_root lit1");
    Zinc_expect_false(lit1->is_group, "is_group lit1");

    Cob_ast* lit2 = Cob_ast_get(re.root, 8);
    Zinc_assert_ptr(lit2, "ptr lit2");
    Zinc_expect_int_equal(lit2->type, Cob_ast_type_literal, "type lit2");
    Zinc_expect_utf8_char_str(lit2->c, lit2->num, "u", "char[4],num lit2");
    Zinc_expect_false(lit2->is_root, "is_root lit2");
    Zinc_expect_false(lit2->is_group, "is_group lit2");

    Cob_ast* lit3 = Cob_ast_get(re.root, 9);
    Zinc_assert_ptr(lit3, "ptr lit3");
    Zinc_expect_int_equal(lit3->type, Cob_ast_type_literal, "type lit3");
    Zinc_expect_utf8_char_str(lit3->c, lit3->num, "r", "char[4],num lit3");
    Zinc_expect_false(lit3->is_root, "is_root lit3");
    Zinc_expect_false(lit3->is_group, "is_group lit3");

    Cob_ast* lit4 = Cob_ast_get(re.root, 10);
    Zinc_assert_ptr(lit4, "ptr lit4");
    Zinc_expect_int_equal(lit4->type, Cob_ast_type_literal, "type lit4");
    Zinc_expect_utf8_char_str(lit4->c, lit4->num, "c", "char[4],num lit4");
    Zinc_expect_false(lit4->is_root, "is_root lit4");
    Zinc_expect_false(lit4->is_group, "is_group lit4");

    Cob_ast* lit5 = Cob_ast_get(re.root, 11);
    Zinc_assert_ptr(lit5, "ptr lit5");
    Zinc_expect_int_equal(lit5->type, Cob_ast_type_literal, "type lit5");
    Zinc_expect_utf8_char_str(lit5->c, lit5->num, "e", "char[4],num lit5");
    Zinc_expect_false(lit5->is_root, "is_root lit5");
    Zinc_expect_false(lit5->is_group, "is_group lit5");

    Cob_ast* colon2 = Cob_ast_get(re.root, 12);
    Zinc_assert_ptr(colon2, "ptr colon2");
    Zinc_expect_int_equal(colon2->type, Cob_ast_type_literal, "type colon2");
    Zinc_expect_utf8_char_str(colon2->c, colon2->num, ":", "char[4],num colon2");
    Zinc_expect_false(colon2->is_root, "is_root colon2");
    Zinc_expect_false(colon2->is_group, "is_group colon2");

    Cob_ast* group2 = Cob_ast_get(re.root, 13);
    Zinc_assert_ptr(group2, "ptr group2");
    Zinc_expect_int_equal(group2->type, Cob_ast_type_group, "type group2");
    Zinc_expect_false(group2->is_root, "is_root group2");
    Zinc_expect_true(group2->is_group, "is_group group2");
    Zinc_expect_size_t_equal(group2->group, 3, "group group2");

    Cob_ast* pc1 = Cob_ast_get(group2, 0);
    Zinc_assert_ptr(pc1, "ptr pc1");
    Zinc_expect_int_equal(pc1->type, Cob_ast_type_positive_closure, "type pc1");
    Zinc_expect_false(pc1->is_root, "is_root pc1");
    Zinc_expect_false(pc1->is_group, "is_group pc1");

    Cob_ast* wc0 = Cob_ast_get(pc1, 0);
    Zinc_assert_ptr(wc0, "ptr wc0");
    Zinc_expect_int_equal(wc0->type, Cob_ast_type_wildcard, "type wc0");
    Zinc_expect_false(wc0->is_root, "is_root wc0");
    Zinc_expect_false(wc0->is_group, "is_group wc0");

    Cob_re_destroy(&re);
}

void test_compile_coverage_line2()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("\\s*(\\-):\\s*(\\d+):(Source:)?(.+)");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 5, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_concat, "type root");
    Zinc_expect_true(re.root->is_root, "is_root root");
    Zinc_expect_false(re.root->is_group, "is_group root");

    Cob_ast* space0 = Cob_ast_get(re.root, 0);
    Zinc_assert_ptr(space0, "ptr space0");
    Zinc_expect_int_equal(space0->type, Cob_ast_type_closure, "type space0");
    Zinc_expect_false(space0->is_root, "is_root space0");
    Zinc_expect_false(space0->is_group, "is_group space0");

    Cob_ast* space00 = Cob_ast_get(space0, 0);
    Zinc_assert_ptr(space00, "ptr space00");
    Zinc_expect_int_equal(space00->type, Cob_ast_type_character_type_space, "type space00");
    Zinc_expect_false(space00->is_root, "is_root space00");
    Zinc_expect_false(space00->is_group, "is_group space00");

    Cob_ast* group0 = Cob_ast_get(re.root, 1);
    Zinc_assert_ptr(group0, "ptr group0");
    Zinc_expect_int_equal(group0->type, Cob_ast_type_group, "type group0");
    Zinc_expect_false(group0->is_root, "is_root group0");
    Zinc_expect_true(group0->is_group, "is_group group0");
    Zinc_expect_size_t_equal(group0->group, 1, "group group0");

    Cob_ast* escape0 = Cob_ast_get(group0, 0);
    Zinc_assert_ptr(escape0, "ptr escape0");
    Zinc_expect_int_equal(escape0->type, Cob_ast_type_escape, "type escape0");
    Zinc_expect_utf8_char_str(escape0->c, escape0->num, "-", "char[4],num escape0");
    Zinc_expect_false(escape0->is_root, "is_root escape0");
    Zinc_expect_false(escape0->is_group, "is_group escape0");

    Cob_ast* colon0 = Cob_ast_get(re.root, 2);
    Zinc_assert_ptr(colon0, "ptr colon0");
    Zinc_expect_int_equal(colon0->type, Cob_ast_type_literal, "type colon0");
    Zinc_expect_utf8_char_str(colon0->c, colon0->num, ":", "char[4],num colon0");
    Zinc_expect_false(colon0->is_root, "is_root colon0");
    Zinc_expect_false(colon0->is_group, "is_group colon0");

    Cob_ast* space1 = Cob_ast_get(re.root, 3);
    Zinc_assert_ptr(space1, "ptr space1");
    Zinc_expect_int_equal(space1->type, Cob_ast_type_closure, "type space1");
    Zinc_expect_false(space1->is_root, "is_root space1");
    Zinc_expect_false(space1->is_group, "is_group space1");

    Cob_ast* space10 = Cob_ast_get(space1, 0);
    Zinc_assert_ptr(space10, "ptr space10");
    Zinc_expect_int_equal(space10->type, Cob_ast_type_character_type_space, "type space10");
    Zinc_expect_false(space10->is_root, "is_root space10");
    Zinc_expect_false(space10->is_group, "is_group space10");

    Cob_ast* group1 = Cob_ast_get(re.root, 4);
    Zinc_assert_ptr(group1, "ptr group1");
    Zinc_expect_int_equal(group1->type, Cob_ast_type_group, "type group1");
    Zinc_expect_false(group1->is_root, "is_root group1");
    Zinc_expect_true(group1->is_group, "is_group group1");
    Zinc_expect_size_t_equal(group1->group, 2, "group group1");

    Cob_ast* digit0 = Cob_ast_get(group1, 0);
    Zinc_assert_ptr(digit0, "ptr digit0");
    Zinc_expect_int_equal(digit0->type, Cob_ast_type_positive_closure, "type digit0");
    Zinc_expect_false(digit0->is_root, "is_root digit0");
    Zinc_expect_false(digit0->is_group, "is_group digit0");

    Cob_ast* digit01 = Cob_ast_get(digit0, 0);
    Zinc_assert_ptr(digit01, "ptr digit01");
    Zinc_expect_int_equal(digit01->type, Cob_ast_type_character_type_digit, "type digit01");
    Zinc_expect_false(digit01->is_root, "is_root digit01");
    Zinc_expect_false(digit01->is_group, "is_group digit01");

    Cob_ast* colon1 = Cob_ast_get(re.root, 5);
    Zinc_assert_ptr(colon1, "ptr colon1");
    Zinc_expect_int_equal(colon1->type, Cob_ast_type_literal, "type colon1");
    Zinc_expect_utf8_char_str(colon1->c, colon1->num, ":", "char[4],num colon1");
    Zinc_expect_false(colon1->is_root, "is_root colon1");
    Zinc_expect_false(colon1->is_group, "is_group colon1");

    Cob_ast* option0 = Cob_ast_get(re.root, 6);
    Zinc_assert_ptr(option0, "ptr option0");
    Zinc_expect_int_equal(option0->type, Cob_ast_type_option, "type option0");
    Zinc_expect_false(option0->is_root, "is_root option0");
    Zinc_expect_false(option0->is_group, "is_group option0");

    Cob_ast* group2 = Cob_ast_get(option0, 0);
    Zinc_assert_ptr(group2, "ptr group2");
    Zinc_expect_int_equal(group2->type, Cob_ast_type_group, "type group2");
    Zinc_expect_false(group2->is_root, "is_root group2");
    Zinc_expect_true(group2->is_group, "is_group group2");
    Zinc_expect_size_t_equal(group2->group, 3, "group group2");

    Cob_ast* concat1 = Cob_ast_get(group2, 0);
    Zinc_assert_ptr(concat1, "ptr concat1");
    Zinc_expect_int_equal(concat1->type, Cob_ast_type_concat, "type concat1");

    Cob_ast* lit0 = Cob_ast_get(concat1, 0);
    Zinc_assert_ptr(lit0, "ptr lit0");
    Zinc_expect_int_equal(lit0->type, Cob_ast_type_literal, "type lit0");
    Zinc_expect_utf8_char_str(lit0->c, lit0->num, "S", "char[4],num lit0");
    Zinc_expect_false(lit0->is_root, "is_root lit0");
    Zinc_expect_false(lit0->is_group, "is_group lit0");

    Cob_ast* lit1 = Cob_ast_get(concat1, 1);
    Zinc_assert_ptr(lit1, "ptr lit1");
    Zinc_expect_int_equal(lit1->type, Cob_ast_type_literal, "type lit1");
    Zinc_expect_utf8_char_str(lit1->c, lit1->num, "o", "char[4],num lit1");
    Zinc_expect_false(lit1->is_root, "is_root lit1");
    Zinc_expect_false(lit1->is_group, "is_group lit1");

    Cob_ast* lit2 = Cob_ast_get(concat1, 2);
    Zinc_assert_ptr(lit2, "ptr lit2");
    Zinc_expect_int_equal(lit2->type, Cob_ast_type_literal, "type lit2");
    Zinc_expect_utf8_char_str(lit2->c, lit2->num, "u", "char[4],num lit2");
    Zinc_expect_false(lit2->is_root, "is_root lit2");
    Zinc_expect_false(lit2->is_group, "is_group lit2");

    Cob_ast* lit3 = Cob_ast_get(concat1, 3);
    Zinc_assert_ptr(lit3, "ptr lit3");
    Zinc_expect_int_equal(lit3->type, Cob_ast_type_literal, "type lit3");
    Zinc_expect_utf8_char_str(lit3->c, lit3->num, "r", "char[4],num lit3");
    Zinc_expect_false(lit3->is_root, "is_root lit3");
    Zinc_expect_false(lit3->is_group, "is_group lit3");

    Cob_ast* lit4 = Cob_ast_get(concat1, 4);
    Zinc_assert_ptr(lit4, "ptr lit4");
    Zinc_expect_int_equal(lit4->type, Cob_ast_type_literal, "type lit4");
    Zinc_expect_utf8_char_str(lit4->c, lit4->num, "c", "char[4],num lit4");
    Zinc_expect_false(lit4->is_root, "is_root lit4");
    Zinc_expect_false(lit4->is_group, "is_group lit4");

    Cob_ast* lit5 = Cob_ast_get(concat1, 5);
    Zinc_assert_ptr(lit5, "ptr lit5");
    Zinc_expect_int_equal(lit5->type, Cob_ast_type_literal, "type lit5");
    Zinc_expect_utf8_char_str(lit5->c, lit5->num, "e", "char[4],num lit5");
    Zinc_expect_false(lit5->is_root, "is_root lit5");
    Zinc_expect_false(lit5->is_group, "is_group lit5");

    Cob_ast* colon2 = Cob_ast_get(concat1, 6);
    Zinc_assert_ptr(colon2, "ptr colon2");
    Zinc_expect_int_equal(colon2->type, Cob_ast_type_literal, "type colon2");
    Zinc_expect_utf8_char_str(colon2->c, colon2->num, ":", "char[4],num colon2");
    Zinc_expect_false(colon2->is_root, "is_root colon2");
    Zinc_expect_false(colon2->is_group, "is_group colon2");

    Cob_ast* group3 = Cob_ast_get(re.root, 7);
    Zinc_assert_ptr(group3, "ptr group3");
    Zinc_expect_int_equal(group3->type, Cob_ast_type_group, "type group3");
    Zinc_expect_false(group3->is_root, "is_root group3");
    Zinc_expect_true(group3->is_group, "is_group group3");
    Zinc_expect_size_t_equal(group3->group, 4, "group group3");

    Cob_ast* pc1 = Cob_ast_get(group3, 0);
    Zinc_assert_ptr(pc1, "ptr pc1");
    Zinc_expect_int_equal(pc1->type, Cob_ast_type_positive_closure, "type pc1");
    Zinc_expect_false(pc1->is_root, "is_root pc1");
    Zinc_expect_false(pc1->is_group, "is_group pc1");

    Cob_ast* wc0 = Cob_ast_get(pc1, 0);
    Zinc_assert_ptr(wc0, "ptr wc0");
    Zinc_expect_int_equal(wc0->type, Cob_ast_type_wildcard, "type wc0");
    Zinc_expect_false(wc0->is_root, "is_root wc0");
    Zinc_expect_false(wc0->is_group, "is_group wc0");

    Cob_re_destroy(&re);
}

void test_compile_number()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("1234");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 1, "group_count");

    Zinc_assert_ptr(re.root, "ptr root");
    Zinc_expect_int_equal(re.root->type, Cob_ast_type_concat, "type root");

    Cob_ast* one = Cob_ast_get(re.root, 0);
    Zinc_expect_int_equal(one->type, Cob_ast_type_literal, "type one");
    Zinc_expect_strcmp(one->c, "1", "c one");

    Cob_ast* two = Cob_ast_get(re.root, 1);
    Zinc_expect_int_equal(two->type, Cob_ast_type_literal, "type two");
    Zinc_expect_strcmp(two->c, "2", "c two");

    Cob_ast* three = Cob_ast_get(re.root, 2);
    Zinc_expect_int_equal(three->type, Cob_ast_type_literal, "type three");
    Zinc_expect_strcmp(three->c, "3", "c three");

    Cob_ast* four = Cob_ast_get(re.root, 3);
    Zinc_expect_int_equal(four->type, Cob_ast_type_literal, "type four");
    Zinc_expect_strcmp(four->c, "4", "c four");

    Cob_re_destroy(&re);
}

void test_compile_missing_group()
{
    Zinc_test_name(__func__);

    Cob_re re = Cob_compile_str("(c|xyx()|a)");

    Zinc_expect_no_errors(re.errors);
    Zinc_expect_no_errors(re.errors);
    Zinc_expect_size_t_equal(re.group_count, 3, "group_count");
    Cob_re_destroy(&re);
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

    test_compile_coverage_line();
    test_compile_coverage_line2();

    test_compile_missing_group();

    test_compile_number();
}