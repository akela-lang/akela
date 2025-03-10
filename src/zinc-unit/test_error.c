#include "zinc/error.h"
#include "zinc/error_unit_test.h"
#include <stdlib.h>
#include "zinc/memory.h"
#include "zinc/unit_test.h"

void test_error_list()
{
    Zinc_test_name(__func__);

    struct Zinc_error* e = NULL;
    Zinc_malloc_safe((void**)&e, sizeof(struct Zinc_error));
    Zinc_error_init(e);

    struct Zinc_error_list el;
    Zinc_error_list_init(&el);
    Zinc_error_list_add(&el, e);

    struct Zinc_error* e0 = el.head;
    Zinc_assert_ptr(e0, "ptr e0");
    Zinc_expect_true(el.head == e, "head");
    Zinc_expect_true(el.tail == e, "tail");

    Zinc_error_list_destroy(&el);
}

void test_error_list_set_buffer()
{
    Zinc_test_name(__func__);
    struct Zinc_error_list* el;
    Zinc_error_list_create(&el);
    struct Zinc_string bf;
    Zinc_string_init(&bf);
    Zinc_string_add_format(&bf, "hello");
    struct Zinc_location loc;
    Zinc_location_init(&loc);
    Zinc_error_list_set(el, &loc, "abc %bf xyz", &bf);

    Zinc_expect_has_errors(el);
    Zinc_expect_source_error(el, "abc hello xyz");

    Zinc_error_list_destroy(el);
    free(el);
    Zinc_string_destroy(&bf);
}

void test_error_set()
{
    Zinc_test_name(__func__);

    Zinc_error_list errors;
    Zinc_error_list_init(&errors);
    Zinc_location loc;
    Zinc_location_init(&loc);
    Zinc_error_list_set(&errors, &loc, "%c", '1');
    Zinc_error* e = errors.head;
    Zinc_expect_string(&e->message, "1", "message e");
    Zinc_error_list_destroy(&errors);
}

void test_error()
{
    test_error_list();
    test_error_list_set_buffer();
    test_error_set();
}
