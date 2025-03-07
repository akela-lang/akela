#include "centipede/parse.h"
#include "centipede/build.h"
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <zinc/input_unicode_string.h>
#include "zinc/os_unix.h"
#include "zinc/spec_error.h"
#include "cobble/match.h"
#include "data.h"
#include "zinc/String_slice.h"
#include <assert.h>
#include "cent.h"
#include "zinc/os_unix.h"
#include "zinc/os_win.h"
#include "zinc/fs.h"

void Run_collect(
    Run_data* data,
    Zinc_string* dir_path,
    Zinc_string* path,
    Zinc_string* file_name);

bool Run_validate_directory(const char* path)
{
    struct stat sb;
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
        fprintf(stderr, "%s\n", Zinc_error_message);
        return false;
    }

    if (!is_dir) {
        fprintf(stderr, "%s is not a directory", path);
        return false;
    }

    return true;
}

void Run_parse_files(Run_data* data, char* dir_name)
{
    Zinc_string dir_path;
    Zinc_string_init(&dir_path);
    Zinc_string_add_str(&dir_path, dir_name);

    Zinc_string_list files;
    Zinc_string_list_init(&files);
    Zinc_list_files(Zinc_string_c_str(&dir_path), &files);

    Zinc_string_node* node = files.head;
    while (node) {
        if (!Zinc_string_compare_str(&node->value, ".") && !Zinc_string_compare_str(&node->value, "..")) {
            Zinc_string_slice name_slice = {
                .p = Zinc_string_c_str(&node->value),
                .size = node->value.size,
            };
            Cob_result ext_mr = Cob_match(&data->ext_re, name_slice);
            if (ext_mr.matched) {
                Zinc_string path;
                Zinc_string_init(&path);
                Zinc_string_add_str(&path, dir_name);
                Zinc_path_append(&path, &node->value);
                Zinc_string_finish(&path);

                if (Zinc_is_reg_file(&node->value)) {
                    Run_collect(data, &dir_path, &path, &node->value);
                }

                Zinc_string_destroy(&path);
            }
            Cob_result_destroy(&ext_mr);
        }
        node = node->next;
    }

    Zinc_string_destroy(&dir_path);
}

void Run_collect(Run_data* data, Zinc_string* dir_path, Zinc_string* path, Zinc_string* file_name)
{
    Run_test* test = NULL;
    Run_test_create(&test);
    Zinc_string_add_string(&test->name, file_name);
    Zinc_string_finish(&test->name);

    FILE* fp = fopen(path->buf, "r");

    Zinc_string line;
    Zinc_string_init(&line);

    int part_count = 0;

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

        Cob_result sep = Cob_match(&data->separator_re, line_slice);

        if (sep.matched) {
            part_count++;
        } else {
            if (part_count == 0) {
                Zinc_string_add_string(&test->ake, &line);
            } else if (part_count == 1) {
                Zinc_string_add_string(&test->llvm, &line);
            } else if (part_count == 2) {
                Zinc_string_add_string(&test->config, &line);
            }
        }

        if (c == EOF) {
            break;
        }
    }

    Zinc_string_finish(&test->ake);
    Zinc_string_finish(&test->llvm);
    Zinc_string_finish(&test->config);

    Zinc_string_destroy(&line);
    fclose(fp);

    test->config_data = Run_get_cent(dir_path, file_name, &test->config);
    Run_test_list_add(&data->tests, test);

    if (test->config_data) {
        if (test->config_data->valid) {
            Cent_value* value = test->config_data->ct->primary->value;
            assert(value);

            Cent_value* solo = Cent_value_get_str(value, "solo");

            if (solo && solo->data.boolean) {
                test->solo = true;
                data->has_solo = true;
            }

            Cent_value* mute = Cent_value_get_str(value, "mute");
            if (mute && mute->data.boolean) {
                test->mute = true;
            }
        }
    }
}