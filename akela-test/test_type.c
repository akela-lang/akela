#include "akela/type_def.h"
#include "zinc/unit_test.h"
#include "akela/ast.h"

void test_type_use1()
{
	test_name(__func__);

	Cob_ast* n;
    Cob_ast_create(&n);
	n->type = Ast_type_type;

	Cob_ast* a;
    Cob_ast_create(&a);
	a->type = Ast_type_type;
    Cob_ast_add(n, a);

	Cob_ast* b;
    Cob_ast_create(&b);
	b->type = Ast_type_type;
    Cob_ast_add(n, b);

	assert_ptr(n, "ptr n");

	Cob_ast* n0 = Ast_node_get(n, 0);
	assert_ptr(n0, "ptr a");
	expect_ptr_equal(n0, a, "a");

	Cob_ast* n1 = Ast_node_get(n, 1);
	assert_ptr(n1, "ptr b");
	expect_ptr_equal(n1, b, "b");

    Cob_ast_destroy(n);
}


void test_type()
{
	test_type_use1();
}
