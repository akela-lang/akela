#include "zinc/assert.h"
#include "zinc/buffer.h"
#include "alba/token.h"

void test_token_hash_buffer()
{
	test_name(__func__);

	struct buffer bf;
	enum result r;
	buffer_init(&bf);

	r = array2buffer("hello", &bf);
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

	struct word_table wt;
	r = word_table_init(&wt, WORD_TABLE_SIZE);
	assert_ok(r, "word_table_init");

	struct token t0;
	token_init(&t0);
	r = array2buffer("t0", &t0.value);
	assert_ok(r, "array2buffer t0");

	struct token t1;
	token_init(&t1);
	r = array2buffer("t1", &t1.value);
	assert_ok(r, "array2buffer t1");

	struct token t2;
	token_init(&t2);
	r = array2buffer("t2", &t2.value);
	assert_ok(r, "array2buffer t2");

	struct word* w;

	w = word_table_get(&wt, &t0.value);
	assert_null(w, "t0");
	r = word_table_add(&wt, &t0.value, t0.type);
	assert_ok(r, "word_table_add");
	w = word_table_get(&wt, &t0.value);
	expect_true(buffer_compare(&w->value, &t0.value), "t0");
	expect_int_equal(w->type, t0.type, "type t0");

	w = word_table_get(&wt, &t1.value);
	assert_null(w, "t1");
	r = word_table_add(&wt, &t1.value, t1.type);
	assert_ok(r, "word_table_add");
	w = word_table_get(&wt, &t1.value);
	expect_true(buffer_compare(&w->value, &t1.value), "t1");
	expect_int_equal(w->type, t1.type, "type t1");

	w = word_table_get(&wt, &t2.value);
	assert_null(w, "t2");
	r = word_table_add(&wt, &t2.value, t2.type);
	assert_ok(r, "word_table_add");
	w = word_table_get(&wt, &t2.value);
	expect_true(buffer_compare(&w->value, &t2.value), "t2");
	expect_int_equal(w->type, t2.type, "type t2");
}

void test_token_word_table_init()
{
	test_name(__func__);

	enum result r;

	struct word_table wt;
	r = word_table_init(&wt, 10);
	assert_ok(r, "word_table_init");

	struct buffer bf;
	buffer_init(&bf);
	r = array2buffer("function", &bf);
	assert_ok(r, "array2buffer function");

	struct word* w;
	
	w = word_table_get(&wt, &bf);
	assert_ptr(w, "ptr t");
	expect_int_equal(w->type, token_function, "function type");
	expect_str(&w->value, "function", "function str");

	struct buffer bf1;
	buffer_init(&bf1);
	r = array2buffer("end", &bf1);
	assert_ok(r, "array2buffer end");

	w = word_table_get(&wt, &bf1);
	assert_ptr(w, "ptr t");
	expect_int_equal(w->type, token_end, "end type");
	expect_str(&w->value, "end", "end str");
}

void test_token()
{
	test_token_hash_buffer();
	test_token_word_table();
	test_token_word_table_init();
}
