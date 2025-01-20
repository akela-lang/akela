#include <stdio.h>
#include "data.h"
#include "zinc/os_unix.h"

void Run_get_type_info(Run_data* data, const char* dir_name)
{
    Zinc_string dir;
    Zinc_string_init(&dir);
    Zinc_string_add_str(&dir, dir_name);
    Zinc_path_append_str(&dir, "lib");
    Zinc_path_append_str(&dir, "data.cent");
    Zinc_string_finish(&dir);

    printf("%s\n", dir.buf);

    Zinc_string_destroy(&dir);
}