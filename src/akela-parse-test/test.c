#include "centipede/parse.h"
#include "centipede/build.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <linux/limits.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <centipede/base.h>
#include <sys/stat.h>
#include "zinc/input_unicode_file.h"
#include "centipede/lex_data.h"
#include "centipede/parse.h"
#include "centipede/build.h"
#include "centipede/module_file.h"
#include "centipede/comp_table.h"
#include "zinc/os_unix.h"
#include <assert.h>

#define NAME "akela-parse-test"

bool Parse_test_validate_directory(char* path);
void Parse_test_append_path(struct Zinc_string* bf, char* path);
void Parse_test_get_files(char* dir_name);
void Parse_test_test_case(struct Zinc_string* dir_path, struct Zinc_string* path, struct Zinc_string* file_name);
void Apt_run(Cent_comp_unit* cu);
void Apt_run_test(Cent_value* test_value);

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [filename]\n", NAME);
        return 1;
    }

    char* path = argv[1];
    if (!Parse_test_validate_directory(path)) {
        return 1;
    }

    Parse_test_get_files(path);

    return 0;
}

bool Parse_test_validate_directory(char* path)
{
    struct stat sb;
    if (stat(path, &sb) == -1) {
        perror(path);
        struct Zinc_string cwd;
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

void Parse_test_append_path(struct Zinc_string* bf, char* path)
{
    Zinc_string_add_char(bf, '/');
    Zinc_string_add_str(bf, path);
}

void Parse_test_get_files(char* dir_name)
{
    struct Zinc_string dir_path;
    Zinc_string_init(&dir_path);
    Zinc_string_add_str(&dir_path, dir_name);
    DIR* d;
    struct dirent* dir;
    d = opendir(dir_name);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                struct Zinc_string path;
                Zinc_string_init(&path);
                Zinc_string_add_str(&path, dir_name);
                Parse_test_append_path(&path, dir->d_name);
                Zinc_string_finish(&path);

                struct Zinc_string file_name;
                Zinc_string_init(&file_name);
                Zinc_string_add_str(&file_name, dir->d_name);
                Zinc_string_finish(&file_name);

                struct stat sb;
                if (stat(path.buf, &sb) == 0 && S_ISREG(sb.st_mode)) {
                    Parse_test_test_case(&dir_path, &path, &file_name);
                }

                Zinc_string_destroy(&path);
                Zinc_string_destroy(&file_name);
            }
        }
    }

    Zinc_string_destroy(&dir_path);
}

void Parse_test_test_case(struct Zinc_string* dir_path, struct Zinc_string* path, struct Zinc_string* file_name)
{
    printf("%s\n", path->buf);

    FILE* fp = fopen(path->buf, "r");
    InputUnicodeFile* input = NULL;
    InputUnicodeFileCreate(&input, fp);

    String_slice slice;
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
            printf("success!\n");
            Apt_run(cu);
        }
    }

    if (cu->errors.head) {
        struct Zinc_error* e = cu->errors.head;
        while (e) {
            Zinc_string_finish(&e->message);
            printf("(%zu,%zu) %s\n", e->loc.line, e->loc.col, e->message.buf);
            e = e->next;
        }
    }

    fclose(fp);
}

void Apt_run(Cent_comp_unit* cu)
{
    if (!cu->value) return;

    Cent_value* test_suite_value = cu->value;
    assert(test_suite_value->type == Cent_value_type_dag);

    Cent_value* mute = Cent_value_get_str(test_suite_value, "mute");
    if (mute && mute->data.boolean) return;

    Cent_value* name = Cent_value_get_str(test_suite_value, "name");
    Zinc_string_finish(&name->data.string);
    printf("%s\n", name->data.string.buf);

    Cent_value* test_value = test_suite_value->data.dag.head;
    while (test_value) {
        Apt_run_test(test_value);
        test_value = test_value->next;
    }
}

void Apt_run_test(Cent_value* test_value)
{
    Cent_value* name = Cent_value_get_str(test_value, "name");
    Zinc_string_finish(&name->data.string);
    printf("%s\n", name->data.string.buf);
    Cent_value* source = Cent_value_get_str(test_value, "source");
    Cent_value* line = source->data.dag.head;
    while (line) {
        Zinc_string_finish(&line->data.string);
        printf("%s", line->data.string.buf);
        line = line->next;
    }
}