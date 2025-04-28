#include <string.h>
#include <stdlib.h>
#include "zinc/result.h"
#include "zinc/unit_test.h"
#include "zinc/zstring.h"
#include <limits.h>
#include "zinc/test.h"
#include "zinc/unit.h"

void Zinc_unit_buffer_init(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Zinc_string bf;
	Zinc_string_init(&bf);
	Zinc_test_expect_size_t_equal(test, bf.size, 0, "size");
	Zinc_test_expect_size_t_equal(test, bf.buf_size, 0, "buf_size");
	Zinc_test_expect_null(test, bf.buf, "buf");
}

void Zinc_unit_buffer_add_char(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Zinc_string bf;
	Zinc_string_init(&bf);

	Zinc_string_add_char(&bf, 'x');
	Zinc_string_add_char(&bf, 'y');
	Zinc_string_add_char(&bf, 'z');

	Zinc_test_expect_string(test, &bf, "xyz", "str");

	Zinc_string_destroy(&bf);
}

void Zinc_unit_test_buffer_add(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Zinc_string* bf = NULL;
    Zinc_string_create(&bf);

    Zinc_string_add(bf, "abc", 3);
    Zinc_string_add(bf, "123", 3);

    Zinc_test_expect_string(test, bf, "abc123", "bf");

    Zinc_string_destroy(bf);
    free(bf);
}

void test_buffer_expand()
{
    Zinc_test_name(__func__);

    struct Zinc_string* bf = NULL;
    Zinc_string_create(&bf);

    Zinc_string_expand(bf, BUFFER_CHUNK + 10);

    Zinc_expect_size_t_equal(bf->buf_size, BUFFER_CHUNK + 10, "buf_size");

    Zinc_string_destroy(bf);
    free(bf);
}

void test_buffer_finish()
{
	Zinc_test_name(__func__);

	struct Zinc_string bf;
	Zinc_string_init(&bf);

	/* allocate bf{} */
	Zinc_string_add_char(&bf, 'x');
	Zinc_string_add_char(&bf, 'y');
	Zinc_string_add_char(&bf, 'z');
	Zinc_string_finish(&bf);

	Zinc_expect_string(&bf, "xyz", "str");
	Zinc_expect_true(strcmp(bf.buf, "xyz") == 0, "strcmp");

	/* destroy bf{} */
	Zinc_string_destroy(&bf);
}

/* static-output */
/* dynamic-temp bf{} */
void test_buffer_clear()
{
	Zinc_test_name(__func__);

	struct Zinc_string bf;
	Zinc_string_init(&bf);

	/* allocate bf{} */
	Zinc_string_add_char(&bf, 'x');
	Zinc_string_add_char(&bf, 'y');
	Zinc_string_add_char(&bf, 'z');

	Zinc_expect_string(&bf, "xyz", "str");
	Zinc_string_clear(&bf);
	Zinc_expect_string(&bf, "", "clear");

	/* destroy bf{} */
	Zinc_string_destroy(&bf);
}

void test_buffer_reset()
{
    Zinc_test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Zinc_string_add_str(&bf, "hello");

    Zinc_expect_string(&bf, "hello", "hello");
    Zinc_string_reset(&bf);

    Zinc_expect_string(&bf, "", "");

    Zinc_string_destroy(&bf);
}

/* static-output */
/* dynamic-temp bf{} bf2{} */
void test_buffer_copy()
{
	Zinc_test_name(__func__);

	struct Zinc_string bf;
	struct Zinc_string bf2;

	Zinc_string_init(&bf);

	/* allocate bf{} */
	Zinc_string_add_char(&bf, 'x');
	Zinc_string_add_char(&bf, 'y');
	Zinc_string_add_char(&bf, 'z');

	Zinc_expect_string(&bf, "xyz", "str");

	Zinc_string_init(&bf2);

	/* allocate bf2{} */
	Zinc_string_copy(&bf, &bf2);

	Zinc_expect_string(&bf2, "xyz", "copy");

	/* destroy bf{} bf2{} */
	Zinc_string_destroy(&bf);
	Zinc_string_destroy(&bf2);
}

/* static-output */
/* dynamic-temp bf{} a */
void test_buffer_buffer2array()
{
	Zinc_test_name(__func__);

	struct Zinc_string bf;
	char* a;

	Zinc_string_init(&bf);

	/* allocate bf{} */
	Zinc_string_add_char(&bf, 'x');
	Zinc_string_add_char(&bf, 'y');
	Zinc_string_add_char(&bf, 'z');

	Zinc_expect_string(&bf, "xyz", "str");

	/* allocate a */
	Zinc_string_create_str(&bf, &a);

	Zinc_expect_true(strcmp(a, "xyz") == 0, "array");

	/* destroy bf{} a */
	Zinc_string_destroy(&bf);
	free(a);
}

/* static-output */
/* dynamic-temp bf{} */
void test_buffer_array2buffer()
{
	Zinc_test_name(__func__);

	char a[] = "xyz";
	struct Zinc_string bf;

	Zinc_string_init(&bf);

	/* allocate bf{} */
	Zinc_string_add_str2(a, &bf);

	Zinc_expect_string(&bf, "xyz", "str");


	/* destroy bf{} */
	Zinc_string_destroy(&bf);
}

void test_string_next()
{
	Zinc_test_name(__func__);

	Zinc_result r;
	Zinc_string bf;
	Zinc_string_init(&bf);

	Zinc_string_add_str(&bf, "abcαβγ");

	char c[5];
	int num;
	bool done;
	size_t pos = 0;

	r = Zinc_string_next(&bf, &pos, c, &num, &done);
	Zinc_assert_ok(r, "ok 0");
	Zinc_expect_strcmp(c, "a", "str 0");
	Zinc_expect_int_equal(num, 1, "num 0");
	Zinc_expect_false(done, "done 0");

	r = Zinc_string_next(&bf, &pos, c, &num, &done);
	Zinc_assert_ok(r, "ok 1");
	Zinc_expect_strcmp(c, "b", "str 1");
	Zinc_expect_int_equal(num, 1, "num 1");
	Zinc_expect_false(done, "done 1");

	r = Zinc_string_next(&bf, &pos, c, &num, &done);
	Zinc_assert_ok(r, "ok 2");
	Zinc_expect_strcmp(c, "c", "str 2");
	Zinc_expect_int_equal(num, 1, "num 2");
	Zinc_expect_false(done, "done 2");

	r = Zinc_string_next(&bf, &pos, c, &num, &done);
	Zinc_assert_ok(r, "ok 3");
	Zinc_expect_strcmp(c, "α", "str 3");
	Zinc_expect_int_equal(num, 2, "num 3");
	Zinc_expect_false(done, "done 3");

	r = Zinc_string_next(&bf, &pos, c, &num, &done);
	Zinc_assert_ok(r, "ok 4");
	Zinc_expect_strcmp(c, "β", "str 4");
	Zinc_expect_int_equal(num, 2, "num 4");
	Zinc_expect_false(done, "done 4");

	r = Zinc_string_next(&bf, &pos, c, &num, &done);
	Zinc_assert_ok(r, "ok 5");
	Zinc_expect_strcmp(c, "γ", "str 5");
	Zinc_expect_int_equal(num, 2, "num 5");
	Zinc_expect_false(done, "done 5");

	r = Zinc_string_next(&bf, &pos, c, &num, &done);
	Zinc_assert_ok(r, "ok 6");
	Zinc_expect_true(done, "done 6");

	Zinc_string_destroy(&bf);
}

/* static-output */
/* dynamic bf{} bf2{} */
void test_buffer_buffer_compare()
{
	Zinc_test_name(__func__);

	struct Zinc_string bf;
	Zinc_string_init(&bf);

	/* allocate bf{} */
	Zinc_string_add_char(&bf, 'x');
	Zinc_string_add_char(&bf, 'y');
	Zinc_string_add_char(&bf, 'z');

	struct Zinc_string bf2;
	Zinc_string_init(&bf2);

	/* allocate bf2{} */
	Zinc_string_add_char(&bf2, 'x');
	Zinc_string_add_char(&bf2, 'y');
	Zinc_string_add_char(&bf2, 'z');

	Zinc_expect_true(Zinc_string_compare(&bf, &bf2), "equal");

	Zinc_string_clear(&bf2);

	/* allocate bf2{} */
	Zinc_string_add_char(&bf2, 'x');
	Zinc_string_add_char(&bf2, 'y');
	Zinc_string_add_char(&bf2, '1');

	Zinc_expect_true(Zinc_string_compare(&bf, &bf2) == 0, "not equal");

	/* destroy bf{} bf2{} */
	Zinc_string_destroy(&bf);
	Zinc_string_destroy(&bf2);
}

/* static-output */
/* dynamic-temp bf{} */
void test_buffer_str_compare()
{
	Zinc_test_name(__func__);

	struct Zinc_string bf;
	Zinc_string_init(&bf);

	/* allocate bf{} */
	Zinc_string_add_char(&bf, 'x');
	Zinc_string_add_char(&bf, 'y');
	Zinc_string_add_char(&bf, 'z');

	Zinc_expect_true(Zinc_string_compare_str(&bf, "xyz") == 1, "equal");
	Zinc_expect_true(Zinc_string_compare_str(&bf, "xy1") == 0, "not equal");

	/* destroy bf{} */
	Zinc_string_destroy(&bf);
}

/* static-output */
/* dynamic-temp bf{} bf2{} */
void test_buffer_uslice()
{
	Zinc_test_name(__func__);

	struct Zinc_string bf;
	struct Zinc_string bf2;
	enum Zinc_result r;

	Zinc_string_init(&bf);

	/* allocate bf{} */
	Zinc_string_add_char(&bf, 'a');
	Zinc_string_add_char(&bf, 'b');
	Zinc_string_add_char(&bf, 'c');
	Zinc_string_add_char(&bf, 'd');
	Zinc_string_add_char(&bf, 'e');
	Zinc_string_add_char(&bf, 'f');

	Zinc_string_init(&bf2);

	/* allocate bf2{} */
	r = Zinc_string_uslice(&bf, &bf2, 2, 5);
	Zinc_assert_ok(r, "buffer_uslice");

	Zinc_assert_ptr(bf2.buf, "ptr buf2.buf");
	Zinc_expect_size_t_equal(bf2.size, 3, "3 bf2.size");
	Zinc_expect_string(&bf2, "cde", "cde bf2");

	/* destory bf{} bf2{} */
	Zinc_string_destroy(&bf);
	Zinc_string_destroy(&bf2);
}

/* static-output */
/* dynamic bf{} bf2{} */
void test_buffer_uslice2()
{
	Zinc_test_name(__func__);

	struct Zinc_string bf;
	struct Zinc_string bf2;
	enum Zinc_result r;

	Zinc_string_init(&bf);

	/* allocate bf{} */
	Zinc_string_add_char(&bf, 'a');
	Zinc_string_add_char(&bf, 'b');
	Zinc_string_add_char(&bf, 'c');
	Zinc_string_add_char(&bf, 'd');
	Zinc_string_add_char(&bf, 'e');
	Zinc_string_add_char(&bf, 'f');

	Zinc_string_init(&bf2);

	/* allocate bf2{} */
	r = Zinc_string_uslice(&bf, &bf2, 2, 1000);
	Zinc_assert_ok(r, "buffer_uslice");

	Zinc_assert_ptr(bf2.buf, "ptr buf2.buf");
	Zinc_expect_int_equal(bf2.size, 4, "4 bf2.size");
	Zinc_expect_string(&bf2, "cdef", "cdef bf2");

	/* destroy bf{} bf2{} */
	Zinc_string_destroy(&bf);
	Zinc_string_destroy(&bf2);
}

void test_buffer_add_format()
{
    Zinc_test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Zinc_string_add_format(&bf, "%% %c %s %d %lf", 'x', "hello", 10, 5.1);
    Zinc_expect_string(&bf, "% x hello 10 5.100000", "bf");

    Zinc_string_destroy(&bf);
}

void test_buffer_add_format_d_max()
{
    Zinc_test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    int d = INT_MAX;
    Zinc_string_add_format(&bf, "%d", d);
    Zinc_string_finish(&bf);
    Zinc_expect_int_equal(atoi(bf.buf), d, "compare");

    Zinc_string_destroy(&bf);
}

void test_buffer_add_format_d_min()
{
    Zinc_test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    int d = INT_MIN;
    Zinc_string_add_format(&bf, "%d", d);
    Zinc_string_finish(&bf);
    Zinc_expect_int_equal((int)strtol(bf.buf, NULL, 10), d, "compare");

    Zinc_string_destroy(&bf);
}

void test_buffer_add_format_zu_max()
{
    Zinc_test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    size_t zu = ULONG_MAX;
    Zinc_string_add_format(&bf, "%zu", zu);
    Zinc_string_finish(&bf);
    Zinc_expect_size_t_equal(strtoul(bf.buf, NULL, 10), zu, "compare");

    Zinc_string_destroy(&bf);
}

void test_buffer_add_format_zu_min()
{
    Zinc_test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    size_t zu = 0;
    Zinc_string_add_format(&bf, "%zu", zu);
    Zinc_string_finish(&bf);
    Zinc_expect_size_t_equal(strtol(bf.buf, NULL, 10), zu, "compare");

    Zinc_string_destroy(&bf);
}

void test_buffer_add_format_s_large()
{
    Zinc_test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    struct Zinc_string input;
    Zinc_string_init(&input);
    for (int i = 0; i < BUFFER_CHUNK * 2 + 10; i++) {
        Zinc_string_add_char(&input, 'x');
    }
    Zinc_string_finish(&input);

    Zinc_string_add_format(&bf, "%s", input.buf);
    Zinc_expect_true(Zinc_string_compare(&bf, &input), "compare");

    Zinc_string_destroy(&bf);
    Zinc_string_destroy(&input);
}

void test_buffer_add_format_buffer()
{
    Zinc_test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    struct Zinc_string bf_in;
    Zinc_string_init(&bf_in);
    Zinc_string_add_str(&bf_in, "hello");

    Zinc_string_add_format(&bf, "%bf", &bf_in);
    Zinc_expect_true(Zinc_string_compare(&bf, &bf_in), "compare");

    Zinc_string_destroy(&bf);
    Zinc_string_destroy(&bf_in);
}

void test_buffer_order_same()
{
    Zinc_test_name(__func__);

    struct Zinc_string a;
    Zinc_string_init(&a);
    Zinc_string_add_str(&a, "abc");

    struct Zinc_string b;
    Zinc_string_init(&b);
    Zinc_string_add_str(&b, "abc");

    Zinc_expect_int_equal(Zinc_string_order(&a, &b), 0, "0");

    Zinc_string_destroy(&a);
    Zinc_string_destroy(&b);
}

void test_buffer_order_less_size()
{
    Zinc_test_name(__func__);

    struct Zinc_string a;
    Zinc_string_init(&a);
    Zinc_string_add_str(&a, "abc");

    struct Zinc_string b;
    Zinc_string_init(&b);
    Zinc_string_add_str(&b, "abc123");

    Zinc_expect_int_equal(Zinc_string_order(&a, &b), -1, "-1");

    Zinc_string_destroy(&a);
    Zinc_string_destroy(&b);
}

void test_buffer_order_greater_size()
{
    Zinc_test_name(__func__);

    struct Zinc_string a;
    Zinc_string_init(&a);
    Zinc_string_add_str(&a, "abc123");

    struct Zinc_string b;
    Zinc_string_init(&b);
    Zinc_string_add_str(&b, "abc");

    Zinc_expect_int_equal(Zinc_string_order(&a, &b), 1, "1");

    Zinc_string_destroy(&a);
    Zinc_string_destroy(&b);
}

void test_buffer_order_less_than()
{
    Zinc_test_name(__func__);

    struct Zinc_string a;
    Zinc_string_init(&a);
    Zinc_string_add_str(&a, "abcx");

    struct Zinc_string b;
    Zinc_string_init(&b);
    Zinc_string_add_str(&b, "abcy");

    Zinc_expect_int_equal(Zinc_string_order(&a, &b), -1, "-1");

    Zinc_string_destroy(&a);
    Zinc_string_destroy(&b);
}

void test_buffer_order_greater_than()
{
    Zinc_test_name(__func__);

    struct Zinc_string a;
    Zinc_string_init(&a);
    Zinc_string_add_str(&a, "abcza;llkejrfladf");

    struct Zinc_string b;
    Zinc_string_init(&b);
    Zinc_string_add_str(&b, "abcykewlaf");

    Zinc_expect_int_equal(Zinc_string_order(&a, &b), 1, "1");

    Zinc_string_destroy(&a);
    Zinc_string_destroy(&b);
}

/* static-output */
void test_buffer()
{
    test_buffer_expand();
	test_buffer_finish();
	test_buffer_clear();
    test_buffer_reset();
	test_buffer_copy();
	test_buffer_buffer2array();
	test_buffer_array2buffer();
	test_string_next();
	test_buffer_buffer_compare();
	test_buffer_str_compare();
	test_buffer_uslice();
	test_buffer_uslice2();
    test_buffer_add_format();
    test_buffer_add_format_d_max();
    test_buffer_add_format_d_min();
    test_buffer_add_format_zu_max();
    test_buffer_add_format_zu_min();
    test_buffer_add_format_s_large();
    test_buffer_add_format_buffer();
    test_buffer_order_same();
    test_buffer_order_less_size();
    test_buffer_order_greater_size();
    test_buffer_order_less_than();
    test_buffer_order_greater_than();
}

void Zinc_unit_buffer(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;

		Zinc_test_register(test, Zinc_unit_buffer_init);
		Zinc_test_register(test, Zinc_unit_buffer_add_char);
		Zinc_test_register(test, Zinc_unit_test_buffer_add);
		return;
	}

	Zinc_test_perform(test);
}