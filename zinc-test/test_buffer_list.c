#include "zinc/buffer_list.h"
#include "zinc/unit_test.h"
#include "zinc/memory.h"

void test_buffer_list1()
{
    test_name(__func__ );

    struct buffer_node* bn0 = NULL;
    malloc_safe((void**)&bn0, sizeof(struct buffer_node));
    buffer_node_init(bn0);
    buffer_init(&bn0->value);
    buffer_copy_str(&bn0->value, "one");

    struct buffer_node* bn1 = NULL;
    malloc_safe((void**)&bn1, sizeof(struct buffer_node));
    buffer_node_init(bn1);
    buffer_init(&bn1->value);
    buffer_copy_str(&bn1->value, "two");

    struct buffer_list bl;
    buffer_list_init(&bl);

    expect_null(bl.head, "head 0");
    expect_null(bl.tail, "tail 0");

    buffer_list_add(&bl, bn0);
    expect_ptr_equal(bl.head, bn0, "head 1");
    expect_ptr_equal(bl.tail, bn0, "tail 1");
    expect_null(bn0->next, "next 1");
    expect_null(bn0->prev, "next 1");

    buffer_list_add(&bl, bn1);
    expect_ptr_equal(bl.head, bn0, "head 2");
    expect_ptr_equal(bl.tail, bn1, "tail 2");
    expect_ptr_equal(bn0->next, bn1, "next bn0 2");
    expect_ptr_equal(bn0->prev, NULL, "prev bn0 2");
    expect_ptr_equal(bn1->next, NULL, "next bn1 2");
    expect_ptr_equal(bn1->prev, bn0, "prev bn1 2");

    buffer_list_destroy(&bl);
}

void test_buffer_list_add_str()
{
    test_name(__func__ );

    struct buffer_list bl;
    buffer_list_init(&bl);
    buffer_list_add_str(&bl, "one");
    buffer_list_add_str(&bl, "two");

    struct buffer_node* bn0 = bl.head;
    expect_str(&bn0->value, "one", "one");

    struct buffer_node* bn1 = bn0->next;
    expect_str(&bn1->value, "two", "two");

    buffer_list_destroy(&bl);
}

void test_buffer_list_add_bf()
{
    test_name(__func__ );

    struct buffer_list bl;
    buffer_list_init(&bl);

    struct buffer bf0;
    buffer_init(&bf0);
    buffer_copy_str(&bf0, "one");

    struct buffer bf1;
    buffer_init(&bf1);
    buffer_copy_str(&bf1, "two");

    buffer_list_add_bf(&bl, &bf0);
    buffer_list_add_bf(&bl, &bf1);

    struct buffer_node* bn0 = bl.head;
    expect_str(&bn0->value, "one", "one");

    struct buffer_node* bn1 = bn0->next;
    expect_str(&bn1->value, "two", "two");

    buffer_destroy(&bf0);
    buffer_destroy(&bf1);
    buffer_list_destroy(&bl);
}

void test_buffer_list_count()
{
    test_name(__func__ );

    struct buffer_list bl;
    buffer_list_init(&bl);
    buffer_list_add_str(&bl, "one");
    buffer_list_add_str(&bl, "two");
    expect_size_t_equal(buffer_list_count(&bl), 2, "2");
    buffer_list_destroy(&bl);
}

void test_buffer_list_split()
{
    test_name(__func__);

    struct buffer bf;
    buffer_init(&bf);
    buffer_copy_str(&bf, "one two three");

    struct buffer_list bl;
    buffer_list_init(&bl);

    buffer_split(&bf, &bl);

    struct buffer_node* bn0 = bl.head;
    assert_ptr(bn0, "ptr bn0");
    expect_str(&bn0->value, "one", "one bn0");

    struct buffer_node* bn1 = bn0->next;
    assert_ptr(bn1, "ptr bn1");
    expect_str(&bn1->value, "two", "two bn1");

    struct buffer_node* bn2 = bn1->next;
    assert_ptr(bn2, "ptr bn2");
    expect_str(&bn2->value, "three", "three bn2");

    buffer_list_destroy(&bl);
    buffer_destroy(&bf);
}

void test_buffer_list_get()
{
    test_name(__func__);

    struct buffer bf;
    buffer_init(&bf);
    buffer_copy_str(&bf, "one two three");

    struct buffer_list bl;
    buffer_list_init(&bl);

    buffer_split(&bf, &bl);

    struct buffer* bf0 = buffer_list_get(&bl, 0);
    assert_ptr(bf0, "ptr bf0");
    expect_str(bf0, "one", "one bf0");

    struct buffer* bf1 = buffer_list_get(&bl, 1);
    assert_ptr(bf1, "ptr bf1");
    expect_str(bf1, "two", "two bf1");

    struct buffer* bf2 = buffer_list_get(&bl, 2);
    assert_ptr(bf2, "ptr bf2");
    expect_str(bf2, "three", "three bf2");

    buffer_list_destroy(&bl);
    buffer_destroy(&bf);
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