#include <string.h>
#include <stdlib.h>
#include "zinc/result.h"
#include "zinc/unit_test.h"
#include "zinc/zstring.h"
#include <limits.h>

/* static-output */
void test_buffer_init()
{
	test_name(__func__);

	struct Zinc_string bf;
	Zinc_string_init(&bf);
	expect_int_equal(bf.size, 0, "size");
	expect_int_equal(bf.buf_size, 0, "buf_size");
	expect_null(bf.buf, "buf");
}

/* static-output */
/* dynamic-temp bf{} */
void test_buffer_add_char()
{
	test_name(__func__);

	struct Zinc_string bf;
	Zinc_string_init(&bf);

	/* allocate bf{} */
	Zinc_string_add_char(&bf, 'x');
	Zinc_string_add_char(&bf, 'y');
	Zinc_string_add_char(&bf, 'z');

	expect_str(&bf, "xyz", "str");

	/* destroy bf{} */
	Zinc_string_destroy(&bf);
}

void test_buffer_add()
{
    test_name(__func__);
    struct Zinc_string* bf = NULL;
    Zinc_string_create(&bf);

    Zinc_string_add(bf, "abc", 3);
    Zinc_string_add(bf, "123", 3);

    expect_str(bf, "abc123", "bf");

    Zinc_string_destroy(bf);
    free(bf);
}

void test_buffer_expand()
{
    test_name(__func__);

    struct Zinc_string* bf = NULL;
    Zinc_string_create(&bf);

    Zinc_string_expand(bf, BUFFER_CHUNK + 10);

    expect_size_t_equal(bf->buf_size, BUFFER_CHUNK + 10, "buf_size");

    Zinc_string_destroy(bf);
    free(bf);
}

void test_buffer_finish()
{
	test_name(__func__);

	struct Zinc_string bf;
	Zinc_string_init(&bf);

	/* allocate bf{} */
	Zinc_string_add_char(&bf, 'x');
	Zinc_string_add_char(&bf, 'y');
	Zinc_string_add_char(&bf, 'z');
	Zinc_string_finish(&bf);

	expect_str(&bf, "xyz", "str");
	expect_true(strcmp(bf.buf, "xyz") == 0, "strcmp");

	/* destroy bf{} */
	Zinc_string_destroy(&bf);
}

/* static-output */
/* dynamic-temp bf{} */
void test_buffer_clear()
{
	test_name(__func__);

	struct Zinc_string bf;
	Zinc_string_init(&bf);

	/* allocate bf{} */
	Zinc_string_add_char(&bf, 'x');
	Zinc_string_add_char(&bf, 'y');
	Zinc_string_add_char(&bf, 'z');

	expect_str(&bf, "xyz", "str");
	Zinc_string_clear(&bf);
	expect_str(&bf, "", "clear");

	/* destroy bf{} */
	Zinc_string_destroy(&bf);
}

void test_buffer_reset()
{
    test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Zinc_string_add_str(&bf, "hello");

    expect_str(&bf, "hello", "hello");
    Zinc_string_reset(&bf);

    expect_str(&bf, "", "");

    Zinc_string_destroy(&bf);
}

/* static-output */
/* dynamic-temp bf{} bf2{} */
void test_buffer_copy()
{
	test_name(__func__);

	struct Zinc_string bf;
	struct Zinc_string bf2;

	Zinc_string_init(&bf);

	/* allocate bf{} */
	Zinc_string_add_char(&bf, 'x');
	Zinc_string_add_char(&bf, 'y');
	Zinc_string_add_char(&bf, 'z');

	expect_str(&bf, "xyz", "str");

	Zinc_string_init(&bf2);

	/* allocate bf2{} */
	Zinc_string_copy(&bf, &bf2);

	expect_str(&bf2, "xyz", "copy");

	/* destroy bf{} bf2{} */
	Zinc_string_destroy(&bf);
	Zinc_string_destroy(&bf2);
}

/* static-output */
/* dynamic-temp bf{} a */
void test_buffer_buffer2array()
{
	test_name(__func__);

	struct Zinc_string bf;
	char* a;

	Zinc_string_init(&bf);

	/* allocate bf{} */
	Zinc_string_add_char(&bf, 'x');
	Zinc_string_add_char(&bf, 'y');
	Zinc_string_add_char(&bf, 'z');

	expect_str(&bf, "xyz", "str");

	/* allocate a */
	Zinc_string_create_str(&bf, &a);

	expect_true(strcmp(a, "xyz") == 0, "array");

	/* destroy bf{} a */
	Zinc_string_destroy(&bf);
	free(a);
}

/* static-output */
/* dynamic-temp bf{} */
void test_buffer_array2buffer()
{
	test_name(__func__);

	char a[] = "xyz";
	struct Zinc_string bf;

	Zinc_string_init(&bf);

	/* allocate bf{} */
	Zinc_string_add_str2(a, &bf);

	expect_str(&bf, "xyz", "str");


	/* destroy bf{} */
	Zinc_string_destroy(&bf);
}

/* static-output */
/* dynamic-temp bf{} x{} */
void test_buffer_next_char()
{
	test_name(__func__);

	enum result r;
	struct Zinc_string bf;
	Zinc_string_init(&bf);

	/* allocate bf{} */
	Zinc_string_add_char(&bf, 'x');
	Zinc_string_add_char(&bf, 'y');
	Zinc_string_add_char(&bf, 'z');

	struct Zinc_string x;
	Zinc_string_init(&x);
	size_t pos = 0;

	/* allocate x{} */
	r = Zinc_next_char(&bf, &pos, &x);
	assert_ok(r, "next_char");

	expect_str(&x, "x", "x");

	/* allocate x{} */
	r = Zinc_next_char(&bf, &pos, &x);
	assert_ok(r, "y");

	expect_str(&x, "y", "y");

	/* allocate x{} */
	r = Zinc_next_char(&bf, &pos, &x);
	assert_ok(r, "z");

	expect_str(&x, "z", "z");

	/* destroy bf{} x{} */
	Zinc_string_destroy(&bf);
	Zinc_string_destroy(&x);
}

/* static-output */
/* dynamic bf{} bf2{} */
void test_buffer_buffer_compare()
{
	test_name(__func__);

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

	expect_true(Zinc_string_compare(&bf, &bf2), "equal");

	Zinc_string_clear(&bf2);

	/* allocate bf2{} */
	Zinc_string_add_char(&bf2, 'x');
	Zinc_string_add_char(&bf2, 'y');
	Zinc_string_add_char(&bf2, '1');

	expect_true(Zinc_string_compare(&bf, &bf2) == 0, "not equal");

	/* destroy bf{} bf2{} */
	Zinc_string_destroy(&bf);
	Zinc_string_destroy(&bf2);
}

/* static-output */
/* dynamic-temp bf{} */
void test_buffer_str_compare()
{
	test_name(__func__);

	struct Zinc_string bf;
	Zinc_string_init(&bf);

	/* allocate bf{} */
	Zinc_string_add_char(&bf, 'x');
	Zinc_string_add_char(&bf, 'y');
	Zinc_string_add_char(&bf, 'z');

	expect_true(Zinc_string_compare_str(&bf, "xyz") == 1, "equal");
	expect_true(Zinc_string_compare_str(&bf, "xy1") == 0, "not equal");

	/* destroy bf{} */
	Zinc_string_destroy(&bf);
}

/* static-output */
/* dynamic-temp bf{} bf2{} */
void test_buffer_uslice()
{
	test_name(__func__);

	struct Zinc_string bf;
	struct Zinc_string bf2;
	enum result r;

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
	assert_ok(r, "buffer_uslice");

	assert_ptr(bf2.buf, "ptr buf2.buf");
	expect_size_t_equal(bf2.size, 3, "3 bf2.size");
	expect_str(&bf2, "cde", "cde bf2");

	/* destory bf{} bf2{} */
	Zinc_string_destroy(&bf);
	Zinc_string_destroy(&bf2);
}

/* static-output */
/* dynamic bf{} bf2{} */
void test_buffer_uslice2()
{
	test_name(__func__);

	struct Zinc_string bf;
	struct Zinc_string bf2;
	enum result r;

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
	assert_ok(r, "buffer_uslice");

	assert_ptr(bf2.buf, "ptr buf2.buf");
	expect_int_equal(bf2.size, 4, "4 bf2.size");
	expect_str(&bf2, "cdef", "cdef bf2");

	/* destroy bf{} bf2{} */
	Zinc_string_destroy(&bf);
	Zinc_string_destroy(&bf2);
}

void test_buffer_add_format()
{
    test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Zinc_string_add_format(&bf, "%% %c %s %d %lf", 'x', "hello", 10, 5.1);
    expect_str(&bf, "% x hello 10 5.100000", "bf");

    Zinc_string_destroy(&bf);
}

void test_buffer_add_format_d_max()
{
    test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    int d = INT_MAX;
    Zinc_string_add_format(&bf, "%d", d);
    Zinc_string_finish(&bf);
    expect_int_equal(atoi(bf.buf), d, "compare");

    Zinc_string_destroy(&bf);
}

void test_buffer_add_format_d_min()
{
    test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    int d = INT_MIN;
    Zinc_string_add_format(&bf, "%d", d);
    Zinc_string_finish(&bf);
    expect_int_equal((int)strtol(bf.buf, NULL, 10), d, "compare");

    Zinc_string_destroy(&bf);
}

void test_buffer_add_format_zu_max()
{
    test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    size_t zu = ULONG_MAX;
    Zinc_string_add_format(&bf, "%zu", zu);
    Zinc_string_finish(&bf);
    expect_size_t_equal(strtoul(bf.buf, NULL, 10), zu, "compare");

    Zinc_string_destroy(&bf);
}

void test_buffer_add_format_zu_min()
{
    test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    size_t zu = 0;
    Zinc_string_add_format(&bf, "%zu", zu);
    Zinc_string_finish(&bf);
    expect_size_t_equal(strtol(bf.buf, NULL, 10), zu, "compare");

    Zinc_string_destroy(&bf);
}

void test_buffer_add_format_s_large()
{
    test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    struct Zinc_string input;
    Zinc_string_init(&input);
    for (int i = 0; i < BUFFER_CHUNK * 2 + 10; i++) {
        Zinc_string_add_char(&input, 'x');
    }
    Zinc_string_finish(&input);

    Zinc_string_add_format(&bf, "%s", input.buf);
    expect_true(Zinc_string_compare(&bf, &input), "compare");

    Zinc_string_destroy(&bf);
    Zinc_string_destroy(&input);
}

void test_buffer_add_format_buffer()
{
    test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    struct Zinc_string bf_in;
    Zinc_string_init(&bf_in);
    Zinc_string_add_str(&bf_in, "hello");

    Zinc_string_add_format(&bf, "%bf", &bf_in);
    expect_true(Zinc_string_compare(&bf, &bf_in), "compare");

    Zinc_string_destroy(&bf);
    Zinc_string_destroy(&bf_in);
}

void test_buffer_order_same()
{
    test_name(__func__);

    struct Zinc_string a;
    Zinc_string_init(&a);
    Zinc_string_add_str(&a, "abc");

    struct Zinc_string b;
    Zinc_string_init(&b);
    Zinc_string_add_str(&b, "abc");

    expect_int_equal(Zinc_string_order(&a, &b), 0, "0");

    Zinc_string_destroy(&a);
    Zinc_string_destroy(&b);
}

void test_buffer_order_less_size()
{
    test_name(__func__);

    struct Zinc_string a;
    Zinc_string_init(&a);
    Zinc_string_add_str(&a, "abc");

    struct Zinc_string b;
    Zinc_string_init(&b);
    Zinc_string_add_str(&b, "abc123");

    expect_int_equal(Zinc_string_order(&a, &b), -1, "-1");

    Zinc_string_destroy(&a);
    Zinc_string_destroy(&b);
}

void test_buffer_order_greater_size()
{
    test_name(__func__);

    struct Zinc_string a;
    Zinc_string_init(&a);
    Zinc_string_add_str(&a, "abc123");

    struct Zinc_string b;
    Zinc_string_init(&b);
    Zinc_string_add_str(&b, "abc");

    expect_int_equal(Zinc_string_order(&a, &b), 1, "1");

    Zinc_string_destroy(&a);
    Zinc_string_destroy(&b);
}

void test_buffer_order_less_than()
{
    test_name(__func__);

    struct Zinc_string a;
    Zinc_string_init(&a);
    Zinc_string_add_str(&a, "abcx");

    struct Zinc_string b;
    Zinc_string_init(&b);
    Zinc_string_add_str(&b, "abcy");

    expect_int_equal(Zinc_string_order(&a, &b), -1, "-1");

    Zinc_string_destroy(&a);
    Zinc_string_destroy(&b);
}

void test_buffer_order_greater_than()
{
    test_name(__func__);

    struct Zinc_string a;
    Zinc_string_init(&a);
    Zinc_string_add_str(&a, "abcza;llkejrfladf");

    struct Zinc_string b;
    Zinc_string_init(&b);
    Zinc_string_add_str(&b, "abcykewlaf");

    expect_int_equal(Zinc_string_order(&a, &b), 1, "1");

    Zinc_string_destroy(&a);
    Zinc_string_destroy(&b);
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
