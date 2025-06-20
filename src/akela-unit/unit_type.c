#include "akela/type.h"
#include "akela/ast.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_type_use1(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_ast* n;
    Ake_ast_create(&n);
	n->type = Ake_ast_type_type;

	Ake_ast* a;
    Ake_ast_create(&a);
	a->type = Ake_ast_type_type;
    Ake_ast_add(n, a);

	Ake_ast* b;
    Ake_ast_create(&b);
	b->type = Ake_ast_type_type;
    Ake_ast_add(n, b);

	if (!Zinc_expect_ptr(test, n, "ptr n")) {
		return Zinc_assert();
	}

	Ake_ast* n0 = Ast_node_get(n, 0);
	if (!Zinc_expect_ptr(test, n0, "ptr a")) {
		return Zinc_assert();
	}
	Zinc_expect_ptr_equal(test, n0, a, "a");

	Ake_ast* n1 = Ast_node_get(n, 1);
	if (!Zinc_expect_ptr(test, n1, "ptr b")) {
		return Zinc_assert();
	}
	Zinc_expect_ptr_equal(test, n1, b, "b");

    Ake_ast_destroy(n);
}

void AkeUnit_TypeDefMatchIntegerTrue(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_TypeDef* td0 = NULL;
	Ake_TypeDefCreate(&td0);
	Ake_TypeDefSet(td0, AKE_TYPE_DEF_INTEGER);
	td0->data.integer.bit_count = 32;

	Ake_TypeDef* td1 = NULL;
	Ake_TypeDefCreate(&td1);
	Ake_TypeDefSet(td1, AKE_TYPE_DEF_INTEGER);
	td1->data.integer.bit_count = 32;

	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1), "true");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchIntegerFalse(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_TypeDef* td0 = NULL;
	Ake_TypeDefCreate(&td0);
	Ake_TypeDefSet(td0, AKE_TYPE_DEF_INTEGER);
	td0->data.integer.bit_count = 32;

	Ake_TypeDef* td1 = NULL;
	Ake_TypeDefCreate(&td1);
	Ake_TypeDefSet(td1, AKE_TYPE_DEF_INTEGER);
	td1->data.integer.bit_count = 16;

	Zinc_expect_false(test, Ake_TypeDefMatch(td0, td1), "true");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchNaturalTrue(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_TypeDef* td0 = NULL;
	Ake_TypeDefCreate(&td0);
	Ake_TypeDefSet(td0, AKE_TYPE_DEF_NATURAL);
	td0->data.natural.bit_count = 32;

	Ake_TypeDef* td1 = NULL;
	Ake_TypeDefCreate(&td1);
	Ake_TypeDefSet(td1, AKE_TYPE_DEF_NATURAL);
	td1->data.natural.bit_count = 32;

	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1), "true");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchNaturalFalse(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_TypeDef* td0 = NULL;
	Ake_TypeDefCreate(&td0);
	Ake_TypeDefSet(td0, AKE_TYPE_DEF_NATURAL);
	td0->data.natural.bit_count = 32;

	Ake_TypeDef* td1 = NULL;
	Ake_TypeDefCreate(&td1);
	Ake_TypeDefSet(td1, AKE_TYPE_DEF_NATURAL);
	td1->data.natural.bit_count = 16;

	Zinc_expect_false(test, Ake_TypeDefMatch(td0, td1), "false");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchRealTrue(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_TypeDef* td0 = NULL;
	Ake_TypeDefCreate(&td0);
	Ake_TypeDefSet(td0, AKE_TYPE_DEF_REAL);
	td0->data.natural.bit_count = 64;

	Ake_TypeDef* td1 = NULL;
	Ake_TypeDefCreate(&td1);
	Ake_TypeDefSet(td1, AKE_TYPE_DEF_REAL);
	td1->data.natural.bit_count = 64;

	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1), "true");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchRealFalse(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_TypeDef* td0 = NULL;
	Ake_TypeDefCreate(&td0);
	Ake_TypeDefSet(td0, AKE_TYPE_DEF_REAL);
	td0->data.natural.bit_count = 64;

	Ake_TypeDef* td1 = NULL;
	Ake_TypeDefCreate(&td1);
	Ake_TypeDefSet(td1, AKE_TYPE_DEF_REAL);
	td1->data.natural.bit_count = 32;

	Zinc_expect_false(test, Ake_TypeDefMatch(td0, td1), "false");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchBoolean(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_TypeDef* td0 = NULL;
	Ake_TypeDefCreate(&td0);
	Ake_TypeDefSet(td0, AKE_TYPE_DEF_BOOLEAN);

	Ake_TypeDef* td1 = NULL;
	Ake_TypeDefCreate(&td1);
	Ake_TypeDefSet(td1, AKE_TYPE_DEF_BOOLEAN);

	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1), "true");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchStructTrue(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_TypeDef* td0 = NULL;
	Ake_TypeDefCreate(&td0);
	Ake_TypeDefSet(td0, AKE_TYPE_DEF_STRUCT);

	Ake_TypeField* tf00 = NULL;
	Ake_TypeFieldCreate(&tf00);

	Ake_TypeDef* td00 = NULL;
	Ake_TypeDefCreate(&td00);
	Ake_TypeDefSet(td00, AKE_TYPE_DEF_INTEGER);
	td00->data.integer.bit_count = 32;

	Ake_TypeUse* tu00 = NULL;
	Ake_TypeUseCreate(&tu00);
	Ake_TypeUseSet(tu00, AKE_TYPE_USE_SCALAR);
	tu00->data.scalar.td = td00;
	tf00->tu = tu00;

	Ake_TypeDefStructAdd(td0, tf00);

	Ake_TypeDef* td1 = NULL;
	Ake_TypeDefCreate(&td1);
	Ake_TypeDefSet(td1, AKE_TYPE_DEF_STRUCT);

	Ake_TypeField* tf10 = NULL;
	Ake_TypeFieldCreate(&tf10);

	Ake_TypeDef* td10 = NULL;
	Ake_TypeDefCreate(&td10);
	Ake_TypeDefSet(td10, AKE_TYPE_DEF_INTEGER);
	td10->data.integer.bit_count = 32;

	Ake_TypeUse* tu10 = NULL;
	Ake_TypeUseCreate(&tu10);
	Ake_TypeUseSet(tu10, AKE_TYPE_USE_SCALAR);
	tu10->data.scalar.td = td10;
	tf10->tu = tu10;

	Ake_TypeDefStructAdd(td1, tf10);

	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1), "true");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchStructFalse(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_TypeDef* td0 = NULL;
	Ake_TypeDefCreate(&td0);
	Ake_TypeDefSet(td0, AKE_TYPE_DEF_STRUCT);

	Ake_TypeField* tf00 = NULL;
	Ake_TypeFieldCreate(&tf00);

	Ake_TypeDef* td00 = NULL;
	Ake_TypeDefCreate(&td00);
	Ake_TypeDefSet(td00, AKE_TYPE_DEF_INTEGER);
	td00->data.integer.bit_count = 32;

	Ake_TypeUse* tu00 = NULL;
	Ake_TypeUseCreate(&tu00);
	Ake_TypeUseSet(tu00, AKE_TYPE_USE_SCALAR);
	tu00->data.scalar.td = td00;
	tf00->tu = tu00;

	Ake_TypeDefStructAdd(td0, tf00);

	Ake_TypeDef* td1 = NULL;
	Ake_TypeDefCreate(&td1);
	Ake_TypeDefSet(td1, AKE_TYPE_DEF_STRUCT);

	Ake_TypeField* tf10 = NULL;
	Ake_TypeFieldCreate(&tf10);

	Ake_TypeDef* td10 = NULL;
	Ake_TypeDefCreate(&td10);
	Ake_TypeDefSet(td10, AKE_TYPE_DEF_INTEGER);
	td10->data.integer.bit_count = 64;

	Ake_TypeUse* tu10 = NULL;
	Ake_TypeUseCreate(&tu10);
	Ake_TypeUseSet(tu10, AKE_TYPE_USE_SCALAR);
	tu10->data.scalar.td = td10;
	tf10->tu = tu10;

	Ake_TypeDefStructAdd(td1, tf10);

	Zinc_expect_false(test, Ake_TypeDefMatch(td0, td1), "false");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_type(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;

		Zinc_test_register(test, AkeUnit_type_use1);
		Zinc_test_register(test, AkeUnit_TypeDefMatchIntegerTrue);
		Zinc_test_register(test, AkeUnit_TypeDefMatchIntegerFalse);
		Zinc_test_register(test, AkeUnit_TypeDefMatchNaturalTrue);
		Zinc_test_register(test, AkeUnit_TypeDefMatchNaturalFalse);
		Zinc_test_register(test, AkeUnit_TypeDefMatchRealTrue);
		Zinc_test_register(test, AkeUnit_TypeDefMatchRealFalse);
		Zinc_test_register(test, AkeUnit_TypeDefMatchBoolean);
		Zinc_test_register(test, AkeUnit_TypeDefMatchStructTrue);
		Zinc_test_register(test, AkeUnit_TypeDefMatchStructFalse);

		return;
	}

	Zinc_test_perform(test);
}
