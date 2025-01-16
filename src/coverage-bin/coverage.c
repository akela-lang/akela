#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <linux/limits.h>
#include <unistd.h>
#include "zinc/string_list.h"
#include "zinc/result.h"
#include "coverage/data.h"
#include <sys/stat.h>
#include "cobble/compile.h"
#include "cobble/compile_data.h"
#include "cobble/compile_tools.h"
#include "cobble/match.h"
#include "cobble/match_tools.h"
#include "zinc/input_unicode_string.h"
#include "zinc/input_unicode.h"
#include "zinc/vector.h"
#include "coverage/parse.h"
#include <assert.h>

void Cover_cwd();
void Cover_append_path(struct Zinc_string* bf, char* path);
void Cover_get_libraries(char* dir_name, Cover_app* test, Cover_app* app);
void Cover_get_files(Cover_library* lib);
void Cover_read_file(Cover_file* file);
void Cover_print_match(struct Zinc_string_list* groups);
void Cover_app_print(Cover_app* app);
void Cover_agg_files(Cover_library* lib);
void Cover_agg_libraries(Cover_app* app);
void Cover_print_app_results(Cover_app* app, char* title);
void Cover_print_library_results(Cover_library* lib);
void Cover_print_file_results(Cover_file* file);

int main(int argc, char** argv)
{
    char usage[] = "coverage <input-dir>";
    if (argc != 2) {
        fprintf(stderr, "Usage: %s\n", usage);
        return 1;
    }
    char* dir_name = argv[1];

    Cover_app app;
    Cover_app_init(&app);
    Zinc_string_add_str(&app.data_path, dir_name);
    Zinc_string_finish(&app.data_path);

    Cover_app test;
    Cover_app_init(&test);
    Zinc_string_add_str(&test.data_path, dir_name);
    Zinc_string_finish(&test.data_path);

    Cover_get_libraries(dir_name, &test, &app);

    Cover_app_destroy(&app);

    return 0;
}

void Cover_cwd()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }
}

void Cover_append_path(struct Zinc_string* bf, char* path)
{
    Zinc_string_add_char(bf, '/');
    Zinc_string_add_str(bf, path);
}

void Cover_get_libraries(char* dir_name, Cover_app* test, Cover_app *app)
{
    Cob_re test_re = Cover_test_dir_re();
    DIR* d;
    struct dirent* dir;
    d = opendir(dir_name);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                struct Zinc_string bf;
                Zinc_string_init(&bf);
                Zinc_string_add_str(&bf, dir_name);
                Cover_append_path(&bf, dir->d_name);
                Zinc_string_finish(&bf);
                struct stat sb;
                if (stat(bf.buf, &sb) == 0 && S_ISDIR(sb.st_mode)) {
                    Cover_library* lib = NULL;
                    Cover_library_create(&lib);
                    Zinc_string_copy(&bf, &lib->path);
                    Zinc_string_add_str(&lib->name, dir->d_name);
                    Zinc_string_finish(&lib->path);
                    Zinc_string_finish(&lib->name);

                    Zinc_string_slice slice = {bf.buf, bf.size};
                    Cob_result mr = Cob_match(&test_re, slice);
                    if (mr.matched) {
                        Cover_library_list_add_sorted(&test->libraries, lib);
                    } else {
                        Cover_library_list_add_sorted(&app->libraries, lib);
                    }
                    Cover_get_files(lib);
                    Cob_result_destroy(&mr);
                }

                Zinc_string_destroy(&bf);
            }
        }
        closedir(d);
    } else {
        perror("opendir() error");
    }

    Cover_agg_libraries(test);
    Cover_agg_libraries(app);
    Cover_print_app_results(test, "Test");
    Cover_print_app_results(app, "Application");
}

void Cover_get_files(Cover_library* lib)
{
    Cob_re re = Cover_gcov_ext_re();
    DIR* d;
    struct dirent* dir;
    d = opendir(lib->path.buf);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                struct Zinc_string bf;
                Zinc_string_init(&bf);
                Zinc_string_copy(&lib->path, &bf);
                Cover_append_path(&bf, dir->d_name);
                Zinc_string_finish(&bf);
                struct stat sb;
                if (stat(bf.buf, &sb) == 0 && S_ISREG(sb.st_mode)) {
                    Zinc_string_slice slice = {bf.buf, bf.size};
                    Cob_result mr = Cob_match(&re, slice);
                    if (mr.matched) {
                        Cover_file* file = NULL;
                        Cover_file_create(&file);
                        Zinc_string_copy(&bf, &file->path);
                        Zinc_string_add_str(&file->name, dir->d_name);
                        Zinc_string_finish(&file->path);
                        Zinc_string_finish(&file->name);
                        Cover_file_list_add_sorted(&lib->files, file);
                        Cover_read_file(file);
                        Cover_print_file_results(file);
                    }
                    Cob_result_destroy(&mr);
                }
                Zinc_string_destroy(&bf);
            }
        }
    }

    Cover_agg_files(lib);
    Cover_print_library_results(lib);
}

void Cover_agg_files(Cover_library* lib)
{
    Cover_file* file = lib->files.head;
    while (file) {
        lib->line_count += file->line_count;
        lib->covered_count += file->covered_count;
        lib->not_covered_count += file->not_covered_count;
        file = file->next;
    }
    lib->coverage_percentage = (double)lib->covered_count / (double)lib->line_count * 100.0;
}

void Cover_agg_libraries(Cover_app* app)
{
    Cover_library* lib = app->libraries.head;
    while (lib) {
        app->line_count += lib->line_count;
        app->covered_count += lib->covered_count;
        app->not_covered_count += lib->not_covered_count;
        lib = lib->next;
    }
    app->coverage_percentage = (double)app->covered_count / (double)app->line_count * 100.0;
}

void Cover_read_file(Cover_file* file)
{
    FILE* fp = fopen(file->path.buf, "r");
    if (!fp) {
        perror("fopen() error");
        return;
    }

    Cob_re re = Cover_gcov_line_re();

    if (re.el->head) {
        printf("compile() error:\n");
        struct Zinc_error* e = re.el->head;
        while (e) {
            Zinc_string_finish(&e->message);
            printf("%s\n", e->message.buf);
            e = e->next;
        }

        Cover_re_cleanup(&re);
        return;
    }

    bool done = false;
    struct Zinc_string bf;
    Zinc_string_init(&bf);

    while (!done) {
        while (true) {
            int c = fgetc(fp);
            if (c == EOF) {
                done = true;
                break;
            }
            if (c == '\n') {
                break;
            }
            Zinc_string_add_char(&bf, (char)c);
        }

        if (bf.size > 0) {
            Zinc_string_slice slice;
            slice.p = bf.buf;
            slice.size = bf.size;
            Cob_result mr = Cob_match(&re, slice);
            if (mr.matched) {
                struct Zinc_string* count = Zinc_string_list_get(&mr.groups, 1);
                struct Zinc_string* line_number = Zinc_string_list_get(&mr.groups, 2);
                struct Zinc_string* source = Zinc_string_list_get(&mr.groups, 3);
                struct Zinc_string* source_path = Zinc_string_list_get(&mr.groups, 4);

                assert(line_number && count && source && source_path);

                Zinc_string_finish(line_number);
                Zinc_string_finish(count);
                Zinc_string_finish(source);
                Zinc_string_finish(source_path);

                bool should_count_line;
                size_t count_value = 0;
                if (count->size > 0 && count->buf[0] == '-') {
                    should_count_line = false;
                } else if (Zinc_string_compare_str(count, "#####")) {
                    should_count_line = true;
                    count_value = 0;
                } else {
                    should_count_line = true;
                    count_value = strtoull(count->buf, NULL, 10);
                }

                if (should_count_line) {
                    file->line_count++;
                    if (count_value >= 1) {
                        file->covered_count++;
                    } else {
                        file->not_covered_count++;
                    }
                }

                Zinc_string_finish(line_number);
                unsigned long long num = strtoull(line_number->buf, NULL, 10);
                if (Zinc_string_compare_str(source, "Source:") && num == 0) {
                    Zinc_string_copy(source_path, &file->source_path);
                }
                Zinc_string_finish(&file->source_path);
            } else {
                Zinc_string_finish(&bf);
                fprintf(stderr, "\n%s\n", file->path.buf);
                fprintf(stderr, "%s\n", bf.buf);
                fprintf(stderr, "did not match\n");
            }

            Cob_result_destroy(&mr);
            Zinc_string_clear(&bf);

        }
    }

    Zinc_string_destroy(&bf);

    fclose(fp);

    if (file->line_count == 0) {
        file->coverage_percentage = 100.0;
    } else {
        file->coverage_percentage =
            (double)file->covered_count / (double)file->line_count * 100.0;
    }
}

void Cover_print_match(struct Zinc_string_list* groups)
{
    printf("matched:\n");

    struct Zinc_string* string = Zinc_string_list_get(groups, 0);
    if (string) {
        Zinc_string_finish(string);
        printf("\tstring: %s\n", string->buf);
    } else {
        printf("\tcould not get string\n");
    }

    struct Zinc_string* count = Zinc_string_list_get(groups, 1);
    if (count) {
        Zinc_string_finish(count);
        printf("\tcount: %s\n", count->buf);
    } else {
        printf("\tcould not get count\n");
    }

    struct Zinc_string* line_number = Zinc_string_list_get(groups, 2);
    if (line_number) {
        Zinc_string_finish(line_number);
        printf("\tline number: %s\n", line_number->buf);
    } else {
        printf("\tcould not get line number\n");
    }

    struct Zinc_string* source = Zinc_string_list_get(groups, 3);
    if (source) {
        Zinc_string_finish(source);
        printf("\tsource: %s\n", source->buf);
    } else {
        printf("\tcould not get source\n");
    }

    struct Zinc_string* filename = Zinc_string_list_get(groups, 4);
    if (filename) {
        Zinc_string_finish(filename);
        printf("\tfilename: %s\n", filename->buf);
    } else {
        printf("\tcould not get filename\n");
    }
}

void Cover_print_app_results(Cover_app* app, char* title)
{
    printf("%s total\n", title);
    printf("\tline count: %zu\n", app->line_count);
    printf("\tcovered count: %zu\n", app->covered_count);
    printf("\tnot covered count: %zu\n", app->not_covered_count);
    printf("\tcoverage (%%): %lf\n", app->coverage_percentage);
    printf("\n");
    for (int i = 0; i < 80; i++) {
        printf("-");
    }
    printf("\n\n");
}

void Cover_print_library_results(Cover_library* lib)
{
    printf("Directory total (%s)\n", lib->name.buf);
    printf("\tline count: %zu\n", lib->line_count);
    printf("\tcovered count: %zu\n", lib->covered_count);
    printf("\tnot covered count: %zu\n", lib->not_covered_count);
    printf("\tcoverage (%%): %lf\n", lib->coverage_percentage);
    printf("\n");
    for (int i = 0; i < 80; i++) {
        printf("-");
    }
    printf("\n\n");
}

void Cover_print_file_results(Cover_file* file)
{
    printf("\tfile: %s\n", file->source_path.buf);
    printf("\tline count: %zu\n", file->line_count);
    printf("\tcovered count: %zu\n", file->covered_count);
    printf("\tnot covered count: %zu\n", file->not_covered_count);
    printf("\tcoverage (%%): %lf\n", file->coverage_percentage);
    printf("\n");
}