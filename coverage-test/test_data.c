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

void test_data()
{
    test_data_file_list();
}