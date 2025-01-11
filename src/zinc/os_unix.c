#ifdef __unix__

#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "result.h"
#include "buffer.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <dirent.h>
#include "buffer_list.h"
#include <fcntl.h>
#include "memory.h"
#include "buffer.h"
#include <assert.h>

enum result get_temp_file(FILE** fp_out, struct buffer* name)
{
    enum result r = result_ok;
    buffer_copy_str(name, "/tmp/zinc-XXXXXX");
    buffer_finish(name);
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

enum result delete_temp_file(struct buffer* name)
{
    if (unlink(name->buf)) {
        return set_error("removal of temp file failed with error [%s]", strerror(errno));
    }
    return result_ok;
}

enum result get_user_home_directory(struct buffer* dir)
{
    const char *homedir;
    if ((homedir = getenv("HOME")) == NULL) {
        return set_error("Could not get home directory");
    }

    buffer_copy_str(dir, homedir);
    return result_ok;
}

void path_join(struct buffer* src1, struct buffer* src2, struct buffer* dest)
{
    buffer_copy(src1, dest);
    buffer_add_char(dest, '/');
    buffer_copy(src2, dest);
}

enum result get_user_app_directory(struct buffer* lower_name, struct buffer* dir)
{
    struct buffer home;
    buffer_init(&home);
    enum result r = get_user_home_directory(&home);
    if (r == result_error) {
        return r;
    }

    struct buffer app;
    buffer_init(&app);
    buffer_copy_str(&app, ".app");

    struct buffer temp;
    buffer_init(&temp);

    path_join(&home, &app, &temp);
    path_join(&temp, lower_name, dir);

    buffer_destroy(&home);
    buffer_destroy(&app);
    buffer_destroy(&temp);

    return result_ok;
}

enum result make_directory(struct buffer* dir)
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
            struct buffer subdir;
            buffer_init(&subdir);
            for (int j = 0; j < end; j++) {
                buffer_add_char(&subdir, dir->buf[j]);
            }
            buffer_finish(&subdir);
            DIR* dp = opendir(subdir.buf);
            bool exists;
            if (dp) {
                closedir(dp);
                exists = true;
            } else if (errno == ENOENT){
                exists = false;
            } else {
                exists = true;
                buffer_destroy(&subdir);
                return set_error("Could not check directory [%s]: %s",
                     strerror(errno), subdir.buf);
            }
            if (!exists) {
                if (mkdir(subdir.buf, 0700)) {
                    buffer_destroy(&subdir);
                    return set_error("Could not make directory [%s]: %s",
                         strerror(errno), subdir.buf);
                }
            }
            buffer_destroy(&subdir);
        }
    }

    return result_ok;
}

enum result delete_directory(struct buffer* dir)  /* NOLINT(misc-no-recursion) */
{
    enum result r;
    buffer_finish(dir);
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

        struct buffer name;
        buffer_init(&name);
        buffer_copy_str(&name, de->d_name);

        struct buffer path;
        buffer_init(&path);

        path_join(dir, &name, &path);

        buffer_finish(&path);
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

        buffer_destroy(&name);
        buffer_destroy(&path);
    }

    if (closedir(dp)) {
        return set_error("Error closing directory: [%s]: %s", strerror(errno), dir->buf);
    }

    if (remove(dir->buf)) {
        return set_error("Could not remove directory: [%s]: %s", strerror(errno), dir->buf);
    }

    return result_ok;
}

bool file_exists(struct buffer* filename)
{
    buffer_finish(filename);
    if (access(filename->buf, F_OK) == 0) {
        return true;
    } else {
        return false;
    }
}

enum result get_dir_files(struct buffer* dir, struct buffer_list* bl)   /* NOLINT(misc-no-recursion) */
{
    enum result r = result_ok;

    buffer_finish(dir);
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

        struct buffer name;
        buffer_init(&name);
        buffer_copy_str(&name, de->d_name);

        struct buffer path;
        buffer_init(&path);

        path_join(dir, &name, &path);
        buffer_finish(&path);

        DIR* dp2 = opendir(path.buf);
        if (dp2) {
            if (closedir(dp2)) {
                buffer_destroy(&name);
                buffer_destroy(&path);
                return set_error("Could not check type directory item: [%s]: %s", strerror(errno), path.buf);
            }

            r = get_dir_files(&path, bl);
            if (r == result_error) {
                buffer_destroy(&name);
                buffer_destroy(&path);
                return r;
            }
        } else {
            buffer_list_add_str(bl, path.buf);
        }

        buffer_destroy(&name);
        buffer_destroy(&path);
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

void split_path(struct buffer* path, struct buffer* dir, struct buffer* filename)
{
    int sep_pos = -1;

    for (int i = 0; i < path->size; i++) {
        if (path->buf[i] == '/') {
            sep_pos = i;
        }
    }

    if (sep_pos >= 0) {
        for (int i = 0; i < sep_pos; i++) {
            buffer_add_char(dir, path->buf[i]);
        }
    }
    buffer_finish(dir);

    for (int i = sep_pos + 1; i < path->size; i++) {
        buffer_add_char(filename, path->buf[i]);
    }
    buffer_finish(filename);
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

void Zinc_get_cwd(struct buffer* cwd)
{
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        buffer_copy_str(cwd, buffer);
    } else {
        perror("getcwd() error");
    }
}

#endif