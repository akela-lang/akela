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
#include "parse_tools.h"

#include "data.h"

#define NAME "akela-parse-test"


void Apt_parse_test_suite(Apt_data* data, Zinc_string* path, Zinc_string* name);

bool Apt_validate_directory(char* path)
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

void Apt_parse_files(Apt_data* data)
{
    Zinc_string_list files;
    Zinc_string_list_init(&files);
    Zinc_list_files(Zinc_string_c_str(&data->dir_path), &files);

    Zinc_string_node* node = files.head;
    while (node) {
        if (!Zinc_string_compare_str(&node->value, ".") && !Zinc_string_compare_str(&node->value, "..")) {
            Zinc_string path;
            Zinc_string_init(&path);
            Zinc_string_add_string(&path, &data->dir_path);
            Zinc_path_append(&path, &node->value);

            if (Zinc_is_reg_file(&path)) {
                Apt_parse_test_suite(data, &path, &node->value);
            }

            Zinc_string_destroy(&path);
        }

        node = node->next;
    }

    Zinc_string_list_destroy(&files);
}

void Apt_parse_test_suite(Apt_data* data, Zinc_string* path, Zinc_string* name)
{
    Apt_test_suite* ts = NULL;
    Apt_test_suite_create(&ts);
    Zinc_string_add_string(&ts->path, path);
    Zinc_string_add_string(&ts->name, name);

    printf("%s\n", path->buf);

    FILE* fp = fopen(path->buf, "r");
    while (true) {
        Zinc_string line;
        Zinc_string_init(&line);
        Apt_line_kind kind;
        Apt_get_line(data, fp, &line, &kind);
        if (kind != Apt_line_kind_regular) {
            break;
        }
        Zinc_string_add_string(&ts->text, &line);
    }

    Apt_suite_list_add(&data->suites, ts);

    fclose(fp);
}