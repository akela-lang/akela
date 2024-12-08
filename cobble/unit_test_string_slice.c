#include "zinc/unit_test.h"
#include "String_slice.h"
#include <string.h>

void expect_string_slice(String_slice* sl, struct buffer* bf, char* message)
{
    test_called();
    bool has_error = false;

    if (sl->size == bf->size) {
        for (size_t i = 0; i < sl->size; i++) {
            if (sl->p[i] != bf->buf[i]) {
                has_error = true;
                break;
            }
        }
    } else {
        has_error = true;
    }

    if (has_error) {
        error_triggered();
        struct buffer actual;
        buffer_init(&actual);
        for (size_t i = 0; i < sl->size; i++) {
            buffer_add_char(&actual, sl->p[i]);
        }
        buffer_finish(&actual);

        buffer_finish(bf);
        fprintf(stderr, "%s = %s error: %s\n", actual.buf, bf->buf, message);
        buffer_destroy(&actual);
    }
}

void expect_string_slice_str(String_slice* sl, char* expected, char* message)
{
    test_called();
    bool has_error = false;

    size_t size = strlen(expected);
    if (sl->size == size) {
        for (size_t i = 0; i < sl->size; i++) {
            if (sl->p[i] != expected[i]) {
                has_error = true;
                break;
            }
        }
    } else {
        has_error = true;
    }

    if (has_error) {
        error_triggered();
        struct buffer actual;
        buffer_init(&actual);
        for (size_t i = 0; i < sl->size; i++) {
            buffer_add_char(&actual, sl->p[i]);
        }
        buffer_finish(&actual);

        fprintf(stderr, "%s = %s error: %s\n", actual.buf, expected, message);
        buffer_destroy(&actual);
    }
}