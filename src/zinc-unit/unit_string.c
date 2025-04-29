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

void Zinc_unit_buffer_expand(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    Zinc_string* bf = NULL;
    Zinc_string_create(&bf);

    Zinc_string_expand(bf, BUFFER_CHUNK + 10);

    Zinc_test_expect_size_t_equal(test, bf->buf_size, BUFFER_CHUNK + 10, "buf_size");

    Zinc_string_destroy(bf);
    free(bf);
}

void Zinc_unit_buffer_finish(Zinc_test* test)
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
	Zinc_string_finish(&bf);

	Zinc_test_expect_string(test, &bf, "xyz", "str");
	Zinc_test_expect_true(test, strcmp(bf.buf, "xyz") == 0, "strcmp");

	Zinc_string_destroy(&bf);
}

void Zinc_unit_buffer_clear(Zinc_test* test)
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
	Zinc_string_clear(&bf);
	Zinc_test_expect_string(test, &bf, "", "clear");

	Zinc_string_destroy(&bf);
}

void Zinc_unit_buffer_reset(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Zinc_string bf;
    Zinc_string_init(&bf);

    Zinc_string_add_str(&bf, "hello");

    Zinc_test_expect_string(test, &bf, "hello", "hello");
    Zinc_string_reset(&bf);

    Zinc_test_expect_string(test, &bf, "", "");

    Zinc_string_destroy(&bf);
}

void Zinc_unit_buffer_copy(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Zinc_string bf;
	Zinc_string bf2;

	Zinc_string_init(&bf);

	Zinc_string_add_char(&bf, 'x');
	Zinc_string_add_char(&bf, 'y');
	Zinc_string_add_char(&bf, 'z');

	Zinc_test_expect_string(test, &bf, "xyz", "str");

	Zinc_string_init(&bf2);

	Zinc_string_copy(&bf, &bf2);

	Zinc_test_expect_string(test, &bf2, "xyz", "copy");

	Zinc_string_destroy(&bf);
	Zinc_string_destroy(&bf2);
}

void Zinc_unit_buffer_buffer2array(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Zinc_string bf;
	char* a;

	Zinc_string_init(&bf);

	Zinc_string_add_char(&bf, 'x');
	Zinc_string_add_char(&bf, 'y');
	Zinc_string_add_char(&bf, 'z');

	Zinc_test_expect_string(test, &bf, "xyz", "str");

	Zinc_string_create_str(&bf, &a);

	Zinc_test_expect_true(test, strcmp(a, "xyz") == 0, "array");

	Zinc_string_destroy(&bf);
	free(a);
}

void Zinc_unit_buffer_array2buffer(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	char a[] = "xyz";
	Zinc_string bf;

	Zinc_string_init(&bf);

	Zinc_string_add_str2(a, &bf);

	Zinc_test_expect_string(test, &bf, "xyz", "str");

	Zinc_string_destroy(&bf);
}

void Zinc_unit_string_next(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Zinc_result r;
	Zinc_string bf;
	Zinc_string_init(&bf);

	Zinc_string_add_str(&bf, "abcαβγ");

	char c[5];
	int num;
	bool done;
	size_t pos = 0;

	r = Zinc_string_next(&bf, &pos, c, &num, &done);
	Zinc_test_assert_ok(test, r, "ok 0");
	Zinc_test_expect_strcmp(test, c, "a", "str 0");
	Zinc_test_expect_int_equal(test, num, 1, "num 0");
	Zinc_test_expect_false(test, done, "done 0");

	r = Zinc_string_next(&bf, &pos, c, &num, &done);
	Zinc_test_assert_ok(test, r, "ok 1");
	Zinc_test_expect_strcmp(test, c, "b", "str 1");
	Zinc_test_expect_int_equal(test, num, 1, "num 1");
	Zinc_test_expect_false(test, done, "done 1");

	r = Zinc_string_next(&bf, &pos, c, &num, &done);
	Zinc_test_assert_ok(test, r, "ok 2");
	Zinc_test_expect_strcmp(test, c, "c", "str 2");
	Zinc_test_expect_int_equal(test, num, 1, "num 2");
	Zinc_test_expect_false(test, done, "done 2");

	r = Zinc_string_next(&bf, &pos, c, &num, &done);
	Zinc_test_assert_ok(test, r, "ok 3");
	Zinc_test_expect_strcmp(test, c, "α", "str 3");
	Zinc_test_expect_int_equal(test, num, 2, "num 3");
	Zinc_test_expect_false(test, done, "done 3");

	r = Zinc_string_next(&bf, &pos, c, &num, &done);
	Zinc_test_assert_ok(test, r, "ok 4");
	Zinc_test_expect_strcmp(test, c, "β", "str 4");
	Zinc_test_expect_int_equal(test, num, 2, "num 4");
	Zinc_test_expect_false(test, done, "done 4");

	r = Zinc_string_next(&bf, &pos, c, &num, &done);
	Zinc_test_assert_ok(test, r, "ok 5");
	Zinc_test_expect_strcmp(test, c, "γ", "str 5");
	Zinc_test_expect_int_equal(test, num, 2, "num 5");
	Zinc_test_expect_false(test, done, "done 5");

	r = Zinc_string_next(&bf, &pos, c, &num, &done);
	Zinc_test_assert_ok(test, r, "ok 6");
	Zinc_test_expect_true(test, done, "done 6");

	Zinc_string_destroy(&bf);
}

void Zinc_unit_buffer_buffer_compare(Zinc_test* test)
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

	Zinc_string bf2;
	Zinc_string_init(&bf2);

	Zinc_string_add_char(&bf2, 'x');
	Zinc_string_add_char(&bf2, 'y');
	Zinc_string_add_char(&bf2, 'z');

	Zinc_test_expect_true(test, Zinc_string_compare(&bf, &bf2), "equal");

	Zinc_string_clear(&bf2);

	Zinc_string_add_char(&bf2, 'x');
	Zinc_string_add_char(&bf2, 'y');
	Zinc_string_add_char(&bf2, '1');

	Zinc_test_expect_true(test, Zinc_string_compare(&bf, &bf2) == 0, "not equal");

	Zinc_string_destroy(&bf);
	Zinc_string_destroy(&bf2);
}

void Zinc_unit_buffer_str_compare(Zinc_test* test)
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

	Zinc_test_expect_true(test, Zinc_string_compare_str(&bf, "xyz") == 1, "equal");
	Zinc_test_expect_true(test, Zinc_string_compare_str(&bf, "xy1") == 0, "not equal");

	Zinc_string_destroy(&bf);
}

void Zinc_unit_buffer_uslice(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Zinc_string bf;
	Zinc_string bf2;
	Zinc_result r;

	Zinc_string_init(&bf);

	Zinc_string_add_char(&bf, 'a');
	Zinc_string_add_char(&bf, 'b');
	Zinc_string_add_char(&bf, 'c');
	Zinc_string_add_char(&bf, 'd');
	Zinc_string_add_char(&bf, 'e');
	Zinc_string_add_char(&bf, 'f');

	Zinc_string_init(&bf2);

	r = Zinc_string_uslice(&bf, &bf2, 2, 5);
	Zinc_test_assert_ok(test, r, "buffer_uslice");

	Zinc_test_assert_ptr(test, bf2.buf, "ptr buf2.buf");
	Zinc_test_expect_size_t_equal(test, bf2.size, 3, "3 bf2.size");
	Zinc_test_expect_string(test, &bf2, "cde", "cde bf2");

	Zinc_string_destroy(&bf);
	Zinc_string_destroy(&bf2);
}

void Zinc_unit_test_buffer_uslice2(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Zinc_string bf;
	Zinc_string bf2;
	Zinc_result r;

	Zinc_string_init(&bf);

	Zinc_string_add_char(&bf, 'a');
	Zinc_string_add_char(&bf, 'b');
	Zinc_string_add_char(&bf, 'c');
	Zinc_string_add_char(&bf, 'd');
	Zinc_string_add_char(&bf, 'e');
	Zinc_string_add_char(&bf, 'f');

	Zinc_string_init(&bf2);

	r = Zinc_string_uslice(&bf, &bf2, 2, 1000);
	Zinc_test_assert_ok(test, r, "buffer_uslice");

	Zinc_test_assert_ptr(test, bf2.buf, "ptr buf2.buf");
	Zinc_test_expect_size_t_equal(test, bf2.size, 4, "4 bf2.size");
	Zinc_test_expect_string(test, &bf2, "cdef", "cdef bf2");

	Zinc_string_destroy(&bf);
	Zinc_string_destroy(&bf2);
}

void Zinc_unit_buffer_add_format(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    Zinc_string bf;
    Zinc_string_init(&bf);

    Zinc_string_add_format(&bf, "%% %c %s %d %lf", 'x', "hello", 10, 5.1);
    Zinc_test_expect_string(test, &bf, "% x hello 10 5.100000", "bf");

    Zinc_string_destroy(&bf);
}

void Zinc_unit_buffer_add_format_d_max(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    Zinc_string bf;
    Zinc_string_init(&bf);

    int d = INT_MAX;
    Zinc_string_add_format(&bf, "%d", d);
    Zinc_string_finish(&bf);
    Zinc_test_expect_int_equal(test, (int)strtol(bf.buf, NULL, 10), d, "compare");

    Zinc_string_destroy(&bf);
}

void Zinc_unit_buffer_add_format_d_min(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    Zinc_string bf;
    Zinc_string_init(&bf);

    int d = INT_MIN;
    Zinc_string_add_format(&bf, "%d", d);
    Zinc_string_finish(&bf);
    Zinc_test_expect_int_equal(test, (int)strtol(bf.buf, NULL, 10), d, "compare");

    Zinc_string_destroy(&bf);
}

void Zinc_unit_test_buffer_add_format_zu_max(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Zinc_string bf;
    Zinc_string_init(&bf);

    size_t zu = ULONG_MAX;
    Zinc_string_add_format(&bf, "%zu", zu);
    Zinc_string_finish(&bf);
    Zinc_test_expect_size_t_equal(test, strtoul(bf.buf, NULL, 10), zu, "compare");

    Zinc_string_destroy(&bf);
}

void Zinc_unit_buffer_add_format_zu_min(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    Zinc_string bf;
    Zinc_string_init(&bf);

    size_t zu = 0;
    Zinc_string_add_format(&bf, "%zu", zu);
    Zinc_string_finish(&bf);
    Zinc_test_expect_size_t_equal(test, strtol(bf.buf, NULL, 10), zu, "compare");

    Zinc_string_destroy(&bf);
}

void Zinc_unit_buffer_add_format_s_large(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    Zinc_string bf;
    Zinc_string_init(&bf);

    Zinc_string input;
    Zinc_string_init(&input);
    for (int i = 0; i < BUFFER_CHUNK * 2 + 10; i++) {
        Zinc_string_add_char(&input, 'x');
    }
    Zinc_string_finish(&input);

    Zinc_string_add_format(&bf, "%s", input.buf);
    Zinc_test_expect_true(test, Zinc_string_compare(&bf, &input), "compare");

    Zinc_string_destroy(&bf);
    Zinc_string_destroy(&input);
}

void Zinc_unit_buffer_add_format_buffer(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    Zinc_string bf;
    Zinc_string_init(&bf);

    Zinc_string bf_in;
    Zinc_string_init(&bf_in);
    Zinc_string_add_str(&bf_in, "hello");

    Zinc_string_add_format(&bf, "%bf", &bf_in);
    Zinc_test_expect_true(test, Zinc_string_compare(&bf, &bf_in), "compare");

    Zinc_string_destroy(&bf);
    Zinc_string_destroy(&bf_in);
}

void Zinc_unit_buffer_order_same(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    Zinc_string a;
    Zinc_string_init(&a);
    Zinc_string_add_str(&a, "abc");

    Zinc_string b;
    Zinc_string_init(&b);
    Zinc_string_add_str(&b, "abc");

    Zinc_test_expect_int_equal(test, Zinc_string_order(&a, &b), 0, "0");

    Zinc_string_destroy(&a);
    Zinc_string_destroy(&b);
}

void Zinc_unit_buffer_order_less_size(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    Zinc_string a;
    Zinc_string_init(&a);
    Zinc_string_add_str(&a, "abc");

    Zinc_string b;
    Zinc_string_init(&b);
    Zinc_string_add_str(&b, "abc123");

    Zinc_test_expect_int_equal(test, Zinc_string_order(&a, &b), -1, "-1");

    Zinc_string_destroy(&a);
    Zinc_string_destroy(&b);
}

void Zinc_unit_test_buffer_order_greater_size(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    Zinc_string a;
    Zinc_string_init(&a);
    Zinc_string_add_str(&a, "abc123");

    Zinc_string b;
    Zinc_string_init(&b);
    Zinc_string_add_str(&b, "abc");

    Zinc_test_expect_int_equal(test, Zinc_string_order(&a, &b), 1, "1");

    Zinc_string_destroy(&a);
    Zinc_string_destroy(&b);
}

void Zinc_unit_buffer_order_less_than(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    Zinc_string a;
    Zinc_string_init(&a);
    Zinc_string_add_str(&a, "abcx");

    Zinc_string b;
    Zinc_string_init(&b);
    Zinc_string_add_str(&b, "abcy");

    Zinc_test_expect_int_equal(test, Zinc_string_order(&a, &b), -1, "-1");

    Zinc_string_destroy(&a);
    Zinc_string_destroy(&b);
}

void Zinc_unit_test_buffer_order_greater_than(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

    Zinc_string a;
    Zinc_string_init(&a);
    Zinc_string_add_str(&a, "abcza;llkejrfladf");

    Zinc_string b;
    Zinc_string_init(&b);
    Zinc_string_add_str(&b, "abcykewlaf");

    Zinc_test_expect_int_equal(test, Zinc_string_order(&a, &b), 1, "1");

    Zinc_string_destroy(&a);
    Zinc_string_destroy(&b);
}

void test_buffer()
{
}

void Zinc_unit_string(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;

		Zinc_test_register(test, Zinc_unit_buffer_init);
		Zinc_test_register(test, Zinc_unit_buffer_add_char);
		Zinc_test_register(test, Zinc_unit_test_buffer_add);
		Zinc_test_register(test, Zinc_unit_buffer_expand);
		Zinc_test_register(test, Zinc_unit_buffer_finish);
		Zinc_test_register(test, Zinc_unit_buffer_clear);
		Zinc_test_register(test, Zinc_unit_buffer_reset);
		Zinc_test_register(test, Zinc_unit_buffer_copy);
		Zinc_test_register(test, Zinc_unit_buffer_buffer2array);
		Zinc_test_register(test, Zinc_unit_buffer_array2buffer);
		Zinc_test_register(test, Zinc_unit_string_next);
		Zinc_test_register(test, Zinc_unit_buffer_buffer_compare);
		Zinc_test_register(test, Zinc_unit_buffer_str_compare);
		Zinc_test_register(test, Zinc_unit_buffer_uslice);
		Zinc_test_register(test, Zinc_unit_test_buffer_uslice2);
		Zinc_test_register(test, Zinc_unit_buffer_add_format);
		Zinc_test_register(test, Zinc_unit_buffer_add_format_d_max);
		Zinc_test_register(test, Zinc_unit_buffer_add_format_d_min);
		Zinc_test_register(test, Zinc_unit_test_buffer_add_format_zu_max);
		Zinc_test_register(test, Zinc_unit_buffer_add_format_zu_min);
		Zinc_test_register(test, Zinc_unit_buffer_add_format_s_large);
		Zinc_test_register(test, Zinc_unit_buffer_add_format_buffer);
		Zinc_test_register(test, Zinc_unit_buffer_order_same);
		Zinc_test_register(test, Zinc_unit_buffer_order_less_size);
		Zinc_test_register(test, Zinc_unit_test_buffer_order_greater_size);
		Zinc_test_register(test, Zinc_unit_buffer_order_less_than);
		Zinc_test_register(test, Zinc_unit_test_buffer_order_greater_than);

		return;
	}

	Zinc_test_perform(test);
}