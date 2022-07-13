#include "assert.h"
#include "alba/allocator.h"
#include "alba/buffer.h"
#include "alba/input.h"
#include "alba/os_win.h"
#include "alba/scan.h"
#include "alba/uconv.h"
#include <windows.h>

void test_input_string()
{
	test_name(__func__);

	struct allocator al;
	struct buffer bf;
	struct string_data sd;
	enum result r;
	input_getchar f;
	input_data d;

	allocator_init(&al);
	buffer_init(&bf);
	r = array2buffer(&al, "hello", &bf);
	assert_ok(r, "array2buffer");
	string_data_init(&bf, &sd);

	f = string_getchar;
	d = &sd;

	struct buffer bf2;
	buffer_init(&bf2);
	int c;
	while ((c = f(d)) != EOF) {
		buffer_add_char(&al, &bf2, c);
	}

	expect_true(buffer_compare(&bf, &bf2), "buffer_compare");

	allocator_destroy(&al);
}

void test_input_file()
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

	input_getchar f = file_getchar;
	input_data d = fp;
	struct buffer bf;

	buffer_init(&bf);
	int c;
	while ((c = f(d)) != EOF) {
		buffer_add_char(&al, &bf, c);
	}

	fclose(fp);

	expect_str(&bf, str, "str");

	allocator_destroy(&al);
}

void test_input()
{
	test_input_string();
	test_input_file();
}
