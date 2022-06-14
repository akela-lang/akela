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
	io_getchar f;
	io_data d;

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

	io_getchar f = file_getchar;
	io_data d = fp;
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

void test_input_get_uchar()
{
	test_name(__func__);

	enum result r;

	struct allocator al;
	allocator_init(&al);

	struct buffer bf;
	buffer_init(&bf);

	struct char_value cv;
	set_char_values(&cv);

	r = array2buffer(&al, "=+- \n*/()", &bf);
	assert_ok(r, "array2buffer");

	struct string_data sd;
	string_data_init(&bf, &sd);

	UConverter* conv;
	r = conv_open(&conv);
	assert_ok(r, "conv_open");

	struct input_state is;
	input_state_init(string_getchar, &sd, conv, &is);

	r = get_uchar(&al, &is);
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

void test_input()
{
	test_input_string();
	test_input_file();
	test_input_get_uchar();
}
