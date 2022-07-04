#include "assert.h"
#include "alba/buffer.h"
#include "alba/token.h"

void test_token_hash_buffer()
{
	test_name(__func__);

	struct allocator al;
	struct buffer bf;
	enum result r;
	allocator_init(&al);
	buffer_init(&bf);

	r = array2buffer(&al, "hello", &bf);
	assert_ok(r, "array2buffer");
	unsigned int val;
	unsigned int size = 10;
	val = hash_buffer(&bf, 10);

	unsigned int x = 0;

	x += (unsigned int)'h';
	x %= size;

	x += (unsigned int)'e';
	x %= size;

	x += (unsigned int)'l';
	x %= size;

	x += (unsigned int)'l';
	x %= size;

	x += (unsigned int)'o';
	x %= size;

	expect_uint_equal(val, x, "val");
}

void test_token_word_table()
{
	test_name(__func__);

	enum result r;

	struct allocator al;
	allocator_init(&al);

	struct word_table wt;
	r = word_table_init(&al, &wt, 10);
	assert_ok(r, "word_table_init");

	struct token t0;
	token_init(&t0);
	r = array2buffer(&al, "t0", &t0.value);
	assert_ok(r, "array2buffer t0");

	struct token t1;
	token_init(&t1);
	r = array2buffer(&al, "t1", &t1.value);
	assert_ok(r, "array2buffer t1");

	struct token t2;
	token_init(&t2);
	r = array2buffer(&al, "t2", &t2.value);
	assert_ok(r, "array2buffer t2");

	struct token* t;

	t = word_table_get(&wt, &t0.value);
	assert_null(t, "t0");
	word_table_add(&wt, &t0);
	t = word_table_get(&wt, &t0.value);
	expect_true(t == &t0, "t0");

	t = word_table_get(&wt, &t1.value);
	assert_null(t, "t1");
	word_table_add(&wt, &t1);
	t = word_table_get(&wt, &t1.value);
	expect_true(t == &t1, "t1");

	t = word_table_get(&wt, &t2.value);
	assert_null(t, "t2");
	word_table_add(&wt, &t2);
	t = word_table_get(&wt, &t2.value);
	expect_true(t == &t2, "t2");
}

void test_token_word_table_init()
{
	test_name(__func__);

	enum result r;

	struct allocator al;
	allocator_init(&al);

	struct word_table wt;
	r = word_table_init(&al, &wt, 10);
	assert_ok(r, "word_table_init");

	struct buffer bf;
	buffer_init(&bf);
	r = array2buffer(&al, "function", &bf);
	assert_ok(r, "array2buffer function");

	struct token* t = word_table_get(&wt, &bf);
	assert_ptr(t, "ptr t");
	expect_int_equal(t->type, token_function, "function type");
	expect_str(&t->value, "function", "function str");
}

void test_token()
{
	test_token_hash_buffer();
	test_token_word_table();
	test_token_word_table_init();
}
