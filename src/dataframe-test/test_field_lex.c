#include <string.h>
#include "zinc/unit_test.h"
#include "dataframe/field.h"
#include "dataframe/field_lex.h"

void TestFieldLexRun(char* text, enum FieldType* type)
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

void TestFieldLexEmpty()
{
    Zinc_test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("", &type);
    Zinc_expect_int_equal(type, FieldTypeEmpty, "empty");
}

void TestFieldLexIntegerUnsigned()
{
    Zinc_test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("100", &type);
    Zinc_expect_int_equal(type, FieldTypeIntU, "int");
}

void TestFieldLexStringIntegerUnsigned() {
    Zinc_test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("10a0", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "string");

    TestFieldLexRun("a100", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "string");

    TestFieldLexRun("100a", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "string");
}

void TestFieldLexString() {
    Zinc_test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("hello", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "string");
}

void TestFieldLexIntegerSign()
{
    Zinc_test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("-100", &type);
    Zinc_expect_int_equal(type, FieldTypeInt, "int");

    TestFieldLexRun("+100", &type);
    Zinc_expect_int_equal(type, FieldTypeInt, "int");
}

void TestFieldLexStringIntegerSign()
{
    Zinc_test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("a+100", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("+a100", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("+100a", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("a-100", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("-a100", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("-100a", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "int");
}

void TestFieldLexFloatIntegerFraction()
{
    Zinc_test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("1.5", &type);
    Zinc_expect_int_equal(type, FieldTypeFloat, "int");
}

void TestFieldLexStringFloatIntegerFraction()
{
    Zinc_test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("a1.5", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("1a.5", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("1.a5", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("1.5a", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "int");
}

void TestFieldLexFloatInteger()
{
    Zinc_test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("31.", &type);
    Zinc_expect_int_equal(type, FieldTypeFloat, "int");
}

void TestFieldLexStringFloatInteger()
{
    Zinc_test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("a31.", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("31a.", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("31.a", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "int");
}

void TestFieldLexFloatFraction()
{
    Zinc_test_name(__func__);

    enum FieldType type;
    TestFieldLexRun(".5", &type);
    Zinc_expect_int_equal(type, FieldTypeFloat, "int");
}

void TestFieldLexStringFloatFraction()
{
    Zinc_test_name(__func__);

    enum FieldType type
            ;
    TestFieldLexRun("a.5", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "string 0");

    TestFieldLexRun(".a5", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "string 1");

    TestFieldLexRun(".5a", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "string 2");
}

void TestFieldLexFloatE()
{
    Zinc_test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("1e", &type);
    Zinc_expect_int_equal(type, FieldTypeFloat, "int");
}

void TestFieldLexStringFloatE()
{
    Zinc_test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("1ae", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "string 0");

    TestFieldLexRun("1ea", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "string 1");
}

void TestFieldLexFloatEUpper()
{
    Zinc_test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("1E", &type);
    Zinc_expect_int_equal(type, FieldTypeFloat, "int");
}

void TestFieldLexStringFloatEUpper()
{
    Zinc_test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("1aE", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "string 0");

    TestFieldLexRun("1Ea", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "string 0");
}

void TestFieldLexFloatExponent()
{
    Zinc_test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("1e3", &type);
    Zinc_expect_int_equal(type, FieldTypeFloat, "int");
}

void TestFieldLexStringFloatExponent()
{
    Zinc_test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("1ea3", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "string 0");

    TestFieldLexRun("1e3a", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "string 1");
}

void TestFieldLexFloatExponentNegative()
{
    Zinc_test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("1235e-30", &type);
    Zinc_expect_int_equal(type, FieldTypeFloat, "int");
}

void TestFieldLexStringFloatExponentNegative()
{
    Zinc_test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("1235ea-30", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "string 0");

    TestFieldLexRun("1235e-a30", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "string 1");
}

void TestFieldLexFloatExponentPositive()
{
    Zinc_test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("1e+43", &type);
    Zinc_expect_int_equal(type, FieldTypeFloat, "int");
}

void TestFieldLexStringFloatExponentPositive()
{
    Zinc_test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("1ea+43", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("1e+a43", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "int");
}

void TestFieldLexStringBooleanTrue()
{
    Zinc_test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("True", &type);
    Zinc_expect_int_equal(type, FieldTypeBool, "0");

    TestFieldLexRun("aTrue", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "1");

    TestFieldLexRun("Traue", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "2");

    TestFieldLexRun("Truea", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "3");
}

void TestFieldLexStringBooleanFalse()
{
    Zinc_test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("False", &type);
    Zinc_expect_int_equal(type, FieldTypeBool, "0");

    TestFieldLexRun("aFalse", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "1");

    TestFieldLexRun("Falase", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "2");

    TestFieldLexRun("Falsea", &type);
    Zinc_expect_int_equal(type, FieldTypeString, "3");
}

void TestFieldLex()
{
    TestFieldLexEmpty();

    TestFieldLexString();

    TestFieldLexIntegerUnsigned();
    TestFieldLexStringIntegerUnsigned();

    TestFieldLexIntegerSign();
    TestFieldLexStringIntegerSign();

    TestFieldLexFloatIntegerFraction();
    TestFieldLexStringFloatIntegerFraction();

    TestFieldLexFloatInteger();
    TestFieldLexStringFloatInteger();

    TestFieldLexFloatFraction();
    TestFieldLexStringFloatFraction();

    TestFieldLexFloatE();
    TestFieldLexStringFloatE();

    TestFieldLexFloatEUpper();
    TestFieldLexStringFloatEUpper();

    TestFieldLexFloatExponent();
    TestFieldLexStringFloatExponent();

    TestFieldLexFloatExponentNegative();
    TestFieldLexStringFloatExponentNegative();

    TestFieldLexFloatExponentPositive();
    TestFieldLexStringFloatExponentPositive();

    TestFieldLexStringBooleanTrue();

    TestFieldLexStringBooleanFalse();
}