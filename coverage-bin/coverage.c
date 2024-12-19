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

void Cov_cwd();
void Cov_append_path(struct buffer* bf, char* path);
void Cov_get_libraries(char* dir_name, Cov_library_list* libraries);
void Cov_get_files(Cov_library* lib);
void Cov_app_print(Cov_app* app);

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

    Cov_get_libraries(dir_name, &app.libraries);
    Cov_app_print(&app);

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

void Cov_get_libraries(char* dir_name, Cov_library_list* libraries)
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
                    Cov_library_list_add_sorted(libraries, lib);
                    Cov_get_files(lib);
                }

                buffer_destroy(&bf);
            }
        }
        closedir(d);
    } else {
        perror("opendir() error");
    }
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
                }
            }
        }
    }
}

void Cov_app_print(Cov_app* app)
{
    Cov_library* lib = app->libraries.head;
    while (lib) {
        printf("%s\n", lib->name.buf);

        Cov_file* file = lib->files.head;
        while (file) {
            printf("\t%s\n", file->name.buf);
            file = file->next;
        }
        lib = lib->next;
    }
}