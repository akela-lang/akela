#include "zinc/unit_test.h"
#include "zinc/input_unicode_file.h"
#include "zinc/os_unix.h"
#include <stdio.h>
#include <string.h>

void test_input_unicode_file_next()
{
    test_name(__func__);

    const char s[] = "hello\nworld";
    size_t s_len = strlen(s);
    FILE* fp = NULL;
    struct buffer name;
    buffer_init(&name);
    enum result r;
    r = get_temp_file(&fp, &name);
    assert_ok(r, "get temp file");
    size_t n = fwrite(s, 1, strlen(s), fp);
    expect_size_t_equal(s_len, n, "len");
    fclose(fp);
    fp = fopen(name.buf, "r");
    assert_ptr(fp, "open file");

    InputUnicodeFile* input = NULL;
    InputUnicodeFileCreate(&input, fp);

    struct buffer bf;
    buffer_init(&bf);

    bool done;
    char c[4];
    int num;
    struct location loc;

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "0 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "0 done");
    expect_int_equal(num, 1, "0 num");
    expect_char_equal(c[0], 'h', "0 char 0");
    expect_size_t_equal(loc.byte_pos, 0, "0 byte_pos");
    expect_size_t_equal(loc.line, 1, "0 line");
    expect_size_t_equal(loc.col, 1, "0 col");
    expect_size_t_equal(loc.size, 1, "0 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "1 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "1 done");
    expect_int_equal(num, 1, "1 num");
    expect_char_equal(c[0], 'e', "1 char 0");
    expect_size_t_equal(loc.byte_pos, 1, "1 byte_pos");
    expect_size_t_equal(loc.line, 1, "1 line");
    expect_size_t_equal(loc.col, 2, "1 col");
    expect_size_t_equal(loc.size, 1, "1 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "2 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "2 done");
    expect_int_equal(num, 1, "2 num");
    expect_char_equal(c[0], 'l', "2 char 0");
    expect_size_t_equal(loc.byte_pos, 2, "2 byte_pos");
    expect_size_t_equal(loc.line, 1, "2 line");
    expect_size_t_equal(loc.col, 3, "2 col");
    expect_size_t_equal(loc.size, 1, "2 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "3 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "3 done");
    expect_int_equal(num, 1, "3 num");
    expect_char_equal(c[0], 'l', "3 char 0");
    expect_size_t_equal(loc.byte_pos, 3, "3 byte_pos");
    expect_size_t_equal(loc.line, 1, "3 line");
    expect_size_t_equal(loc.col, 4, "3 col");
    expect_size_t_equal(loc.size, 1, "3 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "4 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "4 done");
    expect_int_equal(num, 1, "4 num");
    expect_char_equal(c[0], 'o', "4 char 0");
    expect_size_t_equal(loc.byte_pos, 4, "4 byte_pos");
    expect_size_t_equal(loc.line, 1, "4 line");
    expect_size_t_equal(loc.col, 5, "4 col");
    expect_size_t_equal(loc.size, 1, "4 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "5 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "5 done");
    expect_int_equal(num, 1, "5 num");
    expect_char_equal(c[0], '\n', "5 char 0");
    expect_size_t_equal(loc.byte_pos, 5, "5 byte_pos");
    expect_size_t_equal(loc.line, 1, "5 line");
    expect_size_t_equal(loc.col, 6, "5 col");
    expect_size_t_equal(loc.size, 1, "5 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "6 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "6 done");
    expect_int_equal(num, 1, "6 num");
    expect_char_equal(c[0], 'w', "6 char 0");
    expect_size_t_equal(loc.byte_pos, 6, "6 byte_pos");
    expect_size_t_equal(loc.line, 2, "6 line");
    expect_size_t_equal(loc.col, 1, "6 col");
    expect_size_t_equal(loc.size, 1, "6 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "7 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "7 done");
    expect_int_equal(num, 1, "7 num");
    expect_char_equal(c[0], 'o', "7 char 0");
    expect_size_t_equal(loc.byte_pos, 7, "7 byte_pos");
    expect_size_t_equal(loc.line, 2, "7 line");
    expect_size_t_equal(loc.col, 2, "7 col");
    expect_size_t_equal(loc.size, 1, "7 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "8 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "8 done");
    expect_int_equal(num, 1, "8 num");
    expect_char_equal(c[0], 'r', "8 char 0");
    expect_size_t_equal(loc.byte_pos, 8, "8 byte_pos");
    expect_size_t_equal(loc.line, 2, "8 line");
    expect_size_t_equal(loc.col, 3, "8 col");
    expect_size_t_equal(loc.size, 1, "8 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "9 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "9 done");
    expect_int_equal(num, 1, "9 num");
    expect_char_equal(c[0], 'l', "9 char 0");
    expect_size_t_equal(loc.byte_pos, 9, "0 byte_pos");
    expect_size_t_equal(loc.line, 2, "9 line");
    expect_size_t_equal(loc.col, 4, "9 col");
    expect_size_t_equal(loc.size, 1, "9 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "10 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "10 done");
    expect_int_equal(num, 1, "10 num");
    expect_char_equal(c[0], 'd', "10 char 0");
    expect_size_t_equal(loc.byte_pos, 10, "10 byte_pos");
    expect_size_t_equal(loc.line, 2, "10 line");
    expect_size_t_equal(loc.col, 5, "10 col");
    expect_size_t_equal(loc.size, 1, "10 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "11 input unicode next");
    expect_true(done, "11 done");
    expect_int_equal(num, 0, "11 num");
    expect_size_t_equal(loc.byte_pos, 11, "11 byte_pos");
    expect_size_t_equal(loc.line, 2, "11 line");
    expect_size_t_equal(loc.col, 6, "11 col");
    expect_size_t_equal(loc.size, 1, "11 size");

    expect_str(&bf, "hello\nworld", "bf");

    fclose(fp);
    buffer_destroy(&name);
    free(input);
    buffer_destroy(&bf);
}

void test_input_unicode_file_next_multibyte()
{
    test_name(__func__);

    char s[] = "hello\nαβγ";
    size_t s_len = strlen(s);
    FILE* fp = NULL;
    struct buffer name;
    buffer_init(&name);
    enum result r;
    r = get_temp_file(&fp, &name);
    assert_ok(r, "get temp file");
    size_t n = fwrite(s, 1, strlen(s), fp);
    expect_size_t_equal(s_len, n, "len");
    fclose(fp);
    fp = fopen(name.buf, "r");
    assert_ptr(fp, "open file");
    InputUnicodeFile* input = NULL;
    InputUnicodeFileCreate(&input, fp);

    struct buffer bf;
    buffer_init(&bf);

    bool done;
    char c[4];
    int num;
    struct location loc;

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "0 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "0 done");
    expect_int_equal(num, 1, "0 num");
    expect_char_equal(c[0], 'h', "0 char 0");
    expect_size_t_equal(loc.byte_pos, 0, "0 byte_pos");
    expect_size_t_equal(loc.line, 1, "0 line");
    expect_size_t_equal(loc.col, 1, "0 col");
    expect_size_t_equal(loc.size, 1, "0 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "1 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "1 done");
    expect_int_equal(num, 1, "1 num");
    expect_char_equal(c[0], 'e', "1 char 0");
    expect_size_t_equal(loc.byte_pos, 1, "1 byte_pos");
    expect_size_t_equal(loc.line, 1, "1 line");
    expect_size_t_equal(loc.col, 2, "1 col");
    expect_size_t_equal(loc.size, 1, "1 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "2 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "2 done");
    expect_int_equal(num, 1, "2 num");
    expect_char_equal(c[0], 'l', "2 char 0");
    expect_size_t_equal(loc.byte_pos, 2, "2 byte_pos");
    expect_size_t_equal(loc.line, 1, "2 line");
    expect_size_t_equal(loc.col, 3, "2 col");
    expect_size_t_equal(loc.size, 1, "2 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "3 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "3 done");
    expect_int_equal(num, 1, "3 num");
    expect_char_equal(c[0], 'l', "3 char 0");
    expect_size_t_equal(loc.byte_pos, 3, "3 byte_pos");
    expect_size_t_equal(loc.line, 1, "3 line");
    expect_size_t_equal(loc.col, 4, "3 col");
    expect_size_t_equal(loc.size, 1, "3 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "4 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "4 done");
    expect_int_equal(num, 1, "4 num");
    expect_char_equal(c[0], 'o', "4 char 0");
    expect_size_t_equal(loc.byte_pos, 4, "4 byte_pos");
    expect_size_t_equal(loc.line, 1, "4 line");
    expect_size_t_equal(loc.col, 5, "4 col");
    expect_size_t_equal(loc.size, 1, "4 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "5 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "5 done");
    expect_int_equal(num, 1, "5 num");
    expect_char_equal(c[0], '\n', "5 char 0");
    expect_size_t_equal(loc.byte_pos, 5, "5 byte_pos");
    expect_size_t_equal(loc.line, 1, "5 line");
    expect_size_t_equal(loc.col, 6, "5 col");
    expect_size_t_equal(loc.size, 1, "5 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "6 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "6 done");
    expect_int_equal(num, 2, "6 num");
    expect_char_equal(c[0], "α"[0], "6 char 0");
    expect_char_equal(c[1], "α"[1], "6 char 1");
    expect_size_t_equal(loc.byte_pos, 6, "6 byte_pos");
    expect_size_t_equal(loc.line, 2, "6 line");
    expect_size_t_equal(loc.col, 1, "6 col");
    expect_size_t_equal(loc.size, 1, "6 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "7 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "7 done");
    expect_int_equal(num, 2, "7 num");
    expect_char_equal(c[0], "β"[0], "7 char 0");
    expect_char_equal(c[1], "β"[1], "7 char 1");
    expect_size_t_equal(loc.byte_pos, 8, "7 byte_pos");
    expect_size_t_equal(loc.line, 2, "7 line");
    expect_size_t_equal(loc.col, 2, "7 col");
    expect_size_t_equal(loc.size, 1, "7 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "8 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "8 done");
    expect_int_equal(num, 2, "8 num");
    expect_char_equal(c[0], "γ"[0], "8 char 0");
    expect_char_equal(c[1], "γ"[1], "8 char 1");
    expect_size_t_equal(loc.byte_pos, 10, "6 byte_pos");
    expect_size_t_equal(loc.line, 2, "8 line");
    expect_size_t_equal(loc.col, 3, "8 col");
    expect_size_t_equal(loc.size, 1, "8 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "9 input unicode next");
    expect_true(done, "9 done");
    expect_int_equal(num, 0, "9 num");
    expect_size_t_equal(loc.byte_pos, 12, "6 byte_pos");
    expect_size_t_equal(loc.line, 2, "8 line");
    expect_size_t_equal(loc.col, 4, "8 col");
    expect_size_t_equal(loc.size, 1, "8 size");

    expect_str(&bf, "hello\nαβγ", "bf");

    fclose(fp);
    buffer_destroy(&name);
    free(input);
    buffer_destroy(&bf);
}

void test_input_unicode_file_repeat()
{
    test_name(__func__);

    char s[] = "hello";
    size_t s_len = strlen(s);
    FILE* fp = NULL;
    struct buffer name;
    buffer_init(&name);
    enum result r;
    r = get_temp_file(&fp, &name);
    assert_ok(r, "get temp file");
    size_t n = fwrite(s, 1, strlen(s), fp);
    expect_size_t_equal(s_len, n, "len");
    fclose(fp);
    fp = fopen(name.buf, "r");
    assert_ptr(fp, "open file");
    InputUnicodeFile* input = NULL;
    InputUnicodeFileCreate(&input, fp);

    struct buffer bf;
    buffer_init(&bf);

    bool done;
    char c[4];
    int num;
    struct location loc;

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "0 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "0 done");
    expect_int_equal(num, 1, "0 num");
    expect_char_equal(c[0], 'h', "0 char 0");
    expect_size_t_equal(loc.byte_pos, 0, "0 byte_pos");
    expect_size_t_equal(loc.line, 1, "0 line");
    expect_size_t_equal(loc.col, 1, "0 col");
    expect_size_t_equal(loc.size, 1, "0 size");

    InputUnicodeRepeat(input, input->input_vtable);
    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "0r input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "0r done");
    expect_int_equal(num, 1, "0r num");
    expect_char_equal(c[0], 'h', "0r char 0");
    expect_size_t_equal(loc.byte_pos, 0, "0r byte_pos");
    expect_size_t_equal(loc.line, 1, "0r line");
    expect_size_t_equal(loc.col, 1, "0r col");
    expect_size_t_equal(loc.size, 1, "0r size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "1 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "1 done");
    expect_int_equal(num, 1, "1 num");
    expect_char_equal(c[0], 'e', "1 char 0");
    expect_size_t_equal(loc.byte_pos, 1, "1 byte_pos");
    expect_size_t_equal(loc.line, 1, "1 line");
    expect_size_t_equal(loc.col, 2, "1 col");
    expect_size_t_equal(loc.size, 1, "1 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "2 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "2 done");
    expect_int_equal(num, 1, "2 num");
    expect_char_equal(c[0], 'l', "2 char 0");
    expect_size_t_equal(loc.byte_pos, 2, "2 byte_pos");
    expect_size_t_equal(loc.line, 1, "2 line");
    expect_size_t_equal(loc.col, 3, "2 col");
    expect_size_t_equal(loc.size, 1, "2 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "3 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "3 done");
    expect_int_equal(num, 1, "3 num");
    expect_char_equal(c[0], 'l', "3 char 0");
    expect_size_t_equal(loc.byte_pos, 3, "3 byte_pos");
    expect_size_t_equal(loc.line, 1, "3 line");
    expect_size_t_equal(loc.col, 4, "3 col");
    expect_size_t_equal(loc.size, 1, "3 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "4 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "4 done");
    expect_int_equal(num, 1, "4 num");
    expect_char_equal(c[0], 'o', "4 char 0");
    expect_size_t_equal(loc.byte_pos, 4, "4 byte_pos");
    expect_size_t_equal(loc.line, 1, "4 line");
    expect_size_t_equal(loc.col, 5, "4 col");
    expect_size_t_equal(loc.size, 1, "4 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "5 input unicode next");
    expect_true(done, "5 done");
    expect_int_equal(num, 0, "5 num");
    expect_size_t_equal(loc.byte_pos, 5, "5 byte_pos");
    expect_size_t_equal(loc.line, 1, "5 line");
    expect_size_t_equal(loc.col, 6, "5 col");
    expect_size_t_equal(loc.size, 1, "5 size");

    expect_str(&bf, "hhello", "bf");

    fclose(fp);
    buffer_destroy(&name);
    free(input);
    buffer_destroy(&bf);
}

void test_input_unicode_file_seek()
{
    test_name(__func__);

    char s[] = "hello";
    size_t s_len = strlen(s);
    FILE* fp = NULL;
    struct buffer name;
    buffer_init(&name);
    enum result r;
    r = get_temp_file(&fp, &name);
    assert_ok(r, "get temp file");
    size_t n = fwrite(s, 1, strlen(s), fp);
    expect_size_t_equal(s_len, n, "len");
    fclose(fp);
    fp = fopen(name.buf, "r");
    assert_ptr(fp, "open file");
    InputUnicodeFile* input = NULL;
    InputUnicodeFileCreate(&input, fp);

    struct buffer bf;
    buffer_init(&bf);

    bool done;
    char c[4];
    int num;
    struct location loc;

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "0 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "0 done");
    expect_int_equal(num, 1, "0 num");
    expect_char_equal(c[0], 'h', "0 char 0");
    expect_size_t_equal(loc.byte_pos, 0, "0 byte_pos");
    expect_size_t_equal(loc.line, 1, "0 line");
    expect_size_t_equal(loc.col, 1, "0 col");
    expect_size_t_equal(loc.size, 1, "0 size");

    struct location save_loc = loc;

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "1 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "1 done");
    expect_int_equal(num, 1, "1 num");
    expect_char_equal(c[0], 'e', "1 char 0");
    expect_size_t_equal(loc.byte_pos, 1, "1 byte_pos");
    expect_size_t_equal(loc.line, 1, "1 line");
    expect_size_t_equal(loc.col, 2, "1 col");
    expect_size_t_equal(loc.size, 1, "1 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "2 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "2 done");
    expect_int_equal(num, 1, "2 num");
    expect_char_equal(c[0], 'l', "2 char 0");
    expect_size_t_equal(loc.byte_pos, 2, "2 byte_pos");
    expect_size_t_equal(loc.line, 1, "2 line");
    expect_size_t_equal(loc.col, 3, "2 col");
    expect_size_t_equal(loc.size, 1, "2 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "3 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "3 done");
    expect_int_equal(num, 1, "3 num");
    expect_char_equal(c[0], 'l', "3 char 0");
    expect_size_t_equal(loc.byte_pos, 3, "3 byte_pos");
    expect_size_t_equal(loc.line, 1, "3 line");
    expect_size_t_equal(loc.col, 4, "3 col");
    expect_size_t_equal(loc.size, 1, "3 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "4 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "4 done");
    expect_int_equal(num, 1, "4 num");
    expect_char_equal(c[0], 'o', "4 char 0");
    expect_size_t_equal(loc.byte_pos, 4, "4 byte_pos");
    expect_size_t_equal(loc.line, 1, "4 line");
    expect_size_t_equal(loc.col, 5, "4 col");
    expect_size_t_equal(loc.size, 1, "4 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "5 input unicode next");
    expect_true(done, "5 done");
    expect_int_equal(num, 0, "5 num");
    expect_size_t_equal(loc.byte_pos, 5, "5 byte_pos");
    expect_size_t_equal(loc.line, 1, "5 line");
    expect_size_t_equal(loc.col, 6, "5 col");
    expect_size_t_equal(loc.size, 1, "5 size");

    InputUnicodeSeek(input, input->input_vtable, &save_loc);

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "0 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "0 done");
    expect_int_equal(num, 1, "0 num");
    expect_char_equal(c[0], 'h', "0 char 0");
    expect_size_t_equal(loc.byte_pos, 0, "0 byte_pos");
    expect_size_t_equal(loc.line, 1, "0 line");
    expect_size_t_equal(loc.col, 1, "0 col");
    expect_size_t_equal(loc.size, 1, "0 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "0 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "1 done");
    expect_int_equal(num, 1, "1 num");
    expect_char_equal(c[0], 'e', "1 char 0");
    expect_size_t_equal(loc.byte_pos, 1, "1 byte_pos");
    expect_size_t_equal(loc.line, 1, "1 line");
    expect_size_t_equal(loc.col, 2, "1 col");
    expect_size_t_equal(loc.size, 1, "1 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "0 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "2 done");
    expect_int_equal(num, 1, "2 num");
    expect_char_equal(c[0], 'l', "2 char 0");
    expect_size_t_equal(loc.byte_pos, 2, "2 byte_pos");
    expect_size_t_equal(loc.line, 1, "2 line");
    expect_size_t_equal(loc.col, 3, "2 col");
    expect_size_t_equal(loc.size, 1, "2 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "0 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "3 done");
    expect_int_equal(num, 1, "3 num");
    expect_char_equal(c[0], 'l', "3 char 0");
    expect_size_t_equal(loc.byte_pos, 3, "3 byte_pos");
    expect_size_t_equal(loc.line, 1, "3 line");
    expect_size_t_equal(loc.col, 4, "3 col");
    expect_size_t_equal(loc.size, 1, "3 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "0 input unicode next");
    for (int i = 0; i < num; i++) {
        buffer_add_char(&bf, c[i]);
    }
    expect_false(done, "4 done");
    expect_int_equal(num, 1, "4 num");
    expect_char_equal(c[0], 'o', "4 char 0");
    expect_size_t_equal(loc.byte_pos, 4, "4 byte_pos");
    expect_size_t_equal(loc.line, 1, "4 line");
    expect_size_t_equal(loc.col, 5, "4 col");
    expect_size_t_equal(loc.size, 1, "4 size");

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "0 input unicode next");
    expect_true(done, "5 done");
    expect_int_equal(num, 0, "5 num");
    expect_size_t_equal(loc.byte_pos, 5, "5 byte_pos");
    expect_size_t_equal(loc.line, 1, "5 line");
    expect_size_t_equal(loc.col, 6, "5 col");
    expect_size_t_equal(loc.size, 1, "5 size");

    expect_str(&bf, "hellohello", "bf");

    buffer_destroy(&bf);
    fclose(fp);
    buffer_destroy(&name);
    free(input);
}

void test_input_unicode_file_get_all()
{
    test_name(__func__);

    char s[] = "hello";
    size_t s_len = strlen(s);
    FILE* fp = NULL;
    struct buffer name;
    buffer_init(&name);
    enum result r;
    r = get_temp_file(&fp, &name);
    assert_ok(r, "get temp file");
    size_t n = fwrite(s, 1, strlen(s), fp);
    expect_size_t_equal(s_len, n, "len");
    fclose(fp);
    fp = fopen(name.buf, "r");
    assert_ptr(fp, "open file");
    InputUnicodeFile* input = NULL;
    InputUnicodeFileCreate(&input, fp);

    struct buffer bf;
    buffer_init(&bf);

    Vector* output = NULL;

    InputUnicodeGetAll(input, input->input_vtable, &output);
    expect_true(VectorMatchStr(output, s), "match");

    free(input);
    VectorDestroy(output);
    free(output);
    buffer_destroy(&name);
    fclose(fp);
    buffer_destroy(&bf);
}

void test_input_unicode_file_get_location()
{
    test_name(__func__);

    char s[] = "hello";
    size_t s_len = strlen(s);
    FILE* fp = NULL;
    struct buffer name;
    buffer_init(&name);
    enum result r;
    r = get_temp_file(&fp, &name);
    assert_ok(r, "get temp file");
    size_t n = fwrite(s, 1, strlen(s), fp);
    expect_size_t_equal(s_len, n, "len");
    fclose(fp);
    fp = fopen(name.buf, "r");
    assert_ptr(fp, "open file");
    InputUnicodeFile* input = NULL;
    InputUnicodeFileCreate(&input, fp);

    bool done;
    char c[4];
    int num;
    struct location loc;

    r = InputUnicodeNext(input, input->input_vtable, c, &num, &loc, &done);
    assert_ok(r, "0 input unicode next");
    expect_false(done, "0 done");
    expect_int_equal(num, 1, "0 num");
    expect_char_equal(c[0], 'h', "0 char 0");
    expect_size_t_equal(loc.byte_pos, 0, "0 byte_pos");
    expect_size_t_equal(loc.line, 1, "0 line");
    expect_size_t_equal(loc.col, 1, "0 col");
    expect_size_t_equal(loc.size, 1, "0 size");

    struct location next_loc;
    next_loc = InputUnicodeGetLocation(input, input->input_vtable);
    expect_size_t_equal(next_loc.byte_pos, 1, "1 byte_pos");
    expect_size_t_equal(next_loc.line, 1, "1 line");
    expect_size_t_equal(next_loc.col, 2, "1 col");
    expect_size_t_equal(next_loc.size, 1, "1 size");

    fclose(fp);
    buffer_destroy(&name);
    free(input);
}

void test_input_unicode_file()
{
    test_input_unicode_file_next();
    test_input_unicode_file_next_multibyte();
    test_input_unicode_file_repeat();
    test_input_unicode_file_seek();
    test_input_unicode_file_get_all();
    test_input_unicode_file_get_location();
}