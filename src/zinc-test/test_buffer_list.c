#include "zinc/string_list.h"
#include "zinc/unit_test.h"
#include "zinc/memory.h"

void test_buffer_list1()
{
    Zinc_test_name(__func__ );

    struct Zinc_string_node* bn0 = NULL;
    Zinc_malloc_safe((void**)&bn0, sizeof(struct Zinc_string_node));
    Zinc_string_node_init(bn0);
    Zinc_string_init(&bn0->value);
    Zinc_string_add_str(&bn0->value, "one");

    struct Zinc_string_node* bn1 = NULL;
    Zinc_malloc_safe((void**)&bn1, sizeof(struct Zinc_string_node));
    Zinc_string_node_init(bn1);
    Zinc_string_init(&bn1->value);
    Zinc_string_add_str(&bn1->value, "two");

    struct Zinc_string_list bl;
    Zinc_string_list_init(&bl);

    Zinc_expect_null(bl.head, "head 0");
    Zinc_expect_null(bl.tail, "tail 0");

    Zinc_string_list_add(&bl, bn0);
    Zinc_expect_ptr_equal(bl.head, bn0, "head 1");
    Zinc_expect_ptr_equal(bl.tail, bn0, "tail 1");
    Zinc_expect_null(bn0->next, "next 1");
    Zinc_expect_null(bn0->prev, "next 1");

    Zinc_string_list_add(&bl, bn1);
    Zinc_expect_ptr_equal(bl.head, bn0, "head 2");
    Zinc_expect_ptr_equal(bl.tail, bn1, "tail 2");
    Zinc_expect_ptr_equal(bn0->next, bn1, "next bn0 2");
    Zinc_expect_ptr_equal(bn0->prev, NULL, "prev bn0 2");
    Zinc_expect_ptr_equal(bn1->next, NULL, "next bn1 2");
    Zinc_expect_ptr_equal(bn1->prev, bn0, "prev bn1 2");

    Zinc_string_list_destroy(&bl);
}

void test_buffer_list_add_str()
{
    Zinc_test_name(__func__ );

    struct Zinc_string_list bl;
    Zinc_string_list_init(&bl);
    Zinc_string_list_add_str(&bl, "one");
    Zinc_string_list_add_str(&bl, "two");

    struct Zinc_string_node* bn0 = bl.head;
    Zinc_expect_str(&bn0->value, "one", "one");

    struct Zinc_string_node* bn1 = bn0->next;
    Zinc_expect_str(&bn1->value, "two", "two");

    Zinc_string_list_destroy(&bl);
}

void test_buffer_list_add_bf()
{
    Zinc_test_name(__func__ );

    struct Zinc_string_list bl;
    Zinc_string_list_init(&bl);

    struct Zinc_string bf0;
    Zinc_string_init(&bf0);
    Zinc_string_add_str(&bf0, "one");

    struct Zinc_string bf1;
    Zinc_string_init(&bf1);
    Zinc_string_add_str(&bf1, "two");

    Zinc_string_list_add_bf(&bl, &bf0);
    Zinc_string_list_add_bf(&bl, &bf1);

    struct Zinc_string_node* bn0 = bl.head;
    Zinc_expect_str(&bn0->value, "one", "one");

    struct Zinc_string_node* bn1 = bn0->next;
    Zinc_expect_str(&bn1->value, "two", "two");

    Zinc_string_destroy(&bf0);
    Zinc_string_destroy(&bf1);
    Zinc_string_list_destroy(&bl);
}

void test_buffer_list_count()
{
    Zinc_test_name(__func__ );

    struct Zinc_string_list bl;
    Zinc_string_list_init(&bl);
    Zinc_string_list_add_str(&bl, "one");
    Zinc_string_list_add_str(&bl, "two");
    Zinc_expect_size_t_equal(Zinc_string_list_count(&bl), 2, "2");
    Zinc_string_list_destroy(&bl);
}

void test_buffer_list_split()
{
    Zinc_test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);
    Zinc_string_add_str(&bf, "one two three");

    struct Zinc_string_list bl;
    Zinc_string_list_init(&bl);

    Zinc_string_split(&bf, &bl);

    struct Zinc_string_node* bn0 = bl.head;
    Zinc_assert_ptr(bn0, "ptr bn0");
    Zinc_expect_str(&bn0->value, "one", "one bn0");

    struct Zinc_string_node* bn1 = bn0->next;
    Zinc_assert_ptr(bn1, "ptr bn1");
    Zinc_expect_str(&bn1->value, "two", "two bn1");

    struct Zinc_string_node* bn2 = bn1->next;
    Zinc_assert_ptr(bn2, "ptr bn2");
    Zinc_expect_str(&bn2->value, "three", "three bn2");

    Zinc_string_list_destroy(&bl);
    Zinc_string_destroy(&bf);
}

void test_buffer_list_get()
{
    Zinc_test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);
    Zinc_string_add_str(&bf, "one two three");

    struct Zinc_string_list bl;
    Zinc_string_list_init(&bl);

    Zinc_string_split(&bf, &bl);

    struct Zinc_string* bf0 = Zinc_string_list_get(&bl, 0);
    Zinc_assert_ptr(bf0, "ptr bf0");
    Zinc_expect_str(bf0, "one", "one bf0");

    struct Zinc_string* bf1 = Zinc_string_list_get(&bl, 1);
    Zinc_assert_ptr(bf1, "ptr bf1");
    Zinc_expect_str(bf1, "two", "two bf1");

    struct Zinc_string* bf2 = Zinc_string_list_get(&bl, 2);
    Zinc_assert_ptr(bf2, "ptr bf2");
    Zinc_expect_str(bf2, "three", "three bf2");

    Zinc_string_list_destroy(&bl);
    Zinc_string_destroy(&bf);
}

void test_buffer_list()
{
    test_buffer_list1();
    test_buffer_list_add_str();
    test_buffer_list_add_bf();
    test_buffer_list_count();
    test_buffer_list_split();
    test_buffer_list_get();
}