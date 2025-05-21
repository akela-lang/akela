#if defined(_WIN32) || defined(WIN32)

#include <windows.h>

#include "result.h"
#include "memory.h"
#include "os_win.h"
#include "zstring.h"
#include <locale.h>
#include "string_list.h"

void Zinc_set_console_utf8()
{
	SetConsoleOutputCP(CP_UTF8);
}

void Zinc_split_path(Zinc_string* path, Zinc_string* dir, Zinc_string* filename)
{
    int sep_pos = -1;

    for (int i = 0; i < path->size; i++) {
        if (path->buf[i] == '\\') {
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

#endif
