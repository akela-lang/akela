#if defined(unix)

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "zinc/memory.h"
#include "zinc/buffer.h"
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include "os_linux.h"
#include "zinc/result.h"

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

void path_join(struct buffer* path, struct buffer* filename)
{
    buffer_add_char(path, '/');
    buffer_copy(filename, path);
}

errno_t fopen_s(FILE **f, const char *name, const char *mode)
{
    errno_t ret = 0;
    assert(f);
    *f = fopen(name, mode);
    /* Can't be sure about 1-to-1 mapping of errno and MS' errno_t */
    if (!*f)
        ret = errno;
    return ret;
}

#endif
