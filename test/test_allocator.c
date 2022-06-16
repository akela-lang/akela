#include "assert.h"
#include "alba/allocator.h"
#include "alba/result.h"

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
	enum result r = allocator_malloc(&al, &buff, 10 * sizeof(int));
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
	r = allocator_malloc(&al, &buff, sizeof(char) * 2);
	assert_ok(r, "r");
	buff[0] = 'a';
	buff[1] = '\0';
	expect_true(strcmp(buff, "a") == 0, "strcmp");
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
	int* buff[3];
	allocator_init(&al);
	for (int i = 0; i < 3; i++) {
		r = allocator_malloc(&al, &buff[i], sizeof(int));
		assert_ok(r, "r");
		*buff[i] = i;
	}

	struct allocator al2;
	int* buff2[3];
	allocator_init(&al2);
	for (int i = 0; i < 3; i++) {
		r = allocator_malloc(&al2, &buff2[i], sizeof(int));
		assert_ok(r, "r");
		*buff2[i] = i + 3;
	}

	for (int i = 0; i < 3; i++) {
		struct allocator_node* aln = allocator_find(&al, buff[i]);
		assert_ptr(aln, "ptr");
		expect_true(aln->p == buff[i], "p");
		expect_int_equal(*(int*)aln->p, i, "int");
	}

	for (int i = 0; i < 3; i++) {
		struct allocator_node* aln2 = allocator_find(&al2, buff2[i]);
		assert_ptr(aln2, "ptr2");
		expect_true(aln2->p == buff2[i], "p2");
		expect_int_equal(*(int*)aln2->p, i + 3, "int2");
	}

	allocator_transfer(&al, &al2);

	for (int i = 0; i < 3; i++) {
		struct allocator_node* aln3 = allocator_find(&al2, buff[i]);
		assert_ptr(aln3, "ptr3");
		expect_true(aln3->p == buff[i], "p3");
		expect_int_equal(*(int*)aln3->p, i, "int3");
	}

	for (int i = 0; i < 3; i++) {
		struct allocator_node* aln4 = allocator_find(&al2, buff2[i]);
		assert_ptr(aln4, "ptr4");
		expect_true(aln4->p == buff2[i], "p4");
		expect_int_equal(*(int*)aln4->p, i + 3, "int4");
	}

	expect_null(al.head, "head");
	expect_null(al.tail, "tail");

	allocator_destroy(&al2);
}

void test_allocator_remove()
{
	test_name(__func__);

	struct allocator al;
	int* buf;
	enum result r;

	allocator_init(&al);
	r = allocator_malloc(&al, &buf, sizeof(int));
	assert_ok(r, "r");
	*buf = 10;
	struct allocator_node* aln = allocator_find(&al, buf);
	assert_ptr(aln, "ptr");
	struct allocator_node* aln2 = allocator_remove(&al, buf);
	assert_true(aln == aln2, "equal");
	struct allocator_node* aln3 = allocator_find(&al, buf);
	assert_null(aln3, "null");
	allocator_destroy(&al);
}

void test_allocator_add()
{
	test_name(__func__);

	struct allocator al;
	int* buf;
	enum result r;

	allocator_init(&al);
	r = allocator_malloc(&al, &buf, sizeof(int));
	assert_ok(r, "r");
	*buf = 10;

	struct allocator_node* aln = allocator_find(&al, buf);
	assert_ptr(aln, "ptr");
	struct allocator_node* aln2 = allocator_remove(&al, buf);
	assert_true(aln == aln2, "equal");
	struct allocator_node* aln3 = allocator_find(&al, buf);
	assert_null(aln3, "null");

	struct allocator al2;
	allocator_init(&al2);
	allocator_add(&al2, aln2);
	struct allocator_node* aln4 = allocator_find(&al2, buf);
	assert_ptr(aln4, "ptr2");
	assert_true(aln4 == aln2, "equal2");

	allocator_destroy(&al);
	allocator_destroy(&al2);
}

void test_allocator_transfer_item()
{
	test_name(__func__);

	struct allocator al;
	struct allocator al2;
	int* buff;
	enum result r;

	allocator_init(&al);
	allocator_init(&al2);
	allocator_init(&al);
	r = allocator_malloc(&al, &buff, sizeof(int));
	assert_ok(r, "r");
	*buff = 10;

	struct allocator_node* aln = allocator_find(&al, buff);
	assert_ptr(aln, "ptr");
	assert_true((int*)aln->p == buff, "equal");

	allocator_transfer_item(&al, &al2, buff);

	struct allocator_node* aln2 = allocator_find(&al2, buff);
	assert_ptr(aln2, "ptr");
	assert_true((int*)aln2->p == buff, "equal");

	allocator_destroy(&al);
	allocator_destroy(&al2);
}

void test_allocator_destroy_item()
{
	test_name(__func__);

	struct allocator al;
	int* buff;
	enum result r;
	allocator_init(&al);
	r = allocator_malloc(&al, &buff, sizeof(int));
	assert_ok(r, "r");
	*buff = 10;
	struct allocator_node* aln = allocator_find(&al, buff);
	assert_ptr(aln, "ptr");
	assert_true((int*)aln->p == buff, "equal");
	allocator_destroy_item(&al, buff);
	struct allocator_node* aln2 = allocator_find(&al, buff);
	assert_null(aln2, "ptr");
	allocator_destroy(&al);
}

void test_allocator()
{
	test_allocator_init();
	test_allocator_malloc();
	test_allocator_find();
	test_allocator_realloc();
	test_allocator_transfer();
	test_allocator_remove();
	test_allocator_add();
	test_allocator_transfer_item();
	test_allocator_destroy_item();
}
