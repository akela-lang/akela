#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <linux/limits.h>
#include <unistd.h>
#include "zinc/buffer_list.h"
#include "zinc/result.h"
#include "data.h"

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
                printf("%s\n", bf.buf);
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

int main(int argc, char** argv)
{
    char usage[] = "coverage <input-dir>";
    if (argc != 2) {
        fprintf(stderr, "Usage: %s\n", usage);
        return 1;
    }
    char* dir_name = argv[1];

    Cov_app app;
    Cov_get_libraries(dir_name, &app.libraries);

    return 0;
}