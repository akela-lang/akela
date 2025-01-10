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

void Cent_module_file_init(Cent_module_file *mf, struct buffer* dir_path)
{
    buffer_copy(dir_path, &mf->dir_path);
    buffer_finish(&mf->dir_path);
    mf->find = (Cent_module_find)Cent_module_file_find;
    mf->vtable = &Cent_module_file_vtable;
}

void Cent_module_file_create(Cent_module_file** mf, struct buffer* dir_path)
{
    malloc_safe((void**)mf, sizeof(struct Cent_module_file));
    Cent_module_file_init(*mf, dir_path);
}

void Cent_module_file_destroy(Cent_module_file* mf)
{
    buffer_destroy(&mf->dir_path);
}

void Cent_module_file_append_path(struct buffer* path1, struct buffer* path2)
{
    buffer_add_char(path1, '/');
    buffer_copy(path2, path1);
}

Cent_comp_unit* Cent_module_file_find(Cent_module_file* mf, struct buffer* name)
{
    struct buffer path;
    buffer_init(&path);
    buffer_copy(&mf->dir_path, &path);
    Cent_module_file_append_path(&path, name);
    buffer_finish(&path);
    struct stat sb;
    if (stat(path.buf, &sb) == 0 && S_ISREG(sb.st_mode)) {
        FILE* fp = fopen(path.buf, "r");
        if (fp) {
            Cent_comp_unit* cu = NULL;
            Cent_comp_unit_create(&cu);
            struct error_list* errors = NULL;
            error_list_create(&errors);
            cu->errors = errors;
            InputUnicodeFile* input = NULL;
            InputUnicodeFileCreate(&input, fp);
            cu->input = input;
            cu->input_vtable = input->input_vtable;
            return cu;
        }
    }
    buffer_destroy(&path);
    return NULL;
}