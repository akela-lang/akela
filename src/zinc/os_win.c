#if defined(_WIN32) || defined(WIN32)

#include <windows.h>

#include "result.h"
#include "memory.h"
#include "os_win.h"
#include "zstring.h"

enum result Zinc_win_temp_filename(char** buff)
{
	DWORD dwRetVal = 0;
	TCHAR temp[MAX_PATH];
	dwRetVal = GetTempPath(MAX_PATH, temp);
	if (dwRetVal <= 0 || dwRetVal > MAX_PATH) {
		return set_error("could not get temp directory");
	}

	UINT uRetVal = 0;
	TCHAR szTempFileName[MAX_PATH];
	uRetVal = GetTempFileName(temp, // directory for tmp files
		TEXT("ALBA"),     // temp file name prefix
		0,                // create unique name
		szTempFileName);  // buffer for name

	if (!uRetVal) {
		return set_error("could not get temp path");
	}

	malloc_safe((void**)buff, MAX_PATH);

	strncpy_s(*buff, MAX_PATH, szTempFileName, MAX_PATH);

	return result_ok;
}

void Zinc_set_console_utf8()
{
	SetConsoleOutputCP(CP_UTF8);
}

char* Zinc_get_exe_path()
{
    DWORD last_error;
    DWORD result;
    DWORD path_size = 1024;
    char* path = NULL;
    malloc_safe((void**)&path, path_size);

    for (;;)
    {
        #pragma warning(suppress:6387)
        memset(path, 0, path_size);
        #pragma warning(suppress:6387)
        result = GetModuleFileName(0, path, path_size - 1);
        last_error = GetLastError();

        if (0 == result) {
            free(path);
            path = 0;
            break;
        } else if (result == path_size - 1) {
            free(path);
            /* May need to also check for ERROR_SUCCESS here if XP/2K */
            if (ERROR_INSUFFICIENT_BUFFER != last_error)
            {
                path = 0;
                break;
            }
            path_size = path_size * 2;
            malloc_safe((void**)&path, path_size);
        } else {
            break;
        }
    }

    return path;
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
            buffer_add_char(dir, path->buf[i]);
        }
    }
    buffer_finish(dir);

    for (int i = sep_pos + 1; i < path->size; i++) {
        buffer_add_char(filename, path->buf[i]);
    }
    buffer_finish(filename);
}

void path_join(Zinc_string* path, Zinc_string* filename)
{
    buffer_add_char(path, '\\');
    buffer_copy(filename, path);
}

#endif
