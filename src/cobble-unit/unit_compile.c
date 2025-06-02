#include <cobble/match_tools.h>
#include "cobble/compile_data.h"
#include "cobble/compile.h"
#include "cobble/ast.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void CobUnit_test_compile_empty(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("");

    Zinc_expect_no_errors(test, re.errors);
    if (!Zinc_expect_null(test, re.root, "root")) {
		return Zinc_assert();
	}
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    Cob_re_destroy(&re);
}

void CobUnit_compile_literal(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("a");

    Zinc_expect_no_errors(test, re.errors);
    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_true(test, re.root->is_root, "is_group root");
    Zinc_expect_false(test, re.root->is_group, "is_group root");
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_literal, "literal root");
    Zinc_expect_int_equal(test, re.root->num, 1, "num");
    Zinc_expect_char_equal(test, re.root->c[0], 'a', "c root");

    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    Cob_re_destroy(&re);
}

void CobUnit_compile_union_single(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("a|b");

    Zinc_expect_no_errors(test, re.errors);

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_union, "union root");

    Cob_ast* a = Cob_ast_get(re.root, 0);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->type, Cob_ast_type_literal, "literal a");
    Zinc_expect_int_equal(test, a->num, 1, "num");
    Zinc_expect_char_equal(test, a->c[0], 'a', "c[0] a");

    Cob_ast* b = Cob_ast_get(re.root, 1);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->type, Cob_ast_type_literal, "literal b");
    Zinc_expect_int_equal(test, b->num, 1, "num");
    Zinc_expect_char_equal(test, b->c[0], 'b', "c[0] b");

    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    Cob_re_destroy(&re);
}

void CobUnit_compile_union_single_error(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("a|");

    Zinc_expect_has_errors(test, re.errors);
    Zinc_expect_ptr(test, re.root, "ptr root");
    Zinc_expect_has_errors(test, re.errors);

    struct Zinc_error* e = NULL;
    e = Zinc_assert_source_error(test, re.errors, "expected term after union");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 2, "start pos");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line");
    Zinc_expect_size_t_equal(test, e->loc.col, 3, "col");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 3, "end_pos");

    Cob_re_destroy(&re);
}

void CobUnit_compile_union_double(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("a|b|c");

    Zinc_expect_no_errors(test, re.errors);
    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_union, "union root");

    Cob_ast* a = Cob_ast_get(re.root, 0);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->type, Cob_ast_type_literal, "literal a");
    Zinc_expect_int_equal(test, a->num, 1, "num a");
    Zinc_expect_char_equal(test, a->c[0], 'a', "c a");

    Cob_ast* b = Cob_ast_get(re.root, 1);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->type, Cob_ast_type_literal, "literal b");
    Zinc_expect_int_equal(test, b->num, 1, "num b");
    Zinc_expect_char_equal(test, b->c[0], 'b', "c b");

    Cob_ast* c = Cob_ast_get(re.root, 2);
    if (!Zinc_expect_ptr(test, c, "ptr c")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c->type, Cob_ast_type_literal, "literal c");
    Zinc_expect_int_equal(test, c->num, 1, "num c");
    Zinc_expect_char_equal(test, c->c[0], 'c', "c c");

    Cob_re_destroy(&re);
}

void CobUnit_compile_concat_single(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("ab");

    Zinc_expect_no_errors(test, re.errors);
    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_concat, "concat root");

    Cob_ast* a = Cob_ast_get(re.root, 0);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->type, Cob_ast_type_literal, "literal a");
    Zinc_expect_int_equal(test, a->num, 1, "num a");
    Zinc_expect_char_equal(test, a->c[0], 'a', "c a");

    Cob_ast* b = Cob_ast_get(re.root, 1);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->type, Cob_ast_type_literal, "literal b");
    Zinc_expect_int_equal(test, b->num, 1, "num b");
    Zinc_expect_char_equal(test, b->c[0], 'b', "c b");

    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    Cob_re_destroy(&re);
}

void CobUnit_compile_concat_double(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("abc");

    Zinc_expect_no_errors(test, re.errors);
    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_concat, "concat root");

    Cob_ast* a = Cob_ast_get(re.root, 0);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->type, Cob_ast_type_literal, "literal a");
    Zinc_expect_int_equal(test, a->num, 1, "num a");
    Zinc_expect_char_equal(test, a->c[0], 'a', "c a");

    Cob_ast* b = Cob_ast_get(re.root, 1);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->type, Cob_ast_type_literal, "literal b");
    Zinc_expect_int_equal(test, b->num, 1, "num b");
    Zinc_expect_char_equal(test, b->c[0], 'b', "c b");

    Cob_ast* c = Cob_ast_get(re.root, 2);
    if (!Zinc_expect_ptr(test, c, "ptr c")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c->type, Cob_ast_type_literal, "literal c");
    Zinc_expect_int_equal(test, c->num, 1, "num c");
    Zinc_expect_char_equal(test, c->c[0], 'c', "c c");

    Cob_re_destroy(&re);
}

void CobUnit_compile_union_concat(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("ab|cd");

    Zinc_expect_no_errors(test, re.errors);
    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_union, "union root");

    Cob_ast* ab = Cob_ast_get(re.root, 0);
    if (!Zinc_expect_ptr(test, ab, "ptr ab")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, ab->type, Cob_ast_type_concat, "concat ab");

    Cob_ast* a = Cob_ast_get(ab, 0);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->type, Cob_ast_type_literal, "literal a");
    Zinc_expect_int_equal(test, a->num, 1, "num a");
    Zinc_expect_char_equal(test, a->c[0], 'a', "c a");

    Cob_ast* b = Cob_ast_get(ab, 1);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->type, Cob_ast_type_literal, "literal b");
    Zinc_expect_int_equal(test, b->num, 1, "num b");
    Zinc_expect_char_equal(test, b->c[0], 'b', "c b");

    Cob_ast* _cd = Cob_ast_get(re.root, 1);
    if (!Zinc_expect_ptr(test, _cd, "ptr _cd")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, _cd->type, Cob_ast_type_concat, "concat _cd");

    Cob_ast* c = Cob_ast_get(_cd, 0);
    if (!Zinc_expect_ptr(test, c, "ptr c")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c->type, Cob_ast_type_literal, "literal c");
    Zinc_expect_int_equal(test, c->num, 1, "num c");
    Zinc_expect_char_equal(test, c->c[0], 'c', "c c");

    Cob_ast* d = Cob_ast_get(_cd, 1);
    if (!Zinc_expect_ptr(test, d, "ptr d")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, d->type, Cob_ast_type_literal, "literal d");
    Zinc_expect_int_equal(test, d->num, 1, "num d");
    Zinc_expect_char_equal(test, d->c[0], 'd', "c d");

    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    Cob_re_destroy(&re);
}

void CobUnit_compile_closure(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("a*");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_closure, "closure root");

    Cob_ast* lit = Cob_ast_get(re.root, 0);
    if (!Zinc_expect_ptr(test, lit, "ptr lit")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lit->type, Cob_ast_type_literal, "literal lit");
    Zinc_expect_int_equal(test, lit->num, 1, "num lit");
    Zinc_expect_char_equal(test, lit->c[0], 'a', "c a");

    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    Cob_re_destroy(&re);
}

void CobUnit_compile_concat_closure(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("a*b*");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_concat, "concat root");

    Cob_ast* c0 = Cob_ast_get(re.root, 0);
    if (!Zinc_expect_ptr(test, c0, "ptr c0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c0->type, Cob_ast_type_closure, "closure c0");

    Cob_ast* a = Cob_ast_get(c0, 0);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->type, Cob_ast_type_literal, "literal a");
    Zinc_expect_int_equal(test, a->num, 1, "num a");
    Zinc_expect_char_equal(test, a->c[0], 'a', "c a");

    Cob_ast* c1 = Cob_ast_get(re.root, 1);
    if (!Zinc_expect_ptr(test, c1, "ptr c1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c1->type, Cob_ast_type_closure, "closure c1");

    Cob_ast* b = Cob_ast_get(c1, 0);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->type, Cob_ast_type_literal, "literal b");
    Zinc_expect_int_equal(test, b->num, 1, "num b");
    Zinc_expect_char_equal(test, b->c[0], 'b', "c b");


    Cob_re_destroy(&re);
}

void CobUnit_compile_union_closure(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("a*|b*");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_union, "concat root");

    Cob_ast* c0 = Cob_ast_get(re.root, 0);
    if (!Zinc_expect_ptr(test, c0, "ptr c0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c0->type, Cob_ast_type_closure, "closure c0");

    Cob_ast* a = Cob_ast_get(c0, 0);
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->type, Cob_ast_type_literal, "literal a");
    Zinc_expect_int_equal(test, a->num, 1, "num a");
    Zinc_expect_char_equal(test, a->c[0], 'a', "c a");

    Cob_ast* c1 = Cob_ast_get(re.root, 1);
    if (!Zinc_expect_ptr(test, c1, "ptr c1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c1->type, Cob_ast_type_closure, "closure c1");

    Cob_ast* b = Cob_ast_get(c1, 0);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->type, Cob_ast_type_literal, "literal b");
    Zinc_expect_int_equal(test, b->num, 1, "num b");
    Zinc_expect_char_equal(test, b->c[0], 'b', "c b");

    Cob_re_destroy(&re);
}

void CobUnit_compile_positive_closure(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("a+");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_positive_closure, "closure root");

    Cob_ast* lit = Cob_ast_get(re.root, 0);
    if (!Zinc_expect_ptr(test, lit, "ptr lit")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lit->type, Cob_ast_type_literal, "literal lit");
    Zinc_expect_int_equal(test, lit->num, 1, "num lit");
    Zinc_expect_char_equal(test, lit->c[0], 'a', "c a");

    Cob_re_destroy(&re);
}

void CobUnit_compile_repeat(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("a{5}");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_repeat, "repeat root");

    Cob_ast* lit = Cob_ast_get(re.root, 0);
    if (!Zinc_expect_ptr(test, lit, "ptr lit")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lit->type, Cob_ast_type_literal, "literal lit");
    Zinc_expect_int_equal(test, lit->num, 1, "num lit");
    Zinc_expect_char_equal(test, lit->c[0], 'a', "c a");

    Cob_ast* num = Cob_ast_get(re.root, 1);
    if (!Zinc_expect_ptr(test, num, "ptr num")) {
		return Zinc_assert();
	}
    Zinc_expect_u_long_equal(test, num->num_value, 5, "num1 5 root");

    Cob_re_destroy(&re);
}

void CobUnit_compile_repeat_num_error(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("a{x}");

    Zinc_expect_has_errors(test, re.errors);
    Zinc_expect_has_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    struct Zinc_error* e = Zinc_assert_source_error(test, re.errors, "expected digit");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 2, "start_pos");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line");
    Zinc_expect_size_t_equal(test, e->loc.col, 3, "col");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 3, "end_pos");

    Cob_re_destroy(&re);
}

void CobUnit_compile_repeat_range(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("a{5,10}");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_repeat_range, "repeat root");

    Cob_ast* lit = Cob_ast_get(re.root, 0);
    if (!Zinc_expect_ptr(test, lit, "ptr lit")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lit->type, Cob_ast_type_literal, "literal lit");
    Zinc_expect_char_equal(test, lit->c[0], 'a', "c a");

    Cob_ast* num = Cob_ast_get(re.root, 1);
    if (!Zinc_expect_ptr(test, num, "ptr num")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, num->type, Cob_ast_type_number, "type num");
    Zinc_expect_u_long_equal(test, num->num_value, 5, "num num");

    Cob_ast* num2 = Cob_ast_get(re.root, 2);
    if (!Zinc_expect_ptr(test, num2, "ptr num2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, num2->type, Cob_ast_type_number, "type num2");
    Zinc_expect_u_long_equal(test, num2->num_value, 10, "num num2");

    Cob_re_destroy(&re);
}

void CobUnit_compile_repeat_range_num_error(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("a{1,x}");

    Zinc_expect_has_errors(test, re.errors);
    Zinc_expect_has_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    struct Zinc_error* e = Zinc_assert_source_error(test, re.errors, "expected digit");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 4, "byte_pos");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line");
    Zinc_expect_size_t_equal(test, e->loc.col, 5, "col");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 5, "end_pos");

    Cob_re_destroy(&re);
}

void CobUnit_compile_group_concat(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("a(b|c)d");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 2, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_concat, "concat root");

    Cob_ast* lit = Cob_ast_get(re.root, 0);
    if (!Zinc_expect_ptr(test, lit, "ptr lit")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lit->type, Cob_ast_type_literal, "literal lit");
    Zinc_expect_int_equal(test, lit->num, 1, "num lit");
    Zinc_expect_char_equal(test, lit->c[0], 'a', "c lit");

    Cob_ast* gr = Cob_ast_get(re.root, 1);
    if (!Zinc_expect_ptr(test, gr, "ptr gr")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, gr->type, Cob_ast_type_group, "group gr");
    Zinc_expect_true(test, gr->is_group, "is group gr");
    Zinc_expect_size_t_equal(test, gr->group, 1, "group gr");

    Cob_ast* un = Cob_ast_get(gr, 0);
    if (!Zinc_expect_ptr(test, un, "ptr group")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, un->type, Cob_ast_type_union, "union un");

    Cob_ast* b = Cob_ast_get(un, 0);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->type, Cob_ast_type_literal, "literal b");
    Zinc_expect_int_equal(test, b->num, 1, "num b");
    Zinc_expect_char_equal(test, b->c[0], 'b', "c b");

    Cob_ast* c = Cob_ast_get(un, 1);
    if (!Zinc_expect_ptr(test, c, "ptr c")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c->type, Cob_ast_type_literal, "literal c");
    Zinc_expect_int_equal(test, c->num, 1, "num c");
    Zinc_expect_char_equal(test, c->c[0], 'c', "c c");

    Cob_ast* d = Cob_ast_get(re.root, 2);
    if (!Zinc_expect_ptr(test, d, "ptr d")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, d->type, Cob_ast_type_literal, "literal d");
    Zinc_expect_char_equal(test, d->c[0], 'd', "c d");

    Cob_re_destroy(&re);
}

void CobUnit_compile_group_empty(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("a()b");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 2, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_concat, "concat root");

    Cob_ast* lit = Cob_ast_get(re.root, 0);
    if (!Zinc_expect_ptr(test, lit, "ptr lit")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lit->type, Cob_ast_type_literal, "literal lit");
    Zinc_expect_int_equal(test, lit->num, 1, "num lit");
    Zinc_expect_char_equal(test, lit->c[0], 'a', "c a");
    Zinc_expect_false(test, lit->is_group, "is group lit");
    Zinc_expect_size_t_equal(test, lit->group, 0, "group lit");

    Cob_ast* gr = Cob_ast_get(re.root, 1);
    if (!Zinc_expect_ptr(test, gr, "ptr gr")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, gr->type, Cob_ast_type_group, "group gr");
    Zinc_expect_true(test, gr->is_group, "is group gr");
    Zinc_expect_size_t_equal(test, gr->group, 1, "group gr");

    Cob_ast* b = Cob_ast_get(re.root, 2);
    if (!Zinc_expect_ptr(test, b, "ptr b")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, b->type, Cob_ast_type_literal, "literal b");
    Zinc_expect_int_equal(test, b->num, 1, "num b");
    Zinc_expect_char_equal(test, b->c[0], 'b', "c b");

    Cob_re_destroy(&re);
}

void CobUnit_compile_option(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("a?");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_option, "option root");

    Cob_ast* lit = Cob_ast_get(re.root, 0);
    if (!Zinc_expect_ptr(test, lit, "ptr lit")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lit->type, Cob_ast_type_literal, "literal lit");
    Zinc_expect_int_equal(test, lit->num, 1, "num lit");
    Zinc_expect_char_equal(test, lit->c[0], 'a', "c a");

    Cob_re_destroy(&re);
}

void CobUnit_compile_wildcard(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str(".");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_wildcard, "type root");

    Cob_re_destroy(&re);
}

void CobUnit_compile_begin(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("^");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_begin, "type root");

    Cob_re_destroy(&re);
}

void CobUnit_compile_end(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("$");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_end, "type root");

    Cob_re_destroy(&re);
}

void CobUnit_compile_escape_backslash(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("\\\\");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_escape, "type root");
    Zinc_expect_int_equal(test, re.root->num, 1, "num root");
    Zinc_expect_char_equal(test, re.root->c[0], '\\', "c root");

    Cob_re_destroy(&re);
}

void CobUnit_compile_escape_asterisk(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("\\*");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_escape, "type root");
    Zinc_expect_int_equal(test, re.root->num, 1, "num root");
    Zinc_expect_char_equal(test, re.root->c[0], '*', "c root");

    Cob_re_destroy(&re);
}

void CobUnit_compile_character_class(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("[abc]");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_character_class, "type root");

    Cob_ast* c0 = Cob_ast_get(re.root, 0);
    if (!Zinc_expect_ptr(test, c0, "ptr c0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c0->type, Cob_ast_type_literal, "type c0");
    Zinc_expect_int_equal(test, c0->num, 1, "num c0");
    Zinc_expect_char_equal(test, c0->c[0], 'a', "c c0");

    Cob_ast* c1 = Cob_ast_get(re.root, 1);
    if (!Zinc_expect_ptr(test, c1, "ptr c1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c1->type, Cob_ast_type_literal, "type c1");
    Zinc_expect_int_equal(test, c1->num, 1, "num c1");
    Zinc_expect_char_equal(test, c1->c[0], 'b', "c c1");

    Cob_ast* c2 = Cob_ast_get(re.root, 2);
    if (!Zinc_expect_ptr(test, c2, "ptr c2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c2->type, Cob_ast_type_literal, "type c2");
    Zinc_expect_int_equal(test, c2->num, 1, "num c2");
    Zinc_expect_char_equal(test, c2->c[0], 'c', "c c2");

    Cob_re_destroy(&re);
}

void CobUnit_compile_character_class_wildcard_error(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("[.]");

    Zinc_expect_has_errors(test, re.errors);
    Zinc_expect_has_errors(test, re.errors);
    Zinc_expect_source_error(test, re.errors, "unexpected wildcard");

    Cob_re_destroy(&re);
}

void CobUnit_compile_character_class_begin_error(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("[a^]");

    Zinc_expect_has_errors(test, re.errors);
    Zinc_expect_has_errors(test, re.errors);
    Zinc_expect_source_error(test, re.errors, "unexpected begin");

    Cob_re_destroy(&re);
}

void CobUnit_compile_character_class_end_error(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("[$]");

    Zinc_expect_has_errors(test, re.errors);
    Zinc_expect_has_errors(test, re.errors);
    Zinc_expect_source_error(test, re.errors, "unexpected end");

    Cob_re_destroy(&re);
}

void CobUnit_compile_character_range(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("[a-z]");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_character_class, "type root");

    Cob_ast* c0 = Cob_ast_get(re.root, 0);
    if (!Zinc_expect_ptr(test, c0, "ptr c0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c0->type, Cob_ast_type_character_range, "type c0");
    Zinc_expect_int_equal(test, c0->num, 0, "num c0");

    Cob_ast* c00 = Cob_ast_get(c0, 0);
    if (!Zinc_expect_ptr(test, c00, "ptr c00")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c00->type, Cob_ast_type_literal, "type c00");
    Zinc_expect_int_equal(test, c00->num, 1, "num c00");
    Zinc_expect_char_equal(test, c00->c[0], 'a', "c c00");

    Cob_ast* c01 = Cob_ast_get(c0, 1);
    if (!Zinc_expect_ptr(test, c01, "ptr c01")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, c01->type, Cob_ast_type_literal, "type c01");
    Zinc_expect_int_equal(test, c01->num, 1, "num c01");
    Zinc_expect_char_equal(test, c01->c[0], 'z', "c c01");

    Cob_re_destroy(&re);
}

void CobUnit_compile_character_range_error(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("[a-]");

    Zinc_expect_has_errors(test, re.errors);
    Zinc_expect_has_errors(test, re.errors);
    Zinc_expect_source_error(test, re.errors, "expected end character in character range");

    Cob_re_destroy(&re);
}

void CobUnit_compile_character_range_non_ascii_error_left(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("[á-z]");

    Zinc_expect_has_errors(test, re.errors);
    Zinc_expect_has_errors(test, re.errors);
    Zinc_expect_source_error(test, re.errors, "character range must use ascii characters");

    Cob_re_destroy(&re);
}

void CobUnit_compile_character_range_non_ascii_error_right(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("[a-ź]");

    Zinc_expect_has_errors(test, re.errors);
    Zinc_expect_has_errors(test, re.errors);
    Zinc_expect_source_error(test, re.errors, "character range must use ascii characters");

    Cob_re_destroy(&re);
}

void CobUnit_compile_character_type_word(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("\\w");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_character_type_word, "type root");

    Cob_re_destroy(&re);
}

void CobUnit_compile_character_type_word_opposite(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("\\W");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_character_type_word_opposite, "type root");

    Cob_re_destroy(&re);
}

void CobUnit_compile_character_type_digit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("\\d");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_character_type_digit, "type root");

    Cob_re_destroy(&re);
}

void CobUnit_compile_character_type_digit_opposite(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("\\D");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_character_type_digit_opposite, "type root");

    Cob_re_destroy(&re);
}

void CobUnit_compile_character_type_space(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("\\s");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_character_type_space, "type root");

    Cob_re_destroy(&re);
}

void CobUnit_compile_character_type_space_opposite(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("\\S");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_character_type_space_opposite, "type root");

    Cob_re_destroy(&re);
}

void CobUnit_compile_character_type_newline_opposite(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("\\N");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_character_type_newline_opposite, "type root");

    Cob_re_destroy(&re);
}

void CobUnit_compile_coverage_line(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("\\s*(\\-):\\s*(\\d+):Source:(.+)");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 4, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_concat, "type root");
    Zinc_expect_true(test, re.root->is_root, "is_root root");
    Zinc_expect_false(test, re.root->is_group, "is_group root");

    Cob_ast* space0 = Cob_ast_get(re.root, 0);
    if (!Zinc_expect_ptr(test, space0, "ptr space0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, space0->type, Cob_ast_type_closure, "type space0");
    Zinc_expect_false(test, space0->is_root, "is_root space0");
    Zinc_expect_false(test, space0->is_group, "is_group space0");

    Cob_ast* space00 = Cob_ast_get(space0, 0);
    if (!Zinc_expect_ptr(test, space00, "ptr space00")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, space00->type, Cob_ast_type_character_type_space, "type space00");
    Zinc_expect_false(test, space00->is_root, "is_root space00");
    Zinc_expect_false(test, space00->is_group, "is_group space00");

    Cob_ast* group0 = Cob_ast_get(re.root, 1);
    if (!Zinc_expect_ptr(test, group0, "ptr group0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, group0->type, Cob_ast_type_group, "type group0");
    Zinc_expect_false(test, group0->is_root, "is_root group0");
    Zinc_expect_true(test, group0->is_group, "is_group group0");
    Zinc_expect_size_t_equal(test, group0->group, 1, "group group0");

    Cob_ast* escape0 = Cob_ast_get(group0, 0);
    if (!Zinc_expect_ptr(test, escape0, "ptr escape0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, escape0->type, Cob_ast_type_escape, "type escape0");
    Zinc_expect_utf8_char_str(test, escape0->c, escape0->num, "-", "char[4],num escape0");
    Zinc_expect_false(test, escape0->is_root, "is_root escape0");
    Zinc_expect_false(test, escape0->is_group, "is_group escape0");

    Cob_ast* colon0 = Cob_ast_get(re.root, 2);
    if (!Zinc_expect_ptr(test, colon0, "ptr colon0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, colon0->type, Cob_ast_type_literal, "type colon0");
    Zinc_expect_utf8_char_str(test, colon0->c, colon0->num, ":", "char[4],num colon0");
    Zinc_expect_false(test, colon0->is_root, "is_root colon0");
    Zinc_expect_false(test, colon0->is_group, "is_group colon0");

    Cob_ast* space1 = Cob_ast_get(re.root, 3);
    if (!Zinc_expect_ptr(test, space1, "ptr space1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, space1->type, Cob_ast_type_closure, "type space1");
    Zinc_expect_false(test, space1->is_root, "is_root space1");
    Zinc_expect_false(test, space1->is_group, "is_group space1");

    Cob_ast* space10 = Cob_ast_get(space1, 0);
    if (!Zinc_expect_ptr(test, space10, "ptr space10")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, space10->type, Cob_ast_type_character_type_space, "type space10");
    Zinc_expect_false(test, space10->is_root, "is_root space10");
    Zinc_expect_false(test, space10->is_group, "is_group space10");

    Cob_ast* group1 = Cob_ast_get(re.root, 4);
    if (!Zinc_expect_ptr(test, group1, "ptr group1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, group1->type, Cob_ast_type_group, "type group1");
    Zinc_expect_false(test, group1->is_root, "is_root group1");
    Zinc_expect_true(test, group1->is_group, "is_group group1");
    Zinc_expect_size_t_equal(test, group1->group, 2, "group group1");

    Cob_ast* digit0 = Cob_ast_get(group1, 0);
    if (!Zinc_expect_ptr(test, digit0, "ptr digit0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, digit0->type, Cob_ast_type_positive_closure, "type digit0");
    Zinc_expect_false(test, digit0->is_root, "is_root digit0");
    Zinc_expect_false(test, digit0->is_group, "is_group digit0");

    Cob_ast* digit01 = Cob_ast_get(digit0, 0);
    if (!Zinc_expect_ptr(test, digit01, "ptr digit01")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, digit01->type, Cob_ast_type_character_type_digit, "type digit01");
    Zinc_expect_false(test, digit01->is_root, "is_root digit01");
    Zinc_expect_false(test, digit01->is_group, "is_group digit01");

    Cob_ast* colon1 = Cob_ast_get(re.root, 5);
    if (!Zinc_expect_ptr(test, colon1, "ptr colon1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, colon1->type, Cob_ast_type_literal, "type colon1");
    Zinc_expect_utf8_char_str(test, colon1->c, colon1->num, ":", "char[4],num colon1");
    Zinc_expect_false(test, colon1->is_root, "is_root colon1");
    Zinc_expect_false(test, colon1->is_group, "is_group colon1");

    Cob_ast* lit0 = Cob_ast_get(re.root, 6);
    if (!Zinc_expect_ptr(test, lit0, "ptr lit0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lit0->type, Cob_ast_type_literal, "type lit0");
    Zinc_expect_utf8_char_str(test, lit0->c, lit0->num, "S", "char[4],num lit0");
    Zinc_expect_false(test, lit0->is_root, "is_root lit0");
    Zinc_expect_false(test, lit0->is_group, "is_group lit0");

    Cob_ast* lit1 = Cob_ast_get(re.root, 7);
    if (!Zinc_expect_ptr(test, lit1, "ptr lit1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lit1->type, Cob_ast_type_literal, "type lit1");
    Zinc_expect_utf8_char_str(test, lit1->c, lit1->num, "o", "char[4],num lit1");
    Zinc_expect_false(test, lit1->is_root, "is_root lit1");
    Zinc_expect_false(test, lit1->is_group, "is_group lit1");

    Cob_ast* lit2 = Cob_ast_get(re.root, 8);
    if (!Zinc_expect_ptr(test, lit2, "ptr lit2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lit2->type, Cob_ast_type_literal, "type lit2");
    Zinc_expect_utf8_char_str(test, lit2->c, lit2->num, "u", "char[4],num lit2");
    Zinc_expect_false(test, lit2->is_root, "is_root lit2");
    Zinc_expect_false(test, lit2->is_group, "is_group lit2");

    Cob_ast* lit3 = Cob_ast_get(re.root, 9);
    if (!Zinc_expect_ptr(test, lit3, "ptr lit3")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lit3->type, Cob_ast_type_literal, "type lit3");
    Zinc_expect_utf8_char_str(test, lit3->c, lit3->num, "r", "char[4],num lit3");
    Zinc_expect_false(test, lit3->is_root, "is_root lit3");
    Zinc_expect_false(test, lit3->is_group, "is_group lit3");

    Cob_ast* lit4 = Cob_ast_get(re.root, 10);
    if (!Zinc_expect_ptr(test, lit4, "ptr lit4")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lit4->type, Cob_ast_type_literal, "type lit4");
    Zinc_expect_utf8_char_str(test, lit4->c, lit4->num, "c", "char[4],num lit4");
    Zinc_expect_false(test, lit4->is_root, "is_root lit4");
    Zinc_expect_false(test, lit4->is_group, "is_group lit4");

    Cob_ast* lit5 = Cob_ast_get(re.root, 11);
    if (!Zinc_expect_ptr(test, lit5, "ptr lit5")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lit5->type, Cob_ast_type_literal, "type lit5");
    Zinc_expect_utf8_char_str(test, lit5->c, lit5->num, "e", "char[4],num lit5");
    Zinc_expect_false(test, lit5->is_root, "is_root lit5");
    Zinc_expect_false(test, lit5->is_group, "is_group lit5");

    Cob_ast* colon2 = Cob_ast_get(re.root, 12);
    if (!Zinc_expect_ptr(test, colon2, "ptr colon2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, colon2->type, Cob_ast_type_literal, "type colon2");
    Zinc_expect_utf8_char_str(test, colon2->c, colon2->num, ":", "char[4],num colon2");
    Zinc_expect_false(test, colon2->is_root, "is_root colon2");
    Zinc_expect_false(test, colon2->is_group, "is_group colon2");

    Cob_ast* group2 = Cob_ast_get(re.root, 13);
    if (!Zinc_expect_ptr(test, group2, "ptr group2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, group2->type, Cob_ast_type_group, "type group2");
    Zinc_expect_false(test, group2->is_root, "is_root group2");
    Zinc_expect_true(test, group2->is_group, "is_group group2");
    Zinc_expect_size_t_equal(test, group2->group, 3, "group group2");

    Cob_ast* pc1 = Cob_ast_get(group2, 0);
    if (!Zinc_expect_ptr(test, pc1, "ptr pc1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, pc1->type, Cob_ast_type_positive_closure, "type pc1");
    Zinc_expect_false(test, pc1->is_root, "is_root pc1");
    Zinc_expect_false(test, pc1->is_group, "is_group pc1");

    Cob_ast* wc0 = Cob_ast_get(pc1, 0);
    if (!Zinc_expect_ptr(test, wc0, "ptr wc0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, wc0->type, Cob_ast_type_wildcard, "type wc0");
    Zinc_expect_false(test, wc0->is_root, "is_root wc0");
    Zinc_expect_false(test, wc0->is_group, "is_group wc0");

    Cob_re_destroy(&re);
}

void CobUnit_compile_coverage_line2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("\\s*(\\-):\\s*(\\d+):(Source:)?(.+)");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 5, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_concat, "type root");
    Zinc_expect_true(test, re.root->is_root, "is_root root");
    Zinc_expect_false(test, re.root->is_group, "is_group root");

    Cob_ast* space0 = Cob_ast_get(re.root, 0);
    if (!Zinc_expect_ptr(test, space0, "ptr space0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, space0->type, Cob_ast_type_closure, "type space0");
    Zinc_expect_false(test, space0->is_root, "is_root space0");
    Zinc_expect_false(test, space0->is_group, "is_group space0");

    Cob_ast* space00 = Cob_ast_get(space0, 0);
    if (!Zinc_expect_ptr(test, space00, "ptr space00")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, space00->type, Cob_ast_type_character_type_space, "type space00");
    Zinc_expect_false(test, space00->is_root, "is_root space00");
    Zinc_expect_false(test, space00->is_group, "is_group space00");

    Cob_ast* group0 = Cob_ast_get(re.root, 1);
    if (!Zinc_expect_ptr(test, group0, "ptr group0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, group0->type, Cob_ast_type_group, "type group0");
    Zinc_expect_false(test, group0->is_root, "is_root group0");
    Zinc_expect_true(test, group0->is_group, "is_group group0");
    Zinc_expect_size_t_equal(test, group0->group, 1, "group group0");

    Cob_ast* escape0 = Cob_ast_get(group0, 0);
    if (!Zinc_expect_ptr(test, escape0, "ptr escape0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, escape0->type, Cob_ast_type_escape, "type escape0");
    Zinc_expect_utf8_char_str(test, escape0->c, escape0->num, "-", "char[4],num escape0");
    Zinc_expect_false(test, escape0->is_root, "is_root escape0");
    Zinc_expect_false(test, escape0->is_group, "is_group escape0");

    Cob_ast* colon0 = Cob_ast_get(re.root, 2);
    if (!Zinc_expect_ptr(test, colon0, "ptr colon0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, colon0->type, Cob_ast_type_literal, "type colon0");
    Zinc_expect_utf8_char_str(test, colon0->c, colon0->num, ":", "char[4],num colon0");
    Zinc_expect_false(test, colon0->is_root, "is_root colon0");
    Zinc_expect_false(test, colon0->is_group, "is_group colon0");

    Cob_ast* space1 = Cob_ast_get(re.root, 3);
    if (!Zinc_expect_ptr(test, space1, "ptr space1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, space1->type, Cob_ast_type_closure, "type space1");
    Zinc_expect_false(test, space1->is_root, "is_root space1");
    Zinc_expect_false(test, space1->is_group, "is_group space1");

    Cob_ast* space10 = Cob_ast_get(space1, 0);
    if (!Zinc_expect_ptr(test, space10, "ptr space10")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, space10->type, Cob_ast_type_character_type_space, "type space10");
    Zinc_expect_false(test, space10->is_root, "is_root space10");
    Zinc_expect_false(test, space10->is_group, "is_group space10");

    Cob_ast* group1 = Cob_ast_get(re.root, 4);
    if (!Zinc_expect_ptr(test, group1, "ptr group1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, group1->type, Cob_ast_type_group, "type group1");
    Zinc_expect_false(test, group1->is_root, "is_root group1");
    Zinc_expect_true(test, group1->is_group, "is_group group1");
    Zinc_expect_size_t_equal(test, group1->group, 2, "group group1");

    Cob_ast* digit0 = Cob_ast_get(group1, 0);
    if (!Zinc_expect_ptr(test, digit0, "ptr digit0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, digit0->type, Cob_ast_type_positive_closure, "type digit0");
    Zinc_expect_false(test, digit0->is_root, "is_root digit0");
    Zinc_expect_false(test, digit0->is_group, "is_group digit0");

    Cob_ast* digit01 = Cob_ast_get(digit0, 0);
    if (!Zinc_expect_ptr(test, digit01, "ptr digit01")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, digit01->type, Cob_ast_type_character_type_digit, "type digit01");
    Zinc_expect_false(test, digit01->is_root, "is_root digit01");
    Zinc_expect_false(test, digit01->is_group, "is_group digit01");

    Cob_ast* colon1 = Cob_ast_get(re.root, 5);
    if (!Zinc_expect_ptr(test, colon1, "ptr colon1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, colon1->type, Cob_ast_type_literal, "type colon1");
    Zinc_expect_utf8_char_str(test, colon1->c, colon1->num, ":", "char[4],num colon1");
    Zinc_expect_false(test, colon1->is_root, "is_root colon1");
    Zinc_expect_false(test, colon1->is_group, "is_group colon1");

    Cob_ast* option0 = Cob_ast_get(re.root, 6);
    if (!Zinc_expect_ptr(test, option0, "ptr option0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, option0->type, Cob_ast_type_option, "type option0");
    Zinc_expect_false(test, option0->is_root, "is_root option0");
    Zinc_expect_false(test, option0->is_group, "is_group option0");

    Cob_ast* group2 = Cob_ast_get(option0, 0);
    if (!Zinc_expect_ptr(test, group2, "ptr group2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, group2->type, Cob_ast_type_group, "type group2");
    Zinc_expect_false(test, group2->is_root, "is_root group2");
    Zinc_expect_true(test, group2->is_group, "is_group group2");
    Zinc_expect_size_t_equal(test, group2->group, 3, "group group2");

    Cob_ast* concat1 = Cob_ast_get(group2, 0);
    if (!Zinc_expect_ptr(test, concat1, "ptr concat1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, concat1->type, Cob_ast_type_concat, "type concat1");

    Cob_ast* lit0 = Cob_ast_get(concat1, 0);
    if (!Zinc_expect_ptr(test, lit0, "ptr lit0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lit0->type, Cob_ast_type_literal, "type lit0");
    Zinc_expect_utf8_char_str(test, lit0->c, lit0->num, "S", "char[4],num lit0");
    Zinc_expect_false(test, lit0->is_root, "is_root lit0");
    Zinc_expect_false(test, lit0->is_group, "is_group lit0");

    Cob_ast* lit1 = Cob_ast_get(concat1, 1);
    if (!Zinc_expect_ptr(test, lit1, "ptr lit1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lit1->type, Cob_ast_type_literal, "type lit1");
    Zinc_expect_utf8_char_str(test, lit1->c, lit1->num, "o", "char[4],num lit1");
    Zinc_expect_false(test, lit1->is_root, "is_root lit1");
    Zinc_expect_false(test, lit1->is_group, "is_group lit1");

    Cob_ast* lit2 = Cob_ast_get(concat1, 2);
    if (!Zinc_expect_ptr(test, lit2, "ptr lit2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lit2->type, Cob_ast_type_literal, "type lit2");
    Zinc_expect_utf8_char_str(test, lit2->c, lit2->num, "u", "char[4],num lit2");
    Zinc_expect_false(test, lit2->is_root, "is_root lit2");
    Zinc_expect_false(test, lit2->is_group, "is_group lit2");

    Cob_ast* lit3 = Cob_ast_get(concat1, 3);
    if (!Zinc_expect_ptr(test, lit3, "ptr lit3")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lit3->type, Cob_ast_type_literal, "type lit3");
    Zinc_expect_utf8_char_str(test, lit3->c, lit3->num, "r", "char[4],num lit3");
    Zinc_expect_false(test, lit3->is_root, "is_root lit3");
    Zinc_expect_false(test, lit3->is_group, "is_group lit3");

    Cob_ast* lit4 = Cob_ast_get(concat1, 4);
    if (!Zinc_expect_ptr(test, lit4, "ptr lit4")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lit4->type, Cob_ast_type_literal, "type lit4");
    Zinc_expect_utf8_char_str(test, lit4->c, lit4->num, "c", "char[4],num lit4");
    Zinc_expect_false(test, lit4->is_root, "is_root lit4");
    Zinc_expect_false(test, lit4->is_group, "is_group lit4");

    Cob_ast* lit5 = Cob_ast_get(concat1, 5);
    if (!Zinc_expect_ptr(test, lit5, "ptr lit5")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, lit5->type, Cob_ast_type_literal, "type lit5");
    Zinc_expect_utf8_char_str(test, lit5->c, lit5->num, "e", "char[4],num lit5");
    Zinc_expect_false(test, lit5->is_root, "is_root lit5");
    Zinc_expect_false(test, lit5->is_group, "is_group lit5");

    Cob_ast* colon2 = Cob_ast_get(concat1, 6);
    if (!Zinc_expect_ptr(test, colon2, "ptr colon2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, colon2->type, Cob_ast_type_literal, "type colon2");
    Zinc_expect_utf8_char_str(test, colon2->c, colon2->num, ":", "char[4],num colon2");
    Zinc_expect_false(test, colon2->is_root, "is_root colon2");
    Zinc_expect_false(test, colon2->is_group, "is_group colon2");

    Cob_ast* group3 = Cob_ast_get(re.root, 7);
    if (!Zinc_expect_ptr(test, group3, "ptr group3")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, group3->type, Cob_ast_type_group, "type group3");
    Zinc_expect_false(test, group3->is_root, "is_root group3");
    Zinc_expect_true(test, group3->is_group, "is_group group3");
    Zinc_expect_size_t_equal(test, group3->group, 4, "group group3");

    Cob_ast* pc1 = Cob_ast_get(group3, 0);
    if (!Zinc_expect_ptr(test, pc1, "ptr pc1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, pc1->type, Cob_ast_type_positive_closure, "type pc1");
    Zinc_expect_false(test, pc1->is_root, "is_root pc1");
    Zinc_expect_false(test, pc1->is_group, "is_group pc1");

    Cob_ast* wc0 = Cob_ast_get(pc1, 0);
    if (!Zinc_expect_ptr(test, wc0, "ptr wc0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, wc0->type, Cob_ast_type_wildcard, "type wc0");
    Zinc_expect_false(test, wc0->is_root, "is_root wc0");
    Zinc_expect_false(test, wc0->is_group, "is_group wc0");

    Cob_re_destroy(&re);
}

void CobUnit_compile_number(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("1234");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 1, "group_count");

    if (!Zinc_expect_ptr(test, re.root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, re.root->type, Cob_ast_type_concat, "type root");

    Cob_ast* one = Cob_ast_get(re.root, 0);
    Zinc_expect_int_equal(test, one->type, Cob_ast_type_literal, "type one");
    Zinc_expect_strcmp(test, one->c, "1", "c one");

    Cob_ast* two = Cob_ast_get(re.root, 1);
    Zinc_expect_int_equal(test, two->type, Cob_ast_type_literal, "type two");
    Zinc_expect_strcmp(test, two->c, "2", "c two");

    Cob_ast* three = Cob_ast_get(re.root, 2);
    Zinc_expect_int_equal(test, three->type, Cob_ast_type_literal, "type three");
    Zinc_expect_strcmp(test, three->c, "3", "c three");

    Cob_ast* four = Cob_ast_get(re.root, 3);
    Zinc_expect_int_equal(test, four->type, Cob_ast_type_literal, "type four");
    Zinc_expect_strcmp(test, four->c, "4", "c four");

    Cob_re_destroy(&re);
}

void CobUnit_compile_missing_group(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_re re = Cob_compile_str("(c|xyx()|a)");

    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_no_errors(test, re.errors);
    Zinc_expect_size_t_equal(test, re.group_count, 3, "group_count");
    Cob_re_destroy(&re);
}

void CobUnit_compile(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, CobUnit_test_compile_empty);
        Zinc_test_register(test, CobUnit_compile_literal);
        Zinc_test_register(test, CobUnit_compile_union_single);
        Zinc_test_register(test, CobUnit_compile_union_single_error);
        Zinc_test_register(test, CobUnit_compile_union_double);
        Zinc_test_register(test, CobUnit_compile_concat_single);
        Zinc_test_register(test, CobUnit_compile_concat_double);
        Zinc_test_register(test, CobUnit_compile_union_concat);
        Zinc_test_register(test, CobUnit_compile_closure);
        Zinc_test_register(test, CobUnit_compile_concat_closure);
        Zinc_test_register(test, CobUnit_compile_union_closure);
        Zinc_test_register(test, CobUnit_compile_positive_closure);
        Zinc_test_register(test, CobUnit_compile_repeat);
        Zinc_test_register(test, CobUnit_compile_repeat_num_error);
        Zinc_test_register(test, CobUnit_compile_repeat_range);
        Zinc_test_register(test, CobUnit_compile_repeat_range_num_error);
        Zinc_test_register(test, CobUnit_compile_group_concat);
        Zinc_test_register(test, CobUnit_compile_group_empty);
        Zinc_test_register(test, CobUnit_compile_option);
        Zinc_test_register(test, CobUnit_compile_wildcard);
        Zinc_test_register(test, CobUnit_compile_begin);
        Zinc_test_register(test, CobUnit_compile_end);
        Zinc_test_register(test, CobUnit_compile_escape_backslash);
        Zinc_test_register(test, CobUnit_compile_escape_asterisk);
        Zinc_test_register(test, CobUnit_compile_character_class);
        Zinc_test_register(test, CobUnit_compile_character_class_wildcard_error);
        Zinc_test_register(test, CobUnit_compile_character_class_begin_error);
        Zinc_test_register(test, CobUnit_compile_character_class_end_error);
        Zinc_test_register(test, CobUnit_compile_character_range);
        Zinc_test_register(test, CobUnit_compile_character_range_error);
        Zinc_test_register(test, CobUnit_compile_character_range_non_ascii_error_left);
        Zinc_test_register(test, CobUnit_compile_character_range_non_ascii_error_right);
        Zinc_test_register(test, CobUnit_compile_character_type_word);
        Zinc_test_register(test, CobUnit_compile_character_type_word_opposite);
        Zinc_test_register(test, CobUnit_compile_character_type_digit);
        Zinc_test_register(test, CobUnit_compile_character_type_digit_opposite);
        Zinc_test_register(test, CobUnit_compile_character_type_space);
        Zinc_test_register(test, CobUnit_compile_character_type_space_opposite);
        Zinc_test_register(test, CobUnit_compile_character_type_newline_opposite);

        Zinc_test_register(test, CobUnit_compile_coverage_line);
        Zinc_test_register(test, CobUnit_compile_coverage_line2);

        Zinc_test_register(test, CobUnit_compile_missing_group);

        Zinc_test_register(test, CobUnit_compile_number);

        return;
    }

    Zinc_test_perform(test);
}