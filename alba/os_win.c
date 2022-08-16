#if defined(_WIN32) || defined(WIN32)

#include <windows.h>

#include "zinc/result.h"
#include "zinc/memory.h"
#include "os_win.h"

/* dynamic-output buff */
enum result win_temp_filename(char** buff)
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

	/* allocate buff */
	malloc_safe(buff, MAX_PATH);

	strncpy(*buff, szTempFileName, MAX_PATH);

	return result_ok;
}

void set_console_utf8()
{
	SetConsoleOutputCP(CP_UTF8);
}

#endif
