#include "zinc/error.h"
#include "zinc/error_unit_test.h"
#include <stdlib.h>
#include "zinc/memory.h"
#include "zinc/unit_test.h"

void test_error_list()
{
    test_name(__func__);

    struct error* e = NULL;
    malloc_safe((void**)&e, sizeof(struct error));
    error_init(e);

    struct error_list el;
    error_list_init(&el);
    error_list_add(&el, e);

    struct error* e0 = el.head;
    assert_ptr(e0, "ptr e0");
    expect_true(el.head == e, "head");
    expect_true(el.tail == e, "tail");

    error_list_destroy(&el);
}

void test_error_list_set_buffer()
{
    test_name(__func__);
    struct error_list* el;
    error_list_create(&el);
    struct Zinc_string bf;
    Zinc_string_init(&bf);
    Zinc_string_add_format(&bf, "hello");
    struct location loc;
    location_init(&loc);
    error_list_set(el, &loc, "abc %b xyz", &bf);

    expect_has_errors(el);
    expect_source_error(el, "abc hello xyz");

    error_list_destroy(el);
    free(el);
    Zinc_string_destroy(&bf);
}

void test_error()
{
    test_error_list();
    test_error_list_set_buffer();
}