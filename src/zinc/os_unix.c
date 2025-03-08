#include "os.h"

#if IS_UNIX
#include "fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "result.h"
#include "zstring.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <dirent.h>
#include "string_list.h"
#include <fcntl.h>

enum Zinc_result Zinc_get_user_home_directory(Zinc_string* dir)
{
    const char *homedir;
    if ((homedir = getenv("HOME")) == NULL) {
        return Zinc_set_error("Could not get home directory");
    }

    Zinc_string_add_str(dir, homedir);
    return Zinc_result_ok;
}

enum Zinc_result Zinc_get_user_app_directory(Zinc_string* lower_name, Zinc_string* dir)
{
    Zinc_string home;
    Zinc_string_init(&home);
    enum Zinc_result r = Zinc_get_user_home_directory(&home);
    if (r == Zinc_result_error) {
        return r;
    }

    Zinc_string app;
    Zinc_string_init(&app);
    Zinc_string_add_str(&app, ".app");

    Zinc_string temp;
    Zinc_string_init(&temp);

    Zinc_path_join(&home, &app, &temp);
    Zinc_path_join(&temp, lower_name, dir);

    Zinc_string_destroy(&home);
    Zinc_string_destroy(&app);
    Zinc_string_destroy(&temp);

    return Zinc_result_ok;
}

Zinc_result Zinc_make_directory(Zinc_string* dir)
{
    for (int i = 0; i < dir->size; i++) {
        bool is_subdir = false;
        int end;
        if (i > 0 && dir->buf[i] == '/') {
            is_subdir = true;
            end = i;
        } else if (dir->buf[i] != '/' && i == dir->size - 1) {
            is_subdir = true;
            end = i + 1;
        }
        if (is_subdir) {
            Zinc_string subdir;
            Zinc_string_init(&subdir);
            for (int j = 0; j < end; j++) {
                Zinc_string_add_char(&subdir, dir->buf[j]);
            }
            Zinc_string_finish(&subdir);
            DIR* dp = opendir(subdir.buf);
            bool exists;
            if (dp) {
                closedir(dp);
                exists = true;
            } else if (errno == ENOENT){
                exists = false;
            } else {
                exists = true;
                Zinc_string_destroy(&subdir);
                return Zinc_set_error("Could not check directory [%s]: %s",
                     strerror(errno), subdir.buf);
            }
            if (!exists) {
                if (mkdir(subdir.buf, 0700)) {
                    Zinc_string_destroy(&subdir);
                    return Zinc_set_error("Could not make directory [%s]: %s",
                         strerror(errno), subdir.buf);
                }
            }
            Zinc_string_destroy(&subdir);
        }
    }

    return Zinc_result_ok;
}

enum Zinc_result Zinc_delete_directory(Zinc_string* dir)  /* NOLINT(misc-no-recursion) */
{
    enum Zinc_result r;
    Zinc_string_finish(dir);
    DIR* dp = opendir(dir->buf);
    if (!dp) {
        return Zinc_set_error("Could not open directory: [%s]: %s", strerror(errno), dir->buf);
    }

    struct dirent *de = NULL;
    while ((de = readdir(dp)) != NULL) {
        if (strcmp(de->d_name, "..") == 0) {
            continue;
        }
        if (strcmp(de->d_name, ".") == 0) {
            continue;
        }

        struct Zinc_string name;
        Zinc_string_init(&name);
        Zinc_string_add_str(&name, de->d_name);

        struct Zinc_string path;
        Zinc_string_init(&path);

        Zinc_path_join(dir, &name, &path);

        Zinc_string_finish(&path);
        DIR* dp2 = opendir(path.buf);
        if (dp2) {
            if (closedir(dp2)) {
                return Zinc_set_error("Could not check type directory item: [%s]: %s", strerror(errno), path.buf);
            }

            r = Zinc_delete_directory(&path);
            if (r == Zinc_result_error) {
                return r;
            }
        } else {
            if (remove(path.buf)) {
                return Zinc_set_error("Could not remove file: [%s]: %s", strerror(errno), path.buf);
            }
        }

        Zinc_string_destroy(&name);
        Zinc_string_destroy(&path);
    }

    if (closedir(dp)) {
        return Zinc_set_error("Error closing directory: [%s]: %s", strerror(errno), dir->buf);
    }

    if (remove(dir->buf)) {
        return Zinc_set_error("Could not remove directory: [%s]: %s", strerror(errno), dir->buf);
    }

    return Zinc_result_ok;
}

Zinc_result Zinc_get_dir_files(Zinc_string* dir, Zinc_string_list* bl)   /* NOLINT(misc-no-recursion) */
{
    Zinc_result r;

    Zinc_string_finish(dir);
    DIR* dp = opendir(dir->buf);
    if (!dp) {
        return Zinc_set_error("Could not open directory: [%s]: %s", strerror(errno), dir->buf);
    }

    struct dirent *de = NULL;
    while ((de = readdir(dp)) != NULL) {
        if (strcmp(de->d_name, "..") == 0) {
            continue;
        }
        if (strcmp(de->d_name, ".") == 0) {
            continue;
        }

        Zinc_string name;
        Zinc_string_init(&name);
        Zinc_string_add_str(&name, de->d_name);

        Zinc_string path;
        Zinc_string_init(&path);

        Zinc_path_join(dir, &name, &path);
        Zinc_string_finish(&path);

        DIR* dp2 = opendir(path.buf);
        if (dp2) {
            if (closedir(dp2)) {
                Zinc_string_destroy(&name);
                Zinc_string_destroy(&path);
                return Zinc_set_error("Could not check type directory item: [%s]: %s", strerror(errno), path.buf);
            }

            r = Zinc_get_dir_files(&path, bl);
            if (r == Zinc_result_error) {
                Zinc_string_destroy(&name);
                Zinc_string_destroy(&path);
                return r;
            }
        } else {
            Zinc_string_list_add_str(bl, path.buf);
        }

        Zinc_string_destroy(&name);
        Zinc_string_destroy(&path);
    }

    if (closedir(dp)) {
        return Zinc_set_error("Error closing directory: [%s]: %s", strerror(errno), dir->buf);
    }

    return Zinc_result_ok;
}

void Zinc_split_path(Zinc_string* path, Zinc_string* dir, Zinc_string* filename)
{
    int sep_pos = -1;

    for (int i = 0; i < path->size; i++) {
        if (path->buf[i] == '/') {
            sep_pos = i;
        }
    }

    if (sep_pos >= 0) {
        for (int i = 0; i < sep_pos; i++) {
            Zinc_string_add_char(dir, path->buf[i]);
        }
    }
    Zinc_string_finish(dir);

    for (int i = sep_pos + 1; i < path->size; i++) {
        Zinc_string_add_char(filename, path->buf[i]);
    }
    Zinc_string_finish(filename);
}

Zinc_result Zinc_is_reg_file(Zinc_string* path)
{
    Zinc_string_finish(path);
    struct stat sb;
    if (stat(path->buf, &sb) == -1) {
        return Zinc_set_error("Could not stat file: [%s]: %s", strerror(errno), path->buf);
    }

    if (!S_ISREG(sb.st_mode)) {
        return Zinc_set_error("Not a regular file: %s", path->buf);
    }

    return Zinc_result_ok;
}
#endif