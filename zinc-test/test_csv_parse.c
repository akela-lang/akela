#include "zinc/unit_test.h"
#include "zinc/data_frame.h"
#include "zinc/csv_lex.h"
#include "zinc/csv_parse.h"
#include <string.h>
#include "zinc/error_unit_test.h"
#include "zinc/memory.h"
#include "test_csv_parse_tools.h"

void TestCSVParseHeaderName()
{
    test_name(__func__);

    struct CSVParseOutput* parse_output = NULL;
    CSVParseSetup(&parse_output, "First Name\n");

    expect_no_errors(parse_output->el);
    expect_size_t_equal(DataFrameColumnCount(parse_output->df), 1, "column count");
    expect_size_t_equal(DataFrameRowCount(parse_output->df), 0, "row count");
    Series* s = DataFrameSeriesByIndex(parse_output->df, 0);
    assert_ptr(s, "ptr s");
    expect_true(VectorMatchStr(&s->name, "First Name"), "First Name");

    CSVParseTeardown(parse_output);
}

void TestCSVParseRowField()
{
    test_name(__func__);

    struct CSVParseOutput* parse_output = NULL;
    CSVParseSetup(&parse_output,
                  "Count\n"
                  "11\n");

    expect_no_errors(parse_output->el);

    expect_size_t_equal(DataFrameColumnCount(parse_output->df), 1, "column count");
    expect_size_t_equal(DataFrameRowCount(parse_output->df), 1, "row count");

    Series* s = DataFrameSeriesByIndex(parse_output->df, 0);
    assert_ptr(s, "ptr s");
    expect_true(VectorMatchStr(&s->name, "Count"), "Count");

    enum FieldType field_type = *(enum FieldType*)VECTOR_PTR(&s->types, 0);
    expect_int_equal(field_type, FieldTypeIntU, "types");

    expect_int_equal(s->type, FieldTypeIntU, "type");

    Vector* raw0 = *(Vector**)VECTOR_PTR(&s->raw, 0);
    expect_true(VectorMatchStr(raw0, "11"), "11 raw");

    expect_u_long_equal(VECTOR_U_LONG(&s->value, 0), 11, "11 value");

    CSVParseTeardown(parse_output);
}

void TestCSVParseTypes()
{
    test_name(__func__);

    struct CSVParseOutput* parse_output = NULL;
    CSVParseSetup(&parse_output,
                  "Bool,Float,Int,IntU,String,Empty\n"
                  "True,1.2,-3,50,hello,\n"
                  "False,5.1,80,1,world,\n");

    expect_no_errors(parse_output->el);
    expect_size_t_equal(DataFrameColumnCount(parse_output->df), 6, "column count");
    expect_size_t_equal(DataFrameRowCount(parse_output->df), 2, "row count");

    Series* s0 = DataFrameSeriesByIndex(parse_output->df, 0);
    assert_ptr(s0, "ptr s0");
    expect_true(VectorMatchStr(&s0->name, "Bool"), "s0 name");
    expect_int_equal(s0->type, FieldTypeBool, "s0 type");

    expect_int_equal(VECTOR_INT(&s0->types, 0), FieldTypeBool, "s0 types 0");
    expect_true(VectorMatchStr(VECTOR_VECTOR(&s0->raw, 0), "True"), "s0 raw 0");
    expect_int_equal(VECTOR_BOOL(&s0->value, 0), 1, "0 value 0");

    expect_int_equal(VECTOR_INT(&s0->types, 1), FieldTypeBool, "s0 types 1");
    expect_true(VectorMatchStr(VECTOR_VECTOR(&s0->raw, 1), "False"), "s0 raw 1");
    expect_int_equal(VECTOR_BOOL(&s0->value, 1), 0, "s0 value 1");

    Series* s1 = DataFrameSeriesByIndex(parse_output->df, 1);
    assert_ptr(s1, "ptr s1");
    expect_true(VectorMatchStr(&s1->name, "Float"), "s1 name");
    expect_int_equal(s1->type, FieldTypeFloat, "s1 type");

    expect_int_equal(VECTOR_INT(&s1->types, 0), FieldTypeFloat, "s1 types 0");
    expect_true(VectorMatchStr(VECTOR_VECTOR(&s1->raw, 0), "1.2"), "s1 raw 0");
    expect_double_equal(VECTOR_DOUBLE(&s1->value, 0), 1.2, "s1 value 0");

    expect_int_equal(VECTOR_INT(&s1->types, 1), FieldTypeFloat, "s1 types 1");
    expect_true(VectorMatchStr(VECTOR_VECTOR(&s1->raw, 1), "5.1"), "s1 raw 1");
    expect_double_equal(VECTOR_DOUBLE(&s1->value, 1), 5.1, "s1 value 1");

    Series* s2 = DataFrameSeriesByIndex(parse_output->df, 2);
    assert_ptr(s2, "ptr s2");
    expect_true(VectorMatchStr(&s2->name, "Int"), "s2 name");
    expect_int_equal(s2->type, FieldTypeInt, "s2 type");

    expect_int_equal(VECTOR_INT(&s2->types, 0), FieldTypeInt, "s2 types 0");
    expect_true(VectorMatchStr(VECTOR_VECTOR(&s2->raw, 0), "-3"), "s2 raw 0");
    expect_int_equal(VECTOR_INT(&s2->value, 0), -3, "s2 value 0");

    expect_int_equal(VECTOR_INT(&s2->types, 1), FieldTypeIntU, "s2 types 1");
    expect_true(VectorMatchStr(VECTOR_VECTOR(&s2->raw, 1), "80"), "s2 raw 1");
    expect_int_equal(VECTOR_INT(&s2->value, 1), 80, "s2 value 1");

    Series* s3 = DataFrameSeriesByIndex(parse_output->df, 3);
    assert_ptr(s3, "ptr s3");
    expect_true(VectorMatchStr(&s3->name, "IntU"), "s3 name");
    expect_int_equal(s3->type, FieldTypeIntU, "s3 type");

    expect_int_equal(VECTOR_INT(&s3->types, 0), FieldTypeIntU, "s3 types 0");
    expect_true(VectorMatchStr(VECTOR_VECTOR(&s3->raw, 0), "50"), "s3 raw 0");
    expect_int_equal(VECTOR_INT(&s3->value, 0), 50, "s3 value 0");

    expect_int_equal(VECTOR_INT(&s3->types, 1), FieldTypeIntU, "s3 types 1");
    expect_true(VectorMatchStr(VECTOR_VECTOR(&s3->raw, 1), "1"), "s3 raw 1");
    expect_int_equal(VECTOR_INT(&s3->value, 1), 1, "s3 value 1");

    Series* s4 = DataFrameSeriesByIndex(parse_output->df, 4);
    assert_ptr(s4, "ptr s4");
    expect_true(VectorMatchStr(&s4->name, "String"), "s4 name");
    expect_int_equal(s4->type, FieldTypeString, "s4 type");

    expect_int_equal(VECTOR_INT(&s4->types, 0), FieldTypeString, "s4 types 0");
    expect_true(VectorMatchStr(VECTOR_VECTOR(&s4->raw, 0), "hello"), "s4 raw 0");
    expect_vector_str(VECTOR_VECTOR(&s4->value, 0), "hello", "s4 value 0");

    expect_int_equal(VECTOR_INT(&s4->types, 1), FieldTypeString, "s4 types 1");
    expect_true(VectorMatchStr(VECTOR_VECTOR(&s4->raw, 1), "world"), "s4 raw 1");
    expect_vector_str(VECTOR_VECTOR(&s4->value, 1), "world", "s4 value 1");

    Series* s5 = DataFrameSeriesByIndex(parse_output->df, 5);
    assert_ptr(s5, "ptr s5");
    expect_true(VectorMatchStr(&s5->name, "Empty"), "s5 name");
    expect_int_equal(s5->type, FieldTypeEmpty, "s5 type");

    expect_int_equal(VECTOR_INT(&s5->types, 0), FieldTypeEmpty, "s5 types 0");
    expect_true(VectorMatchStr(VECTOR_VECTOR(&s5->raw, 0), ""), "s5 raw 0");

    expect_int_equal(VECTOR_INT(&s5->types, 0), FieldTypeEmpty, "s5 types 0");
    expect_true(VectorMatchStr(VECTOR_VECTOR(&s5->raw, 0), ""), "s5 raw 0");

    expect_size_t_equal(s5->value.count, 0, "s5 value count");

    CSVParseTeardown(parse_output);
}

void TestCSVParse()
{
    TestCSVParseHeaderName();
    TestCSVParseRowField();
    TestCSVParseTypes();
}