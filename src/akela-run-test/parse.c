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
#include "akela/comp_unit.h"
#include "akela-llvm/cg.h"

void Run_append_path(Zinc_string* bf, char* path);
void Run_test_case(
    Run_data* data,
    Zinc_string* dir_path,
    Zinc_string* path,
    Zinc_string* file_name);
void Run_akela(Zinc_string* ake, Zinc_string* llvm, Zinc_string* result);
Run_pair Run_diff(Zinc_string* actual, Zinc_string* expected);
Zinc_string_list* Run_split(Zinc_string* string);
Zinc_string* Run_join(Zinc_string_list* list);
void Run_print_akela(Run_pair* llvm_pair, Run_pair* result_pair, Zinc_string* ake);
void Run_print_llvm(Run_pair* pair);
void Run_print_result(Run_pair* pair);

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

    Run_akela(&ake, &llvm, &result);

    Zinc_string_destroy(&line);
    Zinc_string_destroy(&ake);
    Zinc_string_destroy(&llvm);
    Zinc_string_destroy(&result);

    fclose(fp);
}

void Run_akela(Zinc_string* ake, Zinc_string* llvm, Zinc_string* result)
{
    Zinc_vector* text = NULL;
    Zinc_vector_create(&text, sizeof(char));
    Zinc_vector_add(text, ake->buf, ake->size);

    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, text);

    Ake_comp_unit* cu = NULL;
    Ake_comp_unit_create(&cu);
    Ake_comp_unit_compile(cu, input, input->input_vtable);

    if (!cu->valid) {
        Zinc_error* e = cu->el.head;
        while (e) {
            Zinc_string_finish(&e->message);
            fprintf(stderr, "%zu,%zu: %s\n", e->loc.line, e->loc.col, e->message.buf);
            e = e->next;
        }
        return;
    } else {
        Akela_llvm_cg* cg = NULL;
        Akela_llvm_cg_create(&cg, &cu->el, &cu->extern_list);
        Ake_code_gen_result cg_result;
        Ake_code_gen_result_init(&cg_result);
        Ake_code_gen_jit(cg, &Akela_llvm_vtable, cu->root, &cg_result);

        Run_pair llvm_pair = Run_diff(&cg_result.function_text, llvm);

        if (cu->el.head) {
            struct Zinc_error* e = cu->el.head;
            while (e) {
                Zinc_string_finish(&e->message);
                fprintf(stderr, "%zu,%zu: %s\n", e->loc.line, e->loc.col, e->message.buf);
                e = e->next;
            }
        }

        Run_pair result_pair = Run_diff(&cg_result.value, result);

        Run_print_akela(&llvm_pair, &result_pair, ake);
        Run_print_llvm(&llvm_pair);
        Run_print_result(&result_pair);

        Ake_code_gen_result_destroy(&cg_result);

    }

    Ake_comp_unit_destroy(cu);
    Zinc_vector_destroy(text);
    free(text);
    free(input);
}

void Run_print_akela(Run_pair* llvm_pair, Run_pair* result_pair, Zinc_string* ake)
{
    if (!llvm_pair->matched || !result_pair->matched) {
        fprintf(stderr, "Source:\n");
        Zinc_string_finish(ake);
        fprintf(stderr, "%s\n", ake->buf);
    }
}

void Run_print_llvm(Run_pair* pair)
{
    if (!pair->matched) {
        fprintf(stderr, "llvm is different.\n");
        fprintf(stderr, "Actual:\n");
        Zinc_string_finish(pair->actual);
        fprintf(stderr, "%s\n", pair->actual->buf);
        fprintf(stderr, "Expected:\n");
        Zinc_string_finish(pair->expected);
        fprintf(stderr, "%s\n", pair->expected->buf);
    }
}

void Run_print_result(Run_pair* pair)
{
    if (!pair->matched) {
        fprintf(stderr, "result different.\n");
        fprintf(stderr, "Actual:\n");
        Zinc_string_finish(pair->actual);
        fprintf(stderr, "%s\n", pair->actual->buf);
        fprintf(stderr, "Expected:\n");
        Zinc_string_finish(pair->expected);
        fprintf(stderr, "%s\n", pair->expected->buf);
    }
}

Run_pair Run_diff(Zinc_string* actual, Zinc_string* expected)
{
    Zinc_string_list* actual_list = Run_split(actual);
    Zinc_string_list* expected_list = Run_split(expected);

    size_t actual_count = Zinc_string_list_count(actual_list);
    size_t expected_count = Zinc_string_list_count(expected_list);
    size_t count = 0;
    if (actual_count > expected_count) {
        count = actual_count;
    } else {
        count = expected_count;
    }

    Run_pair pair;
    Run_pair_init(&pair);

    Zinc_string_list actual_diff_list;
    Zinc_string_list_init(&actual_diff_list);
    Zinc_string_list expected_diff_list;
    Zinc_string_list_init(&expected_diff_list);

    for (size_t i = 0; i < count; i++) {

        if (i < actual_count && i < expected_count) {
            Zinc_string* actual_line = Zinc_string_list_get(actual_list, i);
            Zinc_string* expected_line = Zinc_string_list_get(expected_list, i);

            Zinc_string actual_line2;
            Zinc_string_init(&actual_line2);
            Zinc_string expected_line2;
            Zinc_string_init(&expected_line2);

            if (Zinc_string_compare(actual_line, expected_line)) {
                Zinc_string_add_str(&actual_line2, "s ");
                Zinc_string_add_str(&expected_line2, "s ");
            } else {
                pair.matched = false;
                Zinc_string_add_str(&actual_line2, "d ");
                Zinc_string_add_str(&expected_line2, "d ");
            }

            Zinc_string_add_string(&actual_line2, actual_line);
            Zinc_string_list_add_bf(&actual_diff_list, &actual_line2);

            Zinc_string_add_string(&expected_line2, expected_line);
            Zinc_string_list_add_bf(&expected_diff_list, &expected_line2);

            Zinc_string_destroy(&actual_line2);
            Zinc_string_destroy(&expected_line2);
        } else if (i < actual_count) {
            pair.matched = false;
            Zinc_string* actual_line = Zinc_string_list_get(actual_list, i);
            Zinc_string actual_line2;
            Zinc_string_init(&actual_line2);
            Zinc_string_add_str(&actual_line2, "d ");
            Zinc_string_add_string(&actual_line2, actual_line);
            Zinc_string_list_add_bf(&actual_diff_list, &actual_line2);
            Zinc_string_destroy(&actual_line2);
        } else if (i < expected_count) {
            pair.matched = false;
            Zinc_string* expected_line = Zinc_string_list_get(expected_list, i);
            Zinc_string expected_line2;
            Zinc_string_init(&expected_line2);
            Zinc_string_add_str(&expected_line2, "d ");
            Zinc_string_add_string(&expected_line2, expected_line);
            Zinc_string_list_add_bf(&expected_diff_list, &expected_line2);
            Zinc_string_destroy(&expected_line2);
        }
    }

    Zinc_string* actual_diff = Run_join(&actual_diff_list);
    Zinc_string* expected_diff = Run_join(&expected_diff_list);

    pair.actual = actual_diff;
    pair.expected = expected_diff;
    return pair;
}

Zinc_string_list* Run_split(Zinc_string* string)
{
    Zinc_string_list* list = NULL;
    Zinc_string_list_create(&list);

    size_t pos = 0;
    bool done = false;
    while (!done) {
        Zinc_string line;
        Zinc_string_init(&line);
        while (!done) {
            char c[5];
            int num;
            const Zinc_result r = Zinc_string_next(string, &pos, c, &num, &done);
            if (r == Zinc_result_error) {
                fprintf(stderr, "run split error: %s\n", Zinc_error_message);
                return list;
            }

            if (done) {
                break;
            }

            Zinc_string_add(&line, c, num);

            if (num == 1 && c[0] == '\n') {
                Zinc_string_list_add_bf(list, &line);
                break;
            }
        }
        Zinc_string_destroy(&line);
    }

    return list;
}

Zinc_string* Run_join(Zinc_string_list* list)
{
    Zinc_string* result = NULL;
    Zinc_string_create(&result);

    Zinc_string_node* p = list->head;
    while (p) {
        Zinc_string_add_string(result, &p->value);
        p = p->next;
    }

    return result;
}