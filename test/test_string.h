#ifndef _TEST_STRING_H
#define _TEST_STRING_H

#include "assert.h"
#include "alba/ustring.h"
#include "alba/scan.h"
#include "alba/uconv.h"
#include <string.h>

void test_string_init()
{
	test_name(__func__);

	struct string s;
	string_init(&s);
	expect_int_equal(s.size, 0, "size");
	expect_int_equal(s.buf_size, 0, "buf_size");
	expect_null(s.buf, "buf");
}

void test_string_add_char()
{
	test_name(__func__);

	struct allocator al;
	struct string s;	
	allocator_init(&al);
	string_init(&s);
	string_add_char(&al, &s, 'x');
	string_add_char(&al, &s, 'y');
	string_add_char(&al, &s, 'z');
	expect_str(&s, "xyz", "str");
	string_reset(&s);
	allocator_destroy(&al);
}

void test_string_clear()
{
	test_name(__func__);

	struct allocator al;
	struct string s;
	allocator_init(&al);
	string_init(&s);
	string_add_char(&al, &s, 'x');
	string_add_char(&al, &s, 'y');
	string_add_char(&al, &s, 'z');
	expect_str(&s, "xyz", "str");
	string_clear(&s);
	expect_str(&s, "", "clear");
	string_reset(&s);
	allocator_destroy(&al);
}

void test_string_copy()
{
	test_name(__func__);

	struct allocator al;
	struct string s;
	struct string s2;

	allocator_init(&al);
	string_init(&s);
	string_add_char(&al, &s, 'x');
	string_add_char(&al, &s, 'y');
	string_add_char(&al, &s, 'z');
	expect_str(&s, "xyz", "str");

	string_init(&s2);
	string_copy(&al, &s, &s2);

	expect_str(&s2, "xyz", "copy");

	string_reset(&s);
	string_reset(&s2);
	allocator_destroy(&al);
}

void test_string_string2array()
{
	test_name(__func__);

	struct allocator al;
	struct string s;
	char *a;

	allocator_init(&al);
	string_init(&s);
	string_add_char(&al, &s, 'x');
	string_add_char(&al, &s, 'y');
	string_add_char(&al, &s, 'z');
	expect_str(&s, "xyz", "str");
	
	string2array(&al, &s, &a);
	expect_true(strcmp(a, "xyz") == 0, "array");

	string_reset(&s);
	allocator_destroy(&al);
}

void test_string_array2string()
{
	test_name(__func__);

	struct allocator al;
	char a[] = "xyz";
	struct string s;

	allocator_init(&al);
	string_init(&s);
	array2string(&al, a, &s);
	expect_str(&s, "xyz", "str");
	string_reset(&s);
	allocator_destroy(&al);
}

void test_string_next_char()
{
	test_name(__func__);

	struct allocator al;
	struct string s;
	allocator_init(&al);
	string_init(&s);
	string_add_char(&al, &s, 'x');
	string_add_char(&al, &s, 'y');
	string_add_char(&al, &s, 'z');

	struct string x;
	string_init(&x);
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

	string_reset(&s);
	string_reset(&x);
	allocator_destroy(&al);
}

test_string_string_compare()
{
	test_name(__func__);

	struct allocator al;
	struct string s;
	allocator_init(&al);
	string_init(&s);
	string_add_char(&al, &s, 'x');
	string_add_char(&al, &s, 'y');
	string_add_char(&al, &s, 'z');

	struct string s2;
	string_init(&s2);
	string_add_char(&al, &s2, 'x');
	string_add_char(&al, &s2, 'y');
	string_add_char(&al, &s2, 'z');

	expect_true(string_compare(&s, &s2), "equal");

	string_clear(&s2);
	string_add_char(&al, &s2, 'x');
	string_add_char(&al, &s2, 'y');
	string_add_char(&al, &s2, '1');

	expect_true(string_compare(&s, &s2) == 0, "not equal");

	string_reset(&s);
	string_reset(&s2);
	allocator_destroy(&al);
}

test_string_str_compare()
{
	test_name(__func__);

	struct allocator al;
	struct string s;
	allocator_init(&al);
	string_init(&s);
	string_add_char(&al, &s, 'x');
	string_add_char(&al, &s, 'y');
	string_add_char(&al, &s, 'z');

	expect_true(str_compare(&s, "xyz") == 1, "equal");
	expect_true(str_compare(&s, "xy1") == 0, "not equal");

	string_reset(&s);
	allocator_destroy(&al);
}

void test_string_get_uchar()
{
	test_name(__func__);

	enum result r;

	struct allocator al;
	allocator_init(&al);

	struct string s;
	string_init(&s);

	struct char_value cv;
	set_char_values(&cv);
	
	r = array2string(&al, "=+- \n*/()", &s);
	assert_ok(r, "array2string");

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
	test_string_init();
	test_string_add_char();
	test_string_clear();
	test_string_copy();
	test_string_string2array();
	test_string_array2string();
	test_string_next_char();
	test_string_string_compare();
	test_string_str_compare();
	test_string_get_uchar();
}

#endif
