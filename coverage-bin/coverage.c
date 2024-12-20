#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <linux/limits.h>
#include <unistd.h>
#include "zinc/buffer_list.h"
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

void Cvr_cwd();
void Cvr_append_path(struct buffer* bf, char* path);
void Cvr_get_libraries(char* dir_name, Cvr_app* test, Cvr_app* app);
void Cvr_get_files(Cvr_library* lib);
void Cvr_read_file(Cvr_file* file);
void Cvr_print_match(struct buffer_list* groups);
void Cvr_app_print(Cvr_app* app);
void Cvr_agg_files(Cvr_library* lib);
void Cvr_agg_libraries(Cvr_app* app);
void Cvr_print_app_results(Cvr_app* app, char* title);
void Cvr_print_library_results(Cvr_library* lib);
void Cvr_print_file_results(Cvr_file* file);

int main(int argc, char** argv)
{
    char usage[] = "coverage <input-dir>";
    if (argc != 2) {
        fprintf(stderr, "Usage: %s\n", usage);
        return 1;
    }
    char* dir_name = argv[1];

    Cvr_app app;
    Cvr_app_init(&app);
    buffer_copy_str(&app.data_path, dir_name);
    buffer_finish(&app.data_path);

    Cvr_app test;
    Cvr_app_init(&test);
    buffer_copy_str(&test.data_path, dir_name);
    buffer_finish(&test.data_path);

    Cvr_get_libraries(dir_name, &test, &app);

    Cvr_app_destroy(&app);

    return 0;
}

void Cvr_cwd()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }
}

void Cvr_append_path(struct buffer* bf, char* path)
{
    buffer_add_char(bf, '/');
    buffer_add_str(bf, path);
}

void Cvr_get_libraries(char* dir_name, Cvr_app* test, Cvr_app *app)
{
    Cob_re test_re = Cvr_test_dir_re();
    DIR* d;
    struct dirent* dir;
    d = opendir(dir_name);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                struct buffer bf;
                buffer_init(&bf);
                buffer_add_str(&bf, dir_name);
                Cvr_append_path(&bf, dir->d_name);
                buffer_finish(&bf);
                struct stat sb;
                if (stat(bf.buf, &sb) == 0 && S_ISDIR(sb.st_mode)) {
                    Cvr_library* lib = NULL;
                    Cvr_library_create(&lib);
                    buffer_copy(&bf, &lib->path);
                    buffer_add_str(&lib->name, dir->d_name);
                    buffer_finish(&lib->path);
                    buffer_finish(&lib->name);

                    String_slice slice = {bf.buf, bf.size};
                    struct buffer_list groups;
                    buffer_list_init(&groups);
                    if (Cob_match(test_re.root, slice, &groups)) {
                        Cvr_library_list_add_sorted(&test->libraries, lib);
                    } else {
                        Cvr_library_list_add_sorted(&app->libraries, lib);
                    }
                    Cvr_get_files(lib);
                    buffer_list_destroy(&groups);
                }

                buffer_destroy(&bf);
            }
        }
        closedir(d);
    } else {
        perror("opendir() error");
    }

    Cvr_agg_libraries(test);
    Cvr_agg_libraries(app);
    Cvr_print_app_results(test, "Test");
    Cvr_print_app_results(app, "Application");
}

void Cvr_get_files(Cvr_library* lib)
{
    Cob_re re = Cvr_gcov_ext_re();
    DIR* d;
    struct dirent* dir;
    d = opendir(lib->path.buf);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                struct buffer bf;
                buffer_init(&bf);
                buffer_copy(&lib->path, &bf);
                Cvr_append_path(&bf, dir->d_name);
                buffer_finish(&bf);
                struct stat sb;
                if (stat(bf.buf, &sb) == 0 && S_ISREG(sb.st_mode)) {
                    String_slice slice = {bf.buf, bf.size};
                    struct buffer_list groups;
                    buffer_list_init(&groups);
                    if (Cob_match(re.root, slice, &groups)) {
                        Cvr_file* file = NULL;
                        Cvr_file_create(&file);
                        buffer_copy(&bf, &file->path);
                        buffer_copy_str(&file->name, dir->d_name);
                        buffer_finish(&file->path);
                        buffer_finish(&file->name);
                        Cvr_file_list_add_sorted(&lib->files, file);
                        Cvr_read_file(file);
                        Cvr_print_file_results(file);
                    }
                    buffer_list_destroy(&groups);
                }
                buffer_destroy(&bf);
            }
        }
    }

    Cvr_agg_files(lib);
    Cvr_print_library_results(lib);
}

void Cvr_agg_files(Cvr_library* lib)
{
    Cvr_file* file = lib->files.head;
    while (file) {
        lib->line_count += file->line_count;
        lib->covered_count += file->covered_count;
        lib->not_covered_count += file->not_covered_count;
        file = file->next;
    }
    lib->coverage_percentage = (double)lib->covered_count / (double)lib->line_count * 100.0;
}

void Cvr_agg_libraries(Cvr_app* app)
{
    Cvr_library* lib = app->libraries.head;
    while (lib) {
        app->line_count += lib->line_count;
        app->covered_count += lib->covered_count;
        app->not_covered_count += lib->not_covered_count;
        lib = lib->next;
    }
    app->coverage_percentage = (double)app->covered_count / (double)app->line_count * 100.0;
}

void Cvr_read_file(Cvr_file* file)
{
    FILE* fp = fopen(file->path.buf, "r");
    if (!fp) {
        perror("fopen() error");
        return;
    }

    Cob_re re = Cvr_gcov_line_re();

    if (re.el->head) {
        printf("compile() error:\n");
        struct error* e = re.el->head;
        while (e) {
            buffer_finish(&e->message);
            printf("%s\n", e->message.buf);
            e = e->next;
        }

        Cvr_re_cleanup(&re);
        return;
    }

    bool done = false;
    struct buffer bf;
    buffer_init(&bf);

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
            buffer_add_char(&bf, (char)c);
        }

        if (bf.size > 0) {
            String_slice slice;
            slice.p = bf.buf;
            slice.size = bf.size;
            struct buffer_list groups;
            buffer_list_init(&groups);
            bool m = Cob_match(re.root, slice, &groups);
            if (m) {
                struct buffer* count = buffer_list_get(&groups, 1);
                struct buffer* line_number = buffer_list_get(&groups, 2);
                struct buffer* source = buffer_list_get(&groups, 3);
                struct buffer* source_path = buffer_list_get(&groups, 4);

                buffer_finish(line_number);
                buffer_finish(count);
                buffer_finish(source);
                buffer_finish(source_path);

                bool should_count_line;
                size_t count_value = 0;
                if (count->size > 0 && count->buf[0] == '-') {
                    should_count_line = false;
                } else if (buffer_compare_str(count, "#####")) {
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

                buffer_finish(line_number);
                unsigned long long num = strtoull(line_number->buf, NULL, 10);
                if (buffer_compare_str(source, "Source:") && num == 0) {
                    buffer_copy(source_path, &file->source_path);
                }
                buffer_finish(&file->source_path);
            } else {
                buffer_finish(&bf);
                fprintf(stderr, "\n%s\n", file->path.buf);
                fprintf(stderr, "%s\n", bf.buf);
                fprintf(stderr, "did not match\n");
            }

            buffer_list_destroy(&groups);
            buffer_clear(&bf);

        }
    }

    buffer_destroy(&bf);

    fclose(fp);

    if (file->line_count == 0) {
        file->coverage_percentage = 100.0;
    } else {
        file->coverage_percentage =
            (double)file->covered_count / (double)file->line_count * 100.0;
    }
}

void Cvr_print_match(struct buffer_list* groups)
{
    printf("matched:\n");

    struct buffer* string = buffer_list_get(groups, 0);
    if (string) {
        buffer_finish(string);
        printf("\tstring: %s\n", string->buf);
    } else {
        printf("\tcould not get string\n");
    }

    struct buffer* count = buffer_list_get(groups, 1);
    if (count) {
        buffer_finish(count);
        printf("\tcount: %s\n", count->buf);
    } else {
        printf("\tcould not get count\n");
    }

    struct buffer* line_number = buffer_list_get(groups, 2);
    if (line_number) {
        buffer_finish(line_number);
        printf("\tline number: %s\n", line_number->buf);
    } else {
        printf("\tcould not get line number\n");
    }

    struct buffer* source = buffer_list_get(groups, 3);
    if (source) {
        buffer_finish(source);
        printf("\tsource: %s\n", source->buf);
    } else {
        printf("\tcould not get source\n");
    }

    struct buffer* filename = buffer_list_get(groups, 4);
    if (filename) {
        buffer_finish(filename);
        printf("\tfilename: %s\n", filename->buf);
    } else {
        printf("\tcould not get filename\n");
    }
}

void Cvr_print_app_results(Cvr_app* app, char* title)
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

void Cvr_print_library_results(Cvr_library* lib)
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

void Cvr_print_file_results(Cvr_file* file)
{
    printf("\tfile: %s\n", file->source_path.buf);
    printf("\tline count: %zu\n", file->line_count);
    printf("\tcovered count: %zu\n", file->covered_count);
    printf("\tnot covered count: %zu\n", file->not_covered_count);
    printf("\tcoverage (%%): %lf\n", file->coverage_percentage);
    printf("\n");
}