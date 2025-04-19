#include <stdio.h>
#include <zinc/fs.h>

#include "data.h"
#include "zinc/os_unix.h"

void Run_get_type_info(Art_data* data)
{
    Zinc_string path;
    Zinc_string_init(&path);
    Zinc_string_add_string(&path, &data->dir_path);
    Zinc_path_append_str(&path, "lib");
    Zinc_path_append_str(&path, "data.cent");
    printf("%s\n", Zinc_string_c_str(&path));

    Zinc_string name;
    Zinc_string_init(&name);
    Zinc_string_add_str(&name, "data.cent");
    Cent_comp_table* ct = NULL;
    FILE* fp = fopen(Zinc_string_c_str(&path), "r");
    if (!fp) {
        printf("Error opening file %s\n", Zinc_string_c_str(&path));
        exit(1);
    }
    Cent_comp_table_create_fp(&ct, &data->dir_path, &name, fp);
    Cent_comp_unit_parse(ct->primary);
    Cent_comp_unit_build(ct->primary);

    if (ct->primary->errors.head) {
        fprintf(stderr, "data.cent errors:\n");
        Zinc_error_list_print(&ct->primary->errors);
        exit(1);
    }

    Zinc_string_destroy(&name);
    data->type_info = ct;
    Zinc_string_destroy(&path);
}