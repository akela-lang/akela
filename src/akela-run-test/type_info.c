#include <stdio.h>
#include "data.h"
#include "zinc/os_unix.h"
#include "cent.h"

void Run_get_type_info(Run_data* data, const char* dir_name)
{
    Zinc_string dir_path;
    Zinc_string_init(&dir_path);
    Zinc_string_add_str(&dir_path, dir_name);

    Zinc_string file_name;
    Zinc_string_init(&file_name);
    Zinc_string_add_str(&file_name, "data.cent");

    Zinc_string path;
    Zinc_string_init(&path);
    Zinc_string_add_str(&path, dir_name);
    Zinc_path_append_str(&path, "lib");
    Zinc_path_append(&path, &file_name);
    Zinc_string_finish(&path);

    printf("%s\n", path.buf);
    if (Zinc_is_reg_file(&path) == Zinc_result_error) {
        printf("%s\n", Zinc_error_message);
        return;
    }

    Zinc_string type_info_string;
    Zinc_string_init(&type_info_string);
    FILE* fp = fopen(path.buf, "r");
    int c;
    while ((c = fgetc(fp)) != EOF) {
        Zinc_string_add_char(&type_info_string, (char)c);
    }
    fclose(fp);

    Run_cent_data* type_info = Run_get_cent(&dir_path, &file_name, &type_info_string);
    data->type_info = type_info;

    Zinc_string_destroy(&dir_path);
    Zinc_string_destroy(&file_name);
    Zinc_string_destroy(&path);
    Zinc_string_destroy(&type_info_string);
}