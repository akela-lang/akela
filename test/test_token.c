#include "zinc/assert.h"
#include "zinc/buffer.h"
#include "alba/token.h"

/* dynamic-output-none */
void test_token_hash_buffer()
{
	test_name(__func__);

	struct buffer bf;
	buffer_init(&bf);

	/* allocate bf{} */
	array2buffer("hello", &bf);
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

	/* destroy bf{} */
	buffer_destroy(&bf);
}

/* dynamic-output-none */
void test_token_word_table()
{
	test_name(__func__);

	/* allocate wt{} */
	struct word_table wt;
	word_table_init(&wt, WORD_TABLE_SIZE);

	/* allocate t0{} */
	struct token t0;
	token_init(&t0);
	array2buffer("t0", &t0.value);

	/* allocate t1{} */
	struct token t1;
	token_init(&t1);
	array2buffer("t1", &t1.value);

	/* allocate t2{} */
	struct token t2;
	token_init(&t2);
	array2buffer("t2", &t2.value);

	struct word* w;

	w = word_table_get(&wt, &t0.value);
	assert_null(w, "t0");

	/* allocate wt{} */
	word_table_add(&wt, &t0.value, t0.type);
	w = word_table_get(&wt, &t0.value);
	expect_true(buffer_compare(&w->value, &t0.value), "t0");
	expect_int_equal(w->type, t0.type, "type t0");

	w = word_table_get(&wt, &t1.value);
	assert_null(w, "t1");

	/* allocate wt{} */
	word_table_add(&wt, &t1.value, t1.type);
	w = word_table_get(&wt, &t1.value);
	expect_true(buffer_compare(&w->value, &t1.value), "t1");
	expect_int_equal(w->type, t1.type, "type t1");

	w = word_table_get(&wt, &t2.value);
	assert_null(w, "t2");

	/* allocate wt{} */
	word_table_add(&wt, &t2.value, t2.type);
	w = word_table_get(&wt, &t2.value);
	expect_true(buffer_compare(&w->value, &t2.value), "t2");
	expect_int_equal(w->type, t2.type, "type t2");

	/* destroy wt{} t0{} t1{} t2{} */
	word_table_destroy(&wt);
	token_destroy(&t0);
	token_destroy(&t1);
	token_destroy(&t2);
}

/* dynamic-output-none */
void test_token_word_table_init()
{
	test_name(__func__);

	struct word_table wt;

	/* allocate wt{} */
	word_table_init(&wt, 10);

	/* allocate bf{} */
	struct buffer bf;
	buffer_init(&bf);
	array2buffer("function", &bf);

	struct word* w;
	
	w = word_table_get(&wt, &bf);
	assert_ptr(w, "ptr t");
	expect_int_equal(w->type, token_function, "function type");
	expect_str(&w->value, "function", "function str");

	/* allocate bf1{} */
	struct buffer bf1;
	buffer_init(&bf1);
	array2buffer("end", &bf1);

	w = word_table_get(&wt, &bf1);
	assert_ptr(w, "ptr t");
	expect_int_equal(w->type, token_end, "end type");
	expect_str(&w->value, "end", "end str");

	/* destroy wt{} bf{} bf1{} */
	word_table_destroy(&wt);
	buffer_destroy(&bf);
	buffer_destroy(&bf1);
}

/* dynamic-output-none */
void test_token()
{
	test_token_hash_buffer();
	test_token_word_table();
	test_token_word_table_init();
}
