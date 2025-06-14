#include "zinc/input_unicode_file.h"
#include "zinc/os_unix.h"
#include <stdio.h>
#include <string.h>
#include "zinc/fs.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void Zinc_unit_input_unicode_file_next(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    const char s[] = "hello\nworld";
    size_t s_len = strlen(s);
    FILE* fp = NULL;
    Zinc_string name;
    Zinc_string_init(&name);
    Zinc_result r;
    r = Zinc_get_temp_file(&fp, &name);
    if (!Zinc_expect_ok(test, r, "get temp file")) {
        return Zinc_assert();
    }
    size_t n = fwrite(s, 1, strlen(s), fp);
    Zinc_expect_size_t_equal(test, s_len, n, "len");
    fclose(fp);
    fp = fopen(name.buf, "r");
    if (!Zinc_expect_ptr(test, fp, "open file")) {
        return Zinc_assert();
    }

    Zinc_input_unicode_file* input = NULL;
    Zinc_input_unicode_file_create(&input, fp);

    Zinc_string bf;
    Zinc_string_init(&bf);

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "0 line");
    Zinc_expect_size_t_equal(test, loc.col, 1, "0 col");
    Zinc_expect_size_t_equal(test, loc.end, 1, "0 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.col, 2, "1 col");
    Zinc_expect_size_t_equal(test, loc.end, 2, "1 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "2 line");
    Zinc_expect_size_t_equal(test, loc.col, 3, "2 col");
    Zinc_expect_size_t_equal(test, loc.end, 3, "2 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "3 line");
    Zinc_expect_size_t_equal(test, loc.col, 4, "3 col");
    Zinc_expect_size_t_equal(test, loc.end, 4, "3 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "4 line");
    Zinc_expect_size_t_equal(test, loc.col, 5, "4 col");
    Zinc_expect_size_t_equal(test, loc.end, 5, "4 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "5 line");
    Zinc_expect_size_t_equal(test, loc.col, 6, "5 col");
    Zinc_expect_size_t_equal(test, loc.end, 6, "5 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 2, "6 line");
    Zinc_expect_size_t_equal(test, loc.col, 1, "6 col");
    Zinc_expect_size_t_equal(test, loc.end, 7, "6 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 2, "7 line");
    Zinc_expect_size_t_equal(test, loc.col, 2, "7 col");
    Zinc_expect_size_t_equal(test, loc.end, 8, "7 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 2, "8 line");
    Zinc_expect_size_t_equal(test, loc.col, 3, "8 col");
    Zinc_expect_size_t_equal(test, loc.end, 9, "8 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 2, "9 line");
    Zinc_expect_size_t_equal(test, loc.col, 4, "9 col");
    Zinc_expect_size_t_equal(test, loc.end, 10, "9 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 2, "10 line");
    Zinc_expect_size_t_equal(test, loc.col, 5, "10 col");
    Zinc_expect_size_t_equal(test, loc.end, 11, "10 end_pos");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "11 input unicode next")) {
        return Zinc_assert();
    }
    Zinc_expect_true(test, done, "11 done");

    Zinc_expect_string(test, &bf, "hello\nworld", "bf");

    Zinc_string_destroy(&name);
    Zinc_input_unicode_destroy(input, input->vtable);
    free(input);
    Zinc_string_destroy(&bf);
}

void Zinc_unit_input_unicode_file_next_multibyte(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    char s[] = "hello\nαβγ";
    size_t s_len = strlen(s);
    FILE* fp = NULL;
    struct Zinc_string name;
    Zinc_string_init(&name);
    enum Zinc_result r;
    r = Zinc_get_temp_file(&fp, &name);
    if (!Zinc_expect_ok(test, r, "get temp file")) {
        return Zinc_assert();
    }
    size_t n = fwrite(s, 1, strlen(s), fp);
    Zinc_expect_size_t_equal(test, s_len, n, "len");
    fclose(fp);
    fp = fopen(name.buf, "r");
    if (!Zinc_expect_ptr(test, fp, "open file")) {
        return Zinc_assert();
    }
    Zinc_input_unicode_file* input = NULL;
    Zinc_input_unicode_file_create(&input, fp);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "0 line");
    Zinc_expect_size_t_equal(test, loc.col, 1, "0 col");
    Zinc_expect_size_t_equal(test, loc.end, 1, "0 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.col, 2, "1 col");
    Zinc_expect_size_t_equal(test, loc.end, 2, "1 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "2 line");
    Zinc_expect_size_t_equal(test, loc.col, 3, "2 col");
    Zinc_expect_size_t_equal(test, loc.end, 3, "2 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "3 line");
    Zinc_expect_size_t_equal(test, loc.col, 4, "3 col");
    Zinc_expect_size_t_equal(test, loc.end, 4, "3 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "4 line");
    Zinc_expect_size_t_equal(test, loc.col, 5, "4 col");
    Zinc_expect_size_t_equal(test, loc.end, 5, "4 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "5 line");
    Zinc_expect_size_t_equal(test, loc.col, 6, "5 col");
    Zinc_expect_size_t_equal(test, loc.end, 6, "5 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 2, "6 line");
    Zinc_expect_size_t_equal(test, loc.col, 1, "6 col");
    Zinc_expect_size_t_equal(test, loc.end, 8, "6 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 2, "7 line");
    Zinc_expect_size_t_equal(test, loc.col, 2, "7 col");
    Zinc_expect_size_t_equal(test, loc.end, 10, "7 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.start, 10, "6 start_pos");
    Zinc_expect_size_t_equal(test, loc.line, 2, "8 line");
    Zinc_expect_size_t_equal(test, loc.col, 3, "8 col");
    Zinc_expect_size_t_equal(test, loc.end, 12, "8 end_pos");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "9 input unicode next")) {
        return Zinc_assert();
    }
    Zinc_expect_true(test, done, "9 done");
    Zinc_expect_int_equal(test, num, 0, "9 num");

    Zinc_expect_string(test, &bf, "hello\nαβγ", "bf");

    Zinc_string_destroy(&name);
    Zinc_input_unicode_destroy(input, input->vtable);
    free(input);
    Zinc_string_destroy(&bf);
}

void Zinc_unit_input_unicode_file_repeat(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    char s[] = "hello";
    size_t s_len = strlen(s);
    FILE* fp = NULL;
    struct Zinc_string name;
    Zinc_string_init(&name);
    enum Zinc_result r;
    r = Zinc_get_temp_file(&fp, &name);
    if (!Zinc_expect_ok(test, r, "get temp file")) {
        return Zinc_assert();
    }
    size_t n = fwrite(s, 1, strlen(s), fp);
    Zinc_expect_size_t_equal(test, s_len, n, "len");
    fclose(fp);
    fp = fopen(name.buf, "r");
    if (!Zinc_expect_ptr(test, fp, "open file")) {
        return Zinc_assert();
    }
    Zinc_input_unicode_file* input = NULL;
    Zinc_input_unicode_file_create(&input, fp);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "0 line");
    Zinc_expect_size_t_equal(test, loc.col, 1, "0 col");
    Zinc_expect_size_t_equal(test, loc.end, 1, "0 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "0r line");
    Zinc_expect_size_t_equal(test, loc.col, 1, "0r col");
    Zinc_expect_size_t_equal(test, loc.end, 1, "0r end_pos");

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
    Zinc_expect_size_t_equal(test, loc.col, 2, "1 col");
    Zinc_expect_size_t_equal(test, loc.end, 2, "1 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "2 line");
    Zinc_expect_size_t_equal(test, loc.col, 3, "2 col");
    Zinc_expect_size_t_equal(test, loc.end, 3, "2 end_pos");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "3 input unicode next")) {
        Zinc_assert();
    }
    for (int i = 0; i < num; i++) {
        Zinc_string_add_char(&bf, c[i]);
    }
    Zinc_expect_false(test, done, "3 done");
    Zinc_expect_int_equal(test, num, 1, "3 num");
    Zinc_expect_char_equal(test, c[0], 'l', "3 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 3, "3 start_pos");
    Zinc_expect_size_t_equal(test, loc.line, 1, "3 line");
    Zinc_expect_size_t_equal(test, loc.col, 4, "3 col");
    Zinc_expect_size_t_equal(test, loc.end, 4, "3 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "4 line");
    Zinc_expect_size_t_equal(test, loc.col, 5, "4 col");
    Zinc_expect_size_t_equal(test, loc.end, 5, "4 end_pos");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "5 input unicode next")) {
        return Zinc_assert();
    }
    Zinc_expect_true(test, done, "5 done");

    Zinc_expect_string(test, &bf, "hhello", "bf");

    Zinc_string_destroy(&name);
    Zinc_input_unicode_destroy(input, input->vtable);
    free(input);
    Zinc_string_destroy(&bf);
}

void Zinc_unit_input_unicode_file_seek(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    char s[] = "hello";
    size_t s_len = strlen(s);
    FILE* fp = NULL;
    struct Zinc_string name;
    Zinc_string_init(&name);
    enum Zinc_result r;
    r = Zinc_get_temp_file(&fp, &name);
    if (!Zinc_expect_ok(test, r, "get temp file")) {
        return Zinc_assert();
    }
    size_t n = fwrite(s, 1, strlen(s), fp);
    Zinc_expect_size_t_equal(test, s_len, n, "len");
    fclose(fp);
    fp = fopen(name.buf, "r");
    if (!Zinc_expect_ptr(test, fp, "open file")) {
        return Zinc_assert();
    }
    Zinc_input_unicode_file* input = NULL;
    Zinc_input_unicode_file_create(&input, fp);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "0 line");
    Zinc_expect_size_t_equal(test, loc.col, 1, "0 col");
    Zinc_expect_size_t_equal(test, loc.end, 1, "0 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.col, 2, "1 col");
    Zinc_expect_size_t_equal(test, loc.end, 2, "1 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "2 line");
    Zinc_expect_size_t_equal(test, loc.col, 3, "2 col");
    Zinc_expect_size_t_equal(test, loc.end, 3, "2 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "3 line");
    Zinc_expect_size_t_equal(test, loc.col, 4, "3 col");
    Zinc_expect_size_t_equal(test, loc.end, 4, "3 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "4 line");
    Zinc_expect_size_t_equal(test, loc.col, 5, "4 col");
    Zinc_expect_size_t_equal(test, loc.end, 5, "4 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "0 line");
    Zinc_expect_size_t_equal(test, loc.col, 1, "0 col");
    Zinc_expect_size_t_equal(test, loc.end, 1, "0 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "1 line");
    Zinc_expect_size_t_equal(test, loc.col, 2, "1 col");
    Zinc_expect_size_t_equal(test, loc.end, 2, "1 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "2 line");
    Zinc_expect_size_t_equal(test, loc.col, 3, "2 col");
    Zinc_expect_size_t_equal(test, loc.end, 3, "2 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "3 line");
    Zinc_expect_size_t_equal(test, loc.col, 4, "3 col");
    Zinc_expect_size_t_equal(test, loc.end, 4, "3 end_pos");

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "4 line");
    Zinc_expect_size_t_equal(test, loc.col, 5, "4 col");
    Zinc_expect_size_t_equal(test, loc.end, 5, "4 end_pos");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    Zinc_expect_true(test, done, "5 done");

    Zinc_expect_string(test, &bf, "hellohello", "bf");

    Zinc_string_destroy(&bf);
    Zinc_string_destroy(&name);
    Zinc_input_unicode_destroy(input, input->vtable);
    free(input);
}

void Zinc_unit_input_unicode_file_get_all(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    char s[] = "hello";
    size_t s_len = strlen(s);
    FILE* fp = NULL;
    struct Zinc_string name;
    Zinc_string_init(&name);
    enum Zinc_result r;
    r = Zinc_get_temp_file(&fp, &name);
    if (!Zinc_expect_ok(test, r, "get temp file")) {
        return Zinc_assert();
    }
    size_t n = fwrite(s, 1, strlen(s), fp);
    Zinc_expect_size_t_equal(test, s_len, n, "len");
    fclose(fp);
    fp = fopen(name.buf, "r");
    if (!Zinc_expect_ptr(test, fp, "open file")) {
        return Zinc_assert();
    }
    Zinc_input_unicode_file* input = NULL;
    Zinc_input_unicode_file_create(&input, fp);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Zinc_vector* output = NULL;

    Zinc_input_unicode_get_all(input, input->vtable, &output);
    Zinc_expect_true(test, Zinc_vector_match_str(output, s), "match");

    Zinc_input_unicode_destroy(input, input->vtable);
    free(input);
    Zinc_vector_destroy(output);
    free(output);
    Zinc_string_destroy(&name);
    Zinc_string_destroy(&bf);
}

void Zinc_unit_input_unicode_file_get_location(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    char s[] = "hello";
    size_t s_len = strlen(s);
    FILE* fp = NULL;
    struct Zinc_string name;
    Zinc_string_init(&name);
    enum Zinc_result r;
    r = Zinc_get_temp_file(&fp, &name);
    if (!Zinc_expect_ok(test, r, "get temp file")) {
        return Zinc_assert();
    }
    size_t n = fwrite(s, 1, strlen(s), fp);
    Zinc_expect_size_t_equal(test, s_len, n, "len");
    fclose(fp);
    fp = fopen(name.buf, "r");
    if (!Zinc_expect_ptr(test, fp, "open file")) {
        return Zinc_assert();
    }
    Zinc_input_unicode_file* input = NULL;
    Zinc_input_unicode_file_create(&input, fp);

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
    Zinc_expect_size_t_equal(test, loc.line, 1, "0 line");
    Zinc_expect_size_t_equal(test, loc.col, 1, "0 col");
    Zinc_expect_size_t_equal(test, loc.end, 1, "0 end_pos");

    struct Zinc_location next_loc;
    next_loc = Zinc_input_unicode_get_location(input, input->vtable);
    Zinc_expect_size_t_equal(test, next_loc.start, 1, "1 start_pos");
    Zinc_expect_size_t_equal(test, next_loc.line, 1, "1 line");
    Zinc_expect_size_t_equal(test, next_loc.col, 2, "1 col");
    Zinc_expect_size_t_equal(test, next_loc.end, 1, "1 end_pos");

    Zinc_string_destroy(&name);
    Zinc_input_unicode_destroy(input, input->vtable);
    free(input);
}

void Zinc_unit_input_unicode_file_set_bounds(Zinc_test* test)
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
    size_t s_len = strlen(s);
    FILE* fp = NULL;
    Zinc_string name;
    Zinc_string_init(&name);
    Zinc_result r;
    r = Zinc_get_temp_file(&fp, &name);
    if (!Zinc_expect_ok(test, r, "get temp file")) {
        return Zinc_assert();
    }
    size_t n = fwrite(s, 1, strlen(s), fp);
    Zinc_expect_size_t_equal(test, s_len, n, "len");
    fclose(fp);
    fp = fopen(name.buf, "r");
    if (!Zinc_expect_ptr(test, fp, "open file")) {
        return Zinc_assert();
    }
    Zinc_input_unicode_file* input = NULL;
    Zinc_input_unicode_file_create(&input, fp);

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
    Zinc_expect_size_t_equal(test, loc.line, 2, "0 line");
    Zinc_expect_size_t_equal(test, loc.col, 1, "0 col");
    Zinc_expect_size_t_equal(test, loc.end, 5, "0 end_pos");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    Zinc_expect_false(test, done, "0 done");
    Zinc_expect_int_equal(test, num, 1, "0 num");
    Zinc_expect_char_equal(test, c[0], 'w', "0 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 5, "0 start_pos");
    Zinc_expect_size_t_equal(test, loc.line, 2, "0 line");
    Zinc_expect_size_t_equal(test, loc.col, 2, "0 col");
    Zinc_expect_size_t_equal(test, loc.end, 6, "0 end_pos");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    Zinc_expect_false(test, done, "0 done");
    Zinc_expect_int_equal(test, num, 1, "0 num");
    Zinc_expect_char_equal(test, c[0], 'o', "0 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 6, "0 start_pos");
    Zinc_expect_size_t_equal(test, loc.line, 2, "0 line");
    Zinc_expect_size_t_equal(test, loc.col, 3, "0 col");
    Zinc_expect_size_t_equal(test, loc.end, 7, "0 end_pos");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    Zinc_expect_false(test, done, "0 done");
    Zinc_expect_int_equal(test, num, 1, "0 num");
    Zinc_expect_char_equal(test, c[0], '\n', "0 char 0");
    Zinc_expect_size_t_equal(test, loc.start, 7, "0 start_pos");
    Zinc_expect_size_t_equal(test, loc.line, 2, "0 line");
    Zinc_expect_size_t_equal(test, loc.col, 4, "0 col");
    Zinc_expect_size_t_equal(test, loc.end, 8, "0 end_pos");

    r = Zinc_input_unicode_next(input, input->vtable, c, &num, &loc, &done);
    if (!Zinc_expect_ok(test, r, "0 input unicode next")) {
        return Zinc_assert();
    }
    Zinc_expect_true(test, done, "0 done");

    Zinc_string_destroy(&name);
    Zinc_input_unicode_destroy(input, input->vtable);
    free(input);
}

void Zinc_unit_input_unicode_file(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Zinc_unit_input_unicode_file_next);
        Zinc_test_register(test, Zinc_unit_input_unicode_file_next_multibyte);
        Zinc_test_register(test, Zinc_unit_input_unicode_file_repeat);
        Zinc_test_register(test, Zinc_unit_input_unicode_file_seek);
        Zinc_test_register(test, Zinc_unit_input_unicode_file_get_all);
        Zinc_test_register(test, Zinc_unit_input_unicode_file_get_location);
        Zinc_test_register(test, Zinc_unit_input_unicode_file_set_bounds);

        return;
    }

    Zinc_test_perform(test);
}