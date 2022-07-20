#ifndef _OS_WIN_H
#define _OS_WIN_H

#if defined(_WIN32) || defined(WIN32)

/* dynamic-output buff */
enum result win_temp_filename(char** buff);

/* static-output */
void set_console_utf8();

#endif


#endif
