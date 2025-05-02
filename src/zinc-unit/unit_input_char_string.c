#include "zinc/unit_test.h"
#include "zinc/input_char_string.h"
#include <string.h>
#include "zinc/test.h"
#include "zinc/expect.h"

void Zinc_unit_test_input_char_string_get_char_word(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    char buffer[] = "hello";
    size_t buffer_len = strlen(buffer);

    Zinc_input_char_string input_string;
    Zinc_vector text;
    Zinc_vector_init(&text, sizeof(char));
    Zinc_vector_add(&text, buffer, buffer_len);
    Zinc_vector_add_null(&text);
    Zinc_input_char_string_init(&input_string, &text);

    Zinc_test_expect_vector_str(test, &text, buffer, buffer);

    bool done;
    char c;
    struct Zinc_location loc;

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_test_expect_false(test, done, "done 0");
    Zinc_test_expect_char_equal(test, c, 'h', "c 0");
    Zinc_test_expect_size_t_equal(test, loc.line, 1, "line 0");
    Zinc_test_expect_size_t_equal(test, loc.col, 1, "col 0");
    Zinc_test_expect_size_t_equal(test, loc.start_pos, 0, "start_pos 0");
    Zinc_test_expect_size_t_equal(test, loc.end_pos, 1, "end_pos 0");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_test_expect_false(test, done, "done 1");
    Zinc_test_expect_char_equal(test, c, 'e', "c 1");
    Zinc_test_expect_size_t_equal(test, loc.line, 1, "line 1");
    Zinc_test_expect_size_t_equal(test, loc.col, 2, "col 1");
    Zinc_test_expect_size_t_equal(test, loc.start_pos, 1, "start_pos 1");
    Zinc_test_expect_size_t_equal(test, loc.end_pos, 2, "end_pos 1");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_test_expect_false(test, done, "done 2");
    Zinc_test_expect_char_equal(test, c, 'l', "c 2");
    Zinc_test_expect_size_t_equal(test, loc.line, 1, "line 2");
    Zinc_test_expect_size_t_equal(test, loc.col, 3, "col 2");
    Zinc_test_expect_size_t_equal(test, loc.start_pos, 2, "start_pos 2");
    Zinc_test_expect_size_t_equal(test, loc.end_pos, 3, "end_pos 2");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_test_expect_false(test, done, "done 3");
    Zinc_test_expect_char_equal(test, c, 'l', "c 3");
    Zinc_test_expect_size_t_equal(test, loc.line, 1, "line 3");
    Zinc_test_expect_size_t_equal(test, loc.col, 4, "col 3");
    Zinc_test_expect_size_t_equal(test, loc.start_pos, 3, "start_pos 3");
    Zinc_test_expect_size_t_equal(test, loc.end_pos, 4, "end_pos 3");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_test_expect_false(test, done, "done 4");
    Zinc_test_expect_char_equal(test, c, 'o', "c 4");
    Zinc_test_expect_size_t_equal(test, loc.line, 1, "line 4");
    Zinc_test_expect_size_t_equal(test, loc.col, 5, "col 4");
    Zinc_test_expect_size_t_equal(test, loc.start_pos, 4, "start_pos 4");
    Zinc_test_expect_size_t_equal(test, loc.end_pos, 5, "end_pos 4");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_test_expect_true(test, done, "done 5");
    Zinc_test_expect_size_t_equal(test, loc.line, 1, "line 5");
    Zinc_test_expect_size_t_equal(test, loc.col, 6, "col 5");
    Zinc_test_expect_size_t_equal(test, loc.start_pos, 5, "start_pos 5");
    Zinc_test_expect_size_t_equal(test, loc.end_pos, 6, "end_pos 5");

    Zinc_vector_destroy(&text);
}

void Zinc_unit_test_input_char_string_get_char_line(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    char buffer[] = "a\nb";
    size_t buffer_len = strlen(buffer);

    Zinc_input_char_string input_string;
    Zinc_vector text;
    Zinc_vector_init(&text, sizeof(char));
    Zinc_vector_add(&text, buffer, buffer_len);
    Zinc_vector_add_null(&text);
    Zinc_input_char_string_init(&input_string, &text);

    Zinc_expect_vector_str(&text, buffer, buffer);

    bool done;
    char c;
    struct Zinc_location loc;

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_test_expect_false(test, done, "done 0");
    Zinc_test_expect_char_equal(test, c, 'a', "c 0");
    Zinc_test_expect_size_t_equal(test, loc.line, 1, "line 0");
    Zinc_test_expect_size_t_equal(test, loc.col, 1, "col 0");
    Zinc_test_expect_size_t_equal(test, loc.start_pos, 0, "start_pos 0");
    Zinc_test_expect_size_t_equal(test, loc.end_pos, 1, "end_pos 0");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_test_expect_false(test, done, "done 1");
    Zinc_test_expect_char_equal(test, c, '\n', "c 1");
    Zinc_test_expect_size_t_equal(test, loc.line, 1, "line 1");
    Zinc_test_expect_size_t_equal(test, loc.col, 2, "col 1");
    Zinc_test_expect_size_t_equal(test, loc.start_pos, 1, "start_pos 1");
    Zinc_test_expect_size_t_equal(test, loc.end_pos, 2, "end_pos 1");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_test_expect_false(test, done, "done 2");
    Zinc_test_expect_char_equal(test, c, 'b', "c 2");
    Zinc_test_expect_size_t_equal(test, loc.line, 2, "line 2");
    Zinc_test_expect_size_t_equal(test, loc.col, 1, "col 2");
    Zinc_test_expect_size_t_equal(test, loc.start_pos, 2, "start_pos 2");
    Zinc_test_expect_size_t_equal(test, loc.end_pos, 3, "end_pos 2");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_test_expect_true(test, done, "done 5");
    Zinc_test_expect_size_t_equal(test, loc.line, 2, "line 5");
    Zinc_test_expect_size_t_equal(test, loc.col, 2, "col 5");
    Zinc_test_expect_size_t_equal(test, loc.start_pos, 3, "start_pos 5");
    Zinc_test_expect_size_t_equal(test, loc.end_pos, 4, "end_pos 5");

    Zinc_vector_destroy(&text);
}

void Zinc_unit_input_char_string_repeat(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    char buffer[] = "abc";
    size_t buffer_len = strlen(buffer);

    Zinc_input_char_string input_string;
    Zinc_vector text;
    Zinc_vector_init(&text, sizeof(char));
    Zinc_vector_add(&text, buffer, buffer_len);
    Zinc_vector_add_null(&text);
    Zinc_input_char_string_init(&input_string, &text);

    Zinc_test_expect_vector_str(test, &text, buffer, buffer);

    bool done;
    char c;
    struct Zinc_location loc;

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_test_expect_false(test, done, "done 0");
    Zinc_test_expect_char_equal(test, c, 'a', "c 0");
    Zinc_test_expect_size_t_equal(test, loc.line, 1, "line 0");
    Zinc_test_expect_size_t_equal(test, loc.col, 1, "col 0");
    Zinc_test_expect_size_t_equal(test, loc.start_pos, 0, "start_pos 0");
    Zinc_test_expect_size_t_equal(test, loc.end_pos, 1, "end_pos 0");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_test_expect_false(test, done, "done 1");
    Zinc_test_expect_char_equal(test, c, 'b', "c 1");
    Zinc_test_expect_size_t_equal(test, loc.line, 1, "line 1");
    Zinc_test_expect_size_t_equal(test, loc.col, 2, "col 1");
    Zinc_test_expect_size_t_equal(test, loc.start_pos, 1, "start_pos 1");
    Zinc_test_expect_size_t_equal(test, loc.end_pos, 2, "end_pos 1");

    Zinc_input_char_string_repeat(&input_string);

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_test_expect_false(test, done, "done 1r");
    Zinc_test_expect_char_equal(test, c, 'b', "c 1r");
    Zinc_test_expect_size_t_equal(test, loc.line, 1, "line 1r");
    Zinc_test_expect_size_t_equal(test, loc.col, 2, "col 1r");
    Zinc_test_expect_size_t_equal(test, loc.start_pos, 1, "start_pos 1r");
    Zinc_test_expect_size_t_equal(test, loc.end_pos, 2, "end_pos 1r");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_test_expect_false(test, done, "done 2");
    Zinc_test_expect_char_equal(test, c, 'c', "c 2");
    Zinc_test_expect_size_t_equal(test, loc.line, 1, "line 2");
    Zinc_test_expect_size_t_equal(test, loc.col, 3, "col 2");
    Zinc_test_expect_size_t_equal(test, loc.start_pos, 2, "start_pos 2");
    Zinc_test_expect_size_t_equal(test, loc.end_pos, 3, "end_pos 2");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_test_expect_true(test, done, "done 3");
    Zinc_test_expect_size_t_equal(test, loc.line, 1, "line 3");
    Zinc_test_expect_size_t_equal(test, loc.col, 4, "col 3");
    Zinc_test_expect_size_t_equal(test, loc.start_pos, 3, "start_pos 3");
    Zinc_test_expect_size_t_equal(test, loc.end_pos, 4, "end_pos 3");

    Zinc_vector_destroy(&text);
}

void Zinc_unit_test_input_char_string(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Zinc_unit_test_input_char_string_get_char_word);
        Zinc_test_register(test, Zinc_unit_test_input_char_string_get_char_line);
        Zinc_test_register(test, Zinc_unit_input_char_string_repeat);

        return;
    }

    Zinc_test_perform(test);
}