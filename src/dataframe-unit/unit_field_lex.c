#include <string.h>
#include "dataframe/field.h"
#include "dataframe/field_lex.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void DfUnit_FieldLexRun(char* text, enum FieldType* type)
{
    Zinc_vector* text2 = NULL;
    Zinc_vector_create(&text2, sizeof(char));
    size_t len = strlen(text);
    Zinc_vector_add(text2, text, len);

    Zinc_input_char_string* input_string = NULL;
    Zinc_input_char_string_create(&input_string, text2);

    struct FieldLexData* lex_data = NULL;
    FieldLexDataCreate(&lex_data);
    lex_data->input_data = input_string;
    lex_data->input_vtable = input_string->input_vtable;

    FieldLex(lex_data, type);

    Zinc_vector_destroy(text2);
    free(text2);
    free(input_string);
    free(lex_data);
}

void DfUnit_FieldLexEmpty(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;
    DfUnit_FieldLexRun("", &type);
    Zinc_expect_int_equal(test, type, FieldTypeEmpty, "empty");
}

void DfUnit_FieldLexIntegerUnsigned(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;
    DfUnit_FieldLexRun("100", &type);
    Zinc_expect_int_equal(test, type, FieldTypeIntU, "int");
}

void DfUnit_FieldLexStringIntegerUnsigned(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;

    DfUnit_FieldLexRun("10a0", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "string");

    DfUnit_FieldLexRun("a100", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "string");

    DfUnit_FieldLexRun("100a", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "string");
}

void DfUnit_FieldLexString(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;

    DfUnit_FieldLexRun("hello", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "string");
}

void DfUnit_FieldLexIntegerSign(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;
    DfUnit_FieldLexRun("-100", &type);
    Zinc_expect_int_equal(test, type, FieldTypeInt, "int");

    DfUnit_FieldLexRun("+100", &type);
    Zinc_expect_int_equal(test, type, FieldTypeInt, "int");
}

void DfUnit_FieldLexStringIntegerSign(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;

    DfUnit_FieldLexRun("a+100", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "int");

    DfUnit_FieldLexRun("+a100", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "int");

    DfUnit_FieldLexRun("+100a", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "int");

    DfUnit_FieldLexRun("a-100", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "int");

    DfUnit_FieldLexRun("-a100", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "int");

    DfUnit_FieldLexRun("-100a", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "int");
}

void DfUnit_FieldLexFloatIntegerFraction(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;
    DfUnit_FieldLexRun("1.5", &type);
    Zinc_expect_int_equal(test, type, FieldTypeFloat, "int");
}

void DfUnit_FieldLexStringFloatIntegerFraction(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;

    DfUnit_FieldLexRun("a1.5", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "int");

    DfUnit_FieldLexRun("1a.5", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "int");

    DfUnit_FieldLexRun("1.a5", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "int");

    DfUnit_FieldLexRun("1.5a", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "int");
}

void DfUnit_FieldLexFloatInteger(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;
    DfUnit_FieldLexRun("31.", &type);
    Zinc_expect_int_equal(test, type, FieldTypeFloat, "int");
}

void DfUnit_FieldLexStringFloatInteger(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;
    DfUnit_FieldLexRun("a31.", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "int");

    DfUnit_FieldLexRun("31a.", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "int");

    DfUnit_FieldLexRun("31.a", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "int");
}

void DfUnit_FieldLexFloatFraction(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;
    DfUnit_FieldLexRun(".5", &type);
    Zinc_expect_int_equal(test, type, FieldTypeFloat, "int");
}

void DfUnit_FieldLexStringFloatFraction(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;
    DfUnit_FieldLexRun("a.5", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "string 0");

    DfUnit_FieldLexRun(".a5", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "string 1");

    DfUnit_FieldLexRun(".5a", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "string 2");
}

void DfUnit_FieldLexFloatE(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;
    DfUnit_FieldLexRun("1e", &type);
    Zinc_expect_int_equal(test, type, FieldTypeFloat, "int");
}

void DfUnit_FieldLexStringFloatE(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;

    DfUnit_FieldLexRun("1ae", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "string 0");

    DfUnit_FieldLexRun("1ea", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "string 1");
}

void DfUnit_FieldLexFloatEUpper(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;
    DfUnit_FieldLexRun("1E", &type);
    Zinc_expect_int_equal(test, type, FieldTypeFloat, "int");
}

void DfUnit_FieldLexStringFloatEUpper(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;

    DfUnit_FieldLexRun("1aE", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "string 0");

    DfUnit_FieldLexRun("1Ea", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "string 0");
}

void DfUnit_TestFieldLexFloatExponent(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;
    DfUnit_FieldLexRun("1e3", &type);
    Zinc_expect_int_equal(test, type, FieldTypeFloat, "int");
}

void DfUnit_FieldLexStringFloatExponent(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;

    DfUnit_FieldLexRun("1ea3", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "string 0");

    DfUnit_FieldLexRun("1e3a", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "string 1");
}

void DfUnit_FieldLexFloatExponentNegative(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;
    DfUnit_FieldLexRun("1235e-30", &type);
    Zinc_expect_int_equal(test, type, FieldTypeFloat, "int");
}

void DfUnit_FieldLexStringFloatExponentNegative(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;

    DfUnit_FieldLexRun("1235ea-30", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "string 0");

    DfUnit_FieldLexRun("1235e-a30", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "string 1");
}

void DfUnit_FieldLexFloatExponentPositive(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;
    DfUnit_FieldLexRun("1e+43", &type);
    Zinc_expect_int_equal(test, type, FieldTypeFloat, "int");
}

void DfUnit_FieldLexStringFloatExponentPositive(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;

    DfUnit_FieldLexRun("1ea+43", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "int");

    DfUnit_FieldLexRun("1e+a43", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "int");
}

void DfUnit_FieldLexStringBooleanTrue(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;

    DfUnit_FieldLexRun("True", &type);
    Zinc_expect_int_equal(test, type, FieldTypeBool, "0");

    DfUnit_FieldLexRun("aTrue", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "1");

    DfUnit_FieldLexRun("Traue", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "2");

    DfUnit_FieldLexRun("Truea", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "3");
}

void DfUnit_TestFieldLexStringBooleanFalse(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    enum FieldType type;

    DfUnit_FieldLexRun("False", &type);
    Zinc_expect_int_equal(test, type, FieldTypeBool, "0");

    DfUnit_FieldLexRun("aFalse", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "1");

    DfUnit_FieldLexRun("Falase", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "2");

    DfUnit_FieldLexRun("Falsea", &type);
    Zinc_expect_int_equal(test, type, FieldTypeString, "3");
}

void DfUnit_FieldLex(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, DfUnit_FieldLexEmpty);

        Zinc_test_register(test, DfUnit_FieldLexString);

        Zinc_test_register(test, DfUnit_FieldLexIntegerUnsigned);
        Zinc_test_register(test, DfUnit_FieldLexStringIntegerUnsigned);

        Zinc_test_register(test, DfUnit_FieldLexIntegerSign);
        Zinc_test_register(test, DfUnit_FieldLexStringIntegerSign);

        Zinc_test_register(test, DfUnit_FieldLexFloatIntegerFraction);
        Zinc_test_register(test, DfUnit_FieldLexStringFloatIntegerFraction);

        Zinc_test_register(test, DfUnit_FieldLexFloatInteger);
        Zinc_test_register(test, DfUnit_FieldLexStringFloatInteger);

        Zinc_test_register(test, DfUnit_FieldLexFloatFraction);
        Zinc_test_register(test, DfUnit_FieldLexStringFloatFraction);

        Zinc_test_register(test, DfUnit_FieldLexFloatE);
        Zinc_test_register(test, DfUnit_FieldLexStringFloatE);

        Zinc_test_register(test, DfUnit_FieldLexFloatEUpper);
        Zinc_test_register(test, DfUnit_FieldLexStringFloatEUpper);

        Zinc_test_register(test, DfUnit_TestFieldLexFloatExponent);
        Zinc_test_register(test, DfUnit_FieldLexStringFloatExponent);

        Zinc_test_register(test, DfUnit_FieldLexFloatExponentNegative);
        Zinc_test_register(test, DfUnit_FieldLexStringFloatExponentNegative);

        Zinc_test_register(test, DfUnit_FieldLexFloatExponentPositive);
        Zinc_test_register(test, DfUnit_FieldLexStringFloatExponentPositive);

        Zinc_test_register(test, DfUnit_FieldLexStringBooleanTrue);

        Zinc_test_register(test, DfUnit_TestFieldLexStringBooleanFalse);

        return;
    }

    Zinc_test_perform(test);
}