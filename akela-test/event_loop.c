#include "zinc/buffer.h"
#include "zinc/memory.h"
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "zinc/input_unicode.h"
#include "zinc/input_unicode_string.h"
#include "zinc/input_unicode_file.h"

void translate(struct buffer* bf)
{
    for (int i = 0; i < bf->size; i++) {
        int c = toupper(bf->buf[i]);
        putchar(c);
    }
}

void event_loop(void* input_obj, InputUnicodeVTable* input_vtable)
{
    struct buffer* bf = NULL;
    buffer_create(&bf);

    enum result r;
    char c[4];
    int num;
    struct location loc;
    bool done;

    while (true) {
        InputUnicodeNext(input_obj, input_vtable, c, &num, &loc, &done);
        if (done) {
            break;
        }

        if (num == 1 && c[0] == '\n') {
            buffer_add_char(bf, '\n');
            translate(bf);
            buffer_clear(bf);
        } else {
            buffer_add(bf, c, num);
        }
    }

    if (bf->size > 0) {
        translate(bf);
    }

    buffer_destroy(bf);
    free(bf);
}

void usage()
{
    printf("event_loop [command] [argument]\n"
           "\tblank command uses stdin as input\n"
        "\tcommands:\n"
        "\t\tstring [string]... - uses a strings as input\n"
    );
}

int main(int argc, char** argv)
{
    void* input_obj = NULL;
    InputUnicodeVTable* input_vtable;
    InputUnicodeFile* input_file = NULL;
    InputUnicodeString* input_string = NULL;

    if (argc <= 1) {
        InputUnicodeFileCreate(&input_file, stdin);
        input_obj = input_file;
        input_vtable = input_file->input_vtable;
    } else if (argc >= 3 && strcmp(argv[1], "string") == 0) {
        Vector* text = NULL;
        VectorCreate(&text, sizeof(char));
        for (int i = 2; i < argc; i++) {
            char* str = argv[i];
            size_t len = strlen(str);
            if (i >= 3) {
                VectorAdd(text, " ", 1);
            }
            VectorAdd(text, str, len);
        }
        VectorAdd(text, "\n", 1);
        InputUnicodeStringCreate(&input_string, text);
        input_obj = input_string;
        input_vtable = input_string->input_vtable;
    } else {
        usage();
        return 1;
    }

    event_loop(input_obj, input_vtable);

    if (input_string) {
        VectorDestroy(input_string->text);
        free(input_string->text);
    }

    return 0;
}