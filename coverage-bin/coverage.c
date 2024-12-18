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
                Cov_library* lib = NULL;
                Cov_library_create(&lib);

                buffer_add_str(&lib->path, dir_name);
                Cov_append_path(&lib->path, dir->d_name);
                buffer_add_str(&lib->name, dir->d_name);
                buffer_finish(&lib->path);
                buffer_finish(&lib->name);

                Cov_library_list_add(libraries, lib);
            }
        }
        closedir(d);
    } else {
        perror("opendir() error");
    }
}

void Cov_get_files(Cov_library* lib)
{

}

void Cov_app_print(Cov_app* app)
{
    Cov_library* lib = app->libraries.head;
    while (lib) {
        printf("%s\n", lib->name.buf);
        lib = lib->next;
    }
}

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