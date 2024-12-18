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

void Coverage_cwd()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }
}

void Coverage_append_path(struct buffer* bf, char* path)
{
    buffer_add_char(bf, '/');
    buffer_add_str(bf, path);
}

void Coverage_get_libraries(char* dir_name, Coverage_library_list* libraries)
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
                Coverage_append_path(&bf, dir->d_name);
                buffer_finish(&bf);
                printf("%s\n", bf.buf);
            }
        }
        closedir(d);
    } else {
        perror("opendir() error");
    }
}

void Coverage_get_files(Coverage_library* lib)
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

    Coverage_app app;
    Coverage_get_libraries(dir_name, &app.libraries);

    return 0;
}