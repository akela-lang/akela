#include "centipede/parse.h"
#include "centipede/build.h"
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <centipede/base.h>
#include <sys/stat.h>
#include "zinc/input_unicode_file.h"
#include "centipede/module_finder_file.h"
#include "centipede/comp_table.h"
#include "zinc/os_unix.h"
#include "zinc/spec_error.h"
#include "compare.h"
#include "zinc/fs.h"
#include "zinc/string_list.h"

#define NAME "akela-parse-test"


void Run_append_path(Zinc_string* bf, char* path);
void Run_test_case(Zinc_string* dir_path, Zinc_string* path, Zinc_string* file_name);

bool Run_validate_directory(char* path)
{
    if (!Zinc_file_exists(path)) {
        perror(path);
        Zinc_string cwd;
        Zinc_string_init(&cwd);
        Zinc_get_cwd(&cwd);
        Zinc_string_finish(&cwd);
        fprintf(stderr, "current working directory: %s\n", cwd.buf);
        Zinc_string_destroy(&cwd);
        return false;
    }

    bool is_dir;
    Zinc_result r = Zinc_is_directory(path, &is_dir);
    if (r == Zinc_result_error) {
        printf("%s\n", Zinc_error_message);
        return false;
    }

    if (!is_dir) {
        fprintf(stderr, "%s is not a directory", path);
        return false;
    }

    printf("%s\n", path);

    return true;
}

void Run_parse_files(char* dir_name)
{
    Zinc_string dir_path;
    Zinc_string_init(&dir_path);
    Zinc_string_add_str(&dir_path, dir_name);

    Zinc_string_list files;
    Zinc_string_list_init(&files);
    Zinc_list_files(dir_name, &files);

    Zinc_string_node* node = files.head;
    while (node) {
        if (!Zinc_string_compare_str(&node->value, ".") && !Zinc_string_compare_str(&node->value, "..")) {
            Zinc_string path;
            Zinc_string_init(&path);
            Zinc_string_add_str(&path, dir_name);
            Zinc_path_append(&path, &node->value);
            Zinc_string_finish(&path);

            struct stat sb;
            if (Zinc_is_reg_file(&node->value)) {
                Run_test_case(&dir_path, &path, &node->value);
            }

            Zinc_string_destroy(&path);
        }

        node = node->next;
    }

    Zinc_string_destroy(&dir_path);
}

void Run_test_case(Zinc_string* dir_path, Zinc_string* path, Zinc_string* file_name)
{
    printf("%s\n", path->buf);

    FILE* fp = fopen(path->buf, "r");
    Zinc_input_unicode_file* input = NULL;
    Zinc_input_unicode_file_create(&input, fp);

    Zinc_string_slice slice;
    slice.p = file_name->buf;
    slice.size = file_name->size;

    Cent_module_finder_file* mf = NULL;
    Cent_module_finder_file_create(&mf, dir_path);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create(&ct, mf, mf->vtable);

    Cent_comp_unit* cu = NULL;
    Cent_comp_unit_create(&cu, input, input->input_vtable, slice, ct->base);
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