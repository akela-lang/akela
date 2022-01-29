#ifndef _RESULT_H
#define _RESULT_H

#define ERROR_SIZE 100
enum result_enum {
    ok_result,
    error_result
};

extern char error_message[ERROR_SIZE];

enum result_enum set_error(char* error);

#endif
