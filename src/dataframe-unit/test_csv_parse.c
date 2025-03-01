#include "zinc/unit_test.h"
#include "dataframe/data_frame.h"
#include "dataframe/csv_lex.h"
#include "dataframe/csv_parse.h"
#include <string.h>
#include "zinc/error_unit_test.h"
#include "zinc/memory.h"
#include "test_csv_parse_tools.h"

void TestCSVParseHeaderName()
{
    Zinc_test_name(__func__);

    struct CSVParseOutput* parse_output = NULL;
    CSVParseSetup(&parse_output, "First Name\n");

    Zinc_expect_no_errors(parse_output->el);
    Zinc_expect_size_t_equal(DataFrameColumnCount(parse_output->df), 1, "column count");
    Zinc_expect_size_t_equal(DataFrameRowCount(parse_output->df), 0, "row count");
    Series* s = DataFrameSeriesByIndex(parse_output->df, 0);
    Zinc_assert_ptr(s, "ptr s");
    Zinc_expect_true(Zinc_vector_match_str(&s->name, "First Name"), "First Name");

    CSVParseTeardown(parse_output);
}

void TestCSVParseRowField()
{
    Zinc_test_name(__func__);

    struct CSVParseOutput* parse_output = NULL;
    CSVParseSetup(&parse_output,
                  "Count\n"
                  "11\n");

    Zinc_expect_no_errors(parse_output->el);

    Zinc_expect_size_t_equal(DataFrameColumnCount(parse_output->df), 1, "column count");
    Zinc_expect_size_t_equal(DataFrameRowCount(parse_output->df), 1, "row count");

    Series* s = DataFrameSeriesByIndex(parse_output->df, 0);
    Zinc_assert_ptr(s, "ptr s");
    Zinc_expect_true(Zinc_vector_match_str(&s->name, "Count"), "Count");

    enum FieldType field_type = *(enum FieldType*)ZINC_VECTOR_PTR(&s->types, 0);
    Zinc_expect_int_equal(field_type, FieldTypeIntU, "types");

    Zinc_expect_int_equal(s->type, FieldTypeIntU, "type");

    Zinc_vector* raw0 = *(Zinc_vector**)ZINC_VECTOR_PTR(&s->raw, 0);
    Zinc_expect_true(Zinc_vector_match_str(raw0, "11"), "11 raw");

    Zinc_expect_u_long_equal(ZINC_VECTOR_U_LONG(&s->value, 0), 11, "11 value");

    CSVParseTeardown(parse_output);
}

void TestCSVParseTypes()
{
    Zinc_test_name(__func__);

    struct CSVParseOutput* parse_output = NULL;
    CSVParseSetup(&parse_output,
                  "Bool,Float,Int,IntU,String,Empty\n"
                  "True,1.2,-3,50,hello,\n"
                  "False,5.1,80,1,world,\n");

    Zinc_expect_no_errors(parse_output->el);
    Zinc_expect_size_t_equal(DataFrameColumnCount(parse_output->df), 6, "column count");
    Zinc_expect_size_t_equal(DataFrameRowCount(parse_output->df), 2, "row count");

    Series* s0 = DataFrameSeriesByIndex(parse_output->df, 0);
    Zinc_assert_ptr(s0, "ptr s0");
    Zinc_expect_true(Zinc_vector_match_str(&s0->name, "Bool"), "s0 name");
    Zinc_expect_int_equal(s0->type, FieldTypeBool, "s0 type");

    Zinc_expect_int_equal(ZINC_VECTOR_INT(&s0->types, 0), FieldTypeBool, "s0 types 0");
    Zinc_expect_true(Zinc_vector_match_str(ZINC_VECTOR_VECTOR(&s0->raw, 0), "True"), "s0 raw 0");
    Zinc_expect_int_equal(ZINC_VECTOR_BOOL(&s0->value, 0), 1, "0 value 0");

    Zinc_expect_int_equal(ZINC_VECTOR_INT(&s0->types, 1), FieldTypeBool, "s0 types 1");
    Zinc_expect_true(Zinc_vector_match_str(ZINC_VECTOR_VECTOR(&s0->raw, 1), "False"), "s0 raw 1");
    Zinc_expect_int_equal(ZINC_VECTOR_BOOL(&s0->value, 1), 0, "s0 value 1");

    Series* s1 = DataFrameSeriesByIndex(parse_output->df, 1);
    Zinc_assert_ptr(s1, "ptr s1");
    Zinc_expect_true(Zinc_vector_match_str(&s1->name, "Float"), "s1 name");
    Zinc_expect_int_equal(s1->type, FieldTypeFloat, "s1 type");

    Zinc_expect_int_equal(ZINC_VECTOR_INT(&s1->types, 0), FieldTypeFloat, "s1 types 0");
    Zinc_expect_true(Zinc_vector_match_str(ZINC_VECTOR_VECTOR(&s1->raw, 0), "1.2"), "s1 raw 0");
    Zinc_expect_double_equal(ZINC_VECTOR_DOUBLE(&s1->value, 0), 1.2, "s1 value 0");

    Zinc_expect_int_equal(ZINC_VECTOR_INT(&s1->types, 1), FieldTypeFloat, "s1 types 1");
    Zinc_expect_true(Zinc_vector_match_str(ZINC_VECTOR_VECTOR(&s1->raw, 1), "5.1"), "s1 raw 1");
    Zinc_expect_double_equal(ZINC_VECTOR_DOUBLE(&s1->value, 1), 5.1, "s1 value 1");

    Series* s2 = DataFrameSeriesByIndex(parse_output->df, 2);
    Zinc_assert_ptr(s2, "ptr s2");
    Zinc_expect_true(Zinc_vector_match_str(&s2->name, "Int"), "s2 name");
    Zinc_expect_int_equal(s2->type, FieldTypeInt, "s2 type");

    Zinc_expect_int_equal(ZINC_VECTOR_INT(&s2->types, 0), FieldTypeInt, "s2 types 0");
    Zinc_expect_true(Zinc_vector_match_str(ZINC_VECTOR_VECTOR(&s2->raw, 0), "-3"), "s2 raw 0");
    Zinc_expect_int_equal(ZINC_VECTOR_INT(&s2->value, 0), -3, "s2 value 0");

    Zinc_expect_int_equal(ZINC_VECTOR_INT(&s2->types, 1), FieldTypeIntU, "s2 types 1");
    Zinc_expect_true(Zinc_vector_match_str(ZINC_VECTOR_VECTOR(&s2->raw, 1), "80"), "s2 raw 1");
    Zinc_expect_int_equal(ZINC_VECTOR_INT(&s2->value, 1), 80, "s2 value 1");

    Series* s3 = DataFrameSeriesByIndex(parse_output->df, 3);
    Zinc_assert_ptr(s3, "ptr s3");
    Zinc_expect_true(Zinc_vector_match_str(&s3->name, "IntU"), "s3 name");
    Zinc_expect_int_equal(s3->type, FieldTypeIntU, "s3 type");

    Zinc_expect_int_equal(ZINC_VECTOR_INT(&s3->types, 0), FieldTypeIntU, "s3 types 0");
    Zinc_expect_true(Zinc_vector_match_str(ZINC_VECTOR_VECTOR(&s3->raw, 0), "50"), "s3 raw 0");
    Zinc_expect_int_equal(ZINC_VECTOR_INT(&s3->value, 0), 50, "s3 value 0");

    Zinc_expect_int_equal(ZINC_VECTOR_INT(&s3->types, 1), FieldTypeIntU, "s3 types 1");
    Zinc_expect_true(Zinc_vector_match_str(ZINC_VECTOR_VECTOR(&s3->raw, 1), "1"), "s3 raw 1");
    Zinc_expect_int_equal(ZINC_VECTOR_INT(&s3->value, 1), 1, "s3 value 1");

    Series* s4 = DataFrameSeriesByIndex(parse_output->df, 4);
    Zinc_assert_ptr(s4, "ptr s4");
    Zinc_expect_true(Zinc_vector_match_str(&s4->name, "String"), "s4 name");
    Zinc_expect_int_equal(s4->type, FieldTypeString, "s4 type");

    Zinc_expect_int_equal(ZINC_VECTOR_INT(&s4->types, 0), FieldTypeString, "s4 types 0");
    Zinc_expect_true(Zinc_vector_match_str(ZINC_VECTOR_VECTOR(&s4->raw, 0), "hello"), "s4 raw 0");
    Zinc_expect_vector_str(ZINC_VECTOR_VECTOR(&s4->value, 0), "hello", "s4 value 0");

    Zinc_expect_int_equal(ZINC_VECTOR_INT(&s4->types, 1), FieldTypeString, "s4 types 1");
    Zinc_expect_true(Zinc_vector_match_str(ZINC_VECTOR_VECTOR(&s4->raw, 1), "world"), "s4 raw 1");
    Zinc_expect_vector_str(ZINC_VECTOR_VECTOR(&s4->value, 1), "world", "s4 value 1");

    Series* s5 = DataFrameSeriesByIndex(parse_output->df, 5);
    Zinc_assert_ptr(s5, "ptr s5");
    Zinc_expect_true(Zinc_vector_match_str(&s5->name, "Empty"), "s5 name");
    Zinc_expect_int_equal(s5->type, FieldTypeEmpty, "s5 type");

    Zinc_expect_int_equal(ZINC_VECTOR_INT(&s5->types, 0), FieldTypeEmpty, "s5 types 0");
    Zinc_expect_true(Zinc_vector_match_str(ZINC_VECTOR_VECTOR(&s5->raw, 0), ""), "s5 raw 0");

    Zinc_expect_int_equal(ZINC_VECTOR_INT(&s5->types, 0), FieldTypeEmpty, "s5 types 0");
    Zinc_expect_true(Zinc_vector_match_str(ZINC_VECTOR_VECTOR(&s5->raw, 0), ""), "s5 raw 0");

    Zinc_expect_size_t_equal(s5->value.count, 0, "s5 value count");

    CSVParseTeardown(parse_output);
}

void TestCSVParse()
{
    TestCSVParseHeaderName();
    TestCSVParseRowField();
    TestCSVParseTypes();
}