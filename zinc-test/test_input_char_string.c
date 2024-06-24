#include "zinc/unit_test.h"
#include "zinc/input_char_string.h"
#include <string.h>

void TestInputCharStringGetCharWord()
{
    test_name(__func__);

    char buffer[] = "hello";
    size_t buffer_len = strlen(buffer);

    InputCharString input_string;
    Vector text;
    VectorInit(&text, sizeof(char));
    VectorAdd(&text, buffer, buffer_len);
    VectorAddNull(&text);
    InputCharStringInit(&input_string, &text);

    expect_vector_str(&text, buffer, buffer);

    bool done;
    char c;
    struct location loc;

    done = InputCharStringNext(&input_string, &c, &loc);
    expect_false(done, "done 0");
    expect_char_equal(c, 'h', "c 0");
    expect_size_t_equal(loc.line, 1, "line 0");
    expect_size_t_equal(loc.col, 1, "col 0");
    expect_size_t_equal(loc.start_pos, 0, "start_pos 0");
    expect_size_t_equal(loc.end_pos, 1, "end_pos 0");

    done = InputCharStringNext(&input_string, &c, &loc);
    expect_false(done, "done 1");
    expect_char_equal(c, 'e', "c 1");
    expect_size_t_equal(loc.line, 1, "line 1");
    expect_size_t_equal(loc.col, 2, "col 1");
    expect_size_t_equal(loc.start_pos, 1, "start_pos 1");
    expect_size_t_equal(loc.end_pos, 2, "end_pos 1");

    done = InputCharStringNext(&input_string, &c, &loc);
    expect_false(done, "done 2");
    expect_char_equal(c, 'l', "c 2");
    expect_size_t_equal(loc.line, 1, "line 2");
    expect_size_t_equal(loc.col, 3, "col 2");
    expect_size_t_equal(loc.start_pos, 2, "start_pos 2");
    expect_size_t_equal(loc.end_pos, 3, "end_pos 2");

    done = InputCharStringNext(&input_string, &c, &loc);
    expect_false(done, "done 3");
    expect_char_equal(c, 'l', "c 3");
    expect_size_t_equal(loc.line, 1, "line 3");
    expect_size_t_equal(loc.col, 4, "col 3");
    expect_size_t_equal(loc.start_pos, 3, "start_pos 3");
    expect_size_t_equal(loc.end_pos, 4, "end_pos 3");

    done = InputCharStringNext(&input_string, &c, &loc);
    expect_false(done, "done 4");
    expect_char_equal(c, 'o', "c 4");
    expect_size_t_equal(loc.line, 1, "line 4");
    expect_size_t_equal(loc.col, 5, "col 4");
    expect_size_t_equal(loc.start_pos, 4, "start_pos 4");
    expect_size_t_equal(loc.end_pos, 5, "end_pos 4");

    done = InputCharStringNext(&input_string, &c, &loc);
    expect_true(done, "done 5");
    expect_size_t_equal(loc.line, 1, "line 5");
    expect_size_t_equal(loc.col, 6, "col 5");
    expect_size_t_equal(loc.start_pos, 5, "start_pos 5");
    expect_size_t_equal(loc.end_pos, 6, "end_pos 5");

    VectorDestroy(&text);
}

void TestInputCharStringGetCharLine()
{
    test_name(__func__);

    char buffer[] = "a\nb";
    size_t buffer_len = strlen(buffer);

    InputCharString input_string;
    Vector text;
    VectorInit(&text, sizeof(char));
    VectorAdd(&text, buffer, buffer_len);
    VectorAddNull(&text);
    InputCharStringInit(&input_string, &text);

    expect_vector_str(&text, buffer, buffer);

    bool done;
    char c;
    struct location loc;

    done = InputCharStringNext(&input_string, &c, &loc);
    expect_false(done, "done 0");
    expect_char_equal(c, 'a', "c 0");
    expect_size_t_equal(loc.line, 1, "line 0");
    expect_size_t_equal(loc.col, 1, "col 0");
    expect_size_t_equal(loc.start_pos, 0, "start_pos 0");
    expect_size_t_equal(loc.end_pos, 1, "end_pos 0");

    done = InputCharStringNext(&input_string, &c, &loc);
    expect_false(done, "done 1");
    expect_char_equal(c, '\n', "c 1");
    expect_size_t_equal(loc.line, 1, "line 1");
    expect_size_t_equal(loc.col, 2, "col 1");
    expect_size_t_equal(loc.start_pos, 1, "start_pos 1");
    expect_size_t_equal(loc.end_pos, 2, "end_pos 1");

    done = InputCharStringNext(&input_string, &c, &loc);
    expect_false(done, "done 2");
    expect_char_equal(c, 'b', "c 2");
    expect_size_t_equal(loc.line, 2, "line 2");
    expect_size_t_equal(loc.col, 1, "col 2");
    expect_size_t_equal(loc.start_pos, 2, "start_pos 2");
    expect_size_t_equal(loc.end_pos, 3, "end_pos 2");

    done = InputCharStringNext(&input_string, &c, &loc);
    expect_true(done, "done 5");
    expect_size_t_equal(loc.line, 2, "line 5");
    expect_size_t_equal(loc.col, 2, "col 5");
    expect_size_t_equal(loc.start_pos, 3, "start_pos 5");
    expect_size_t_equal(loc.end_pos, 4, "end_pos 5");

    VectorDestroy(&text);
}

void TestInputCharStringRepeat()
{
    test_name(__func__);

    char buffer[] = "abc";
    size_t buffer_len = strlen(buffer);

    InputCharString input_string;
    Vector text;
    VectorInit(&text, sizeof(char));
    VectorAdd(&text, buffer, buffer_len);
    VectorAddNull(&text);
    InputCharStringInit(&input_string, &text);

    expect_vector_str(&text, buffer, buffer);

    bool done;
    char c;
    struct location loc;

    done = InputCharStringNext(&input_string, &c, &loc);
    expect_false(done, "done 0");
    expect_char_equal(c, 'a', "c 0");
    expect_size_t_equal(loc.line, 1, "line 0");
    expect_size_t_equal(loc.col, 1, "col 0");
    expect_size_t_equal(loc.start_pos, 0, "start_pos 0");
    expect_size_t_equal(loc.end_pos, 1, "end_pos 0");

    done = InputCharStringNext(&input_string, &c, &loc);
    expect_false(done, "done 1");
    expect_char_equal(c, 'b', "c 1");
    expect_size_t_equal(loc.line, 1, "line 1");
    expect_size_t_equal(loc.col, 2, "col 1");
    expect_size_t_equal(loc.start_pos, 1, "start_pos 1");
    expect_size_t_equal(loc.end_pos, 2, "end_pos 1");

    InputCharStringRepeat(&input_string);

    done = InputCharStringNext(&input_string, &c, &loc);
    expect_false(done, "done 1r");
    expect_char_equal(c, 'b', "c 1r");
    expect_size_t_equal(loc.line, 1, "line 1r");
    expect_size_t_equal(loc.col, 2, "col 1r");
    expect_size_t_equal(loc.start_pos, 1, "start_pos 1r");
    expect_size_t_equal(loc.end_pos, 2, "end_pos 1r");

    done = InputCharStringNext(&input_string, &c, &loc);
    expect_false(done, "done 2");
    expect_char_equal(c, 'c', "c 2");
    expect_size_t_equal(loc.line, 1, "line 2");
    expect_size_t_equal(loc.col, 3, "col 2");
    expect_size_t_equal(loc.start_pos, 2, "start_pos 2");
    expect_size_t_equal(loc.end_pos, 3, "end_pos 2");

    done = InputCharStringNext(&input_string, &c, &loc);
    expect_true(done, "done 3");
    expect_size_t_equal(loc.line, 1, "line 3");
    expect_size_t_equal(loc.col, 4, "col 3");
    expect_size_t_equal(loc.start_pos, 3, "start_pos 3");
    expect_size_t_equal(loc.end_pos, 4, "end_pos 3");

    VectorDestroy(&text);
}

void TestInputCharString()
{
    TestInputCharStringGetCharWord();
    TestInputCharStringGetCharLine();
    TestInputCharStringRepeat();
}