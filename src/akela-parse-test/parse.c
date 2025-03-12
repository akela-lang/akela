#include "centipede/parse.h"
#include "centipede/build.h"
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <centipede/base.h>
#include <sys/stat.h>
#include <zinc/os_win.h>

#include "zinc/input_unicode_file.h"
#include "centipede/module_finder_file.h"
#include "centipede/comp_table.h"
#include "zinc/os_unix.h"
#include "zinc/spec_error.h"
#include "compare.h"
#include "zinc/fs.h"
#include "zinc/string_list.h"
#include "cobble/compile.h"
#include "cobble/match.h"

#define NAME "akela-parse-test"


void Run_append_path(Zinc_string* bf, char* path);
void Parse_test_suite(Zinc_string* path);
void Apt_get_line(FILE* fp, Zinc_string* line, bool* done);

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

            if (Zinc_is_reg_file(&path)) {
                Parse_test_suite(&path);
            }

            Zinc_string_destroy(&path);
        }

        node = node->next;
    }

    Zinc_string_destroy(&dir_path);
    Zinc_string_list_destroy(&files);
}

void Parse_test_suite(Zinc_string* path)
{
    printf("%s\n", path->buf);
    Cob_re test_suite_seq = Cob_compile_str("^======\n$");
    Cob_re test_case_sep = Cob_compile_str("^######\n$");
    Cob_re section_sep = Cob_compile_str("^###\n$");

    Zinc_string test_suite;
    Zinc_string_init(&test_suite);

    Zinc_string section;
    Zinc_string_init(&section);

    FILE* fp = fopen(path->buf, "r");
    while (true) {
        Zinc_string line;
        Zinc_string_init(&line);
        bool done = false;
        Apt_get_line(fp, &line, &done);
        if (done) {
            break;
        }
    }

    Zinc_string_destroy(&test_suite);
    Zinc_string_destroy(&section);
    fclose(fp);
}

void Apt_get_line(FILE* fp, Zinc_string* line, bool* done)
{
    int c;
    while ((c = fgetc(fp)) != EOF) {
        Zinc_string_add_char(line, (char)c);
        if (c == '\n') {
            break;
        }
    }
    if (c == EOF) {
        *done = true;
    }
}