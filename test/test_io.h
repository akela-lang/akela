#include "assert.h"
#include "alba/allocator.h"
#include "alba/ustring.h"
#include "alba/io.h"
#include "alba/os_win.h"
#include <windows.h>

void test_io_string()
{
	test_name(__func__);

	struct allocator al;
	struct string s;
	struct string_data sd;
	enum result r;
	io_getchar f;
	io_data d;

	allocator_init(&al);
	string_init(&s);
	r = array2string(&al, "hello", &s);
	assert_ok(r, "array2string");
	string_data_init(&s, &sd);

	f = string_getchar;
	d = &sd;

	struct string s2;
	string_init(&s2);
	int c;
	while ((c = f(d)) != EOF) {
		string_add_char(&al, &s2, c);
	}

	expect_true(string_compare(&s, &s2), "string_compare");

	allocator_destroy(&al);
}

void test_io_file()
{
	test_name(__func__);

	enum result r;
	struct allocator al;
	char* filename;

	allocator_init(&al);

	r = win_temp_filename(&al, &filename);
	assert_ok(r, "win_temp_filename");

	FILE* fp = fopen(filename, "w");
	assert_ptr(fp, "fopen");

	char* str = "x + 5";
	size_t n = fwrite(str, sizeof(char), strlen(str), fp);

	expect_int_equal(n, strlen(str), "fwrite");

	fclose(fp);

	fp = fopen(filename, "r");
	assert_ptr(fp, "fopen");

	io_getchar f = file_getchar;
	io_data d = fp;
	struct string s;

	string_init(&s);
	int c;
	while ((c = f(d)) != EOF) {
		string_add_char(&al, &s, c);
	}

	fclose(fp);

	expect_str(&s, str, "str");

	allocator_destroy(&al);
}

void test_io()
{
	test_io_string();
	test_io_file();
}
