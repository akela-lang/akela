#include "os.h"
#include <assert.h>
#include <stdbool.h>
#include "result.h"
#include "zstring.h"

#if IS_UNIX
    #include <sys/stat.h>
    #include <unistd.h>
#elif IS_WIN
    #include "windows.h"
    #include <io.h>
#endif

void Zinc_path_join(Zinc_string* src1, Zinc_string* src2, Zinc_string* dest)
{
    Zinc_string_copy(src1, dest);
#if IS_UNIX
    Zinc_string_add_char(dest, '/');
#elif IS_WIN
    Zinc_string_add_char(dest, '\\');
#else
    assert(false && "unknown OS");
#endif
    Zinc_string_copy(src2, dest);
}

bool Zinc_file_exists(const char* filename)
{
#if IS_UNIX
    Zinc_string_finish(filename);
    if (access(filename->buf, F_OK) == 0) {
        return true;
    } else {
        return false;
    }
#elif IS_WIN
    if (_access(filename, 0) == 0) {
        return true;
    } else {
        return false;
    }
#else
    assert(false && "unknown OS");
#endif
}

Zinc_result Zinc_is_directory(const char* path, bool* is_dir)
{
#if IS_UNIX
    struct stat sb;
    if (stat(path, &sb) == -1) {
        return Zinc_set_error("Could not stat file: [%s]: %s", strerror(errno), path);
    }

    is_dir* = S_ISDIR(sb.st_mode)
    return Zinc_result_ok;
#elif IS_WIN
    DWORD attributes = GetFileAttributesA(path);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        return Zinc_set_error("invalid file path: %s", path);
    }

    *is_dir = attributes & FILE_ATTRIBUTE_DIRECTORY;
    return Zinc_result_ok;
#else
    assert(false && "unknown OS");
#endif
}

int Zinc_fopen_s(FILE **f, const char *name, const char *mode)
{
    int ret = 0;
    assert(f);
    *f = fopen(name, mode);
    /* Can't be sure about 1-to-1 mapping of errno and MS' errno_t */
    if (!*f)
        ret = errno;
    return ret;
}

void Zinc_path_append(Zinc_string* path, Zinc_string* path2)
{
#if IS_UNIX
    Zinc_string_add_char(path, '/');
#elif IS_WIN
    Zinc_string_add_char(path, '\\');
#else
    assert(false && "unknown OS");
#endif
    Zinc_string_add_string(path, path2);
}

void Zinc_path_append_str(Zinc_string* path, const char* path2)
{
#if IS_UNIX
    Zinc_string_add_char(path, '/');
#elif IS_WIN
    Zinc_string_add_char(path, '\\');
#else
    assert(false && "unknown OS");
#endif
    Zinc_string_add_str(path, path2);
}

Zinc_result Zinc_get_cwd(Zinc_string* cwd)
{
#if IS_UNIX
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        Zinc_string_add_str(cwd, buffer);
        return Zinc_result_ok;
    } else {
        return Zinc_set_error("getcwd: %s\n", strerror());
    }
#elif IS_WIN
    char buffer[MAX_PATH];

    if (GetCurrentDirectory(MAX_PATH, buffer)) {
        Zinc_string_add_str(cwd, buffer);
        return Zinc_result_ok;
    } else {
        return Zinc_set_error("Failed to get current directory. Error: %lu\n", GetLastError());
    }
#else
    assert(false && "unknow OS");
#endif
}

enum Zinc_result Zinc_get_temp_file(FILE** fp_out, Zinc_string* name)
{
#if IS_UNIX
    enum Zinc_result r = Zinc_result_ok;
    Zinc_string_add_str(name, "/tmp/zinc-XXXXXX");
    errno = 0;
    int fd = mkstemp(Zinc_string_c_str(name));
    if (fd < 1) {
        return Zinc_set_error("creation of temp file failed with error [%s]", strerror(errno));
    }
    FILE* fp = fdopen(fd, "w");
    if (!fp) {
        return Zinc_set_error("creation of temp file failed with error [%s]", strerror(errno));
    }
    *fp_out = fp;

    return r;
#elif IS_WIN
	DWORD dwRetVal = 0;
	TCHAR temp[MAX_PATH];
	dwRetVal = GetTempPath(MAX_PATH, temp);
	if (dwRetVal <= 0 || dwRetVal > MAX_PATH) {
		return Zinc_set_error("could not get temp directory");
	}

	UINT uRetVal = 0;
	TCHAR szTempFileName[MAX_PATH];
	uRetVal = GetTempFileName(temp, // directory for tmp files
		TEXT("zinc"),     // temp file name prefix
		0,                // create unique name
		szTempFileName);  // buffer for name

	if (!uRetVal) {
		return Zinc_set_error("could not get temp path");
	}

    Zinc_string_add_str(name, szTempFileName);
	FILE *fp = fopen(Zinc_string_c_str(name), "w");
	if (!fp) {
		return Zinc_set_error("could not open temp file");
	}
	*fp_out = fp;

	return Zinc_result_ok;
#else
    #error "unknown OS"
#endif
}

enum Zinc_result Zinc_close_temp_file(FILE* fp)
{
    if (fclose(fp)) {
        return Zinc_set_error("close of temp file failed with error [%s]", strerror(errno));
    }
    return Zinc_result_ok;
}

enum Zinc_result Zinc_delete_temp_file(Zinc_string* name)
{
    if (unlink(name->buf)) {
        return Zinc_set_error("removal of temp file failed with error [%s]", strerror(errno));
    }
    return Zinc_result_ok;
}

enum Zinc_result Zinc_get_exe_path(char** path)
{
#if IS_UNIX
    enum Zinc_result r = Zinc_result_ok;
    int buf_size = 1024;

    Zinc_malloc_safe((void**)path, buf_size + 1);

    size_t size = readlink("/proc/self/exe", *path, buf_size);
    if (size == -1) {
        r = Zinc_set_error("path not read");
    }
    else {
        (*path)[size] = '\0';
    }

    return r;
#elif IS_WIN
    DWORD last_error;
    DWORD result;
    DWORD path_size = 1024;
    Zinc_malloc_safe((void**)path, path_size);

    for (;;)
    {
#pragma warning(suppress:6387)
        memset(*path, 0, path_size);
#pragma warning(suppress:6387)
        result = GetModuleFileNameA(0, *path, path_size - 1);
        last_error = GetLastError();

        if (0 == result) {
            free(*path);
            *path = NULL;
			return Zinc_set_error("Could not get executable path: %lu", last_error);
        } else if (result == path_size - 1) {
            free(*path);
            /* May need to also check for ERROR_SUCCESS here if XP/2K */
            if (ERROR_INSUFFICIENT_BUFFER != last_error)
            {
                free(*path);
                *path = NULL;
				return Zinc_set_error("Could not get executable path: %lu", last_error);
            }
            path_size = path_size * 2;
            Zinc_malloc_safe((void**)path, path_size);
        } else {
            break;
        }
    }

    return Zinc_result_ok;
#else #error "unknown OS"
#endif
}