#include "zinc/unit_test.h"
#include "coverage/data.h"

void test_data_file_list_add()
{
    test_name(__func__);

    Cvr_file* file0 = NULL;
    Cvr_file_create(&file0);

    Cvr_file* file1 = NULL;
    Cvr_file_create(&file1);

    Cvr_file* file2 = NULL;
    Cvr_file_create(&file2);

    Cvr_file_list* list = NULL;
    Cvr_file_list_create(&list);

    Cvr_file_list_add(list, file0);
    Cvr_file_list_add(list, file1);
    Cvr_file_list_add(list, file2);

    expect_ptr_equal(list->head, file0, "head list");
    expect_ptr_equal(file0->prev, NULL, "prev file0");
    expect_ptr_equal(file0->next, file1, "next file0");
    expect_ptr_equal(file1->prev, file0, "prev file1");
    expect_ptr_equal(file1->next, file2, "next file1");
    expect_ptr_equal(file2->prev, file1, "prev file2");
    expect_ptr_equal(file2->next, NULL, "next file2");
    expect_ptr_equal(list->tail, file2, "tail list");

    Cvr_file_list_destroy(list);
    free(list);
}

void test_data_library_add()
{
    test_name(__func__);

    Cvr_library* lib0 = NULL;
    Cvr_library_create(&lib0);

    Cvr_library* lib1 = NULL;
    Cvr_library_create(&lib1);

    Cvr_library* lib2 = NULL;
    Cvr_library_create(&lib2);

    Cvr_library_list* list = NULL;
    Cvr_library_list_create(&list);

    Cvr_library_list_add(list, lib0);
    Cvr_library_list_add(list, lib1);
    Cvr_library_list_add(list, lib2);

    expect_ptr_equal(list->head, lib0, "head list");
    expect_ptr_equal(lib0->prev, NULL, "prev lib0");
    expect_ptr_equal(lib0->next, lib1, "next lib0");
    expect_ptr_equal(lib1->prev, lib0, "prev lib1");
    expect_ptr_equal(lib1->next, lib2, "next lib1");
    expect_ptr_equal(lib2->prev, lib1, "prev lib2");
    expect_ptr_equal(lib2->next, NULL, "next lib2");
    expect_ptr_equal(list->tail, lib2, "tail list");

    Cvr_library_list_destroy(list);
    free(list);
}

void test_data_file_add_sort()
{
    test_name(__func__);

    Cvr_file* file_c = NULL;
    Cvr_file_create(&file_c);
    buffer_copy_str(&file_c->name, "c");

    Cvr_file* file_a = NULL;
    Cvr_file_create(&file_a);
    buffer_copy_str(&file_a->name, "a");

    Cvr_file* file_d = NULL;
    Cvr_file_create(&file_d);
    buffer_copy_str(&file_d->name, "d");

    Cvr_file* file_b = NULL;
    Cvr_file_create(&file_b);
    buffer_copy_str(&file_b->name, "b");

    Cvr_file_list* list = NULL;
    Cvr_file_list_create(&list);

    Cvr_file_list_add_sorted(list, file_c);
    Cvr_file_list_add_sorted(list, file_a);
    Cvr_file_list_add_sorted(list, file_d);
    Cvr_file_list_add_sorted(list, file_b);

    expect_ptr_equal(list->head, file_a, "head list");
    expect_ptr_equal(file_a->prev, NULL, "prev file_a");
    expect_ptr_equal(file_a->next, file_b, "next file_a");
    expect_ptr_equal(file_b->prev, file_a, "prev file_b");
    expect_ptr_equal(file_b->next, file_c, "next file_b");
    expect_ptr_equal(file_c->prev, file_b, "prev file_c");
    expect_ptr_equal(file_c->next, file_d, "next file_c");
    expect_ptr_equal(file_d->prev, file_c, "prev file_d");
    expect_ptr_equal(file_d->next, NULL, "next file_d");
    expect_ptr_equal(list->tail, file_d, "tail list");

    Cvr_file_list_destroy(list);
    free(list);
}

void test_data_library_add_sort()
{
    test_name(__func__);

    Cvr_library* lib_c = NULL;
    Cvr_library_create(&lib_c);
    buffer_copy_str(&lib_c->name, "c");

    Cvr_library* lib_a = NULL;
    Cvr_library_create(&lib_a);
    buffer_copy_str(&lib_a->name, "a");

    Cvr_library* lib_d = NULL;
    Cvr_library_create(&lib_d);
    buffer_copy_str(&lib_d->name, "d");

    Cvr_library* lib_b = NULL;
    Cvr_library_create(&lib_b);
    buffer_copy_str(&lib_b->name, "b");

    Cvr_library_list* list = NULL;
    Cvr_library_list_create(&list);

    Cvr_library_list_add_sorted(list, lib_c);
    Cvr_library_list_add_sorted(list, lib_a);
    Cvr_library_list_add_sorted(list, lib_d);
    Cvr_library_list_add_sorted(list, lib_b);

    expect_ptr_equal(list->head, lib_a, "head list");
    expect_ptr_equal(lib_a->prev, NULL, "prev lib0");
    expect_ptr_equal(lib_a->next, lib_b, "next lib0");
    expect_ptr_equal(lib_b->prev, lib_a, "prev lib1");
    expect_ptr_equal(lib_b->next, lib_c, "next lib1");
    expect_ptr_equal(lib_c->prev, lib_b, "prev lib2");
    expect_ptr_equal(lib_c->next, lib_d, "next lib2");
    expect_ptr_equal(lib_d->prev, lib_c, "prev lib2");
    expect_ptr_equal(lib_d->next, NULL, "next lib2");
    expect_ptr_equal(list->tail, lib_d, "tail list");

    Cvr_library_list_destroy(list);
    free(list);
}

void test_data()
{
    test_data_file_list_add();
    test_data_library_add();
    test_data_file_add_sort();
    test_data_library_add_sort();
}