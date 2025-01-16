#include "module_file.h"
#include <zinc/memory.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "zinc/piece.h"
#include "zinc/input_unicode_file.h"
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <errno.h>
#include "zinc/String_slice.h"

Cent_module_vtable Cent_module_file_vtable = {
    .find_offset = offsetof(Cent_module_file, find),
};

void Cent_module_file_init(Cent_module_file *mf, struct Zinc_string* dir_path)
{
    Zinc_string_copy(dir_path, &mf->dir_path);
    Zinc_string_finish(&mf->dir_path);
    mf->find = (Cent_module_find)Cent_module_file_find;
    mf->vtable = &Cent_module_file_vtable;
}

void Cent_module_file_create(Cent_module_file** mf, struct Zinc_string* dir_path)
{
    Zinc_malloc_safe((void**)mf, sizeof(struct Cent_module_file));
    Cent_module_file_init(*mf, dir_path);
}

void Cent_module_file_destroy(Cent_module_file* mf)
{
    Zinc_string_destroy(&mf->dir_path);
}

void Cent_module_file_append_path(struct Zinc_string* path1, struct Zinc_string* path2)
{
    Zinc_string_add_char(path1, '/');
    Zinc_string_copy(path2, path1);
}

Cent_input_data Cent_module_file_find(Cent_module_file* mf, struct Zinc_string* name)
{
    Cent_input_data data = {NULL, NULL};
    struct Zinc_string path;
    Zinc_string_init(&path);
    Zinc_string_copy(&mf->dir_path, &path);
    Cent_module_file_append_path(&path, name);
    Zinc_string_finish(&path);
    struct stat sb;
    if (stat(path.buf, &sb) == 0 && S_ISREG(sb.st_mode)) {
        FILE* fp = fopen(path.buf, "r");
        if (fp) {
            Zinc_input_unicode_file* input = NULL;
            Zinc_input_unicode_file_create(&input, fp);
            data.input = input;
            data.input_vtable = input->input_vtable;
            return data;
        }
    }
    Zinc_string_destroy(&path);
    return data;
}