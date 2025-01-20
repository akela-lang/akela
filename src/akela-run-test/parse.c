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
#include <centipede/comp_table.h>
#include <centipede/module_file.h>

#include "centipede/base.h"

#include "akela/comp_unit.h"
#include "akela-llvm/cg.h"

void Run_append_path(Zinc_string* bf, char* path);
void Run_collect(
    Run_data* data,
    Zinc_string* dir_path,
    Zinc_string* path,
    Zinc_string* file_name);
Run_config_data* Run_get_config(Zinc_string* dir_path, Zinc_string* file_name, Zinc_string* config_string);

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

void Run_parse_files(Run_data* data, char* dir_name)
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
                Zinc_string_slice name_slice = {dir->d_name, strlen(dir->d_name)};
                Cob_result ext_mr = Cob_match(&data->ext_re, name_slice);
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
                        Run_collect(data, &dir_path, &path, &file_name);
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

void Run_collect(Run_data* data, Zinc_string* dir_path, Zinc_string* path, Zinc_string* file_name)
{
    printf("%s\n", path->buf);

    Run_test* test = NULL;
    Run_test_create(&test);

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

    test->config_data = Run_get_config(dir_path, file_name, &test->config);
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

Run_config_data* Run_get_config(Zinc_string* dir_path, Zinc_string* file_name, Zinc_string* config_string)
{
    Cent_module_file* mf = NULL;
    Cent_module_file_create(&mf, dir_path);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create(&ct, mf, mf->vtable, Cent_base_create());

    Zinc_vector* v = NULL;
    Zinc_vector_create(&v, sizeof(char));
    Zinc_vector_add(v, config_string->buf, config_string->size);

    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, v);

    Zinc_string_slice file_name_slice = {file_name->buf, file_name->size};
    Cent_comp_unit* cu = NULL;
    Cent_comp_unit_create(
        &cu,
        input,
        input->input_vtable,
        file_name_slice,
        mf,
        mf->vtable, ct->base);
    Cent_comp_table_add_str(ct, "*config*", cu);
    ct->primary = cu;
    cu->ct = ct;
    cu->pd.cu = cu;
    cu->pd.ct = ct;

    bool valid = false;
    Cent_comp_unit_parse(cu);
    if (cu->errors.head) {
        Zinc_error* e = cu->errors.head;
        while (e) {
            Zinc_string_finish(&e->message);
            printf("(%zu,%zu): %s\n", e->loc.line, e->loc.col, e->message.buf);
            e = e->next;
        }
    } else {
        Cent_comp_unit_build(cu);
        if (cu->errors.head) {
            Zinc_error* e = cu->errors.head;
            while (e) {
                Zinc_string_finish(&e->message);
                printf("(%zu,%zu): %s\n", e->loc.line, e->loc.col, e->message.buf);
                e = e->next;
            }
        } else {
            valid = true;
        }
    }

    Run_config_data* config_data = NULL;
    Run_config_data_create(&config_data, file_name);
    config_data->mf = mf;
    config_data->ct = ct;
    config_data->v = v;
    config_data->input = input;
    config_data->valid = valid;

    if (valid) {
        return config_data;
    }

    Run_config_data_destroy(config_data);

    return NULL;
}