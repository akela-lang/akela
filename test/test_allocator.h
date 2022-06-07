#ifndef _TEST_ALLOCATOR_H
#define _TEST_ALLOCATOR_H

#include "assert.h"
#include "setup.h"
#include "alba/allocator.h"

void test_allocator_init()
{
	test_name(__func__);

	struct allocator al;
	allocator_init(&al);
	expect_null(al.head, "head");
	expect_null(al.tail, "tail");
}

void test_allocator_malloc()
{
	test_name(__func__);

	struct allocator al;
	allocator_init(&al);
	int* buff;
	enum result r = allocator_malloc(&al, &buff, 10*sizeof(int));
	for (int i = 0; i < 10; i++) {
		buff[i] = i;
	}
	expect_int_equal(r, result_ok, "r");
	struct allocator_node* aln = allocator_find(&al, buff);
	assert_ptr(aln, "ptr");
	expect_true(aln->p == buff, "p");
	allocator_destroy(&al);
	expect_null(al.head, "head");
	expect_null(al.tail, "tail");
}

void test_allocator_find()
{
	test_name(__func__);

	struct allocator al;
	allocator_init(&al);
	int* buff[10];
	for (int i = 0; i < 10; i++) {
		enum result r = allocator_malloc(&al, &buff[i], sizeof(int));
		*buff[i] = i;
		expect_int_equal(r, result_ok, "r");
	}
	for (int i = 0; i < 10; i++) {
		struct allocator_node* aln = allocator_find(&al, buff[i]);
		assert_ptr(aln, "ptr");
		expect_true(aln->p == buff[i], "p");
		expect_int_equal(*(int*)aln->p, i, "value");
	}
	allocator_destroy(&al);
	expect_null(al.head, "head");
	expect_null(al.tail, "tail");
}

void test_allocator_realloc()
{
	test_name(__func__);

	struct allocator al;
	allocator_init(&al);
	char* buff;
	enum result r;
	r = allocator_malloc(&al, &buff, sizeof(char)*2);
	assert_ok(r, "r");
	buff[0] = 'a';
	buff[1] = '\0';
	expect_true(strcmp(buff, "a")==0, "strcmp");
	r = allocator_realloc(&al, &buff, sizeof(char) * 3);
	assert_ok(r, "r2");
	buff[0] = 'a';
	buff[1] = 'b';
	buff[2] = '\0';
	expect_true(strcmp(buff, "ab") == 0, "strcmp");
	allocator_destroy(&al);
}

void test_allocator_transfer()
{
	test_name(__func__);

	enum result r;

	struct allocator al;
	int* buff;
	allocator_init(&al);
	r = allocator_malloc(&al, &buff, sizeof(int));
	assert_ok(r, "r");
	*buff = 15;

	struct allocator al2;
	int* buff2;
	allocator_init(&al2);
	r = allocator_malloc(&al2, &buff2, sizeof(int));
	assert_ok(r, "r");
	*buff2 = 20;

	struct allocator_node* aln = allocator_find(&al, buff);
	assert_ptr(aln, "ptr");
	expect_true(aln->p == buff, "p");

	struct allocator_node* aln2 = allocator_find(&al2, buff2);
	assert_ptr(aln2, "ptr2");
	expect_true(aln2->p == buff2, "p2");

	allocator_transfer(&al, &al2);

	struct allocator_node* aln3 = allocator_find(&al2, buff);
	assert_ptr(aln3, "ptr3");
	expect_true(aln3->p == buff, "p3");

	struct allocator_node* aln4 = allocator_find(&al2, buff2);
	assert_ptr(aln4, "ptr4");
	expect_true(aln4->p == buff2, "p4");

	expect_null(al.head, "head");
	expect_null(al.tail, "tail");

	allocator_destroy(&al2);
}

void test_allocator()
{
	test_allocator_init();
	test_allocator_malloc();
	test_allocator_find();
	test_allocator_realloc();
	test_allocator_transfer();
}

#endif