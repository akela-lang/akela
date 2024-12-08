set(dataframe_test_files
        test.c
        test_csv_lex.c
        test_csv_parse.c
        test_csv_parse_tools.c
        test_csv_write.c
        test_data_frame.c
        test_field_lex.c
)

add_executable(dataframe-test ${dataframe_test_files})
target_link_libraries(dataframe-test dataframe zinc)