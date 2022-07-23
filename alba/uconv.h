#ifndef _UCONV_H
#define _UCONV_H

#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>

/* dynamic-output dest */
/* resource-input conv */
/* resource-use conv */
enum result char2uchar(UConverter* conv, char* src, size_t src_size, UChar** dest, size_t dest_size, size_t* len);

/* dynamic-output dest */
/* resource-input conv */
/* resource-use conv */
enum result uchar2char(UConverter* conv, UChar* src, size_t src_size, char** dest, size_t dest_size, size_t* len);

/* dynamic-output-none */
/* resource-output conv */
enum result conv_open(UConverter** conv);

/* dynamic-output-none */
/* resource-destroy conv */
void conv_close(UConverter* conv);

#endif
