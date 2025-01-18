#include "centipede/parse.h"
#include "centipede/build.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <centipede/base.h>
#include <sys/stat.h>
#include <zinc/input_unicode_string.h>

#include "zinc/input_unicode_file.h"
#include "zinc/os_unix.h"
#include "zinc/spec_error.h"
#include "cobble/compile.h"
#include "cobble/match.h"
#include "data.h"
#include "zinc/String_slice.h"
#include <assert.h>

void Run_append_path(Zinc_string* bf, char* path);
void Run_test_case(Run_data* data, Zinc_string* dir_path, Zinc_string* path, Zinc_string* file_name);

bool Run_validate_directory(const char* path)
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

void Run_parse_files(char* dir_name)
{
    Run_data data;
    Run_data_init(&data);
    Zinc_string dir_path;
    Zinc_string_init(&dir_path);
    Zinc_string_add_str(&dir_path, dir_name);
    DIR* d;
    struct dirent* dir;
    d = opendir(dir_name);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                Zinc_string_slice name_slice = {dir->d_name, strlen(dir->d_name)};
                Cob_result ext_mr = Cob_match(&data.ext_re, name_slice);
                if (ext_mr.matched) {
                    Zinc_string path;
                    Zinc_string_init(&path);
                    Zinc_string_add_str(&path, dir_name);
                    Run_append_path(&path, dir->d_name);
                    Zinc_string_finish(&path);

                    Zinc_string file_name;
                    Zinc_string_init(&file_name);
                    Zinc_string_add_str(&file_name, dir->d_name);
                    Zinc_string_finish(&file_name);

                    struct stat sb;
                    if (stat(path.buf, &sb) == 0 && S_ISREG(sb.st_mode)) {
                        Run_test_case(&data, &dir_path, &path, &file_name);
                    }

                    Zinc_string_destroy(&path);
                    Zinc_string_destroy(&file_name);
                }
                Cob_result_destroy(&ext_mr);
            }
        }
    }

    Zinc_string_destroy(&dir_path);
}

void Run_append_path(Zinc_string* bf, char* path)
{
    Zinc_string_add_char(bf, '/');
    Zinc_string_add_str(bf, path);
}

void Run_test_case(Run_data* data, Zinc_string* dir_path, Zinc_string* path, Zinc_string* file_name)
{
    printf("%s\n", path->buf);

    FILE* fp = fopen(path->buf, "r");

    Zinc_string ake;
    Zinc_string_init(&ake);

    Zinc_string llvm;
    Zinc_string_init(&llvm);

    Zinc_string result;
    Zinc_string_init(&result);

    Zinc_string line;
    Zinc_string_init(&line);

    while (true) {
        int c;
        Zinc_string_clear(&line);
        while ((c = fgetc(fp)) != EOF) {
            Zinc_string_add_char(&line, (char)c);
            if (c == '\n') {
                break;
            }
        }
        Zinc_string_slice line_slice = {line.buf, line.size};

        Cob_result llvm_mr = Cob_match(&data->llvm_re, line_slice);
        Cob_result result_mr = Cob_match(&data->result_re, line_slice);

        if (result_mr.matched) {
            Zinc_string* bf = Zinc_string_list_get(&result_mr.groups, 1);
            assert(bf);
            Zinc_string_add_string(&result, bf);
        } else if (llvm_mr.matched) {
            Zinc_string* bf = Zinc_string_list_get(&llvm_mr.groups, 1);
            assert(bf);
            Zinc_string_add_string(&llvm, bf);
        } else {
            Zinc_string_add_string(&ake, &line);
        }

        if (c == EOF) {
            break;
        }
    }

    Zinc_string_finish(&ake);
    printf("Akela:\n%s", ake.buf);

    Zinc_string_finish(&llvm);
    printf("LLVM:\n%s", llvm.buf);

    Zinc_string_finish(&result);
    printf("Result:\n%s", result.buf);

    Zinc_string_destroy(&line);
    Zinc_string_destroy(&ake);
    Zinc_string_destroy(&llvm);
    Zinc_string_destroy(&result);

    fclose(fp);
}

void Run_akela(Zinc_string* ake)
{

}