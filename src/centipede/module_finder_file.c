#include "module_finder_file.h"
#include <zinc/memory.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "zinc/piece.h"
#include "zinc/input_unicode_file.h"
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include "zinc/string_slice.h"
#include "zinc/os_unix.h"
#include "zinc/os_win.h"

Cent_module_finder_vtable Cent_module_finder_file_vtable = {
    .find_offset = offsetof(Cent_module_finder_file, find),
    .destroy_offset = offsetof(Cent_module_finder_file, destroy),
};

void Cent_module_finder_file_init(Cent_module_finder_file *mf, Zinc_string* dir_path)
{
    Zinc_string_init(&mf->dir_path);
    Zinc_string_copy(dir_path, &mf->dir_path);
    Zinc_string_finish(&mf->dir_path);
    mf->find = (Cent_module_finder_find_interface)Cent_module_finder_file_find;
    mf->destroy = (Cent_module_finder_destroy_interface)Cent_module_finder_file_destroy;
    mf->vtable = &Cent_module_finder_file_vtable;
}

void Cent_module_finder_file_create(Cent_module_finder_file** mf, Zinc_string* dir_path)
{
    Zinc_malloc_safe((void**)mf, sizeof(Cent_module_finder_file));
    Cent_module_finder_file_init(*mf, dir_path);
}

void Cent_module_finder_file_append_path(Zinc_string* path1, Zinc_string* path2)
{
    Zinc_string_add_char(path1, '/');
    Zinc_string_copy(path2, path1);
}

Cent_input_data Cent_module_finder_file_find(Cent_module_finder_file* mf, Zinc_string* name)
{
    Cent_input_data data = {NULL, NULL};
    Zinc_string path;
    Zinc_string_init(&path);
    Zinc_string_copy(&mf->dir_path, &path);
    Cent_module_finder_file_append_path(&path, name);
    Zinc_string_finish(&path);
    struct stat sb;
	Zinc_result r = Zinc_is_reg_file(&path);
    if (r == Zinc_result_ok) {
        FILE* fp = fopen(path.buf, "r");
        if (fp) {
            Zinc_input_unicode_file* input = NULL;
            Zinc_input_unicode_file_create(&input, fp);
            data.input = input;
            data.input_vtable = input->vtable;
            Zinc_string_destroy(&path);
            return data;
        }
    }
    Zinc_string_destroy(&path);
    return data;
}

void Cent_module_finder_file_destroy(Cent_module_finder_file* mf)
{
    Zinc_string_destroy(&mf->dir_path);
}