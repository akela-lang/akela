#include "centipede/parse_tools.h"
#include "centipede/comp_table.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void CentUnit_build_number_integer(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct, "241");

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr value")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_natural, "type root");
    Zinc_expect_uint64_t_equal(test, root->data.natural, 241, "integer root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_number_fp(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct, "1.5");

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr value")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_real, "type root");
    Zinc_expect_double_equal(test, root->data.real, 1.5, "fp root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_string(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct, "\"hello\"");

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr value")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_string, "type root");
    Zinc_expect_string(test, &root->data.string, "hello", "string root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_boolean_true(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct, "true");

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr value")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_boolean, "type root");
    Zinc_expect_true(test, root->data.boolean, "string root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_boolean_false(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct, "false");

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr value")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_boolean, "type root");
    Zinc_expect_false(test, root->data.boolean, "string root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_enum0(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Symbol_type {\n"
        "    Variable\n"
        "    Type\n"
        "    Info\n"
        "}\n"
        "Symbol_type::Variable\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_enum, "type root");
    Zinc_expect_string(test, &root->data.enumeration.enum_type->name, "Symbol_type", "id1 root");
    Zinc_expect_string(test, &root->data.enumeration.enum_value->display, "Variable", "id2 root");
    Zinc_expect_size_t_equal(test, root->data.enumeration.number, 0, "enumeration number root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_enum1(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Symbol_type {\n"
        "    Variable\n"
        "    Type\n"
        "    Info\n"
        "}\n"
        "Symbol_type::Type\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr value")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_enum, "type root");
    Zinc_expect_string(test, &root->data.enumeration.enum_type->name, "Symbol_type", "id1 root");
    Zinc_expect_string(test, &root->data.enumeration.enum_value->display, "Type", "id2 root");
    Zinc_expect_size_t_equal(test, root->data.enumeration.number, 1, "enumeration number root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_enum2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Symbol_type {\n"
        "    Variable\n"
        "    Type\n"
        "    Info\n"
        "}\n"
        "Symbol_type::Info\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr value")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_enum, "type root");
    Zinc_expect_string(test, &root->data.enumeration.enum_type->name, "Symbol_type", "id1 root");
    Zinc_expect_string(test, &root->data.enumeration.enum_value->display, "Info", "id2 root");
    Zinc_expect_size_t_equal(test, root->data.enumeration.number, 2, "enumeration number root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_assign(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "const a = 1;\n"
        "a\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    Zinc_expect_ptr(test, root, "ptr");

    Cent_environment* top = Cent_get_environment(ct->primary->pr.root);
    if (!Zinc_expect_ptr(test, top, "ptr top")) {
		return Zinc_assert();
	}

    Cent_symbol* sym = Cent_environment_get_str(top, "a");
    if (!Zinc_expect_ptr(test, sym, "ptr sym")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, sym->type, Cent_symbol_type_variable, "type sym");

    if (!Zinc_expect_ptr(test, root, "ptr value")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_natural, "type number");
    Zinc_expect_uint64_t_equal(test, root->data.natural, 1, "integer value");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_object(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "Test {}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_dag, "type root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_object_prop_set(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "Test {\n"
        "    .a = 1\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr root")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_dag, "type root");
    Cent_value* a = Cent_value_get_str(root, "a");
    if (!Zinc_expect_ptr(test, a, "ptr a")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a->type, Cent_value_type_natural, "type number");
    Zinc_expect_long_long_equal(test, a->data.natural, 1, "integer value");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_property_set_variable(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Source {\n"
        "    children {\n"
        "        String\n"
        "    }\n"
        "}\n"
        "element Test {\n"
        "    properties {\n"
        "        source: Source\n"
        "    }\n"
        "}\n"
        "const a = Source {\"x + 5\"};\n"
        "Test {\n"
        "    .source = a\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr value")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_dag, "type root");
    Zinc_expect_string(test, &root->name, "Test", "name root");

    Cent_value* source = Cent_value_get_str(root, "source");
    if (!Zinc_expect_ptr(test, source, "ptr source")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, source->type, Cent_value_type_dag, "type source");
    Zinc_expect_string(test, &source->name, "Source", "name source");

    Cent_value* line0 = source->data.dag.head;
    if (!Zinc_expect_ptr(test, line0, "ptr line0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, line0->type, Cent_value_type_string, "type line0");
    Zinc_expect_string(test, &line0->name, "String", "name line0");
    Zinc_expect_string(test, &line0->data.string, "x + 5", "string line0");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_namespace_enum(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "use types;\n"
        "Groceries {\n"
        "    types::Grocery_item::Milk\n"
        "    types::Grocery_item::Carrots\n"
        "}\n"
    );

    Cent_comp_table_add_module_str(ct, "types.cent",
        "enum Grocery_item {\n"
        "    Milk\n"
        "    Cereal\n"
        "    Steak\n"
        "    Potatoes\n"
        "    Carrots\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr value")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_dag, "type root");
    Zinc_expect_string(test, &root->name, "Groceries", "name root");

    Cent_value* item0 = root->data.dag.head;
    if (!Zinc_expect_ptr(test, item0, "ptr item0")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, item0->type, Cent_value_type_enum, "type item0");
    Zinc_expect_size_t_equal(test, item0->data.enumeration.number, 0, "number item0");

    Cent_value* item1 = item0->next;
    if (!Zinc_expect_ptr(test, item0, "ptr item1")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, item1->type, Cent_value_type_enum, "type item1");
    Zinc_expect_size_t_equal(test, item1->data.enumeration.number, 4, "number item1");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_namespace_variable(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "use variables\n"
        "const a = 1\n"
        "const b = 2\n"
        "const c = 3\n"
        "Foo {\n"
        "    a\n"
        "    b\n"
        "    c\n"
        "    variables::a\n"
        "}\n"
    );

    Cent_comp_table_add_module_str(ct, "variables.cent",
        "use original\n"
        "const a = original::c\n"
        "const b = 5\n"
        "const c = 6\n"
    );

    Cent_comp_table_add_module_str(ct, "original.cent",
    "const a = 7\n"
    "const b = 8\n"
    "const c = 9\n"
);

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr value")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_dag, "type root");

    Cent_value* a = root->data.dag.head;
    if (!Zinc_expect_ptr(test, a, "ptr a2")) {
		return Zinc_assert();
	}

    Cent_value* b = a->next;
    if (!Zinc_expect_ptr(test, b, "ptr a2")) {
		return Zinc_assert();
	}

    Cent_value* c = b->next;
    if (!Zinc_expect_ptr(test, c, "ptr a2")) {
		return Zinc_assert();
	}

    Cent_value* a2 = c->next;
    if (!Zinc_expect_ptr(test, a2, "ptr a2")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, a2->type, Cent_value_type_natural, "type a2");
    Zinc_expect_uint64_t_equal(test, a2->data.natural, 9, "integer a2");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_namespace_submodules(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "use foo::bar;\n"
        "foo::bar::a\n"
    );

    Cent_comp_table_add_module_str(ct, "foo/bar.cent",
        "const a = 190;\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr value")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_natural, "type root");
    Zinc_expect_uint64_t_equal(test, root->data.natural, 190, "integer root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_namespace_glob_value(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "use foo::bar::*;\n"
        "a\n"
    );

    Cent_comp_table_add_module_str(ct, "foo/bar.cent",
        "const a = 190;\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr value")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_natural, "type root");
    Zinc_expect_uint64_t_equal(test, root->data.integer, 190, "integer root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_const(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "const a = 245;\n"
        "a\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr value")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_natural, "type root");
    Zinc_expect_uint64_t_equal(test, root->data.natural, 245, "integer root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_cast_natural_to_integer(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Integer\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = 100\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;
    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr value")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_dag, "type root");
    Zinc_expect_string(test, &root->name, "Foo", "name root");

    Cent_value* x = Cent_value_get_str(root, "x");
    if (!Zinc_expect_ptr(test, x, "ptr x")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, x->type, Cent_value_type_integer, "type x");
    Zinc_expect_string(test, &x->name, "Integer", "name x");
    Zinc_expect_int64_t_equal(test, x->data.integer, 100, "integer x");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_cast_natural_to_integer_error_too_large(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Integer\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = 9223372036854775808\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, 
        errors,
        "Natural (9223372036854775808) too large to cast to Integer");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_cast_integer_to_natural_error(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Natural\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = -100\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "Integer (-100) is negative; cannot cast to Natural");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_cast_natural_to_real(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Real\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = 100\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr value")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_dag, "type root");
    Zinc_expect_string(test, &root->name, "Foo", "name root");

    Cent_value* x = Cent_value_get_str(root, "x");
    if (!Zinc_expect_ptr(test, x, "ptr x")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, x->type, Cent_value_type_real, "type x");
    Zinc_expect_string(test, &x->name, "Real", "name x");
    Zinc_expect_double_equal(test, x->data.real, 100, "integer x");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_cast_real_to_natural(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Natural\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = 100.0\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr value")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_dag, "type root");
    Zinc_expect_string(test, &root->name, "Foo", "name root");

    Cent_value* x = Cent_value_get_str(root, "x");
    if (!Zinc_expect_ptr(test, x, "ptr x")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, x->type, Cent_value_type_natural, "type x");
    Zinc_expect_string(test, &x->name, "Natural", "name x");
    Zinc_expect_uint64_t_equal(test, x->data.natural, 100, "integer x");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_cast_real_to_natural_error_negative(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Natural\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = -100.0\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "Real (-100.000000) is negative; cannot cast to Natural");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_cast_real_to_natural_error_too_large(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Natural\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = 1.0e20\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, 
        errors,
        "Real (100000000000000000000.000000) is too large; cannot cast to Natural");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_cast_integer_to_real(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Real\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = -100\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr value")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_dag, "type root");
    Zinc_expect_string(test, &root->name, "Foo", "name root");

    Cent_value* x = Cent_value_get_str(root, "x");
    if (!Zinc_expect_ptr(test, x, "ptr x")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, x->type, Cent_value_type_real, "type x");
    Zinc_expect_string(test, &x->name, "Real", "name x");
    Zinc_expect_double_equal(test, x->data.real, -100.000000, "integer x");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_cast_real_to_integer(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Integer\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = 100.0\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr value")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_dag, "type root");
    Zinc_expect_string(test, &root->name, "Foo", "name root");

    Cent_value* x = Cent_value_get_str(root, "x");
    if (!Zinc_expect_ptr(test, x, "ptr x")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, x->type, Cent_value_type_integer, "type x");
    Zinc_expect_string(test, &x->name, "Integer", "name x");
    Zinc_expect_int64_t_equal(test, x->data.integer, 100, "integer x");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_cast_real_to_integer_too_large_of_a_negative(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Integer\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = -1.0e20\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, 
        errors,
        "Real (-100000000000000000000.000000) is too large of a negative; cannot cast to Integer");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_cast_real_to_integer_too_large(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "    properties {\n"
        "        x: Integer\n"
        "    }\n"
        "}\n"
        "Foo {\n"
        "    .x = 1.0e20\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, 
        errors,
        "Real (100000000000000000000.000000) is too large; cannot cast to Integer");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_variant(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
    "element Monster(MonsterKind) {\n"
    "    properties {\n"
    "        hp: Integer\n"
    "        attack: Integer\n"
    "        defense: Integer\n"
    "    }\n"
    "}\n"
    "enum MonsterKind {\n"
    "    Bat\n"
    "    Rat\n"
    "    Orc\n"
    "    Goblin\n"
    "    Skeleton\n"
    "    Human\n"
    "    Dog\n"
    "}\n"
    "variant Monster::Bat {\n"
    "    properties {\n"
    "        siphon: Integer\n"
    "    }\n"
    "}\n"
    "variant Monster::Human {\n"
    "    properties {\n"
    "        name: String\n"
    "    }\n"
    "}\n"
    "Monsters {\n"
    "    Monster::Bat {"
    "       .hp = 10\n"
    "       .attack = 2\n"
    "       .defense = 1\n"
    "       .siphon = 1\n"
    "    }\n"
    "    Monster {\n"
    "        .@tag = MonsterKind::Human\n"
    "        .hp = 20\n"
    "        .attack = 1\n"
    "        .defense = 1\n"
    "        .name = \"Tom\"\n"
    "    }\n"
    "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;
    Zinc_expect_no_errors(test, errors);

    if (!Zinc_expect_ptr(test, root, "ptr value")) {
		return Zinc_assert();
	}
    Zinc_expect_int_equal(test, root->type, Cent_value_type_dag, "type root");
    Zinc_expect_string(test, &root->name, "Monsters", "name root");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_cycle(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
    "const foo = Foo {\n"
    "    bar\n"
    "}\n"
    "const bar = Bar {\n"
    "    foo\n"
    "}\n"
    "foo\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;
    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "cycle detected");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build_cycle2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
    "const foo = Foo {\n"
    "    foo\n"
    "}\n"
    "foo\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Cent_value* root = ct->primary->value;
    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "cycle detected");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_build(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, CentUnit_build_number_integer);
        Zinc_test_register(test, CentUnit_build_number_fp);
        Zinc_test_register(test, CentUnit_build_string);
        Zinc_test_register(test, CentUnit_build_boolean_true);
        Zinc_test_register(test, CentUnit_build_boolean_false);
        Zinc_test_register(test, CentUnit_build_enum0);
        Zinc_test_register(test, CentUnit_build_enum1);
        Zinc_test_register(test, CentUnit_build_enum2);
        Zinc_test_register(test, CentUnit_build_assign);
        Zinc_test_register(test, CentUnit_build_object);
        Zinc_test_register(test, CentUnit_build_object_prop_set);
        Zinc_test_register(test, CentUnit_build_property_set_variable);
        Zinc_test_register(test, CentUnit_build_namespace_enum);
        Zinc_test_register(test, CentUnit_build_namespace_variable);
        Zinc_test_register(test, CentUnit_build_namespace_submodules);
        Zinc_test_register(test, CentUnit_build_namespace_glob_value);
        Zinc_test_register(test, CentUnit_build_const);

        Zinc_test_register(test, CentUnit_build_cast_natural_to_integer);
        Zinc_test_register(test, CentUnit_build_cast_natural_to_integer_error_too_large);
        Zinc_test_register(test, CentUnit_build_cast_integer_to_natural_error);

        Zinc_test_register(test, CentUnit_build_cast_natural_to_real);
        Zinc_test_register(test, CentUnit_build_cast_real_to_natural);
        Zinc_test_register(test, CentUnit_build_cast_real_to_natural_error_negative);
        Zinc_test_register(test, CentUnit_build_cast_real_to_natural_error_too_large);

        Zinc_test_register(test, CentUnit_build_cast_integer_to_real);
        Zinc_test_register(test, CentUnit_build_cast_real_to_integer);
        Zinc_test_register(test, CentUnit_build_cast_real_to_integer_too_large_of_a_negative);
        Zinc_test_register(test, CentUnit_build_cast_real_to_integer_too_large);

        Zinc_test_register(test, CentUnit_build_variant);

        Zinc_test_register(test, CentUnit_build_cycle);
        Zinc_test_register(test, CentUnit_build_cycle2);

        return;
    }

    Zinc_test_perform(test);
}
