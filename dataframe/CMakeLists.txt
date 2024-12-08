set(dataframe_files
        csv_lex.c
        csv_parse.c
        csv_write.c
        data_frame.c
        field.c
        field_lex.c
        series.c
)

add_library(dataframe ${dataframe_files})