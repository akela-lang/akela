#include "zinc/unit_test.h"
#include "zinc/input_unicode_string.h"
#include <string.h>

void test_input_unicode_string_next()
{
    Zinc_test_name(__func__);

    char s[] = "hello\nworld";
    Zinc_vector* vector = NULL;
    Zinc_vector_create(&vector, sizeof(char));
    Zinc_vector_add(vector, s, strlen(s));
    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, vector);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    enum Zinc_result r;
    bool done;
    char c[4];
    int num;
    struct Zinc_location loc;

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "0 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "0 done");
    Zinc_expect_int_equal(num, 1, "0 num");
    Zinc_expect_char_equal(c[0], 'h', "0 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 0, "0 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 1, "0 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "0 line");
    Zinc_expect_size_t_equal(loc.col, 1, "0 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "1 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "1 done");
    Zinc_expect_int_equal(num, 1, "1 num");
    Zinc_expect_char_equal(c[0], 'e', "1 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 1, "1 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 2, "1 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "1 line");
    Zinc_expect_size_t_equal(loc.col, 2, "1 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "2 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "2 done");
    Zinc_expect_int_equal(num, 1, "2 num");
    Zinc_expect_char_equal(c[0], 'l', "2 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 2, "2 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 3, "2 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "2 line");
    Zinc_expect_size_t_equal(loc.col, 3, "2 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "3 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "3 done");
    Zinc_expect_int_equal(num, 1, "3 num");
    Zinc_expect_char_equal(c[0], 'l', "3 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 3, "3 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 4, "3 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "3 line");
    Zinc_expect_size_t_equal(loc.col, 4, "3 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "4 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "4 done");
    Zinc_expect_int_equal(num, 1, "4 num");
    Zinc_expect_char_equal(c[0], 'o', "4 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 4, "4 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 5, "4 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "4 line");
    Zinc_expect_size_t_equal(loc.col, 5, "4 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "5 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "5 done");
    Zinc_expect_int_equal(num, 1, "5 num");
    Zinc_expect_char_equal(c[0], '\n', "5 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 5, "5 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 6, "5 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "5 line");
    Zinc_expect_size_t_equal(loc.col, 6, "5 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "6 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "6 done");
    Zinc_expect_int_equal(num, 1, "6 num");
    Zinc_expect_char_equal(c[0], 'w', "6 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 6, "6 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 7, "6 end_pos");
    Zinc_expect_size_t_equal(loc.line, 2, "6 line");
    Zinc_expect_size_t_equal(loc.col, 1, "6 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "7 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "7 done");
    Zinc_expect_int_equal(num, 1, "7 num");
    Zinc_expect_char_equal(c[0], 'o', "7 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 7, "7 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 8, "7 end_pos");
    Zinc_expect_size_t_equal(loc.line, 2, "7 line");
    Zinc_expect_size_t_equal(loc.col, 2, "7 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "8 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "8 done");
    Zinc_expect_int_equal(num, 1, "8 num");
    Zinc_expect_char_equal(c[0], 'r', "8 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 8, "8 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 9, "8 end_pos");
    Zinc_expect_size_t_equal(loc.line, 2, "8 line");
    Zinc_expect_size_t_equal(loc.col, 3, "8 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "9 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "9 done");
    Zinc_expect_int_equal(num, 1, "9 num");
    Zinc_expect_char_equal(c[0], 'l', "9 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 9, "0 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 10, "9 end_pos");
    Zinc_expect_size_t_equal(loc.line, 2, "9 line");
    Zinc_expect_size_t_equal(loc.col, 4, "9 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "10 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "10 done");
    Zinc_expect_int_equal(num, 1, "10 num");
    Zinc_expect_char_equal(c[0], 'd', "10 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 10, "10 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 11, "10 end_pos");
    Zinc_expect_size_t_equal(loc.line, 2, "10 line");
    Zinc_expect_size_t_equal(loc.col, 5, "10 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "11 input unicode next");
    Zinc_expect_true(done, "11 done");
    Zinc_expect_int_equal(num, 0, "11 num");
    Zinc_expect_size_t_equal(loc.start_pos, 11, "10 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 12, "10 end_pos");
    Zinc_expect_size_t_equal(loc.line, 2, "10 line");
    Zinc_expect_size_t_equal(loc.col, 6, "10 col");

    Zinc_expect_string(&bf, "hello\nworld", "bf");

    free(input);
    Zinc_vector_destroy(vector);
    free(vector);
    Zinc_string_destroy(&bf);
}

void test_input_unicode_string_next_multibyte()
{
    Zinc_test_name(__func__);

    char s[] = "hello\nαβγ";
    Zinc_vector* vector = NULL;
    Zinc_vector_create(&vector, sizeof(char));
    Zinc_vector_add(vector, s, strlen(s));
    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, vector);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    enum Zinc_result r;
    bool done;
    char c[4];
    int num;
    struct Zinc_location loc;

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "0 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "0 done");
    Zinc_expect_int_equal(num, 1, "0 num");
    Zinc_expect_char_equal(c[0], 'h', "0 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 0, "0 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 1, "0 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "0 line");
    Zinc_expect_size_t_equal(loc.col, 1, "0 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "1 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "1 done");
    Zinc_expect_int_equal(num, 1, "1 num");
    Zinc_expect_char_equal(c[0], 'e', "1 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 1, "1 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 2, "1 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "1 line");
    Zinc_expect_size_t_equal(loc.col, 2, "1 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "2 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "2 done");
    Zinc_expect_int_equal(num, 1, "2 num");
    Zinc_expect_char_equal(c[0], 'l', "2 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 2, "2 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 3, "2 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "2 line");
    Zinc_expect_size_t_equal(loc.col, 3, "2 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "3 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "3 done");
    Zinc_expect_int_equal(num, 1, "3 num");
    Zinc_expect_char_equal(c[0], 'l', "3 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 3, "3 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 4, "3 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "3 line");
    Zinc_expect_size_t_equal(loc.col, 4, "3 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "4 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "4 done");
    Zinc_expect_int_equal(num, 1, "4 num");
    Zinc_expect_char_equal(c[0], 'o', "4 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 4, "4 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 5, "4 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "4 line");
    Zinc_expect_size_t_equal(loc.col, 5, "4 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "5 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "5 done");
    Zinc_expect_int_equal(num, 1, "5 num");
    Zinc_expect_char_equal(c[0], '\n', "5 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 5, "5 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 6, "5 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "5 line");
    Zinc_expect_size_t_equal(loc.col, 6, "5 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "6 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "6 done");
    Zinc_expect_int_equal(num, 2, "6 num");
    Zinc_expect_char_equal(c[0], "α"[0], "6 char 0");
    Zinc_expect_char_equal(c[1], "α"[1], "6 char 1");
    Zinc_expect_size_t_equal(loc.start_pos, 6, "6 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 8, "6 size");
    Zinc_expect_size_t_equal(loc.line, 2, "6 line");
    Zinc_expect_size_t_equal(loc.col, 1, "6 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "7 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "7 done");
    Zinc_expect_int_equal(num, 2, "7 num");
    Zinc_expect_char_equal(c[0], "β"[0], "7 char 0");
    Zinc_expect_char_equal(c[1], "β"[1], "7 char 1");
    Zinc_expect_size_t_equal(loc.start_pos, 8, "7 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 10, "7 end_pos");
    Zinc_expect_size_t_equal(loc.line, 2, "7 line");
    Zinc_expect_size_t_equal(loc.col, 2, "7 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "8 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "8 done");
    Zinc_expect_int_equal(num, 2, "8 num");
    Zinc_expect_char_equal(c[0], "γ"[0], "8 char 0");
    Zinc_expect_char_equal(c[1], "γ"[1], "8 char 1");
    Zinc_expect_size_t_equal(loc.start_pos, 10, "8 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 12, "8 end_pos");
    Zinc_expect_size_t_equal(loc.line, 2, "8 line");
    Zinc_expect_size_t_equal(loc.col, 3, "8 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "9 input unicode next");
    Zinc_expect_true(done, "9 done");
    Zinc_expect_int_equal(num, 0, "9 num");
    Zinc_expect_size_t_equal(loc.start_pos, 12, "6 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 13, "8 end_pos");
    Zinc_expect_size_t_equal(loc.line, 2, "8 line");
    Zinc_expect_size_t_equal(loc.col, 4, "8 col");

    Zinc_expect_string(&bf, "hello\nαβγ", "bf");

    free(input);
    Zinc_vector_destroy(vector);
    free(vector);
    Zinc_string_destroy(&bf);
}

void test_input_unicode_string_repeat()
{
    Zinc_test_name(__func__);

    char s[] = "hello";
    Zinc_vector* vector = NULL;
    Zinc_vector_create(&vector, sizeof(char));
    Zinc_vector_add(vector, s, strlen(s));
    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, vector);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    enum Zinc_result r;
    bool done;
    char c[4];
    int num;
    struct Zinc_location loc;

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "0 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "0 done");
    Zinc_expect_int_equal(num, 1, "0 num");
    Zinc_expect_char_equal(c[0], 'h', "0 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 0, "0 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 1, "0 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "0 line");
    Zinc_expect_size_t_equal(loc.col, 1, "0 col");

    Zinc_input_unicode_repeat(input, input->input_vtable);
    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "0r input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "0r done");
    Zinc_expect_int_equal(num, 1, "0r num");
    Zinc_expect_char_equal(c[0], 'h', "0r char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 0, "0r start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 1, "0r end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "0r line");
    Zinc_expect_size_t_equal(loc.col, 1, "0r col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "1 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "1 done");
    Zinc_expect_int_equal(num, 1, "1 num");
    Zinc_expect_char_equal(c[0], 'e', "1 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 1, "1 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 2, "1 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "1 line");
    Zinc_expect_size_t_equal(loc.col, 2, "1 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "2 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "2 done");
    Zinc_expect_int_equal(num, 1, "2 num");
    Zinc_expect_char_equal(c[0], 'l', "2 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 2, "2 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 3, "2 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "2 line");
    Zinc_expect_size_t_equal(loc.col, 3, "2 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "3 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "3 done");
    Zinc_expect_int_equal(num, 1, "3 num");
    Zinc_expect_char_equal(c[0], 'l', "3 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 3, "3 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 4, "3 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "3 line");
    Zinc_expect_size_t_equal(loc.col, 4, "3 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "4 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "4 done");
    Zinc_expect_int_equal(num, 1, "4 num");
    Zinc_expect_char_equal(c[0], 'o', "4 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 4, "4 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 5, "4 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "4 line");
    Zinc_expect_size_t_equal(loc.col, 5, "4 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "5 input unicode next");
    Zinc_expect_true(done, "5 done");
    Zinc_expect_int_equal(num, 0, "5 num");
    Zinc_expect_size_t_equal(loc.start_pos, 5, "5 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 6, "5 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "5 line");
    Zinc_expect_size_t_equal(loc.col, 6, "5 col");

    Zinc_expect_string(&bf, "hhello", "bf");

    free(input);
    Zinc_vector_destroy(vector);
    free(vector);
    Zinc_string_destroy(&bf);
}

void test_input_unicode_string_seek()
{
    Zinc_test_name(__func__);

    char s[] = "hello";
    Zinc_vector* vector = NULL;
    Zinc_vector_create(&vector, sizeof(char));
    Zinc_vector_add(vector, s, strlen(s));
    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, vector);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    enum Zinc_result r;
    bool done;
    char c[4];
    int num;
    struct Zinc_location loc;

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "0 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "0 done");
    Zinc_expect_int_equal(num, 1, "0 num");
    Zinc_expect_char_equal(c[0], 'h', "0 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 0, "0 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 1, "0 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "0 line");
    Zinc_expect_size_t_equal(loc.col, 1, "0 col");

    struct Zinc_location save_loc = loc;

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "1 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "1 done");
    Zinc_expect_int_equal(num, 1, "1 num");
    Zinc_expect_char_equal(c[0], 'e', "1 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 1, "1 start_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "1 line");
    Zinc_expect_size_t_equal(loc.end_pos, 2, "1 end_pos");
    Zinc_expect_size_t_equal(loc.col, 2, "1 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "2 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "2 done");
    Zinc_expect_int_equal(num, 1, "2 num");
    Zinc_expect_char_equal(c[0], 'l', "2 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 2, "2 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 3, "2 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "2 line");
    Zinc_expect_size_t_equal(loc.col, 3, "2 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "3 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "3 done");
    Zinc_expect_int_equal(num, 1, "3 num");
    Zinc_expect_char_equal(c[0], 'l', "3 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 3, "3 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 4, "3 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "3 line");
    Zinc_expect_size_t_equal(loc.col, 4, "3 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "4 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "4 done");
    Zinc_expect_int_equal(num, 1, "4 num");
    Zinc_expect_char_equal(c[0], 'o', "4 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 4, "4 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 5, "4 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "4 line");
    Zinc_expect_size_t_equal(loc.col, 5, "4 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "5 input unicode next");
    Zinc_expect_true(done, "5 done");
    Zinc_expect_int_equal(num, 0, "5 num");
    Zinc_expect_size_t_equal(loc.start_pos, 5, "5 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 6, "5 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "5 line");
    Zinc_expect_size_t_equal(loc.col, 6, "5 col");

    Zinc_input_unicode_seek(input, input->input_vtable, &save_loc);

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "0 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "0 done");
    Zinc_expect_int_equal(num, 1, "0 num");
    Zinc_expect_char_equal(c[0], 'h', "0 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 0, "0 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 1, "0 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "0 line");
    Zinc_expect_size_t_equal(loc.col, 1, "0 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "0 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "1 done");
    Zinc_expect_int_equal(num, 1, "1 num");
    Zinc_expect_char_equal(c[0], 'e', "1 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 1, "1 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 2, "1 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "1 line");
    Zinc_expect_size_t_equal(loc.col, 2, "1 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "0 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "2 done");
    Zinc_expect_int_equal(num, 1, "2 num");
    Zinc_expect_char_equal(c[0], 'l', "2 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 2, "2 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 3, "2 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "2 line");
    Zinc_expect_size_t_equal(loc.col, 3, "2 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "0 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "3 done");
    Zinc_expect_int_equal(num, 1, "3 num");
    Zinc_expect_char_equal(c[0], 'l', "3 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 3, "3 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 4, "3 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "3 line");
    Zinc_expect_size_t_equal(loc.col, 4, "3 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "0 input unicode next");
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(done, "4 done");
    Zinc_expect_int_equal(num, 1, "4 num");
    Zinc_expect_char_equal(c[0], 'o', "4 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 4, "4 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 5, "4 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "4 line");
    Zinc_expect_size_t_equal(loc.col, 5, "4 col");

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "0 input unicode next");
    Zinc_expect_true(done, "5 done");
    Zinc_expect_int_equal(num, 0, "5 num");
    Zinc_expect_size_t_equal(loc.start_pos, 5, "5 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 6, "5 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "5 line");
    Zinc_expect_size_t_equal(loc.col, 6, "5 col");

    Zinc_expect_string(&bf, "hellohello", "bf");

    free(input);
    Zinc_vector_destroy(vector);
    free(vector);
    Zinc_string_destroy(&bf);
}

void test_input_unicode_string_get_all()
{
    Zinc_test_name(__func__);

    char s[] = "hello";
    Zinc_vector* vector = NULL;
    Zinc_vector_create(&vector, sizeof(char));
    Zinc_vector_add(vector, s, strlen(s));
    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, vector);

    Zinc_vector* output = NULL;

    Zinc_input_unicode_get_all(input, input->input_vtable, &output);
    Zinc_expect_true(Zinc_vector_match(vector, output), "match");

    free(input);
    Zinc_vector_destroy(vector);
    free(vector);
}

void test_input_unicode_string_get_location()
{
    Zinc_test_name(__func__);

    char s[] = "hello";
    Zinc_vector* vector = NULL;
    Zinc_vector_create(&vector, sizeof(char));
    Zinc_vector_add(vector, s, strlen(s));
    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, vector);

    enum Zinc_result r;
    bool done;
    char c[4];
    int num;
    struct Zinc_location loc;

    r = Zinc_input_unicode_next(input, input->input_vtable, c, &num, &loc, &done);
    Zinc_assert_ok(r, "0 input unicode next");
    Zinc_expect_false(done, "0 done");
    Zinc_expect_int_equal(num, 1, "0 num");
    Zinc_expect_char_equal(c[0], 'h', "0 char 0");
    Zinc_expect_size_t_equal(loc.start_pos, 0, "0 start_pos");
    Zinc_expect_size_t_equal(loc.end_pos, 1, "0 end_pos");
    Zinc_expect_size_t_equal(loc.line, 1, "0 line");
    Zinc_expect_size_t_equal(loc.col, 1, "0 col");

    struct Zinc_location next_loc;
    next_loc = Zinc_input_unicode_get_location(input, input->input_vtable);
    Zinc_expect_size_t_equal(next_loc.start_pos, 1, "1 start_pos");
    Zinc_expect_size_t_equal(next_loc.end_pos, 0, "1 end_pos");
    Zinc_expect_size_t_equal(next_loc.line, 1, "1 line");
    Zinc_expect_size_t_equal(next_loc.col, 2, "1 col");

    free(input);
    Zinc_vector_destroy(vector);
    free(vector);
}

void test_input_unicode_string()
{
    test_input_unicode_string_next();
    test_input_unicode_string_next_multibyte();
    test_input_unicode_string_repeat();
    test_input_unicode_string_seek();
    test_input_unicode_string_get_all();
    test_input_unicode_string_get_location();
}