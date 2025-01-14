#include "unit_test.h"
#include "error.h"
#include <string.h>

void assert_no_errors(struct Zinc_error_list* el)
{
    test_called();

    if (!el->head) return;
    error_triggered();
    fprintf(stderr, "assert no errors: has errors\n");
    struct Zinc_error* e = el->head;
    while (e) {
        Zinc_string_finish(&e->message);
        fprintf(stderr, "(%zu,%zu): %s\n", e->loc.line, e->loc.col, e->message.buf);
        e = e->next;
    }

    error_triggered();
    panic();
}

void expect_no_errors(struct Zinc_error_list* el)
{
    test_called();

    if (!el->head) return;
    error_triggered();
    fprintf(stderr, "expect no errors: has errors\n");
    struct Zinc_error* e = el->head;
    while (e) {
        Zinc_string_finish(&e->message);
        fprintf(stderr, "(%zu,%zu): %s\n", e->loc.line, e->loc.col, e->message.buf);
        e = e->next;
    }

    error_triggered();
}

void expect_error_count(struct Zinc_error_list* el, size_t count)
{
    test_called();

    size_t actual_count = 0;
    struct Zinc_error* e = el->head;
    while (e) {
        actual_count++;
        e = e->next;
    }
    if (actual_count == count) return;

    error_triggered();
    fprintf(stderr, "expected error count (%zu) (%zu)\n", actual_count, count);
}

void assert_has_errors(struct Zinc_error_list* el)
{
    test_called();

    if (el->head) return;
    error_triggered();
    fprintf(stderr, "assert has errors: there are no errors\n");

    error_triggered();
    panic();
}

void expect_has_errors(struct Zinc_error_list* el)
{
    test_called();

    if (el->head) return;
    error_triggered();
    fprintf(stderr, "expect has errors: there are no errors\n");

    error_triggered();
}

struct Zinc_error* assert_source_error(struct Zinc_error_list* el, const char message[])
{
    test_called();
    struct Zinc_error* e = el->head;
    while (e) {
        Zinc_string_finish(&e->message);
        if (strcmp(e->message.buf, message) == 0) {
            return e;
        }
        e = e->next;
    }
    error_triggered();
    fprintf(stderr, "error not found: %s\n", message);
    panic();
    return NULL;
}

struct Zinc_error* expect_source_error(struct Zinc_error_list* el, const char message[])
{
    test_called();
    struct Zinc_error* e = el->head;
    while (e) {
        Zinc_string_finish(&e->message);
        if (strcmp(e->message.buf, message) == 0) {
            return e;
        }
        e = e->next;
    }
    error_triggered();
    fprintf(stderr, "error not found: %s\n", message);
    e = el->head;
    if (e) {
        fprintf(stderr, "Errors:\n");
    }
    while (e) {
        Zinc_string_finish(&e->message);
        fprintf(stderr, "%s\n", e->message.buf);
        e = e->next;
    }
    return NULL;
}
