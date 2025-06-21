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

	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1, NULL), "true");

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

	Zinc_expect_false(test, Ake_TypeDefMatch(td0, td1, NULL), "true");

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

	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1, NULL), "true");

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

	Zinc_expect_false(test, Ake_TypeDefMatch(td0, td1, NULL), "false");

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

	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1, NULL), "true");

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

	Zinc_expect_false(test, Ake_TypeDefMatch(td0, td1, NULL), "false");

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

	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1, NULL), "true");

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
	tf00->td = td00;

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
	tf10->td = td10;

	Ake_TypeDefStructAdd(td1, tf10);

	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1, NULL), "true");

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
	tf00->td = td00;

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
	tf10->td = td10;

	Ake_TypeDefStructAdd(td1, tf10);

	Zinc_expect_false(test, Ake_TypeDefMatch(td0, td1, NULL), "false");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchArrayTrue(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_TypeDef* td0 = NULL;
	Ake_TypeDefCreate(&td0);
	Ake_TypeDefSet(td0, AKE_TYPE_DEF_ARRAY);

	Ake_TypeDef* td00 = NULL;
	Ake_TypeDefCreate(&td00);
	Ake_TypeDefSet(td00, AKE_TYPE_DEF_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.array.td = td00;

	Ake_TypeDef* td1 = NULL;
	Ake_TypeDefCreate(&td1);
	Ake_TypeDefSet(td1, AKE_TYPE_DEF_ARRAY);

	Ake_TypeDef* td10 = NULL;
	Ake_TypeDefCreate(&td10);
	Ake_TypeDefSet(td10, AKE_TYPE_DEF_INTEGER);
	td10->data.integer.bit_count = 32;
	td1->data.array.td = td10;

	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1, NULL), "true");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchArrayFalse(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_TypeDef* td0 = NULL;
	Ake_TypeDefCreate(&td0);
	Ake_TypeDefSet(td0, AKE_TYPE_DEF_ARRAY);

	Ake_TypeDef* td00 = NULL;
	Ake_TypeDefCreate(&td00);
	Ake_TypeDefSet(td00, AKE_TYPE_DEF_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.array.td = td00;

	Ake_TypeDef* td1 = NULL;
	Ake_TypeDefCreate(&td1);
	Ake_TypeDefSet(td1, AKE_TYPE_DEF_ARRAY);

	Ake_TypeDef* td10 = NULL;
	Ake_TypeDefCreate(&td10);
	Ake_TypeDefSet(td10, AKE_TYPE_DEF_INTEGER);
	td10->data.integer.bit_count = 64;
	td1->data.array.td = td10;

	Zinc_expect_false(test, Ake_TypeDefMatch(td0, td1, NULL), "false");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchArrayConstTrue(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_TypeDef* td0 = NULL;
	Ake_TypeDefCreate(&td0);
	Ake_TypeDefSet(td0, AKE_TYPE_DEF_ARRAY_CONST);

	Ake_TypeDef* td00 = NULL;
	Ake_TypeDefCreate(&td00);
	Ake_TypeDefSet(td00, AKE_TYPE_DEF_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.array_const.td = td00;

	Ake_TypeDef* td1 = NULL;
	Ake_TypeDefCreate(&td1);
	Ake_TypeDefSet(td1, AKE_TYPE_DEF_ARRAY_CONST);

	Ake_TypeDef* td10 = NULL;
	Ake_TypeDefCreate(&td10);
	Ake_TypeDefSet(td10, AKE_TYPE_DEF_INTEGER);
	td10->data.integer.bit_count = 32;
	td1->data.array_const.td = td10;

	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1, NULL), "match");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchArrayConstFalse(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_TypeDef* td0 = NULL;
	Ake_TypeDefCreate(&td0);
	Ake_TypeDefSet(td0, AKE_TYPE_DEF_ARRAY_CONST);

	Ake_TypeDef* td00 = NULL;
	Ake_TypeDefCreate(&td00);
	Ake_TypeDefSet(td00, AKE_TYPE_DEF_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.array_const.td = td00;

	Ake_TypeDef* td1 = NULL;
	Ake_TypeDefCreate(&td1);
	Ake_TypeDefSet(td1, AKE_TYPE_DEF_ARRAY_CONST);

	Ake_TypeDef* td10 = NULL;
	Ake_TypeDefCreate(&td10);
	Ake_TypeDefSet(td10, AKE_TYPE_DEF_INTEGER);
	td10->data.integer.bit_count = 64;
	td1->data.array_const.td = td10;

	Zinc_expect_false(test, Ake_TypeDefMatch(td0, td1, NULL), "match");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchSliceTrue(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_TypeDef* td0 = NULL;
	Ake_TypeDefCreate(&td0);
	Ake_TypeDefSet(td0, AKE_TYPE_DEF_SLICE);

	Ake_TypeDef* td00 = NULL;
	Ake_TypeDefCreate(&td00);
	Ake_TypeDefSet(td00, AKE_TYPE_DEF_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.slice.td = td00;

	Ake_TypeDef* td1 = NULL;
	Ake_TypeDefCreate(&td1);
	Ake_TypeDefSet(td1, AKE_TYPE_DEF_SLICE);

	Ake_TypeDef* td10 = NULL;
	Ake_TypeDefCreate(&td10);
	Ake_TypeDefSet(td10, AKE_TYPE_DEF_INTEGER);
	td10->data.integer.bit_count = 32;
	td1->data.slice.td = td10;

	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1, NULL), "match");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchSliceFalse(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_TypeDef* td0 = NULL;
	Ake_TypeDefCreate(&td0);
	Ake_TypeDefSet(td0, AKE_TYPE_DEF_SLICE);

	Ake_TypeDef* td00 = NULL;
	Ake_TypeDefCreate(&td00);
	Ake_TypeDefSet(td00, AKE_TYPE_DEF_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.slice.td = td00;

	Ake_TypeDef* td1 = NULL;
	Ake_TypeDefCreate(&td1);
	Ake_TypeDefSet(td1, AKE_TYPE_DEF_SLICE);

	Ake_TypeDef* td10 = NULL;
	Ake_TypeDefCreate(&td10);
	Ake_TypeDefSet(td10, AKE_TYPE_DEF_INTEGER);
	td10->data.integer.bit_count = 64;
	td1->data.slice.td = td10;

	Zinc_expect_false(test, Ake_TypeDefMatch(td0, td1, NULL), "match");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchPointerTrue(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_TypeDef* td0 = NULL;
	Ake_TypeDefCreate(&td0);
	Ake_TypeDefSet(td0, AKE_TYPE_DEF_POINTER);

	Ake_TypeDef* td00 = NULL;
	Ake_TypeDefCreate(&td00);
	Ake_TypeDefSet(td00, AKE_TYPE_DEF_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.pointer.td = td00;

	Ake_TypeDef* td1 = NULL;
	Ake_TypeDefCreate(&td1);
	Ake_TypeDefSet(td1, AKE_TYPE_DEF_POINTER);

	Ake_TypeDef* td10 = NULL;
	Ake_TypeDefCreate(&td10);
	Ake_TypeDefSet(td10, AKE_TYPE_DEF_INTEGER);
	td10->data.integer.bit_count = 32;
	td1->data.pointer.td = td10;

	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1, NULL), "match");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchPointerFalse(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_TypeDef* td0 = NULL;
	Ake_TypeDefCreate(&td0);
	Ake_TypeDefSet(td0, AKE_TYPE_DEF_POINTER);

	Ake_TypeDef* td00 = NULL;
	Ake_TypeDefCreate(&td00);
	Ake_TypeDefSet(td00, AKE_TYPE_DEF_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.pointer.td = td00;

	Ake_TypeDef* td1 = NULL;
	Ake_TypeDefCreate(&td1);
	Ake_TypeDefSet(td1, AKE_TYPE_DEF_POINTER);

	Ake_TypeDef* td10 = NULL;
	Ake_TypeDefCreate(&td10);
	Ake_TypeDefSet(td10, AKE_TYPE_DEF_INTEGER);
	td10->data.integer.bit_count = 64;
	td1->data.pointer.td = td10;

	Zinc_expect_false(test, Ake_TypeDefMatch(td0, td1, NULL), "match");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchFunctionTrue(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_TypeDef* f0 = NULL;
	Ake_TypeDefCreate(&f0);
	Ake_TypeDefSet(f0, AKE_TYPE_DEF_FUNCTION);

	Ake_TypeDef* arg00 = NULL;
	Ake_TypeDefCreate(&arg00);
	Ake_TypeDefSet(arg00, AKE_TYPE_DEF_INTEGER);
	arg00->data.integer.bit_count = 32;

	Ake_TypeParam* tp00 = NULL;
	Ake_TypeParamCreate(&tp00);
	tp00->td = arg00;
	Ake_TypeDefInputAdd(f0, tp00);

	Ake_TypeDef* out0 = NULL;
	Ake_TypeDefCreate(&out0);
	Ake_TypeDefSet(out0, AKE_TYPE_DEF_INTEGER);
	out0->data.integer.bit_count = 32;
	f0->data.function.output = out0;

	Ake_TypeDef* f1 = NULL;
	Ake_TypeDefCreate(&f1);
	Ake_TypeDefSet(f1, AKE_TYPE_DEF_FUNCTION);

	Ake_TypeDef* td10 = NULL;
	Ake_TypeDefCreate(&td10);
	Ake_TypeDefSet(td10, AKE_TYPE_DEF_INTEGER);
	td10->data.integer.bit_count = 32;

	Ake_TypeParam* tp10 = NULL;
	Ake_TypeParamCreate(&tp10);
	tp10->td = td10;
	Ake_TypeDefInputAdd(f1, tp10);

	Ake_TypeDef* out1 = NULL;
	Ake_TypeDefCreate(&out1);
	Ake_TypeDefSet(out1, AKE_TYPE_DEF_INTEGER);
	out1->data.integer.bit_count = 32;
	f1->data.function.output = out1;

	Zinc_expect_true(test, Ake_TypeDefMatch(f0, f1, NULL), "match");

	Ake_TypeDefDestroy(f0);
}

void AkeUnit_TypeDefMatchFunctionFalse(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_TypeDef* f0 = NULL;
	Ake_TypeDefCreate(&f0);
	Ake_TypeDefSet(f0, AKE_TYPE_DEF_FUNCTION);

	Ake_TypeDef* arg00 = NULL;
	Ake_TypeDefCreate(&arg00);
	Ake_TypeDefSet(arg00, AKE_TYPE_DEF_INTEGER);
	arg00->data.integer.bit_count = 32;

	Ake_TypeParam* tp00 = NULL;
	Ake_TypeParamCreate(&tp00);
	tp00->td = arg00;
	Ake_TypeDefInputAdd(f0, tp00);

	Ake_TypeDef* out0 = NULL;
	Ake_TypeDefCreate(&out0);
	Ake_TypeDefSet(out0, AKE_TYPE_DEF_INTEGER);
	out0->data.integer.bit_count = 32;
	f0->data.function.output = out0;

	Ake_TypeDef* f1 = NULL;
	Ake_TypeDefCreate(&f1);
	Ake_TypeDefSet(f1, AKE_TYPE_DEF_FUNCTION);

	Ake_TypeDef* td10 = NULL;
	Ake_TypeDefCreate(&td10);
	Ake_TypeDefSet(td10, AKE_TYPE_DEF_INTEGER);
	td10->data.integer.bit_count = 64;

	Ake_TypeParam* tp10 = NULL;
	Ake_TypeParamCreate(&tp10);
	tp10->td = td10;
	Ake_TypeDefInputAdd(f1, tp10);

	Ake_TypeDef* out1 = NULL;
	Ake_TypeDefCreate(&out1);
	Ake_TypeDefSet(out1, AKE_TYPE_DEF_INTEGER);
	out1->data.integer.bit_count = 32;
	f1->data.function.output = out1;

	Zinc_expect_false(test, Ake_TypeDefMatch(f0, f1, NULL), "match");

	Ake_TypeDefDestroy(f0);
}

void AkeUnit_TypeDefMatchCastTrue(Zinc_test* test)
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

	bool cast = false;
	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1, &cast), "match");
	Zinc_expect_false(test, cast, "cast");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchCastTrue2(Zinc_test* test)
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
	Ake_TypeDefSet(td1, AKE_TYPE_DEF_NATURAL);
	td1->data.natural.bit_count = 32;

	bool cast = false;
	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1, &cast), "match");
	Zinc_expect_true(test, cast, "cast");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchCastTrue3(Zinc_test* test)
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
	td1->data.integer.bit_count = 64;

	bool cast = false;
	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1, &cast), "match");
	Zinc_expect_true(test, cast, "cast");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchCastTrue4(Zinc_test* test)
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
	td1->data.natural.bit_count = 64;

	bool cast = false;
	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1, &cast), "match");
	Zinc_expect_true(test, cast, "cast");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeDefMatchCastTrue5(Zinc_test* test)
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
	td0->data.real.bit_count = 32;

	Ake_TypeDef* td1 = NULL;
	Ake_TypeDefCreate(&td1);
	Ake_TypeDefSet(td1, AKE_TYPE_DEF_REAL);
	td1->data.real.bit_count = 64;

	bool cast = false;
	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1, &cast), "match");
	Zinc_expect_true(test, cast, "cast");

	Ake_TypeDefDestroy(td0);
}

void AkeUnit_TypeCloneInteger(Zinc_test* test)
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

	Ake_TypeDef* td1 = Ake_TypeDefClone(td0);

	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1, NULL), "clone");
}

void AkeUnit_TypeCloneNatural(Zinc_test* test)
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

	Ake_TypeDef* td1 = Ake_TypeDefClone(td0);

	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1, NULL), "clone");
}

void AkeUnit_TypeCloneReal(Zinc_test* test)
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
	td0->data.real.bit_count = 32;

	Ake_TypeDef* td1 = Ake_TypeDefClone(td0);

	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1, NULL), "clone");
}

void AkeUnit_TypeCloneBoolean(Zinc_test* test)
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

	Ake_TypeDef* td1 = Ake_TypeDefClone(td0);

	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1, NULL), "clone");
}

void AkeUnit_TypeCloneStruct(Zinc_test* test)
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

	Ake_TypeField* tf0 = NULL;
	Ake_TypeFieldCreate(&tf0);

	Ake_TypeDef* td00 = NULL;
	Ake_TypeDefCreate(&td00);
	Ake_TypeDefSet(td00, AKE_TYPE_DEF_INTEGER);
	td00->data.integer.bit_count = 32;
	tf0->td = td00;
	Ake_TypeDefStructAdd(td0, tf0);

	Ake_TypeField* tf1 = NULL;
	Ake_TypeFieldCreate(&tf1);

	Ake_TypeDef* td01 = NULL;
	Ake_TypeDefCreate(&td01);
	Ake_TypeDefSet(td01, AKE_TYPE_DEF_NATURAL);
	td01->data.integer.bit_count = 32;
	tf1->td = td01;
	Ake_TypeDefStructAdd(td0, tf1);

	Ake_TypeDef* td1 = Ake_TypeDefClone(td0);

	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1, NULL), "clone");
}

void AkeUnit_TypeCloneArray(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_TypeDef* td0 = NULL;
	Ake_TypeDefCreate(&td0);
	Ake_TypeDefSet(td0, AKE_TYPE_DEF_ARRAY);
	td0->data.array.dim = 10;

	Ake_TypeDef* td00 = NULL;
	Ake_TypeDefCreate(&td00);
	Ake_TypeDefSet(td00, AKE_TYPE_DEF_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.array.td = td00;

	Ake_TypeDef* td1 = Ake_TypeDefClone(td0);

	Zinc_expect_true(test, Ake_TypeDefMatch(td0, td1, NULL), "clone");
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
		Zinc_test_register(test, AkeUnit_TypeDefMatchArrayTrue);
		Zinc_test_register(test, AkeUnit_TypeDefMatchArrayFalse);
		Zinc_test_register(test, AkeUnit_TypeDefMatchArrayConstTrue);
		Zinc_test_register(test, AkeUnit_TypeDefMatchArrayConstFalse);
		Zinc_test_register(test, AkeUnit_TypeDefMatchSliceTrue);
		Zinc_test_register(test, AkeUnit_TypeDefMatchSliceFalse);
		Zinc_test_register(test, AkeUnit_TypeDefMatchPointerTrue);
		Zinc_test_register(test, AkeUnit_TypeDefMatchPointerFalse);
		Zinc_test_register(test, AkeUnit_TypeDefMatchFunctionTrue);
		Zinc_test_register(test, AkeUnit_TypeDefMatchFunctionFalse);
		Zinc_test_register(test, AkeUnit_TypeDefMatchCastTrue);
		Zinc_test_register(test, AkeUnit_TypeDefMatchCastTrue2);
		Zinc_test_register(test, AkeUnit_TypeDefMatchCastTrue3);
		Zinc_test_register(test, AkeUnit_TypeDefMatchCastTrue4);
		Zinc_test_register(test, AkeUnit_TypeDefMatchCastTrue5);
		Zinc_test_register(test, AkeUnit_TypeCloneInteger);
		Zinc_test_register(test, AkeUnit_TypeCloneNatural);
		Zinc_test_register(test, AkeUnit_TypeCloneReal);
		Zinc_test_register(test, AkeUnit_TypeCloneBoolean);
		Zinc_test_register(test, AkeUnit_TypeCloneStruct);
		Zinc_test_register(test, AkeUnit_TypeCloneArray);

		return;
	}

	Zinc_test_perform(test);
}
