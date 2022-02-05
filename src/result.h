#ifndef _RESULT_H
#define _RESULT_H

#define ERROR_SIZE 100
enum result {
    result_ok,
    result_error
};

extern char error_message[ERROR_SIZE];

enum result set_error(const char* fmt, ...);

#endif
