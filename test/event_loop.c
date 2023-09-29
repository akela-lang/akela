#include "zinc/buffer.h"
#include "zinc/memory.h"
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "akela/text_interface.h"

void get_text_stdin(struct get_text_data* gtd)
{
    if (gtd->input_done) {
        return;
    }

    struct buffer* bf = gtd->input_data;

    int c;
    while (true) {
        c = getchar();
        if (c == EOF) {
            gtd->input_done = true;
            break;
        }

        buffer_add_char(bf, (char)c);
        if (c == '\n') break;
    }

    if (!gtd->text.s) {
        gtd->text.s = bf->buf;
    }
    gtd->text.end = bf->size;
}

void get_text_buffer(struct get_text_data* gtd)
{
    if (gtd->input_done) {
        return;
    }

    struct buffer* src = gtd->input_data;
    gtd->text.s = src->buf;
    gtd->text.end = src->size;
    gtd->input_done = true;
}

void translate(struct get_text_data* gtd)
{
    while (gtd->text.start < gtd->text.end) {
        int c = toupper(gtd->text.s[gtd->text.start++]);
        putchar(c);
    }
}

void event_loop(get_text_interface gti, void* input_data)
{
    struct get_text_data gtd;
    get_text_data_init(&gtd, input_data);
    while (!gtd.input_done || gtd.text.start < gtd.text.end) {
        if (gtd.text.start < gtd.text.end) {
            translate(&gtd);
        } else if (!gtd.input_done) {
            gti(&gtd);
        }
    }
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
    if (argc <= 1) {
        struct buffer bf;
        buffer_init(&bf);
        event_loop(get_text_stdin, &bf);
        buffer_destroy(&bf);
        return 0;
    } else {
        if (strcmp(argv[1], "string") == 0) {
            struct buffer bf;
            buffer_init(&bf);
            for (int i = 2; i < argc; i++) {
                array2buffer(argv[i], &bf);
                buffer_add_char(&bf, '\n');
            }
            event_loop(get_text_buffer, &bf);
            buffer_destroy(&bf);
            return 0;
        }
    }
    usage();
    return 1;
}