#include "zinc/error.h"
#include <stdlib.h>
#include "zinc/memory.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void Zinc_unit_error_list(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_error* e = NULL;
    Zinc_malloc_safe((void**)&e, sizeof(struct Zinc_error));
    Zinc_error_init(e);

    Zinc_error_list el;
    Zinc_error_list_init(&el);
    Zinc_error_list_add(&el, e);

    Zinc_error* e0 = el.head;
    if (!Zinc_expect_ptr(test, e0, "ptr e0")) {
        return Zinc_assert();
    }
    Zinc_expect_true(test, el.head == e, "head");
    Zinc_expect_true(test, el.tail == e, "tail");

    Zinc_error_list_destroy(&el);
}

void Zinc_unit_error_list_set_buffer(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_error_list* el;
    Zinc_error_list_create(&el);
    Zinc_string bf;
    Zinc_string_init(&bf);
    Zinc_string_add_format(&bf, "hello");
    Zinc_location loc;
    Zinc_location_init(&loc);
    Zinc_error_list_set(el, &loc, "abc %bf xyz", &bf);

    Zinc_expect_has_errors(test, el);
    Zinc_expect_source_error(test, el, "abc hello xyz");

    Zinc_error_list_destroy(el);
    free(el);
    Zinc_string_destroy(&bf);
}

void Zinc_unit_error_set(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_error_list errors;
    Zinc_error_list_init(&errors);
    Zinc_location loc;
    Zinc_location_init(&loc);
    Zinc_error_list_set(&errors, &loc, "%c", '1');
    Zinc_error* e = errors.head;
    Zinc_expect_string(test, &e->message, "1", "message e");
    Zinc_error_list_destroy(&errors);
}

void Zinc_unit_error(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Zinc_unit_error_list);
        Zinc_test_register(test, Zinc_unit_error_list_set_buffer);
        Zinc_test_register(test, Zinc_unit_error_set);

        return;
    }

    Zinc_test_perform(test);
}