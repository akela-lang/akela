#include <string.h>
#include <stdlib.h>
#include "zinc/result.h"
#include "zinc/unit_test.h"
#include "zinc/buffer.h"
#include <limits.h>

/* static-output */
void test_buffer_init()
{
	test_name(__func__);

	struct buffer bf;
	buffer_init(&bf);
	expect_int_equal(bf.size, 0, "size");
	expect_int_equal(bf.buf_size, 0, "buf_size");
	expect_null(bf.buf, "buf");
}

/* static-output */
/* dynamic-temp bf{} */
void test_buffer_add_char()
{
	test_name(__func__);

	struct buffer bf;
	buffer_init(&bf);

	/* allocate bf{} */
	buffer_add_char(&bf, 'x');
	buffer_add_char(&bf, 'y');
	buffer_add_char(&bf, 'z');

	expect_str(&bf, "xyz", "str");

	/* destroy bf{} */
	buffer_destroy(&bf);
}

void test_buffer_add()
{
    test_name(__func__);
    struct buffer* bf = NULL;
    buffer_create(&bf);

    buffer_add(bf, "abc", 3);
    buffer_add(bf, "123", 3);

    expect_str(bf, "abc123", "bf");

    buffer_destroy(bf);
    free(bf);
}

void test_buffer_expand()
{
    test_name(__func__);

    struct buffer* bf = NULL;
    buffer_create(&bf);

    buffer_expand(bf, BUFFER_CHUNK + 10);

    expect_size_t_equal(bf->buf_size, BUFFER_CHUNK + 10, "buf_size");

    buffer_destroy(bf);
    free(bf);
}

void test_buffer_finish()
{
	test_name(__func__);

	struct buffer bf;
	buffer_init(&bf);

	/* allocate bf{} */
	buffer_add_char(&bf, 'x');
	buffer_add_char(&bf, 'y');
	buffer_add_char(&bf, 'z');
	buffer_finish(&bf);

	expect_str(&bf, "xyz", "str");
	expect_true(strcmp(bf.buf, "xyz") == 0, "strcmp");

	/* destroy bf{} */
	buffer_destroy(&bf);
}

/* static-output */
/* dynamic-temp bf{} */
void test_buffer_clear()
{
	test_name(__func__);

	struct buffer bf;
	buffer_init(&bf);

	/* allocate bf{} */
	buffer_add_char(&bf, 'x');
	buffer_add_char(&bf, 'y');
	buffer_add_char(&bf, 'z');

	expect_str(&bf, "xyz", "str");
	buffer_clear(&bf);
	expect_str(&bf, "", "clear");

	/* destroy bf{} */
	buffer_destroy(&bf);
}

void test_buffer_reset()
{
    test_name(__func__);

    struct buffer bf;
    buffer_init(&bf);

    buffer_copy_str(&bf, "hello");

    expect_str(&bf, "hello", "hello");
    buffer_reset(&bf);

    expect_str(&bf, "", "");

    buffer_destroy(&bf);
}

/* static-output */
/* dynamic-temp bf{} bf2{} */
void test_buffer_copy()
{
	test_name(__func__);

	struct buffer bf;
	struct buffer bf2;

	buffer_init(&bf);

	/* allocate bf{} */
	buffer_add_char(&bf, 'x');
	buffer_add_char(&bf, 'y');
	buffer_add_char(&bf, 'z');

	expect_str(&bf, "xyz", "str");

	buffer_init(&bf2);

	/* allocate bf2{} */
	buffer_copy(&bf, &bf2);

	expect_str(&bf2, "xyz", "copy");

	/* destroy bf{} bf2{} */
	buffer_destroy(&bf);
	buffer_destroy(&bf2);
}

/* static-output */
/* dynamic-temp bf{} a */
void test_buffer_buffer2array()
{
	test_name(__func__);

	struct buffer bf;
	char* a;

	buffer_init(&bf);

	/* allocate bf{} */
	buffer_add_char(&bf, 'x');
	buffer_add_char(&bf, 'y');
	buffer_add_char(&bf, 'z');

	expect_str(&bf, "xyz", "str");

	/* allocate a */
	buffer2array(&bf, &a);

	expect_true(strcmp(a, "xyz") == 0, "array");

	/* destroy bf{} a */
	buffer_destroy(&bf);
	free(a);
}

/* static-output */
/* dynamic-temp bf{} */
void test_buffer_array2buffer()
{
	test_name(__func__);

	char a[] = "xyz";
	struct buffer bf;

	buffer_init(&bf);

	/* allocate bf{} */
	array2buffer(a, &bf);

	expect_str(&bf, "xyz", "str");


	/* destroy bf{} */
	buffer_destroy(&bf);
}

/* static-output */
/* dynamic-temp bf{} x{} */
void test_buffer_next_char()
{
	test_name(__func__);

	enum result r;
	struct buffer bf;
	buffer_init(&bf);

	/* allocate bf{} */
	buffer_add_char(&bf, 'x');
	buffer_add_char(&bf, 'y');
	buffer_add_char(&bf, 'z');

	struct buffer x;
	buffer_init(&x);
	size_t pos = 0;

	/* allocate x{} */
	r = next_char(&bf, &pos, &x);
	assert_ok(r, "next_char");

	expect_str(&x, "x", "x");

	/* allocate x{} */
	r = next_char(&bf, &pos, &x);
	assert_ok(r, "y");

	expect_str(&x, "y", "y");

	/* allocate x{} */
	r = next_char(&bf, &pos, &x);
	assert_ok(r, "z");

	expect_str(&x, "z", "z");

	/* destroy bf{} x{} */
	buffer_destroy(&bf);
	buffer_destroy(&x);
}

/* static-output */
/* dynamic bf{} bf2{} */
void test_buffer_buffer_compare()
{
	test_name(__func__);

	struct buffer bf;
	buffer_init(&bf);

	/* allocate bf{} */
	buffer_add_char(&bf, 'x');
	buffer_add_char(&bf, 'y');
	buffer_add_char(&bf, 'z');

	struct buffer bf2;
	buffer_init(&bf2);

	/* allocate bf2{} */
	buffer_add_char(&bf2, 'x');
	buffer_add_char(&bf2, 'y');
	buffer_add_char(&bf2, 'z');

	expect_true(buffer_compare(&bf, &bf2), "equal");

	buffer_clear(&bf2);

	/* allocate bf2{} */
	buffer_add_char(&bf2, 'x');
	buffer_add_char(&bf2, 'y');
	buffer_add_char(&bf2, '1');

	expect_true(buffer_compare(&bf, &bf2) == 0, "not equal");

	/* destroy bf{} bf2{} */
	buffer_destroy(&bf);
	buffer_destroy(&bf2);
}

/* static-output */
/* dynamic-temp bf{} */
void test_buffer_str_compare()
{
	test_name(__func__);

	struct buffer bf;
	buffer_init(&bf);

	/* allocate bf{} */
	buffer_add_char(&bf, 'x');
	buffer_add_char(&bf, 'y');
	buffer_add_char(&bf, 'z');

	expect_true(buffer_compare_str(&bf, "xyz") == 1, "equal");
	expect_true(buffer_compare_str(&bf, "xy1") == 0, "not equal");

	/* destroy bf{} */
	buffer_destroy(&bf);
}

/* static-output */
/* dynamic-temp bf{} bf2{} */
void test_buffer_uslice()
{
	test_name(__func__);

	struct buffer bf;
	struct buffer bf2;
	enum result r;

	buffer_init(&bf);

	/* allocate bf{} */
	buffer_add_char(&bf, 'a');
	buffer_add_char(&bf, 'b');
	buffer_add_char(&bf, 'c');
	buffer_add_char(&bf, 'd');
	buffer_add_char(&bf, 'e');
	buffer_add_char(&bf, 'f');

	buffer_init(&bf2);

	/* allocate bf2{} */
	r = buffer_uslice(&bf, &bf2, 2, 5);
	assert_ok(r, "buffer_uslice");

	assert_ptr(bf2.buf, "ptr buf2.buf");
	expect_size_t_equal(bf2.size, 3, "3 bf2.size");
	expect_str(&bf2, "cde", "cde bf2");

	/* destory bf{} bf2{} */
	buffer_destroy(&bf);
	buffer_destroy(&bf2);
}

/* static-output */
/* dynamic bf{} bf2{} */
void test_buffer_uslice2()
{
	test_name(__func__);

	struct buffer bf;
	struct buffer bf2;
	enum result r;

	buffer_init(&bf);

	/* allocate bf{} */
	buffer_add_char(&bf, 'a');
	buffer_add_char(&bf, 'b');
	buffer_add_char(&bf, 'c');
	buffer_add_char(&bf, 'd');
	buffer_add_char(&bf, 'e');
	buffer_add_char(&bf, 'f');

	buffer_init(&bf2);

	/* allocate bf2{} */
	r = buffer_uslice(&bf, &bf2, 2, 1000);
	assert_ok(r, "buffer_uslice");

	assert_ptr(bf2.buf, "ptr buf2.buf");
	expect_int_equal(bf2.size, 4, "4 bf2.size");
	expect_str(&bf2, "cdef", "cdef bf2");

	/* destroy bf{} bf2{} */
	buffer_destroy(&bf);
	buffer_destroy(&bf2);
}

void test_buffer_add_format()
{
    test_name(__func__);

    struct buffer bf;
    buffer_init(&bf);

    buffer_add_format(&bf, "%% %c %s %d %lf", 'x', "hello", 10, 5.1);
    expect_str(&bf, "% x hello 10 5.100000", "bf");

    buffer_destroy(&bf);
}

void test_buffer_add_format_d_max()
{
    test_name(__func__);

    struct buffer bf;
    buffer_init(&bf);

    int d = INT_MAX;
    buffer_add_format(&bf, "%d", d);
    buffer_finish(&bf);
    expect_int_equal(atoi(bf.buf), d, "compare");

    buffer_destroy(&bf);
}

void test_buffer_add_format_d_min()
{
    test_name(__func__);

    struct buffer bf;
    buffer_init(&bf);

    int d = INT_MIN;
    buffer_add_format(&bf, "%d", d);
    buffer_finish(&bf);
    expect_int_equal((int)strtol(bf.buf, NULL, 10), d, "compare");

    buffer_destroy(&bf);
}

void test_buffer_add_format_zu_max()
{
    test_name(__func__);

    struct buffer bf;
    buffer_init(&bf);

    size_t zu = ULONG_MAX;
    buffer_add_format(&bf, "%zu", zu);
    buffer_finish(&bf);
    expect_size_t_equal(strtoul(bf.buf, NULL, 10), zu, "compare");

    buffer_destroy(&bf);
}

void test_buffer_add_format_zu_min()
{
    test_name(__func__);

    struct buffer bf;
    buffer_init(&bf);

    size_t zu = 0;
    buffer_add_format(&bf, "%zu", zu);
    buffer_finish(&bf);
    expect_size_t_equal(strtol(bf.buf, NULL, 10), zu, "compare");

    buffer_destroy(&bf);
}

void test_buffer_add_format_s_large()
{
    test_name(__func__);

    struct buffer bf;
    buffer_init(&bf);

    struct buffer input;
    buffer_init(&input);
    for (int i = 0; i < BUFFER_CHUNK * 2 + 10; i++) {
        buffer_add_char(&input, 'x');
    }
    buffer_finish(&input);

    buffer_add_format(&bf, "%s", input.buf);
    expect_true(buffer_compare(&bf, &input), "compare");

    buffer_destroy(&bf);
    buffer_destroy(&input);
}

void test_buffer_add_format_buffer()
{
    test_name(__func__);

    struct buffer bf;
    buffer_init(&bf);

    struct buffer bf_in;
    buffer_init(&bf_in);
    buffer_copy_str(&bf_in, "hello");

    buffer_add_format(&bf, "%bf", &bf_in);
    expect_true(buffer_compare(&bf, &bf_in), "compare");

    buffer_destroy(&bf);
    buffer_destroy(&bf_in);
}

void test_buffer_order_same()
{
    test_name(__func__);

    struct buffer a;
    buffer_init(&a);
    buffer_copy_str(&a, "abc");

    struct buffer b;
    buffer_init(&b);
    buffer_copy_str(&b, "abc");

    expect_int_equal(buffer_order(&a, &b), 0, "0");

    buffer_destroy(&a);
    buffer_destroy(&b);
}

void test_buffer_order_less_size()
{
    test_name(__func__);

    struct buffer a;
    buffer_init(&a);
    buffer_copy_str(&a, "abc");

    struct buffer b;
    buffer_init(&b);
    buffer_copy_str(&b, "abc123");

    expect_int_equal(buffer_order(&a, &b), -1, "-1");

    buffer_destroy(&a);
    buffer_destroy(&b);
}

void test_buffer_order_greater_size()
{
    test_name(__func__);

    struct buffer a;
    buffer_init(&a);
    buffer_copy_str(&a, "abc123");

    struct buffer b;
    buffer_init(&b);
    buffer_copy_str(&b, "abc");

    expect_int_equal(buffer_order(&a, &b), 1, "1");

    buffer_destroy(&a);
    buffer_destroy(&b);
}

void test_buffer_order_less_than()
{
    test_name(__func__);

    struct buffer a;
    buffer_init(&a);
    buffer_copy_str(&a, "abcx");

    struct buffer b;
    buffer_init(&b);
    buffer_copy_str(&b, "abcy");

    expect_int_equal(buffer_order(&a, &b), -1, "-1");

    buffer_destroy(&a);
    buffer_destroy(&b);
}

void test_buffer_order_greater_than()
{
    test_name(__func__);

    struct buffer a;
    buffer_init(&a);
    buffer_copy_str(&a, "abcza;llkejrfladf");

    struct buffer b;
    buffer_init(&b);
    buffer_copy_str(&b, "abcykewlaf");

    expect_int_equal(buffer_order(&a, &b), 1, "1");

    buffer_destroy(&a);
    buffer_destroy(&b);
}

/* static-output */
void test_buffer()
{
	test_buffer_init();
	test_buffer_add_char();
    test_buffer_add();
    test_buffer_expand();
	test_buffer_finish();
	test_buffer_clear();
    test_buffer_reset();
	test_buffer_copy();
	test_buffer_buffer2array();
	test_buffer_array2buffer();
	test_buffer_next_char();
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
