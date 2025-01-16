#include <string.h>
#include "zinc/unit_test.h"
#include "dataframe/field.h"
#include "dataframe/field_lex.h"

void TestFieldLexRun(char* text, enum FieldType* type)
{
    Vector* text2 = NULL;
    VectorCreate(&text2, sizeof(char));
    size_t len = strlen(text);
    VectorAdd(text2, text, len);

    Zinc_input_char_string* input_string = NULL;
    Zinc_input_char_string_create(&input_string, text2);

    struct FieldLexData* lex_data = NULL;
    FieldLexDataCreate(&lex_data);
    lex_data->input_data = input_string;
    lex_data->input_vtable = input_string->input_vtable;

    FieldLex(lex_data, type);

    VectorDestroy(text2);
    free(text2);
    free(input_string);
    free(lex_data);
}

void TestFieldLexEmpty()
{
    test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("", &type);
    expect_int_equal(type, FieldTypeEmpty, "empty");
}

void TestFieldLexIntegerUnsigned()
{
    test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("100", &type);
    expect_int_equal(type, FieldTypeIntU, "int");
}

void TestFieldLexStringIntegerUnsigned() {
    test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("10a0", &type);
    expect_int_equal(type, FieldTypeString, "string");

    TestFieldLexRun("a100", &type);
    expect_int_equal(type, FieldTypeString, "string");

    TestFieldLexRun("100a", &type);
    expect_int_equal(type, FieldTypeString, "string");
}

void TestFieldLexString() {
    test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("hello", &type);
    expect_int_equal(type, FieldTypeString, "string");
}

void TestFieldLexIntegerSign()
{
    test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("-100", &type);
    expect_int_equal(type, FieldTypeInt, "int");

    TestFieldLexRun("+100", &type);
    expect_int_equal(type, FieldTypeInt, "int");
}

void TestFieldLexStringIntegerSign()
{
    test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("a+100", &type);
    expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("+a100", &type);
    expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("+100a", &type);
    expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("a-100", &type);
    expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("-a100", &type);
    expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("-100a", &type);
    expect_int_equal(type, FieldTypeString, "int");
}

void TestFieldLexFloatIntegerFraction()
{
    test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("1.5", &type);
    expect_int_equal(type, FieldTypeFloat, "int");
}

void TestFieldLexStringFloatIntegerFraction()
{
    test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("a1.5", &type);
    expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("1a.5", &type);
    expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("1.a5", &type);
    expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("1.5a", &type);
    expect_int_equal(type, FieldTypeString, "int");
}

void TestFieldLexFloatInteger()
{
    test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("31.", &type);
    expect_int_equal(type, FieldTypeFloat, "int");
}

void TestFieldLexStringFloatInteger()
{
    test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("a31.", &type);
    expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("31a.", &type);
    expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("31.a", &type);
    expect_int_equal(type, FieldTypeString, "int");
}

void TestFieldLexFloatFraction()
{
    test_name(__func__);

    enum FieldType type;
    TestFieldLexRun(".5", &type);
    expect_int_equal(type, FieldTypeFloat, "int");
}

void TestFieldLexStringFloatFraction()
{
    test_name(__func__);

    enum FieldType type
            ;
    TestFieldLexRun("a.5", &type);
    expect_int_equal(type, FieldTypeString, "string 0");

    TestFieldLexRun(".a5", &type);
    expect_int_equal(type, FieldTypeString, "string 1");

    TestFieldLexRun(".5a", &type);
    expect_int_equal(type, FieldTypeString, "string 2");
}

void TestFieldLexFloatE()
{
    test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("1e", &type);
    expect_int_equal(type, FieldTypeFloat, "int");
}

void TestFieldLexStringFloatE()
{
    test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("1ae", &type);
    expect_int_equal(type, FieldTypeString, "string 0");

    TestFieldLexRun("1ea", &type);
    expect_int_equal(type, FieldTypeString, "string 1");
}

void TestFieldLexFloatEUpper()
{
    test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("1E", &type);
    expect_int_equal(type, FieldTypeFloat, "int");
}

void TestFieldLexStringFloatEUpper()
{
    test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("1aE", &type);
    expect_int_equal(type, FieldTypeString, "string 0");

    TestFieldLexRun("1Ea", &type);
    expect_int_equal(type, FieldTypeString, "string 0");
}

void TestFieldLexFloatExponent()
{
    test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("1e3", &type);
    expect_int_equal(type, FieldTypeFloat, "int");
}

void TestFieldLexStringFloatExponent()
{
    test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("1ea3", &type);
    expect_int_equal(type, FieldTypeString, "string 0");

    TestFieldLexRun("1e3a", &type);
    expect_int_equal(type, FieldTypeString, "string 1");
}

void TestFieldLexFloatExponentNegative()
{
    test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("1235e-30", &type);
    expect_int_equal(type, FieldTypeFloat, "int");
}

void TestFieldLexStringFloatExponentNegative()
{
    test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("1235ea-30", &type);
    expect_int_equal(type, FieldTypeString, "string 0");

    TestFieldLexRun("1235e-a30", &type);
    expect_int_equal(type, FieldTypeString, "string 1");
}

void TestFieldLexFloatExponentPositive()
{
    test_name(__func__);

    enum FieldType type;
    TestFieldLexRun("1e+43", &type);
    expect_int_equal(type, FieldTypeFloat, "int");
}

void TestFieldLexStringFloatExponentPositive()
{
    test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("1ea+43", &type);
    expect_int_equal(type, FieldTypeString, "int");

    TestFieldLexRun("1e+a43", &type);
    expect_int_equal(type, FieldTypeString, "int");
}

void TestFieldLexStringBooleanTrue()
{
    test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("True", &type);
    expect_int_equal(type, FieldTypeBool, "0");

    TestFieldLexRun("aTrue", &type);
    expect_int_equal(type, FieldTypeString, "1");

    TestFieldLexRun("Traue", &type);
    expect_int_equal(type, FieldTypeString, "2");

    TestFieldLexRun("Truea", &type);
    expect_int_equal(type, FieldTypeString, "3");
}

void TestFieldLexStringBooleanFalse()
{
    test_name(__func__);

    enum FieldType type;

    TestFieldLexRun("False", &type);
    expect_int_equal(type, FieldTypeBool, "0");

    TestFieldLexRun("aFalse", &type);
    expect_int_equal(type, FieldTypeString, "1");

    TestFieldLexRun("Falase", &type);
    expect_int_equal(type, FieldTypeString, "2");

    TestFieldLexRun("Falsea", &type);
    expect_int_equal(type, FieldTypeString, "3");
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