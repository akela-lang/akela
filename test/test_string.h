#ifndef _TEST_STRING_H
#define _TEST_STRING_H

#include "setup.h"
#include "assert.h"
#include "alba/ustring.h"
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

	struct string s;
	string_init(&s);
	string_add_char(&al, &s, 'x');
	string_add_char(&al, &s, 'y');
	string_add_char(&al, &s, 'z');
	expect_str(&s, "xyz", "str");
	string_reset(&s);
}

void test_string_clear()
{
	test_name(__func__);

	struct string s;
	string_init(&s);
	string_add_char(&al, &s, 'x');
	string_add_char(&al, &s, 'y');
	string_add_char(&al, &s, 'z');
	expect_str(&s, "xyz", "str");
	string_clear(&s);
	expect_str(&s, "", "clear");
	string_reset(&s);
}

void test_string_copy()
{
	test_name(__func__);

	struct string s;
	struct string s2;

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
}

void test_string_string2array()
{
	test_name(__func__);

	struct string s;
	char *a;

	string_init(&s);
	string_add_char(&al, &s, 'x');
	string_add_char(&al, &s, 'y');
	string_add_char(&al, &s, 'z');
	expect_str(&s, "xyz", "str");
	
	string2array(&al, &s, &a);
	expect_true(strcmp(a, "xyz") == 0, "array");

	string_reset(&s);
	free(a);
}

void test_string_array2string()
{
	test_name(__func__);

	char a[] = "xyz";
	struct string s;

	string_init(&s);
	array2string(&al, a, &s);
	expect_str(&s, "xyz", "str");
	string_reset(&s);
}

void test_string_next_char()
{
	test_name(__func__);

	struct string s;
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
}

test_string_string_compare()
{
	test_name(__func__);

	struct string s;
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
}

test_string_str_compare()
{
	test_name(__func__);

	struct string s;
	string_init(&s);
	string_add_char(&al, &s, 'x');
	string_add_char(&al, &s, 'y');
	string_add_char(&al, &s, 'z');

	expect_true(str_compare(&s, "xyz") == 1, "equal");
	expect_true(str_compare(&s, "xy1") == 0, "not equal");

	string_reset(&s);
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
}

#endif
