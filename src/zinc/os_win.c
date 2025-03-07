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

Zinc_result Zinc_is_reg_file(Zinc_string* path)
{
    char* path_str = Zinc_string_c_str(path);
    DWORD attributes = GetFileAttributesA(path_str);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        return Zinc_set_error("invalid file path: %s", path);
    }
    if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
        return Zinc_set_error("path is a directory: %s", path);
    }

    return Zinc_result_ok;
}

wchar_t* Zinc_char_to_wchar(const char* str) {
    int len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    if (len == 0) return NULL;  // Conversion failed

    wchar_t* wstr = (wchar_t*)malloc(len * sizeof(wchar_t));
    if (!wstr) return NULL;  // Memory allocation failed

    MultiByteToWideChar(CP_ACP, 0, str, -1, wstr, len);
    return wstr;  // Caller must free this memory
}

char* Zinc_wchar_to_char(wchar_t* wstr) {
    setlocale(LC_ALL, "");  // Set locale to support multibyte encoding

#pragma warning(suppress : 4996)
    size_t len = wcstombs(NULL, wstr, 0) + 1;  // Get required buffer size

    if (len == (size_t)-1) {
        perror("wcstombs failed");
        return NULL;
    }

    char* mbstr = malloc(len);
    if (!mbstr) {
        perror("malloc failed");
        return NULL;
    }

#pragma warning(suppress : 4996)
    wcstombs(mbstr, wstr, len);  // Convert wide string to multibyte
    printf("Converted string: %s\n", mbstr);

    // free mbstr after call
    return mbstr;
}

void Zinc_list_files(const char* directory, Zinc_string_list* files) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    char searchPath[MAX_PATH];

    snprintf(searchPath, sizeof(searchPath), "%s\\*", directory);
    printf("Searching: %s\n", searchPath);

    hFind = FindFirstFileA(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Failed to open directory: %s\n", directory);
        printf("Error Code: %lu\n", GetLastError());
        return;
    }

    Zinc_string file;
    Zinc_string_init(&file);
    do {
        char* file_str = Zinc_wchar_to_char(findFileData.cFileName);
        Zinc_string_clear(&file);
        Zinc_string_add_str(&file, file_str);
        free(file_str);
        Zinc_string_list_add(files, &file);
        printf("%ls\n", findFileData.cFileName);
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);
}

void Zinc_list_files2(const wchar_t* directory, Zinc_string_list* files) {
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind;
    wchar_t searchPath[MAX_PATH];

    swprintf(searchPath, MAX_PATH, L"%s\\*", directory);
    wprintf(L"Directory: [%s]\n", directory);
    wprintf(L"Search Path: [%s]\n", searchPath);

    DWORD attributes = GetFileAttributesW(directory);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        wprintf(L"Error: Directory does not exist. GetLastError() = %lu\n", GetLastError());
        return;
    }
    if (!(attributes & FILE_ATTRIBUTE_DIRECTORY)) {
        wprintf(L"Error: Path is not a directory.\n");
        return;
    }

    hFind = FindFirstFileW(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        wprintf(L"Failed to open directory: %s\n", directory);
        wprintf(L"Error Code: %lu\n", GetLastError());
        return;
    }

    Zinc_string file;
    Zinc_string_init(&file);
    do {
        Zinc_string_clear(&file);
        char* file_str = Zinc_wchar_to_char(findFileData.cFileName);
        Zinc_string_add_str(&file, file_str);
        free(file_str);
        Zinc_string_list_add(files, &file);
        wprintf(L"%s\n", findFileData.cFileName);
    } while (FindNextFileW(hFind, &findFileData) != 0);

    FindClose(hFind);
}

#endif
