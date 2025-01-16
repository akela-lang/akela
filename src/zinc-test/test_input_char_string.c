#include "zinc/unit_test.h"
#include "zinc/input_char_string.h"
#include <string.h>

void TestInputCharStringGetCharWord()
{
    Zinc_test_name(__func__);

    char buffer[] = "hello";
    size_t buffer_len = strlen(buffer);

    Zinc_input_char_string input_string;
    Vector text;
    VectorInit(&text, sizeof(char));
    VectorAdd(&text, buffer, buffer_len);
    VectorAddNull(&text);
    Zinc_input_char_string_init(&input_string, &text);

    Zinc_expect_vector_str(&text, buffer, buffer);

    bool done;
    char c;
    struct Zinc_location loc;

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_expect_false(done, "done 0");
    Zinc_expect_char_equal(c, 'h', "c 0");
    Zinc_expect_size_t_equal(loc.line, 1, "line 0");
    Zinc_expect_size_t_equal(loc.col, 1, "col 0");
    Zinc_expect_size_t_equal(loc.start_pos, 0, "start_pos 0");
    Zinc_expect_size_t_equal(loc.end_pos, 1, "end_pos 0");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_expect_false(done, "done 1");
    Zinc_expect_char_equal(c, 'e', "c 1");
    Zinc_expect_size_t_equal(loc.line, 1, "line 1");
    Zinc_expect_size_t_equal(loc.col, 2, "col 1");
    Zinc_expect_size_t_equal(loc.start_pos, 1, "start_pos 1");
    Zinc_expect_size_t_equal(loc.end_pos, 2, "end_pos 1");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_expect_false(done, "done 2");
    Zinc_expect_char_equal(c, 'l', "c 2");
    Zinc_expect_size_t_equal(loc.line, 1, "line 2");
    Zinc_expect_size_t_equal(loc.col, 3, "col 2");
    Zinc_expect_size_t_equal(loc.start_pos, 2, "start_pos 2");
    Zinc_expect_size_t_equal(loc.end_pos, 3, "end_pos 2");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_expect_false(done, "done 3");
    Zinc_expect_char_equal(c, 'l', "c 3");
    Zinc_expect_size_t_equal(loc.line, 1, "line 3");
    Zinc_expect_size_t_equal(loc.col, 4, "col 3");
    Zinc_expect_size_t_equal(loc.start_pos, 3, "start_pos 3");
    Zinc_expect_size_t_equal(loc.end_pos, 4, "end_pos 3");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_expect_false(done, "done 4");
    Zinc_expect_char_equal(c, 'o', "c 4");
    Zinc_expect_size_t_equal(loc.line, 1, "line 4");
    Zinc_expect_size_t_equal(loc.col, 5, "col 4");
    Zinc_expect_size_t_equal(loc.start_pos, 4, "start_pos 4");
    Zinc_expect_size_t_equal(loc.end_pos, 5, "end_pos 4");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_expect_true(done, "done 5");
    Zinc_expect_size_t_equal(loc.line, 1, "line 5");
    Zinc_expect_size_t_equal(loc.col, 6, "col 5");
    Zinc_expect_size_t_equal(loc.start_pos, 5, "start_pos 5");
    Zinc_expect_size_t_equal(loc.end_pos, 6, "end_pos 5");

    VectorDestroy(&text);
}

void TestInputCharStringGetCharLine()
{
    Zinc_test_name(__func__);

    char buffer[] = "a\nb";
    size_t buffer_len = strlen(buffer);

    Zinc_input_char_string input_string;
    Vector text;
    VectorInit(&text, sizeof(char));
    VectorAdd(&text, buffer, buffer_len);
    VectorAddNull(&text);
    Zinc_input_char_string_init(&input_string, &text);

    Zinc_expect_vector_str(&text, buffer, buffer);

    bool done;
    char c;
    struct Zinc_location loc;

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_expect_false(done, "done 0");
    Zinc_expect_char_equal(c, 'a', "c 0");
    Zinc_expect_size_t_equal(loc.line, 1, "line 0");
    Zinc_expect_size_t_equal(loc.col, 1, "col 0");
    Zinc_expect_size_t_equal(loc.start_pos, 0, "start_pos 0");
    Zinc_expect_size_t_equal(loc.end_pos, 1, "end_pos 0");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_expect_false(done, "done 1");
    Zinc_expect_char_equal(c, '\n', "c 1");
    Zinc_expect_size_t_equal(loc.line, 1, "line 1");
    Zinc_expect_size_t_equal(loc.col, 2, "col 1");
    Zinc_expect_size_t_equal(loc.start_pos, 1, "start_pos 1");
    Zinc_expect_size_t_equal(loc.end_pos, 2, "end_pos 1");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_expect_false(done, "done 2");
    Zinc_expect_char_equal(c, 'b', "c 2");
    Zinc_expect_size_t_equal(loc.line, 2, "line 2");
    Zinc_expect_size_t_equal(loc.col, 1, "col 2");
    Zinc_expect_size_t_equal(loc.start_pos, 2, "start_pos 2");
    Zinc_expect_size_t_equal(loc.end_pos, 3, "end_pos 2");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_expect_true(done, "done 5");
    Zinc_expect_size_t_equal(loc.line, 2, "line 5");
    Zinc_expect_size_t_equal(loc.col, 2, "col 5");
    Zinc_expect_size_t_equal(loc.start_pos, 3, "start_pos 5");
    Zinc_expect_size_t_equal(loc.end_pos, 4, "end_pos 5");

    VectorDestroy(&text);
}

void TestInputCharStringRepeat()
{
    Zinc_test_name(__func__);

    char buffer[] = "abc";
    size_t buffer_len = strlen(buffer);

    Zinc_input_char_string input_string;
    Vector text;
    VectorInit(&text, sizeof(char));
    VectorAdd(&text, buffer, buffer_len);
    VectorAddNull(&text);
    Zinc_input_char_string_init(&input_string, &text);

    Zinc_expect_vector_str(&text, buffer, buffer);

    bool done;
    char c;
    struct Zinc_location loc;

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_expect_false(done, "done 0");
    Zinc_expect_char_equal(c, 'a', "c 0");
    Zinc_expect_size_t_equal(loc.line, 1, "line 0");
    Zinc_expect_size_t_equal(loc.col, 1, "col 0");
    Zinc_expect_size_t_equal(loc.start_pos, 0, "start_pos 0");
    Zinc_expect_size_t_equal(loc.end_pos, 1, "end_pos 0");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_expect_false(done, "done 1");
    Zinc_expect_char_equal(c, 'b', "c 1");
    Zinc_expect_size_t_equal(loc.line, 1, "line 1");
    Zinc_expect_size_t_equal(loc.col, 2, "col 1");
    Zinc_expect_size_t_equal(loc.start_pos, 1, "start_pos 1");
    Zinc_expect_size_t_equal(loc.end_pos, 2, "end_pos 1");

    Zinc_input_char_string_repeat(&input_string);

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_expect_false(done, "done 1r");
    Zinc_expect_char_equal(c, 'b', "c 1r");
    Zinc_expect_size_t_equal(loc.line, 1, "line 1r");
    Zinc_expect_size_t_equal(loc.col, 2, "col 1r");
    Zinc_expect_size_t_equal(loc.start_pos, 1, "start_pos 1r");
    Zinc_expect_size_t_equal(loc.end_pos, 2, "end_pos 1r");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_expect_false(done, "done 2");
    Zinc_expect_char_equal(c, 'c', "c 2");
    Zinc_expect_size_t_equal(loc.line, 1, "line 2");
    Zinc_expect_size_t_equal(loc.col, 3, "col 2");
    Zinc_expect_size_t_equal(loc.start_pos, 2, "start_pos 2");
    Zinc_expect_size_t_equal(loc.end_pos, 3, "end_pos 2");

    done = Zinc_input_char_string_next(&input_string, &c, &loc);
    Zinc_expect_true(done, "done 3");
    Zinc_expect_size_t_equal(loc.line, 1, "line 3");
    Zinc_expect_size_t_equal(loc.col, 4, "col 3");
    Zinc_expect_size_t_equal(loc.start_pos, 3, "start_pos 3");
    Zinc_expect_size_t_equal(loc.end_pos, 4, "end_pos 3");

    VectorDestroy(&text);
}

void TestInputCharString()
{
    TestInputCharStringGetCharWord();
    TestInputCharStringGetCharLine();
    TestInputCharStringRepeat();
}