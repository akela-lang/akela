#ifndef _UCONV_H
#define _UCONV_H

enum result char2uchar(struct allocator* al, UConverter* conv, char* src, size_t src_size, UChar** dest, size_t dest_size, size_t* len);
enum result uchar2char(struct allocator* al, UConverter* conv, UChar* src, size_t src_size, char** dest, size_t dest_size, size_t* len);
enum result conv_open(UConverter** conv);
void conv_close(UConverter* conv);

#endif
