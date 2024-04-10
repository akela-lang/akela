#include "zinc/unit_test.h"
#include "zinc/buffer.h"
#include "akela/input.h"
#include "akela/os_win.h"
#include "akela/lex.h"
#include "akela/uconv.h"

#if defined(WIN32)
#include <windows.h>
#endif

/* dynamic-output-none */
/* dynamic-temp bf{} bf2{} */
void test_input_string()
{
	test_name(__func__);

	struct buffer bf;
	struct string_data sd;
	input_getchar f;
	input_data d;

	buffer_init(&bf);

	/* allocate bf{} */
	array2buffer("hello", &bf);
	string_data_init(&bf, &sd);

	f = (input_getchar)string_getchar;
	d = &sd;

	struct buffer bf2;
	buffer_init(&bf2);

	/* allocate bf2{} */
	int c;
	while ((c = f(d)) != EOF) {
		buffer_add_char(&bf2, c);
	}

	expect_true(buffer_compare(&bf, &bf2), "buffer_compare");

	/* destroy bf{} bf2{} */
	buffer_destroy(&bf);
	buffer_destroy(&bf2);
}

#if defined(WIN32)

/* dynamic-output-none */
/* dynamic-temp bf{} */
void test_input_file()
{
	test_name(__func__);

	enum result r;
	char* filename;

	/* allocate filename */
	r = win_temp_filename(&filename);
	assert_ok(r, "win_temp_filename");

	/* open resource fp */
	FILE* fp = NULL;
	int err = fopen_s(&fp, filename, "w");
	assert_int_equal(err, 0, "err");
	assert_ptr(fp, "fopen");

	const char* str = "x + 5";
	size_t n = fwrite(str, sizeof(char), strlen(str), fp);

	expect_int_equal(n, strlen(str), "fwrite");

	/* close resource fp */
	fclose(fp);

	/* open resource fp */
	err = fopen_s(&fp, filename, "r");
	assert_int_equal(err, 0, "err");
	assert_ptr(fp, "fopen");

	input_getchar f = (input_getchar)file_getchar;
	input_data d = fp;
	struct buffer bf;

	buffer_init(&bf);
	int c;
	while ((c = f(d)) != EOF) {
		/* allocate bf{} */
		buffer_add_char(&bf, c);
	}

	/* close resource fp */
	fclose(fp);

	expect_str(&bf, str, "str");

	/* destroy filename bf{} */
	free(filename);
	buffer_destroy(&bf);
}

#endif

/* dynamic-output-none */
void test_input()
{
	test_input_string();

#if defined(WIN32)
	test_input_file();
#endif

}
