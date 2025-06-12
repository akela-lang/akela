#include "zinc/input_unicode_string.h"
#include <string.h>
#include "zinc/test.h"
#include "zinc/expect.h"

void Zinc_unit_input_unicode_string_next(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "0 done");
    Zinc_expect_int_equal(test, num, 1, "0 num");
    Zinc_expect_char_equal(test, c[0], 'h', "0 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 0, "0 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 1, "0 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "0 line");
    Zinc_expect_size_t_equal(test, loc.col, 1, "0 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "1 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "1 done");
    Zinc_expect_int_equal(test, num, 1, "1 num");
    Zinc_expect_char_equal(test, c[0], 'e', "1 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 1, "1 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 2, "1 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "1 line");
    Zinc_expect_size_t_equal(test, loc.col, 2, "1 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "2 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "2 done");
    Zinc_expect_int_equal(test, num, 1, "2 num");
    Zinc_expect_char_equal(test, c[0], 'l', "2 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 2, "2 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 3, "2 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "2 line");
    Zinc_expect_size_t_equal(test, loc.col, 3, "2 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "3 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "3 done");
    Zinc_expect_int_equal(test, num, 1, "3 num");
    Zinc_expect_char_equal(test, c[0], 'l', "3 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 3, "3 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 4, "3 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "3 line");
    Zinc_expect_size_t_equal(test, loc.col, 4, "3 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "4 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "4 done");
    Zinc_expect_int_equal(test, num, 1, "4 num");
    Zinc_expect_char_equal(test, c[0], 'o', "4 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 4, "4 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 5, "4 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "4 line");
    Zinc_expect_size_t_equal(test, loc.col, 5, "4 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "5 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "5 done");
    Zinc_expect_int_equal(test, num, 1, "5 num");
    Zinc_expect_char_equal(test, c[0], '\n', "5 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 5, "5 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 6, "5 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "5 line");
    Zinc_expect_size_t_equal(test, loc.col, 6, "5 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "6 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "6 done");
    Zinc_expect_int_equal(test, num, 1, "6 num");
    Zinc_expect_char_equal(test, c[0], 'w', "6 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 6, "6 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 7, "6 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 2, "6 line");
    Zinc_expect_size_t_equal(test, loc.col, 1, "6 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "7 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "7 done");
    Zinc_expect_int_equal(test, num, 1, "7 num");
    Zinc_expect_char_equal(test, c[0], 'o', "7 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 7, "7 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 8, "7 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 2, "7 line");
    Zinc_expect_size_t_equal(test, loc.col, 2, "7 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "8 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "8 done");
    Zinc_expect_int_equal(test, num, 1, "8 num");
    Zinc_expect_char_equal(test, c[0], 'r', "8 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 8, "8 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 9, "8 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 2, "8 line");
    Zinc_expect_size_t_equal(test, loc.col, 3, "8 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "9 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "9 done");
    Zinc_expect_int_equal(test, num, 1, "9 num");
    Zinc_expect_char_equal(test, c[0], 'l', "9 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 9, "0 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 10, "9 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 2, "9 line");
    Zinc_expect_size_t_equal(test, loc.col, 4, "9 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "10 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "10 done");
    Zinc_expect_int_equal(test, num, 1, "10 num");
    Zinc_expect_char_equal(test, c[0], 'd', "10 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 10, "10 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 11, "10 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 2, "10 line");
    Zinc_expect_size_t_equal(test, loc.col, 5, "10 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "11 input unicode next")) {
        return Zinc_assert();
    }
    Zinc_expect_true(test, done, "11 done");
    Zinc_expect_int_equal(test, num, 0, "11 num");

    Zinc_expect_string(test, &bf, "hello\nworld", "bf");

    Zinc_input_unicode_destroy(input, input->vtable);
    free(input);
    Zinc_string_destroy(&bf);
}

void Zinc_unit_input_unicode_string_next_multibyte(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "0 done");
    Zinc_expect_int_equal(test, num, 1, "0 num");
    Zinc_expect_char_equal(test, c[0], 'h', "0 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 0, "0 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 1, "0 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "0 line");
    Zinc_expect_size_t_equal(test, loc.col, 1, "0 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "1 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "1 done");
    Zinc_expect_int_equal(test, num, 1, "1 num");
    Zinc_expect_char_equal(test, c[0], 'e', "1 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 1, "1 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 2, "1 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "1 line");
    Zinc_expect_size_t_equal(test, loc.col, 2, "1 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "2 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "2 done");
    Zinc_expect_int_equal(test, num, 1, "2 num");
    Zinc_expect_char_equal(test, c[0], 'l', "2 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 2, "2 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 3, "2 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "2 line");
    Zinc_expect_size_t_equal(test, loc.col, 3, "2 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "3 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "3 done");
    Zinc_expect_int_equal(test, num, 1, "3 num");
    Zinc_expect_char_equal(test, c[0], 'l', "3 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 3, "3 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 4, "3 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "3 line");
    Zinc_expect_size_t_equal(test, loc.col, 4, "3 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "4 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "4 done");
    Zinc_expect_int_equal(test, num, 1, "4 num");
    Zinc_expect_char_equal(test, c[0], 'o', "4 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 4, "4 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 5, "4 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "4 line");
    Zinc_expect_size_t_equal(test, loc.col, 5, "4 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "5 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "5 done");
    Zinc_expect_int_equal(test, num, 1, "5 num");
    Zinc_expect_char_equal(test, c[0], '\n', "5 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 5, "5 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 6, "5 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "5 line");
    Zinc_expect_size_t_equal(test, loc.col, 6, "5 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "6 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "6 done");
    Zinc_expect_int_equal(test, num, 2, "6 num");
    Zinc_expect_char_equal(test, c[0], "α"[0], "6 char 0");
    Zinc_expect_char_equal(test, c[1], "α"[1], "6 char 1");
    Zinc_expect_size_t_equal(test, loc.start, 6, "6 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 8, "6 size");
    Zinc_expect_size_t_equal(test, loc.line, 2, "6 line");
    Zinc_expect_size_t_equal(test, loc.col, 1, "6 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "7 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "7 done");
    Zinc_expect_int_equal(test, num, 2, "7 num");
    Zinc_expect_char_equal(test, c[0], "β"[0], "7 char 0");
    Zinc_expect_char_equal(test, c[1], "β"[1], "7 char 1");
    Zinc_expect_size_t_equal(test, loc.start, 8, "7 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 10, "7 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 2, "7 line");
    Zinc_expect_size_t_equal(test, loc.col, 2, "7 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "8 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "8 done");
    Zinc_expect_int_equal(test, num, 2, "8 num");
    Zinc_expect_char_equal(test, c[0], "γ"[0], "8 char 0");
    Zinc_expect_char_equal(test, c[1], "γ"[1], "8 char 1");
    Zinc_expect_size_t_equal(test, loc.start, 10, "8 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 12, "8 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 2, "8 line");
    Zinc_expect_size_t_equal(test, loc.col, 3, "8 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "9 input unicode next")) {
        return Zinc_assert();
    }
    Zinc_expect_true(test, done, "9 done");
    Zinc_expect_int_equal(test, num, 0, "9 num");

    Zinc_expect_string(test, &bf, "hello\nαβγ", "bf");

    Zinc_input_unicode_destroy(input, input->vtable);
    free(input);
    Zinc_string_destroy(&bf);
}

void Zinc_unit_input_unicode_string_repeat(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "0 done");
    Zinc_expect_int_equal(test, num, 1, "0 num");
    Zinc_expect_char_equal(test, c[0], 'h', "0 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 0, "0 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 1, "0 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "0 line");
    Zinc_expect_size_t_equal(test, loc.col, 1, "0 col");

    Zinc_input_unicode_repeat(input, input->vtable);
    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0r input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "0r done");
    Zinc_expect_int_equal(test, num, 1, "0r num");
    Zinc_expect_char_equal(test, c[0], 'h', "0r char 0");
    Zinc_expect_size_t_equal(test, loc.start, 0, "0r start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 1, "0r end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "0r line");
    Zinc_expect_size_t_equal(test, loc.col, 1, "0r col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "1 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "1 done");
    Zinc_expect_int_equal(test, num, 1, "1 num");
    Zinc_expect_char_equal(test, c[0], 'e', "1 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 1, "1 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 2, "1 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "1 line");
    Zinc_expect_size_t_equal(test, loc.col, 2, "1 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "2 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "2 done");
    Zinc_expect_int_equal(test, num, 1, "2 num");
    Zinc_expect_char_equal(test, c[0], 'l', "2 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 2, "2 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 3, "2 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "2 line");
    Zinc_expect_size_t_equal(test, loc.col, 3, "2 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "3 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "3 done");
    Zinc_expect_int_equal(test, num, 1, "3 num");
    Zinc_expect_char_equal(test, c[0], 'l', "3 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 3, "3 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 4, "3 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "3 line");
    Zinc_expect_size_t_equal(test, loc.col, 4, "3 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "4 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "4 done");
    Zinc_expect_int_equal(test, num, 1, "4 num");
    Zinc_expect_char_equal(test, c[0], 'o', "4 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 4, "4 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 5, "4 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "4 line");
    Zinc_expect_size_t_equal(test, loc.col, 5, "4 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "5 input unicode next")) {
        return Zinc_assert();
    }
    Zinc_expect_true(test, done, "5 done");
    Zinc_expect_int_equal(test, num, 0, "5 num");

    Zinc_expect_string(test, &bf, "hhello", "bf");

    Zinc_input_unicode_destroy(input, input->vtable);
    free(input);
    Zinc_string_destroy(&bf);
}

void Zinc_unit_input_unicode_string_seek(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "0 done");
    Zinc_expect_int_equal(test, num, 1, "0 num");
    Zinc_expect_char_equal(test, c[0], 'h', "0 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 0, "0 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 1, "0 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "0 line");
    Zinc_expect_size_t_equal(test, loc.col, 1, "0 col");

    struct Zinc_location save_loc = loc;

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "1 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "1 done");
    Zinc_expect_int_equal(test, num, 1, "1 num");
    Zinc_expect_char_equal(test, c[0], 'e', "1 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 1, "1 start_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "1 line");
    Zinc_expect_size_t_equal(test, loc.end, 2, "1 end_pos");
    Zinc_expect_size_t_equal(test, loc.col, 2, "1 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "2 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "2 done");
    Zinc_expect_int_equal(test, num, 1, "2 num");
    Zinc_expect_char_equal(test, c[0], 'l', "2 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 2, "2 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 3, "2 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "2 line");
    Zinc_expect_size_t_equal(test, loc.col, 3, "2 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "3 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "3 done");
    Zinc_expect_int_equal(test, num, 1, "3 num");
    Zinc_expect_char_equal(test, c[0], 'l', "3 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 3, "3 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 4, "3 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "3 line");
    Zinc_expect_size_t_equal(test, loc.col, 4, "3 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "4 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "4 done");
    Zinc_expect_int_equal(test, num, 1, "4 num");
    Zinc_expect_char_equal(test, c[0], 'o', "4 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 4, "4 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 5, "4 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "4 line");
    Zinc_expect_size_t_equal(test, loc.col, 5, "4 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "5 input unicode next")) {
        return Zinc_assert();
    }
    Zinc_expect_true(test, done, "5 done");

    Zinc_input_unicode_seek(input, input->vtable, &save_loc);

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "0 done");
    Zinc_expect_int_equal(test, num, 1, "0 num");
    Zinc_expect_char_equal(test, c[0], 'h', "0 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 0, "0 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 1, "0 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "0 line");
    Zinc_expect_size_t_equal(test, loc.col, 1, "0 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "1 done");
    Zinc_expect_int_equal(test, num, 1, "1 num");
    Zinc_expect_char_equal(test, c[0], 'e', "1 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 1, "1 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 2, "1 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "1 line");
    Zinc_expect_size_t_equal(test, loc.col, 2, "1 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "2 done");
    Zinc_expect_int_equal(test, num, 1, "2 num");
    Zinc_expect_char_equal(test, c[0], 'l', "2 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 2, "2 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 3, "2 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "2 line");
    Zinc_expect_size_t_equal(test, loc.col, 3, "2 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "3 done");
    Zinc_expect_int_equal(test, num, 1, "3 num");
    Zinc_expect_char_equal(test, c[0], 'l', "3 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 3, "3 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 4, "3 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "3 line");
    Zinc_expect_size_t_equal(test, loc.col, 4, "3 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "4 done");
    Zinc_expect_int_equal(test, num, 1, "4 num");
    Zinc_expect_char_equal(test, c[0], 'o', "4 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 4, "4 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 5, "4 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "4 line");
    Zinc_expect_size_t_equal(test, loc.col, 5, "4 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    Zinc_expect_true(test, done, "5 done");

    Zinc_expect_string(test, &bf, "hellohello", "bf");

    Zinc_input_unicode_destroy(input, input->vtable);
    free(input);
    Zinc_string_destroy(&bf);
}

void Zinc_unit_input_unicode_string_get_all(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    char s[] = "hello";
    Zinc_vector* vector = NULL;
    Zinc_vector_create(&vector, sizeof(char));
    Zinc_vector_add(vector, s, strlen(s));
    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, vector);

    Zinc_vector* output = NULL;

    Zinc_input_unicode_get_all(input, input->vtable, &output);
    Zinc_expect_true(test, Zinc_vector_match(vector, output), "match");

    Zinc_input_unicode_destroy(input, input->vtable);
    free(input);
}

void Zinc_unit_input_unicode_string_get_location(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    Zinc_expect_false(test, done, "0 done");
    Zinc_expect_int_equal(test, num, 1, "0 num");
    Zinc_expect_char_equal(test, c[0], 'h', "0 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 0, "0 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 1, "0 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "0 line");
    Zinc_expect_size_t_equal(test, loc.col, 1, "0 col");

    struct Zinc_location next_loc;
    next_loc = Zinc_input_unicode_get_location(input, input->vtable);
    Zinc_expect_size_t_equal(test, next_loc.start, 1, "1 start_pos");
    Zinc_expect_size_t_equal(test, next_loc.end, 0, "1 end_pos");
    Zinc_expect_size_t_equal(test, next_loc.line, 1, "1 line");
    Zinc_expect_size_t_equal(test, next_loc.col, 2, "1 col");

    Zinc_input_unicode_destroy(input, input->vtable);
    free(input);
}

void Zinc_unit_input_unicode_string_set_bounds(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    char s[] = "one\n"
        "two\n"
        "three\n";
    Zinc_vector* vector = NULL;
    Zinc_vector_create(&vector, sizeof(char));
    Zinc_vector_add(vector, s, strlen(s));
    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, vector);

    Zinc_result r;
    bool done;
    char c[4];
    int num;
    Zinc_location loc;

    Zinc_input_bounds bounds = {
        .loc = {
            .start = 4,
            .end = 0,
            .line = 2,
            .col = 1,
        },
        .end = 8,
    };
    Zinc_input_unicode_set_bounds(input, input->vtable, &bounds);

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    Zinc_expect_false(test, done, "0 done");
    Zinc_expect_int_equal(test, num, 1, "0 num");
    Zinc_expect_char_equal(test, c[0], 't', "0 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 4, "0 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 5, "0 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 2, "0 line");
    Zinc_expect_size_t_equal(test, loc.col, 1, "0 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    Zinc_expect_false(test, done, "0 done");
    Zinc_expect_int_equal(test, num, 1, "0 num");
    Zinc_expect_char_equal(test, c[0], 'w', "0 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 5, "0 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 6, "0 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 2, "0 line");
    Zinc_expect_size_t_equal(test, loc.col, 2, "0 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    Zinc_expect_false(test, done, "0 done");
    Zinc_expect_int_equal(test, num, 1, "0 num");
    Zinc_expect_char_equal(test, c[0], 'o', "0 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 6, "0 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 7, "0 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 2, "0 line");
    Zinc_expect_size_t_equal(test, loc.col, 3, "0 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    Zinc_expect_false(test, done, "0 done");
    Zinc_expect_int_equal(test, num, 1, "0 num");
    Zinc_expect_char_equal(test, c[0], '\n', "0 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 7, "0 start_pos");
    Zinc_expect_size_t_equal(test, loc.end, 8, "0 end_pos");
    Zinc_expect_size_t_equal(test, loc.line, 2, "0 line");
    Zinc_expect_size_t_equal(test, loc.col, 4, "0 col");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    Zinc_expect_true(test, done, "0 done");
    Zinc_expect_int_equal(test, num, 0, "0 num");

    Zinc_input_unicode_destroy(input, input->vtable);
    free(input);
}

void Zinc_unit_input_unicode_string(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Zinc_unit_input_unicode_string_next);
        Zinc_test_register(test, Zinc_unit_input_unicode_string_next_multibyte);
        Zinc_test_register(test, Zinc_unit_input_unicode_string_repeat);
        Zinc_test_register(test, Zinc_unit_input_unicode_string_seek);
        Zinc_test_register(test, Zinc_unit_input_unicode_string_get_all);
        Zinc_test_register(test, Zinc_unit_input_unicode_string_get_location);
        Zinc_test_register(test, Zinc_unit_input_unicode_string_set_bounds);

        return;
    }

    Zinc_test_perform(test);
}