#include "zinc/buffer.h"
#include "zinc/memory.h"
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "zinc/input_unicode.h"
#include "zinc/input_unicode_string.h"
#include "zinc/input_unicode_file.h"

void translate(struct Zinc_string* bf)
{
    for (int i = 0; i < bf->size; i++) {
        int c = toupper(bf->buf[i]);
        putchar(c);
    }
}

void event_loop(void* input_obj, Zinc_input_unicode_vtable* input_vtable)
{
    struct Zinc_string* bf = NULL;
    Zinc_string_create(&bf);

    enum result r;
    char c[4];
    int num;
    struct Zinc_location loc;
    bool done;

    while (true) {
        Zinc_input_unicode_next(input_obj, input_vtable, c, &num, &loc, &done);
        if (done) {
            break;
        }

        if (num == 1 && c[0] == '\n') {
            Zinc_string_add_char(bf, '\n');
            translate(bf);
            Zinc_string_clear(bf);
        } else {
            Zinc_string_add(bf, c, num);
        }
    }

    if (bf->size > 0) {
        translate(bf);
    }

    Zinc_string_destroy(bf);
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
    Zinc_input_unicode_vtable* input_vtable;
    Zinc_input_unicode_file* input_file = NULL;
    InputUnicodeString* input_string = NULL;

    if (argc <= 1) {
        Zinc_input_unicode_file_create(&input_file, stdin);
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