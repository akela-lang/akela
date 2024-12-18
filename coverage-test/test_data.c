#include "zinc/unit_test.h"
#include "coverage/data.h"

void test_data_file_list()
{
    test_name(__func__);

    Cov_file* file0 = NULL;
    Cov_file_create(&file0);

    Cov_file* file1 = NULL;
    Cov_file_create(&file1);

    Cov_file* file2 = NULL;
    Cov_file_create(&file2);

    Cov_file_list* list = NULL;
    Cov_file_list_create(&list);

    Cov_file_list_add(list, file0);
    Cov_file_list_add(list, file1);
    Cov_file_list_add(list, file2);

    expect_ptr_equal(list->head, file0, "head list");
    expect_ptr_equal(file0->prev, NULL, "prev file0");
    expect_ptr_equal(file0->next, file1, "next file0");
    expect_ptr_equal(file1->prev, file0, "prev file1");
    expect_ptr_equal(file1->next, file2, "next file1");
    expect_ptr_equal(file2->prev, file1, "prev file2");
    expect_ptr_equal(file2->next, NULL, "next file2");
    expect_ptr_equal(list->tail, file2, "tail list");

    Cov_file_list_destroy(list);
    free(list);
}

void test_data_library()
{
    test_name(__func__);

    Cov_library* lib0 = NULL;
    Cov_library_create(&lib0);

    Cov_library* lib1 = NULL;
    Cov_library_create(&lib1);

    Cov_library* lib2 = NULL;
    Cov_library_create(&lib2);

    Cov_library_list* list = NULL;
    Cov_library_list_create(&list);

    Cov_library_list_add(list, lib0);
    Cov_library_list_add(list, lib1);
    Cov_library_list_add(list, lib2);

    expect_ptr_equal(list->head, lib0, "head list");
    expect_ptr_equal(lib0->prev, NULL, "prev lib0");
    expect_ptr_equal(lib0->next, lib1, "next lib0");
    expect_ptr_equal(lib1->prev, lib0, "prev lib1");
    expect_ptr_equal(lib1->next, lib2, "next lib1");
    expect_ptr_equal(lib2->prev, lib1, "prev lib2");
    expect_ptr_equal(lib2->next, NULL, "next lib2");
    expect_ptr_equal(list->tail, lib2, "tail list");

    Cov_library_list_destroy(list);
    free(list);
}

void test_data()
{
    test_data_file_list();
    test_data_library();
}