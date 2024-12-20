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

void Cov_cwd();
void Cov_append_path(struct buffer* bf, char* path);
void Cov_get_libraries(char* dir_name, Cov_app* app);
void Cov_get_files(Cov_library* lib);
void Cov_read_file(Cov_file* file);
void Cov_print_match(struct buffer_list* groups);
void Cov_app_print(Cov_app* app);
void Cov_agg_files(Cov_library* lib);
void Cov_agg_libraries(Cov_app* app);
void Cov_print_app_results(Cov_app* app);
void Cov_print_library_results(Cov_library* lib);
void Cov_print_file_results(Cov_file* file);

int main(int argc, char** argv)
{
    char usage[] = "coverage <input-dir>";
    if (argc != 2) {
        fprintf(stderr, "Usage: %s\n", usage);
        return 1;
    }
    char* dir_name = argv[1];

    Cov_app app;
    Cov_app_init(&app);
    buffer_copy_str(&app.data_path, dir_name);
    buffer_finish(&app.data_path);

    Cov_get_libraries(dir_name, &app);

    Cov_app_destroy(&app);

    return 0;
}

void Cov_cwd()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }
}

void Cov_append_path(struct buffer* bf, char* path)
{
    buffer_add_char(bf, '/');
    buffer_add_str(bf, path);
}

void Cov_get_libraries(char* dir_name, Cov_app *app)
{
    DIR* d;
    struct dirent* dir;
    d = opendir(dir_name);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                struct buffer bf;
                buffer_init(&bf);
                buffer_add_str(&bf, dir_name);
                Cov_append_path(&bf, dir->d_name);
                buffer_finish(&bf);
                struct stat sb;
                if (stat(bf.buf, &sb) == 0 && S_ISDIR(sb.st_mode)) {
                    Cov_library* lib = NULL;
                    Cov_library_create(&lib);
                    buffer_copy(&bf, &lib->path);
                    buffer_add_str(&lib->name, dir->d_name);
                    buffer_finish(&lib->path);
                    buffer_finish(&lib->name);
                    Cov_library_list_add_sorted(&app->libraries, lib);
                    Cov_get_files(lib);
                }

                buffer_destroy(&bf);
            }
        }
        closedir(d);
    } else {
        perror("opendir() error");
    }

    Cov_agg_libraries(app);
    Cov_print_app_results(app);
}

void Cov_get_files(Cov_library* lib)
{
    DIR* d;
    struct dirent* dir;
    d = opendir(lib->path.buf);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                struct buffer bf;
                buffer_init(&bf);
                buffer_copy(&lib->path, &bf);
                Cov_append_path(&bf, dir->d_name);
                buffer_finish(&bf);
                struct stat sb;
                if (stat(bf.buf, &sb) == 0 && S_ISREG(sb.st_mode)) {
                    Cov_file* file = NULL;
                    Cov_file_create(&file);
                    buffer_copy(&bf, &file->path);
                    buffer_copy_str(&file->name, dir->d_name);
                    buffer_finish(&file->path);
                    buffer_finish(&file->name);
                    Cov_file_list_add_sorted(&lib->files, file);
                    Cov_read_file(file);
                    Cov_print_file_results(file);
                }
            }
        }
    }

    Cov_agg_files(lib);
    Cov_print_library_results(lib);
}

void Cov_agg_files(Cov_library* lib)
{
    Cov_file* file = lib->files.head;
    while (file) {
        lib->line_count += file->line_count;
        lib->covered_count += file->covered_count;
        lib->not_covered_count += file->not_covered_count;
        file = file->next;
    }
    lib->coverage_percentage = (double)lib->covered_count / (double)lib->line_count * 100.0;
}

void Cov_agg_libraries(Cov_app* app)
{
    Cov_library* lib = app->libraries.head;
    while (lib) {
        app->line_count += lib->line_count;
        app->covered_count += lib->covered_count;
        app->not_covered_count += lib->not_covered_count;
        lib = lib->next;
    }
    app->coverage_percentage = (double)app->covered_count / (double)app->line_count * 100.0;
}

void Cov_read_file(Cov_file* file)
{
    FILE* fp = fopen(file->path.buf, "r");
    if (!fp) {
        perror("fopen() error");
        return;
    }

    Vector* text = NULL;
    VectorCreate(&text, 1);
    char* s = "\\s*(\\-|#####|\\d+)\\*?:\\s*(\\d+):(Source:)?(.*)";
    size_t len = strlen(s);
    VectorAdd(text, s, len);

    InputUnicodeString* input = NULL;
    InputUnicodeStringCreate(&input, text);

    struct error_list* el = NULL;
    error_list_create(&el);

    Compile_data* cd = NULL;
    compile_data_create(&cd, input, input->input_vtable, el);

    Ast_node* root = NULL;
    bool valid = compile(cd, &root);
    if (!valid) {
        printf("compile() error:\n");
        struct error* e = el->head;
        while (e) {
            buffer_finish(&e->message);
            printf("%s\n", e->message.buf);
            e = e->next;
        }

        VectorDestroy(text);
        free(text);
        free(input);
        error_list_destroy(el);
        free(el);
        compile_data_destroy(cd);
        free(cd);
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
            bool m = re_match(root, slice, &groups);
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

    VectorDestroy(text);
    free(text);
    free(input);
    error_list_destroy(el);
    free(el);
    compile_data_destroy(cd);
    free(cd);

    if (file->line_count == 0) {
        file->coverage_percentage = 100.0;
    } else {
        file->coverage_percentage =
            (double)file->covered_count / (double)file->line_count * 100.0;
    }
}

void Cov_print_match(struct buffer_list* groups)
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

void Cov_print_app_results(Cov_app* app)
{
    printf("App total\n");
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

void Cov_print_library_results(Cov_library* lib)
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

void Cov_print_file_results(Cov_file* file)
{
    printf("\tfile: %s\n", file->source_path.buf);
    printf("\tline count: %zu\n", file->line_count);
    printf("\tcovered count: %zu\n", file->covered_count);
    printf("\tnot covered count: %zu\n", file->not_covered_count);
    printf("\tcoverage (%%): %lf\n", file->coverage_percentage);
    printf("\n");
}