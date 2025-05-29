#include "zinc/input_char_file.h"
#include "zinc/input_char_string.h"
#include <string.h>
#include "zinc/fs.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void Zinc_unit_test_input_char_file_next(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    FILE* fp = NULL;
    Zinc_string path;
    Zinc_string_init(&path);

    Zinc_result r = Zinc_get_temp_file(&fp, &path);
    Zinc_test_assert_ok(test, r, "get temp file");
    Zinc_test_assert_ptr(test, fp, "fp");

    char text[] = "hello file\n";
    size_t len = strlen(text);
    fwrite(text, 1, len, fp);
    fclose(fp);

    fp = fopen(Zinc_string_c_str(&path), "r");
    Zinc_test_assert_ptr(test, fp, "fp");
    Zinc_input_char_file* input;
    Zinc_input_char_file_create(&input, fp);

    Zinc_vector* text_actual;
    Zinc_vector_create(&text_actual, sizeof(char));
    bool done;
    char c;
    struct Zinc_location loc;
    size_t start_pos = 0;
    size_t col = 1;
    size_t line = 1;
    size_t end_pos = 1;
    do {
        done = Zinc_input_char_next(input, input->input_vtable, &c, &loc);
        if (done) break;
        Zinc_vector_add(text_actual, &c, 1);
        Zinc_test_expect_size_t_equal(test, loc.start_pos, start_pos, "start_pos");
        Zinc_test_expect_size_t_equal(test, loc.line, line, "line");
        Zinc_test_expect_size_t_equal(test, loc.col, col, "col");
        Zinc_test_expect_size_t_equal(test, loc.end_pos, end_pos, "end_pos");
        start_pos++;
        end_pos++;
        col++;
        if (c == '\n') {
            line++;
            col = 1;
        }
    } while (true);

    Zinc_test_expect_vector_str(test, text_actual, text, "text");

    fclose(fp);
    Zinc_vector_destroy(text_actual);
    free(text_actual);
    free(input);
    Zinc_string_destroy(&path);
}

void Zinc_unit_test_input_char_file_repeat(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    FILE* fp = NULL;
    Zinc_string name;
    Zinc_string_init(&name);

    Zinc_result r = Zinc_get_temp_file(&fp, &name);
    Zinc_test_assert_ok(test, r, "get temp file");
	char text[] = "hello file\n";
    char text_expected[] = "hhello file\n";
    size_t len = strlen(text);
    fwrite(text, 1, len, fp);
    fclose(fp);

    fp = fopen(Zinc_string_c_str(&name), "r");
    Zinc_test_assert_ptr(test, fp, "fp");
    Zinc_input_char_file* input;
    Zinc_input_char_file_create(&input, fp);

    Zinc_vector* text_actual;
    Zinc_vector_create(&text_actual, sizeof(char));
    bool done;
    char c;
    struct Zinc_location loc;
    size_t start_pos = 0;
    size_t col = 1;
    size_t line = 1;
    size_t end_pos = 1;
    bool did_repeat = false;
    do {
        done = Zinc_input_char_next(input, input->input_vtable, &c, &loc);
        if (done) break;
        Zinc_vector_add(text_actual, &c, 1);
        Zinc_test_expect_size_t_equal(test, loc.start_pos, start_pos, "start_pos");
        Zinc_test_expect_size_t_equal(test, loc.line, line, "line");
        Zinc_test_expect_size_t_equal(test, loc.col, col, "col");
        Zinc_test_expect_size_t_equal(test, loc.end_pos, end_pos, "end_pos");
        if (!did_repeat) {
            Zinc_input_char_file_repeat(input);
            did_repeat = true;
            continue;
        }
        start_pos++;
        end_pos++;
        col++;
        if (c == '\n') {
            line++;
            col = 1;
        }
    } while (true);

    Zinc_test_expect_vector_str(test, text_actual, text_expected, "text");

    fclose(fp);
    Zinc_vector_destroy(text_actual);
    free(text_actual);
    free(input);
    Zinc_string_destroy(&name);
}

void Zinc_unit_test_input_char_file_seek(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    FILE* fp = NULL;
    Zinc_string name;
    Zinc_string_init(&name);
    Zinc_result r = Zinc_get_temp_file(&fp, &name);
    Zinc_test_assert_ok(test, r, "get temp file");
    char text[] = "hello file\n";
    char text_expected[] = "file\n";
    size_t len = strlen(text);
    fwrite(text, 1, len, fp);
    fclose(fp);

    fp = fopen(Zinc_string_c_str(&name), "r");
    Zinc_test_assert_ptr(test, fp, "fp");
    Zinc_input_char_file* input;
    Zinc_input_char_file_create(&input, fp);

    Zinc_vector* text_actual;
    Zinc_vector_create(&text_actual, sizeof(char));
    bool done;
    char c;
    struct Zinc_location loc;
    loc.start_pos = 6;
    Zinc_input_char_seek(input, input->input_vtable, &loc);
    do {
        done = Zinc_input_char_next(input, input->input_vtable, &c, &loc);
        if (done) break;
        Zinc_vector_add(text_actual, &c, 1);
    } while (true);

    Zinc_test_expect_vector_str(test, text_actual, text_expected, "text");

    fclose(fp);
    Zinc_vector_destroy(text_actual);
    free(text_actual);
    free(input);
    Zinc_string_destroy(&name);
}

void Zinc_unit_test_input_char_file_get_all(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_string name;
    Zinc_string_init(&name);
    FILE* fp = NULL;
	Zinc_result r = Zinc_get_temp_file(&fp, &name);
    Zinc_test_assert_ok(test, r, "get temp file");
    Zinc_test_assert_ptr(test, fp, "fp");
    char text[] = "hello file\n";
    size_t len = strlen(text);
    Zinc_test_assert_ptr(test, fp, "fp");
    fwrite(text, 1, len, fp);
    fclose(fp);

    fp = fopen(Zinc_string_c_str(&name), "r");
    Zinc_test_assert_ptr(test, fp, "fp");
    Zinc_input_char_file* input;
    Zinc_input_char_file_create(&input, fp);

    Zinc_vector* text_actual;
    Zinc_input_char_file_get_all(input, &text_actual);

    Zinc_test_expect_vector_str(test, text_actual, text, "text");

    fclose(fp);
    Zinc_vector_destroy(text_actual);
    free(text_actual);
    free(input);
    Zinc_string_destroy(&name);
}

void Zinc_unit_input_char_file(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Zinc_unit_test_input_char_file_next);
        Zinc_test_register(test, Zinc_unit_test_input_char_file_repeat);
        Zinc_test_register(test, Zinc_unit_test_input_char_file_seek);
        Zinc_test_register(test, Zinc_unit_test_input_char_file_get_all);

        return;
    }

    Zinc_test_perform(test);
}