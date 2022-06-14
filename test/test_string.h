#ifndef _TEST_STRING_H
#define _TEST_STRING_H

#include "assert.h"
#include "alba/buffer.h"
#include "alba/scan.h"
#include "alba/uconv.h"
#include <string.h>

void test_buffer_init()
{
	test_name(__func__);

	struct buffer s;
	buffer_init(&s);
	expect_int_equal(s.size, 0, "size");
	expect_int_equal(s.buf_size, 0, "buf_size");
	expect_null(s.buf, "buf");
}

void test_buffer_add_char()
{
	test_name(__func__);

	struct allocator al;
	struct buffer s;	
	allocator_init(&al);
	buffer_init(&s);
	buffer_add_char(&al, &s, 'x');
	buffer_add_char(&al, &s, 'y');
	buffer_add_char(&al, &s, 'z');
	expect_str(&s, "xyz", "str");
	buffer_reset(&s);
	allocator_destroy(&al);
}

void test_buffer_clear()
{
	test_name(__func__);

	struct allocator al;
	struct buffer s;
	allocator_init(&al);
	buffer_init(&s);
	buffer_add_char(&al, &s, 'x');
	buffer_add_char(&al, &s, 'y');
	buffer_add_char(&al, &s, 'z');
	expect_str(&s, "xyz", "str");
	buffer_clear(&s);
	expect_str(&s, "", "clear");
	buffer_reset(&s);
	allocator_destroy(&al);
}

void test_buffer_copy()
{
	test_name(__func__);

	struct allocator al;
	struct buffer s;
	struct buffer s2;

	allocator_init(&al);
	buffer_init(&s);
	buffer_add_char(&al, &s, 'x');
	buffer_add_char(&al, &s, 'y');
	buffer_add_char(&al, &s, 'z');
	expect_str(&s, "xyz", "str");

	buffer_init(&s2);
	buffer_copy(&al, &s, &s2);

	expect_str(&s2, "xyz", "copy");

	buffer_reset(&s);
	buffer_reset(&s2);
	allocator_destroy(&al);
}

void test_string_buffer2array()
{
	test_name(__func__);

	struct allocator al;
	struct buffer s;
	char *a;

	allocator_init(&al);
	buffer_init(&s);
	buffer_add_char(&al, &s, 'x');
	buffer_add_char(&al, &s, 'y');
	buffer_add_char(&al, &s, 'z');
	expect_str(&s, "xyz", "str");
	
	buffer2array(&al, &s, &a);
	expect_true(strcmp(a, "xyz") == 0, "array");

	buffer_reset(&s);
	allocator_destroy(&al);
}

void test_string_array2buffer()
{
	test_name(__func__);

	struct allocator al;
	char a[] = "xyz";
	struct buffer s;

	allocator_init(&al);
	buffer_init(&s);
	array2buffer(&al, a, &s);
	expect_str(&s, "xyz", "str");
	buffer_reset(&s);
	allocator_destroy(&al);
}

void test_string_next_char()
{
	test_name(__func__);

	struct allocator al;
	struct buffer s;
	allocator_init(&al);
	buffer_init(&s);
	buffer_add_char(&al, &s, 'x');
	buffer_add_char(&al, &s, 'y');
	buffer_add_char(&al, &s, 'z');

	struct buffer x;
	buffer_init(&x);
	size_t pos = 0;
	enum result r;

	r = next_char(&al, &s, &pos, &x);
	assert_ok(r, "r");
	expect_str(&x, "x", "x");

	r = next_char(&al, &s, &pos, &x);
	assert_ok(r, "r2");
	expect_str(&x, "y", "y");

	r = next_char(&al, &s, &pos, &x);
	assert_ok(r, "r3");
	expect_str(&x, "z", "z");

	buffer_reset(&s);
	buffer_reset(&x);
	allocator_destroy(&al);
}

test_string_buffer_compare()
{
	test_name(__func__);

	struct allocator al;
	struct buffer s;
	allocator_init(&al);
	buffer_init(&s);
	buffer_add_char(&al, &s, 'x');
	buffer_add_char(&al, &s, 'y');
	buffer_add_char(&al, &s, 'z');

	struct buffer s2;
	buffer_init(&s2);
	buffer_add_char(&al, &s2, 'x');
	buffer_add_char(&al, &s2, 'y');
	buffer_add_char(&al, &s2, 'z');

	expect_true(buffer_compare(&s, &s2), "equal");

	buffer_clear(&s2);
	buffer_add_char(&al, &s2, 'x');
	buffer_add_char(&al, &s2, 'y');
	buffer_add_char(&al, &s2, '1');

	expect_true(buffer_compare(&s, &s2) == 0, "not equal");

	buffer_reset(&s);
	buffer_reset(&s2);
	allocator_destroy(&al);
}

test_string_str_compare()
{
	test_name(__func__);

	struct allocator al;
	struct buffer s;
	allocator_init(&al);
	buffer_init(&s);
	buffer_add_char(&al, &s, 'x');
	buffer_add_char(&al, &s, 'y');
	buffer_add_char(&al, &s, 'z');

	expect_true(str_compare(&s, "xyz") == 1, "equal");
	expect_true(str_compare(&s, "xy1") == 0, "not equal");

	buffer_reset(&s);
	allocator_destroy(&al);
}

void test_string_get_uchar()
{
	test_name(__func__);

	enum result r;

	struct allocator al;
	allocator_init(&al);

	struct buffer s;
	buffer_init(&s);

	struct char_value cv;
	set_char_values(&cv);
	
	r = array2buffer(&al, "=+- \n*/()", &s);
	assert_ok(r, "array2buffer");

	struct string_data sd;
	string_data_init(&s, &sd);

	UConverter* conv;
	r = conv_open(&conv);
	assert_ok(r, "conv_open");

	struct input_state is;
	input_state_init(string_getchar, &sd, conv, &is);

	r = get_uchar(&al,&is);
	assert_ok(r, "get_uchar equal");
	assert_int_equal(is.uc, cv.equal, "equal");
	assert_true(!is.done, "done equal");

	r = get_uchar(&al, &is);
	assert_ok(r, "get_uchar plus");
	assert_int_equal(is.uc, cv.plus, "plus");
	assert_true(!is.done, "done plus");

	r = get_uchar(&al, &is);
	assert_ok(r, "get_uchar minus");
	assert_int_equal(is.uc, cv.minus, "minus");
	assert_true(!is.done, "done minus");

	r = get_uchar(&al, &is);
	assert_ok(r, "get_uchar space");
	assert_int_equal(is.uc, cv.space, "space");
	assert_true(!is.done, "done space");

	r = get_uchar(&al, &is);
	assert_ok(r, "get_uchar newline");
	assert_int_equal(is.uc, cv.newline, "newline");
	assert_true(!is.done, "done newline");

	r = get_uchar(&al, &is);
	assert_ok(r, "get_uchar mult");
	assert_int_equal(is.uc, cv.mult, "mult");
	assert_true(!is.done, "done mult");

	r = get_uchar(&al, &is);
	assert_ok(r, "get_uchar divide");
	assert_int_equal(is.uc, cv.divide, "divide");
	assert_true(!is.done, "done divide");

	r = get_uchar(&al, &is);
	assert_ok(r, "get_uchar left_paren");
	assert_int_equal(is.uc, cv.left_paren, "left_paren");
	assert_true(!is.done, "done left_paren");

	r = get_uchar(&al, &is);
	assert_ok(r, "get_uchar right_paren");
	assert_int_equal(is.uc, cv.right_paren, "right_paren");
	assert_true(!is.done, "done right_paren");

	r = get_uchar(&al, &is);
	assert_ok(r, "get_uchar done");
	assert_true(is.done, "done done");

	allocator_destroy(&al);
	conv_close(conv);
}

void test_string()
{
	test_buffer_init();
	test_buffer_add_char();
	test_buffer_clear();
	test_buffer_copy();
	test_string_buffer2array();
	test_string_array2buffer();
	test_string_next_char();
	test_string_buffer_compare();
	test_string_str_compare();
	test_string_get_uchar();
}

#endif
