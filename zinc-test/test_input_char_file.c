#include "zinc/unit_test.h"
#include "zinc/input_char_file.h"
#include "zinc/input_char_string.h"
#include <string.h>

void TestInputCharFileNext()
{
    test_name(__func__);

    char filename[] = "temp.txt";
    char text[] = "hello file\n";
    FILE *fp;
    size_t len = strlen(text);
    fp = fopen(filename, "w");
    fwrite(text, 1, len, fp);
    fclose(fp);

    fp = fopen(filename, "r");
    InputCharFile* input;
    InputCharFileCreate(&input, fp);

    Vector* text_actual;
    VectorCreate(&text_actual, sizeof(char));
    bool done;
    char c;
    struct location loc;
    size_t byte_pos = 0;
    size_t col = 1;
    size_t line = 1;
    size_t size = 1;
    do {
        done = InputCharNext(input, input->input_vtable, &c, &loc);
        if (done) break;
        VectorAdd(text_actual, &c, 1);
        expect_size_t_equal(loc.byte_pos, byte_pos, "byte_pos");
        expect_size_t_equal(loc.line, line, "line");
        expect_size_t_equal(loc.col, col, "col");
        expect_size_t_equal(loc.size, size, "size");
        byte_pos++;
        col++;
        if (c == '\n') {
            line++;
            col = 1;
        }
    } while (true);

    expect_vector_str(text_actual, text, "text");

    fclose(fp);
    VectorDestroy(text_actual);
    free(text_actual);
    free(input);
}

void TestInputCharFileRepeat()
{
    test_name(__func__);

    char filename[] = "temp.txt";
    char text[] = "hello file\n";
    char text_expected[] = "hhello file\n";
    FILE *fp;
    size_t len = strlen(text);
    fp = fopen(filename, "w");
    fwrite(text, 1, len, fp);
    fclose(fp);

    fp = fopen(filename, "r");
    InputCharFile* input;
    InputCharFileCreate(&input, fp);

    Vector* text_actual;
    VectorCreate(&text_actual, sizeof(char));
    bool done;
    char c;
    struct location loc;
    size_t byte_pos = 0;
    size_t col = 1;
    size_t line = 1;
    size_t size = 1;
    bool did_repeat = false;
    do {
        done = InputCharNext(input, input->input_vtable, &c, &loc);
        if (done) break;
        VectorAdd(text_actual, &c, 1);
        expect_size_t_equal(loc.byte_pos, byte_pos, "byte_pos");
        expect_size_t_equal(loc.line, line, "line");
        expect_size_t_equal(loc.col, col, "col");
        expect_size_t_equal(loc.size, size, "size");
        if (!did_repeat) {
            InputCharFileRepeat(input);
            did_repeat = true;
            continue;
        }
        byte_pos++;
        col++;
        if (c == '\n') {
            line++;
            col = 1;
        }
    } while (true);

    expect_vector_str(text_actual, text_expected, "text");

    fclose(fp);
    VectorDestroy(text_actual);
    free(text_actual);
    free(input);
}

void TestInputCharFileSeek()
{
    test_name(__func__);

    char filename[] = "temp.txt";
    char text[] = "hello file\n";
    char text_expected[] = "file\n";
    FILE *fp;
    size_t len = strlen(text);
    fp = fopen(filename, "w");
    fwrite(text, 1, len, fp);
    fclose(fp);

    fp = fopen(filename, "r");
    InputCharFile* input;
    InputCharFileCreate(&input, fp);

    Vector* text_actual;
    VectorCreate(&text_actual, sizeof(char));
    bool done;
    char c;
    struct location loc;
    loc.byte_pos = 6;
    InputCharSeek(input, input->input_vtable, &loc);
    do {
        done = InputCharNext(input, input->input_vtable, &c, &loc);
        if (done) break;
        VectorAdd(text_actual, &c, 1);
    } while (true);

    expect_vector_str(text_actual, text_expected, "text");

    fclose(fp);
    VectorDestroy(text_actual);
    free(text_actual);
    free(input);
}

void TestInputCharFileGetAll()
{
    test_name(__func__);

    char filename[] = "temp.txt";
    char text[] = "hello file\n";
    FILE *fp;
    size_t len = strlen(text);
    fp = fopen(filename, "w");
    fwrite(text, 1, len, fp);
    fclose(fp);

    fp = fopen(filename, "r");
    InputCharFile* input;
    InputCharFileCreate(&input, fp);

    Vector* text_actual;
    InputCharFileGetAll(input, &text_actual);

    expect_vector_str(text_actual, text, "text");

    fclose(fp);
    VectorDestroy(text_actual);
    free(text_actual);
    free(input);
}

void TestInputCharFile()
{
    TestInputCharFileNext();
    TestInputCharFileRepeat();
    TestInputCharFileSeek();
    TestInputCharFileGetAll();
}