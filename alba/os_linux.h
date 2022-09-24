#ifndef _OS_LINUX_H
#define _OS_LINUX_H

#if defined(unix)

#include <stdio.h>

typedef int errno_t;

char* get_exe_path();
void split_path(struct buffer* path, struct buffer* dir, struct buffer* filename);
void path_join(struct buffer* path, struct buffer* filename);
errno_t fopen_s(FILE **f, const char *name, const char *mode);

#endif

#endif
