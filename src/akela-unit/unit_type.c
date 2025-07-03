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

	Ake_Ast* n;
    Ake_ast_create(&n);
	n->kind = Ake_ast_type_type;

	Ake_Ast* a;
    Ake_ast_create(&a);
	a->kind = Ake_ast_type_type;
    Ake_ast_add(n, a);

	Ake_Ast* b;
    Ake_ast_create(&b);
	b->kind = Ake_ast_type_type;
    Ake_ast_add(n, b);

	if (!Zinc_expect_ptr(test, n, "ptr n")) {
		return Zinc_assert();
	}

	Ake_Ast* n0 = Ake_ast_get(n, 0);
	if (!Zinc_expect_ptr(test, n0, "ptr a")) {
		return Zinc_assert();
	}
	Zinc_expect_ptr_equal(test, n0, a, "a");

	Ake_Ast* n1 = Ake_ast_get(n, 1);
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

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_INTEGER);
	td0->data.integer.bit_count = 32;

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_INTEGER);
	td1->data.integer.bit_count = 32;

	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, NULL), "true");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchIntegerFalse(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_INTEGER);
	td0->data.integer.bit_count = 32;

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_INTEGER);
	td1->data.integer.bit_count = 16;

	Zinc_expect_false(test, Ake_TypeMatch(td0, td1, NULL), "true");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchNaturalTrue(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_NATURAL);
	td0->data.natural.bit_count = 32;

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_NATURAL);
	td1->data.natural.bit_count = 32;

	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, NULL), "true");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchNaturalFalse(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_NATURAL);
	td0->data.natural.bit_count = 32;

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_NATURAL);
	td1->data.natural.bit_count = 16;

	Zinc_expect_false(test, Ake_TypeMatch(td0, td1, NULL), "false");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchRealTrue(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_REAL);
	td0->data.natural.bit_count = 64;

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_REAL);
	td1->data.natural.bit_count = 64;

	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, NULL), "true");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchRealFalse(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_REAL);
	td0->data.natural.bit_count = 64;

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_REAL);
	td1->data.natural.bit_count = 32;

	Zinc_expect_false(test, Ake_TypeMatch(td0, td1, NULL), "false");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchBoolean(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_BOOLEAN);

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_BOOLEAN);

	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, NULL), "true");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchStructTrue(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_STRUCT);

	Ake_TypeField* tf00 = NULL;
	Ake_TypeFieldCreate(&tf00);

	Ake_Type* td00 = NULL;
	Ake_TypeCreate(&td00);
	Ake_TypeSet(td00, AKE_TYPE_INTEGER);
	td00->data.integer.bit_count = 32;
	tf00->type = td00;

	Ake_TypeStructAdd(td0, tf00);

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_STRUCT);

	Ake_TypeField* tf10 = NULL;
	Ake_TypeFieldCreate(&tf10);

	Ake_Type* td10 = NULL;
	Ake_TypeCreate(&td10);
	Ake_TypeSet(td10, AKE_TYPE_INTEGER);
	td10->data.integer.bit_count = 32;
	tf10->type = td10;

	Ake_TypeStructAdd(td1, tf10);

	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, NULL), "true");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchStructFalse(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_STRUCT);

	Ake_TypeField* tf00 = NULL;
	Ake_TypeFieldCreate(&tf00);

	Ake_Type* td00 = NULL;
	Ake_TypeCreate(&td00);
	Ake_TypeSet(td00, AKE_TYPE_INTEGER);
	td00->data.integer.bit_count = 32;
	tf00->type = td00;

	Ake_TypeStructAdd(td0, tf00);

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_STRUCT);

	Ake_TypeField* tf10 = NULL;
	Ake_TypeFieldCreate(&tf10);

	Ake_Type* td10 = NULL;
	Ake_TypeCreate(&td10);
	Ake_TypeSet(td10, AKE_TYPE_INTEGER);
	td10->data.integer.bit_count = 64;
	tf10->type = td10;

	Ake_TypeStructAdd(td1, tf10);

	Zinc_expect_false(test, Ake_TypeMatch(td0, td1, NULL), "false");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchArrayTrue(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_ARRAY);

	Ake_Type* td00 = NULL;
	Ake_TypeCreate(&td00);
	Ake_TypeSet(td00, AKE_TYPE_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.array.type = td00;

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_ARRAY);

	Ake_Type* td10 = NULL;
	Ake_TypeCreate(&td10);
	Ake_TypeSet(td10, AKE_TYPE_INTEGER);
	td10->data.integer.bit_count = 32;
	td1->data.array.type = td10;

	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, NULL), "true");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchArrayFalse(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_ARRAY);

	Ake_Type* td00 = NULL;
	Ake_TypeCreate(&td00);
	Ake_TypeSet(td00, AKE_TYPE_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.array.type = td00;

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_ARRAY);

	Ake_Type* td10 = NULL;
	Ake_TypeCreate(&td10);
	Ake_TypeSet(td10, AKE_TYPE_INTEGER);
	td10->data.integer.bit_count = 64;
	td1->data.array.type = td10;

	Zinc_expect_false(test, Ake_TypeMatch(td0, td1, NULL), "false");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchArrayConstTrue(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_ARRAY);
	td0->data.array.is_const = true;

	Ake_Type* td00 = NULL;
	Ake_TypeCreate(&td00);
	Ake_TypeSet(td00, AKE_TYPE_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.array.type = td00;

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_ARRAY);
	td1->data.array.is_const = true;

	Ake_Type* td10 = NULL;
	Ake_TypeCreate(&td10);
	Ake_TypeSet(td10, AKE_TYPE_INTEGER);
	td10->data.integer.bit_count = 32;
	td1->data.array.type = td10;

	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, NULL), "match");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchArrayConstFalse(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_ARRAY);
	td0->data.array.is_const = true;

	Ake_Type* td00 = NULL;
	Ake_TypeCreate(&td00);
	Ake_TypeSet(td00, AKE_TYPE_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.array.type = td00;

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_ARRAY);
	td1->data.array.is_const = true;

	Ake_Type* td10 = NULL;
	Ake_TypeCreate(&td10);
	Ake_TypeSet(td10, AKE_TYPE_INTEGER);
	td10->data.integer.bit_count = 64;
	td1->data.array.type = td10;

	Zinc_expect_false(test, Ake_TypeMatch(td0, td1, NULL), "match");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchSliceTrue(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_SLICE);

	Ake_Type* td00 = NULL;
	Ake_TypeCreate(&td00);
	Ake_TypeSet(td00, AKE_TYPE_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.slice.type = td00;

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_SLICE);

	Ake_Type* td10 = NULL;
	Ake_TypeCreate(&td10);
	Ake_TypeSet(td10, AKE_TYPE_INTEGER);
	td10->data.integer.bit_count = 32;
	td1->data.slice.type = td10;

	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, NULL), "match");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchSliceFalse(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_SLICE);

	Ake_Type* td00 = NULL;
	Ake_TypeCreate(&td00);
	Ake_TypeSet(td00, AKE_TYPE_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.slice.type = td00;

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_SLICE);

	Ake_Type* td10 = NULL;
	Ake_TypeCreate(&td10);
	Ake_TypeSet(td10, AKE_TYPE_INTEGER);
	td10->data.integer.bit_count = 64;
	td1->data.slice.type = td10;

	Zinc_expect_false(test, Ake_TypeMatch(td0, td1, NULL), "match");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchPointerTrue(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_POINTER);

	Ake_Type* td00 = NULL;
	Ake_TypeCreate(&td00);
	Ake_TypeSet(td00, AKE_TYPE_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.pointer.type = td00;

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_POINTER);

	Ake_Type* td10 = NULL;
	Ake_TypeCreate(&td10);
	Ake_TypeSet(td10, AKE_TYPE_INTEGER);
	td10->data.integer.bit_count = 32;
	td1->data.pointer.type = td10;

	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, NULL), "match");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchPointerFalse(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_POINTER);

	Ake_Type* td00 = NULL;
	Ake_TypeCreate(&td00);
	Ake_TypeSet(td00, AKE_TYPE_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.pointer.type = td00;

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_POINTER);

	Ake_Type* td10 = NULL;
	Ake_TypeCreate(&td10);
	Ake_TypeSet(td10, AKE_TYPE_INTEGER);
	td10->data.integer.bit_count = 64;
	td1->data.pointer.type = td10;

	Zinc_expect_false(test, Ake_TypeMatch(td0, td1, NULL), "match");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchFunctionTrue(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* f0 = NULL;
	Ake_TypeCreate(&f0);
	Ake_TypeSet(f0, AKE_TYPE_FUNCTION);

	Ake_Type* arg00 = NULL;
	Ake_TypeCreate(&arg00);
	Ake_TypeSet(arg00, AKE_TYPE_INTEGER);
	arg00->data.integer.bit_count = 32;

	Ake_TypeParam* tp00 = NULL;
	Ake_TypeParamCreate(&tp00);
	tp00->type = arg00;
	Ake_TypeInputAdd(f0, tp00);

	Ake_Type* out0 = NULL;
	Ake_TypeCreate(&out0);
	Ake_TypeSet(out0, AKE_TYPE_INTEGER);
	out0->data.integer.bit_count = 32;
	f0->data.function.output = out0;

	Ake_Type* f1 = NULL;
	Ake_TypeCreate(&f1);
	Ake_TypeSet(f1, AKE_TYPE_FUNCTION);

	Ake_Type* td10 = NULL;
	Ake_TypeCreate(&td10);
	Ake_TypeSet(td10, AKE_TYPE_INTEGER);
	td10->data.integer.bit_count = 32;

	Ake_TypeParam* tp10 = NULL;
	Ake_TypeParamCreate(&tp10);
	tp10->type = td10;
	Ake_TypeInputAdd(f1, tp10);

	Ake_Type* out1 = NULL;
	Ake_TypeCreate(&out1);
	Ake_TypeSet(out1, AKE_TYPE_INTEGER);
	out1->data.integer.bit_count = 32;
	f1->data.function.output = out1;

	Zinc_expect_true(test, Ake_TypeMatch(f0, f1, NULL), "match");

	Ake_TypeDestroy(f0);
}

void AkeUnit_TypeDefMatchFunctionFalse(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* f0 = NULL;
	Ake_TypeCreate(&f0);
	Ake_TypeSet(f0, AKE_TYPE_FUNCTION);

	Ake_Type* arg00 = NULL;
	Ake_TypeCreate(&arg00);
	Ake_TypeSet(arg00, AKE_TYPE_INTEGER);
	arg00->data.integer.bit_count = 32;

	Ake_TypeParam* tp00 = NULL;
	Ake_TypeParamCreate(&tp00);
	tp00->type = arg00;
	Ake_TypeInputAdd(f0, tp00);

	Ake_Type* out0 = NULL;
	Ake_TypeCreate(&out0);
	Ake_TypeSet(out0, AKE_TYPE_INTEGER);
	out0->data.integer.bit_count = 32;
	f0->data.function.output = out0;

	Ake_Type* f1 = NULL;
	Ake_TypeCreate(&f1);
	Ake_TypeSet(f1, AKE_TYPE_FUNCTION);

	Ake_Type* td10 = NULL;
	Ake_TypeCreate(&td10);
	Ake_TypeSet(td10, AKE_TYPE_INTEGER);
	td10->data.integer.bit_count = 64;

	Ake_TypeParam* tp10 = NULL;
	Ake_TypeParamCreate(&tp10);
	tp10->type = td10;
	Ake_TypeInputAdd(f1, tp10);

	Ake_Type* out1 = NULL;
	Ake_TypeCreate(&out1);
	Ake_TypeSet(out1, AKE_TYPE_INTEGER);
	out1->data.integer.bit_count = 32;
	f1->data.function.output = out1;

	Zinc_expect_false(test, Ake_TypeMatch(f0, f1, NULL), "match");

	Ake_TypeDestroy(f0);
}

void AkeUnit_TypeDefMatchCastTrue(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_INTEGER);
	td0->data.integer.bit_count = 32;

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_INTEGER);
	td1->data.integer.bit_count = 32;

	bool cast = false;
	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, &cast), "match");
	Zinc_expect_false(test, cast, "cast");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchCastTrue2(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_INTEGER);
	td0->data.integer.bit_count = 32;

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_NATURAL);
	td1->data.natural.bit_count = 32;

	bool cast = false;
	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, &cast), "match");
	Zinc_expect_true(test, cast, "cast");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchCastTrue3(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_INTEGER);
	td0->data.integer.bit_count = 32;

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_INTEGER);
	td1->data.integer.bit_count = 64;

	bool cast = false;
	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, &cast), "match");
	Zinc_expect_true(test, cast, "cast");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchCastTrue4(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_NATURAL);
	td0->data.natural.bit_count = 32;

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_NATURAL);
	td1->data.natural.bit_count = 64;

	bool cast = false;
	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, &cast), "match");
	Zinc_expect_true(test, cast, "cast");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeDefMatchCastTrue5(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_REAL);
	td0->data.real.bit_count = 32;

	Ake_Type* td1 = NULL;
	Ake_TypeCreate(&td1);
	Ake_TypeSet(td1, AKE_TYPE_REAL);
	td1->data.real.bit_count = 64;

	bool cast = false;
	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, &cast), "match");
	Zinc_expect_true(test, cast, "cast");

	Ake_TypeDestroy(td0);
}

void AkeUnit_TypeCloneInteger(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_INTEGER);
	td0->data.integer.bit_count = 32;

	Ake_Type* td1 = Ake_TypeClone(td0);

	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, NULL), "clone");
}

void AkeUnit_TypeCloneNatural(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_NATURAL);
	td0->data.natural.bit_count = 32;

	Ake_Type* td1 = Ake_TypeClone(td0);

	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, NULL), "clone");
}

void AkeUnit_TypeCloneReal(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_REAL);
	td0->data.real.bit_count = 32;

	Ake_Type* td1 = Ake_TypeClone(td0);

	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, NULL), "clone");
}

void AkeUnit_TypeCloneBoolean(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_BOOLEAN);

	Ake_Type* td1 = Ake_TypeClone(td0);

	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, NULL), "clone");
}

void AkeUnit_TypeCloneStruct(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_STRUCT);

	Ake_TypeField* tf0 = NULL;
	Ake_TypeFieldCreate(&tf0);

	Ake_Type* td00 = NULL;
	Ake_TypeCreate(&td00);
	Ake_TypeSet(td00, AKE_TYPE_INTEGER);
	td00->data.integer.bit_count = 32;
	tf0->type = td00;
	Ake_TypeStructAdd(td0, tf0);

	Ake_TypeField* tf1 = NULL;
	Ake_TypeFieldCreate(&tf1);

	Ake_Type* td01 = NULL;
	Ake_TypeCreate(&td01);
	Ake_TypeSet(td01, AKE_TYPE_NATURAL);
	td01->data.integer.bit_count = 32;
	tf1->type = td01;
	Ake_TypeStructAdd(td0, tf1);

	Ake_Type* td1 = Ake_TypeClone(td0);

	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, NULL), "clone");
}

void AkeUnit_TypeCloneArray(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_ARRAY);
	td0->data.array.dim = 10;

	Ake_Type* td00 = NULL;
	Ake_TypeCreate(&td00);
	Ake_TypeSet(td00, AKE_TYPE_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.array.type = td00;

	Ake_Type* td1 = Ake_TypeClone(td0);

	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, NULL), "clone");
}

void AkeUnit_TypeCloneArrayConst(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_ARRAY);
	td0->data.array.is_const = true;
	td0->data.array.dim = 10;

	Ake_Type* td00 = NULL;
	Ake_TypeCreate(&td00);
	Ake_TypeSet(td00, AKE_TYPE_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.array.type = td00;

	Ake_Type* td1 = Ake_TypeClone(td0);

	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, NULL), "clone");
}

void AkeUnit_TypeCloneSlice(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_SLICE);

	Ake_Type* td00 = NULL;
	Ake_TypeCreate(&td00);
	Ake_TypeSet(td00, AKE_TYPE_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.slice.type = td00;

	Ake_Type* td1 = Ake_TypeClone(td0);

	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, NULL), "clone");
}

void AkeUnit_TypeClonePointer(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_POINTER);

	Ake_Type* td00 = NULL;
	Ake_TypeCreate(&td00);
	Ake_TypeSet(td00, AKE_TYPE_INTEGER);
	td00->data.integer.bit_count = 32;
	td0->data.pointer.type = td00;

	Ake_Type* td1 = Ake_TypeClone(td0);

	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, NULL), "clone");
}

void AkeUnit_TypeCloneFunction(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Ake_Type* td0 = NULL;
	Ake_TypeCreate(&td0);
	Ake_TypeSet(td0, AKE_TYPE_FUNCTION);

	Ake_Type* td00 = NULL;
	Ake_TypeCreate(&td00);
	Ake_TypeSet(td00, AKE_TYPE_INTEGER);
	td00->data.integer.bit_count = 32;

	Ake_TypeParam* arg0 = NULL;
	Ake_TypeParamCreate(&arg0);
	arg0->type = td00;
	Ake_TypeInputAdd(td0, arg0);

	Ake_Type* td01 = NULL;
	Ake_TypeCreate(&td01);
	Ake_TypeSet(td01, AKE_TYPE_NATURAL);
	td01->data.natural.bit_count = 32;

	Ake_TypeParam* arg1 = NULL;
	Ake_TypeParamCreate(&arg1);
	arg1->type = td01;
	Ake_TypeInputAdd(td0, arg1);

	Ake_Type* out = NULL;
	Ake_TypeCreate(&out);
	Ake_TypeSet(out, AKE_TYPE_INTEGER);
	out->data.integer.bit_count = 32;
	td0->data.function.output = out;

	Ake_Type* td1 = Ake_TypeClone(td0);

	Zinc_expect_true(test, Ake_TypeMatch(td0, td1, NULL), "clone");
}

void AkeUnit_TypeName(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	Zinc_expect_strcmp(test, Ake_TypeName(AKE_TYPE_NONE), "none", "none");
	Zinc_expect_strcmp(test, Ake_TypeName(AKE_TYPE_INTEGER), "integer", "integer");
	Zinc_expect_strcmp(test, Ake_TypeName(AKE_TYPE_NATURAL), "natural", "natural");
	Zinc_expect_strcmp(test, Ake_TypeName(AKE_TYPE_REAL), "real", "real");
	Zinc_expect_strcmp(test, Ake_TypeName(AKE_TYPE_BOOLEAN), "boolean", "boolean");
	Zinc_expect_strcmp(test, Ake_TypeName(AKE_TYPE_STRUCT), "struct", "struct");
	Zinc_expect_strcmp(test, Ake_TypeName(AKE_TYPE_ARRAY), "array", "array");
	Zinc_expect_strcmp(test, Ake_TypeName(AKE_TYPE_SLICE), "slice", "slice");
	Zinc_expect_strcmp(test, Ake_TypeName(AKE_TYPE_POINTER), "pointer", "pointer");
	Zinc_expect_strcmp(test, Ake_TypeName(AKE_TYPE_FUNCTION), "function", "function");
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
		Zinc_test_register(test, AkeUnit_TypeCloneArrayConst);
		Zinc_test_register(test, AkeUnit_TypeCloneSlice);
		Zinc_test_register(test, AkeUnit_TypeClonePointer);
		Zinc_test_register(test, AkeUnit_TypeCloneFunction);
		Zinc_test_register(test, AkeUnit_TypeName);

		return;
	}

	Zinc_test_perform(test);
}
