#include "centipede/parse.h"
#include "centipede/build.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <centipede/base.h>
#include <sys/stat.h>
#include "zinc/input_unicode_file.h"
#include "centipede/module_file.h"
#include "centipede/comp_table.h"
#include "zinc/os_unix.h"
#include "zinc/spec_error.h"
#include "compare.h"

#define NAME "akela-parse-test"


void Apt_append_path(Zinc_string* bf, char* path);
void Apt_test_case(Zinc_string* dir_path, Zinc_string* path, Zinc_string* file_name);

bool Apt_validate_directory(char* path)
{
    struct stat sb;
    if (stat(path, &sb) == -1) {
        perror(path);
        Zinc_string cwd;
        Zinc_string_init(&cwd);
        Zinc_get_cwd(&cwd);
        Zinc_string_finish(&cwd);
        fprintf(stderr, "current working directory: %s\n", cwd.buf);
        Zinc_string_destroy(&cwd);
        return false;
    }

    if (!S_ISDIR(sb.st_mode)) {
        fprintf(stderr, "%s is not a directory", path);
        return false;
    }

    printf("%s\n", path);

    return true;
}

void Apt_parse_files(char* dir_name)
{
    Zinc_string dir_path;
    Zinc_string_init(&dir_path);
    Zinc_string_add_str(&dir_path, dir_name);
    DIR* d;
    struct dirent* dir;
    d = opendir(dir_name);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                Zinc_string path;
                Zinc_string_init(&path);
                Zinc_string_add_str(&path, dir_name);
                Apt_append_path(&path, dir->d_name);
                Zinc_string_finish(&path);

                Zinc_string file_name;
                Zinc_string_init(&file_name);
                Zinc_string_add_str(&file_name, dir->d_name);
                Zinc_string_finish(&file_name);

                struct stat sb;
                if (stat(path.buf, &sb) == 0 && S_ISREG(sb.st_mode)) {
                    Apt_test_case(&dir_path, &path, &file_name);
                }

                Zinc_string_destroy(&path);
                Zinc_string_destroy(&file_name);
            }
        }
    }

    Zinc_string_destroy(&dir_path);
}

void Apt_append_path(Zinc_string* bf, char* path)
{
    Zinc_string_add_char(bf, '/');
    Zinc_string_add_str(bf, path);
}

void Apt_test_case(Zinc_string* dir_path, Zinc_string* path, Zinc_string* file_name)
{
    printf("%s\n", path->buf);

    FILE* fp = fopen(path->buf, "r");
    Zinc_input_unicode_file* input = NULL;
    Zinc_input_unicode_file_create(&input, fp);

    Zinc_string_slice slice;
    slice.p = file_name->buf;
    slice.size = file_name->size;

    Cent_module_file* mf = NULL;
    Cent_module_file_create(&mf, dir_path);

    Cent_environment* base = Cent_base_create();

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create(&ct, mf, mf->vtable, base);

    Cent_comp_unit* cu = NULL;
    Cent_comp_unit_create(&cu, input, input->input_vtable, slice, mf, mf->vtable, base);
    cu->pd.cu = cu;
    cu->pd.ct = ct;

    Cent_comp_table_add(ct, file_name, cu);

    Cent_comp_unit_parse(cu);
    if (!cu->errors.head) {
        Cent_comp_unit_build(cu);
        if (!cu->errors.head) {
            Apt_run(file_name, cu);
        }
    }

    if (cu->errors.head) {
        Zinc_error* e = cu->errors.head;
        while (e) {
            Zinc_string_finish(&e->message);
            printf("(%zu,%zu) %s\n", e->loc.line, e->loc.col, e->message.buf);
            e = e->next;
        }
    }

    fclose(fp);
}