#include "assert.h"
#include "alba/buffer.h"
#include "alba/token.h"

void test_token_hash_buffer()
{
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

void test_token()
{
	test_token_hash_buffer();
}
