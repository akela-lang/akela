#ifdef __unix__

#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "result.h"
#include "zstring.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <dirent.h>
#include "buffer_list.h"
#include <fcntl.h>
#include "memory.h"
#include <assert.h>

enum result get_temp_file(FILE** fp_out, struct Zinc_string* name)
{
    enum result r = result_ok;
    Zinc_string_add_str(name, "/tmp/zinc-XXXXXX");
    Zinc_string_finish(name);
    errno = 0;
    int fd = mkstemp(name->buf);
    if (fd < 1) {
        return set_error("creation of temp file failed with error [%s]", strerror(errno));
    }
    FILE* fp = fdopen(fd, "w");
    if (!fp) {
        return set_error("creation of temp file failed with error [%s]", strerror(errno));
    }
    *fp_out = fp;

    return r;
}

enum result close_temp_file(FILE* fp)
{
    if (fclose(fp)) {
        return set_error("close of temp file failed with error [%s]", strerror(errno));
    }
    return result_ok;
}

enum result delete_temp_file(struct Zinc_string* name)
{
    if (unlink(name->buf)) {
        return set_error("removal of temp file failed with error [%s]", strerror(errno));
    }
    return result_ok;
}

enum result get_user_home_directory(struct Zinc_string* dir)
{
    const char *homedir;
    if ((homedir = getenv("HOME")) == NULL) {
        return set_error("Could not get home directory");
    }

    Zinc_string_add_str(dir, homedir);
    return result_ok;
}

void path_join(struct Zinc_string* src1, struct Zinc_string* src2, struct Zinc_string* dest)
{
    Zinc_string_copy(src1, dest);
    Zinc_string_add_char(dest, '/');
    Zinc_string_copy(src2, dest);
}

enum result get_user_app_directory(struct Zinc_string* lower_name, struct Zinc_string* dir)
{
    struct Zinc_string home;
    Zinc_string_init(&home);
    enum result r = get_user_home_directory(&home);
    if (r == result_error) {
        return r;
    }

    struct Zinc_string app;
    Zinc_string_init(&app);
    Zinc_string_add_str(&app, ".app");

    struct Zinc_string temp;
    Zinc_string_init(&temp);

    path_join(&home, &app, &temp);
    path_join(&temp, lower_name, dir);

    Zinc_string_destroy(&home);
    Zinc_string_destroy(&app);
    Zinc_string_destroy(&temp);

    return result_ok;
}

enum result make_directory(struct Zinc_string* dir)
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
            struct Zinc_string subdir;
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
                return set_error("Could not check directory [%s]: %s",
                     strerror(errno), subdir.buf);
            }
            if (!exists) {
                if (mkdir(subdir.buf, 0700)) {
                    Zinc_string_destroy(&subdir);
                    return set_error("Could not make directory [%s]: %s",
                         strerror(errno), subdir.buf);
                }
            }
            Zinc_string_destroy(&subdir);
        }
    }

    return result_ok;
}

enum result delete_directory(struct Zinc_string* dir)  /* NOLINT(misc-no-recursion) */
{
    enum result r;
    Zinc_string_finish(dir);
    DIR* dp = opendir(dir->buf);
    if (!dp) {
        return set_error("Could not open directory: [%s]: %s", strerror(errno), dir->buf);
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

        path_join(dir, &name, &path);

        Zinc_string_finish(&path);
        DIR* dp2 = opendir(path.buf);
        if (dp2) {
            if (closedir(dp2)) {
                return set_error("Could not check type directory item: [%s]: %s", strerror(errno), path.buf);
            }

            r = delete_directory(&path);
            if (r == result_error) {
                return r;
            }
        } else {
            if (remove(path.buf)) {
                return set_error("Could not remove file: [%s]: %s", strerror(errno), path.buf);
            }
        }

        Zinc_string_destroy(&name);
        Zinc_string_destroy(&path);
    }

    if (closedir(dp)) {
        return set_error("Error closing directory: [%s]: %s", strerror(errno), dir->buf);
    }

    if (remove(dir->buf)) {
        return set_error("Could not remove directory: [%s]: %s", strerror(errno), dir->buf);
    }

    return result_ok;
}

bool file_exists(struct Zinc_string* filename)
{
    Zinc_string_finish(filename);
    if (access(filename->buf, F_OK) == 0) {
        return true;
    } else {
        return false;
    }
}

enum result get_dir_files(struct Zinc_string* dir, struct Zinc_string_list* bl)   /* NOLINT(misc-no-recursion) */
{
    enum result r = result_ok;

    Zinc_string_finish(dir);
    DIR* dp = opendir(dir->buf);
    if (!dp) {
        return set_error("Could not open directory: [%s]: %s", strerror(errno), dir->buf);
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

        path_join(dir, &name, &path);
        Zinc_string_finish(&path);

        DIR* dp2 = opendir(path.buf);
        if (dp2) {
            if (closedir(dp2)) {
                Zinc_string_destroy(&name);
                Zinc_string_destroy(&path);
                return set_error("Could not check type directory item: [%s]: %s", strerror(errno), path.buf);
            }

            r = get_dir_files(&path, bl);
            if (r == result_error) {
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
        return set_error("Error closing directory: [%s]: %s", strerror(errno), dir->buf);
    }

    return result_ok;
}

enum result get_exe_path(char** path)
{
    enum result r = result_ok;
    int buf_size = 1024;

    malloc_safe((void**)path, buf_size+1);

    size_t size = readlink("/proc/self/exe", *path, buf_size);
    if (size == -1) {
        r = set_error("path not read");
    } else {
        (*path)[size] = '\0';
    }

    return r;
}

void split_path(struct Zinc_string* path, struct Zinc_string* dir, struct Zinc_string* filename)
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

int fopen_s(FILE **f, const char *name, const char *mode)
{
    int ret = 0;
    assert(f);
    *f = fopen(name, mode);
    /* Can't be sure about 1-to-1 mapping of errno and MS' errno_t */
    if (!*f)
        ret = errno;
    return ret;
}

void Zinc_get_cwd(struct Zinc_string* cwd)
{
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        Zinc_string_add_str(cwd, buffer);
    } else {
        perror("getcwd() error");
    }
}

#endif