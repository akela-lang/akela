#include "coverage/data.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void CoverUnit_data_file_list_add(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cover_file* file0 = NULL;
    Cover_file_create(&file0);

    Cover_file* file1 = NULL;
    Cover_file_create(&file1);

    Cover_file* file2 = NULL;
    Cover_file_create(&file2);

    Cover_file_list* list = NULL;
    Cover_file_list_create(&list);

    Cover_file_list_add(list, file0);
    Cover_file_list_add(list, file1);
    Cover_file_list_add(list, file2);

    Zinc_expect_ptr_equal(test, list->head, file0, "head list");
    Zinc_expect_ptr_equal(test, file0->prev, NULL, "prev file0");
    Zinc_expect_ptr_equal(test, file0->next, file1, "next file0");
    Zinc_expect_ptr_equal(test, file1->prev, file0, "prev file1");
    Zinc_expect_ptr_equal(test, file1->next, file2, "next file1");
    Zinc_expect_ptr_equal(test, file2->prev, file1, "prev file2");
    Zinc_expect_ptr_equal(test, file2->next, NULL, "next file2");
    Zinc_expect_ptr_equal(test, list->tail, file2, "tail list");

    Cover_file_list_destroy(list);
    free(list);
}

void CoverUnit_data_library_add(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cover_library* lib0 = NULL;
    Cover_library_create(&lib0);

    Cover_library* lib1 = NULL;
    Cover_library_create(&lib1);

    Cover_library* lib2 = NULL;
    Cover_library_create(&lib2);

    Cover_library_list* list = NULL;
    Cover_library_list_create(&list);

    Cover_library_list_add(list, lib0);
    Cover_library_list_add(list, lib1);
    Cover_library_list_add(list, lib2);

    Zinc_expect_ptr_equal(test, list->head, lib0, "head list");
    Zinc_expect_ptr_equal(test, lib0->prev, NULL, "prev lib0");
    Zinc_expect_ptr_equal(test, lib0->next, lib1, "next lib0");
    Zinc_expect_ptr_equal(test, lib1->prev, lib0, "prev lib1");
    Zinc_expect_ptr_equal(test, lib1->next, lib2, "next lib1");
    Zinc_expect_ptr_equal(test, lib2->prev, lib1, "prev lib2");
    Zinc_expect_ptr_equal(test, lib2->next, NULL, "next lib2");
    Zinc_expect_ptr_equal(test, list->tail, lib2, "tail list");

    Cover_library_list_destroy(list);
    free(list);
}

void CoverUnit_data_file_add_sort(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cover_file* file_c = NULL;
    Cover_file_create(&file_c);
    Zinc_string_add_str(&file_c->name, "c");

    Cover_file* file_a = NULL;
    Cover_file_create(&file_a);
    Zinc_string_add_str(&file_a->name, "a");

    Cover_file* file_d = NULL;
    Cover_file_create(&file_d);
    Zinc_string_add_str(&file_d->name, "d");

    Cover_file* file_b = NULL;
    Cover_file_create(&file_b);
    Zinc_string_add_str(&file_b->name, "b");

    Cover_file_list* list = NULL;
    Cover_file_list_create(&list);

    Cover_file_list_add_sorted(list, file_c);
    Cover_file_list_add_sorted(list, file_a);
    Cover_file_list_add_sorted(list, file_d);
    Cover_file_list_add_sorted(list, file_b);

    Zinc_expect_ptr_equal(test, list->head, file_a, "head list");
    Zinc_expect_ptr_equal(test, file_a->prev, NULL, "prev file_a");
    Zinc_expect_ptr_equal(test, file_a->next, file_b, "next file_a");
    Zinc_expect_ptr_equal(test, file_b->prev, file_a, "prev file_b");
    Zinc_expect_ptr_equal(test, file_b->next, file_c, "next file_b");
    Zinc_expect_ptr_equal(test, file_c->prev, file_b, "prev file_c");
    Zinc_expect_ptr_equal(test, file_c->next, file_d, "next file_c");
    Zinc_expect_ptr_equal(test, file_d->prev, file_c, "prev file_d");
    Zinc_expect_ptr_equal(test, file_d->next, NULL, "next file_d");
    Zinc_expect_ptr_equal(test, list->tail, file_d, "tail list");

    Cover_file_list_destroy(list);
    free(list);
}

void CoverUnit_data_library_add_sort(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cover_library* lib_c = NULL;
    Cover_library_create(&lib_c);
    Zinc_string_add_str(&lib_c->name, "c");

    Cover_library* lib_a = NULL;
    Cover_library_create(&lib_a);
    Zinc_string_add_str(&lib_a->name, "a");

    Cover_library* lib_d = NULL;
    Cover_library_create(&lib_d);
    Zinc_string_add_str(&lib_d->name, "d");

    Cover_library* lib_b = NULL;
    Cover_library_create(&lib_b);
    Zinc_string_add_str(&lib_b->name, "b");

    Cover_library_list* list = NULL;
    Cover_library_list_create(&list);

    Cover_library_list_add_sorted(list, lib_c);
    Cover_library_list_add_sorted(list, lib_a);
    Cover_library_list_add_sorted(list, lib_d);
    Cover_library_list_add_sorted(list, lib_b);

    Zinc_expect_ptr_equal(test, list->head, lib_a, "head list");
    Zinc_expect_ptr_equal(test, lib_a->prev, NULL, "prev lib0");
    Zinc_expect_ptr_equal(test, lib_a->next, lib_b, "next lib0");
    Zinc_expect_ptr_equal(test, lib_b->prev, lib_a, "prev lib1");
    Zinc_expect_ptr_equal(test, lib_b->next, lib_c, "next lib1");
    Zinc_expect_ptr_equal(test, lib_c->prev, lib_b, "prev lib2");
    Zinc_expect_ptr_equal(test, lib_c->next, lib_d, "next lib2");
    Zinc_expect_ptr_equal(test, lib_d->prev, lib_c, "prev lib2");
    Zinc_expect_ptr_equal(test, lib_d->next, NULL, "next lib2");
    Zinc_expect_ptr_equal(test, list->tail, lib_d, "tail list");

    Cover_library_list_destroy(list);
    free(list);
}

void CoverUnit_data(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, CoverUnit_data_file_list_add);
        Zinc_test_register(test, CoverUnit_data_library_add);
        Zinc_test_register(test, CoverUnit_data_file_add_sort);
        Zinc_test_register(test, CoverUnit_data_library_add_sort);

        return;
    }

    Zinc_test_perform(test);
}