#ifndef _TEST_STRING_H
#define _TEST_STRING_H

#include "setup.h"
#include "assert.h"
#include "alba/ustring.h"

void test_string_init()
{
	test_name(__func__);

	struct string s;
	string_init(&s);
	expect_int_equal(s.size, 0, "size");
	expect_int_equal(s.buf_size, 0, "buf_size");
	expect_null(s.buf, "buf");
}

void test_string()
{
	test_string_init();
}

#endif
