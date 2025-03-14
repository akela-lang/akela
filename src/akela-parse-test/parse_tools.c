#include "data.h"
#include "parse_tools.h"
#include "cobble/match.h"
#include "cobble/compile.h"
#include "zinc/memory.h"

void Apt_header_init(Apt_header* header)
{
    Zinc_string_init(&header->title);
}

void Apt_header_destroy(Apt_header* header)
{
    Zinc_string_destroy(&header->title);
}

Cob_re Apt_compile_header()
{
    return Cob_compile_str("^(#+) *(.+) *\n?$");
}

bool Apt_get_header(Cob_re* re, Zinc_string_slice* slice, Apt_header* header)
{
    Cob_result mr = Cob_match(re, *slice);
    if (mr.matched) {
        Zinc_string* hashes = Zinc_string_list_get(&mr.groups, 1);
        header->level = (int)hashes->size;
        Zinc_string* title = Zinc_string_list_get(&mr.groups, 2);
        Zinc_string_add_string(&header->title, title);
        return true;
    }

    return false;
}

void Apt_read_line(FILE* fp, Zinc_string* line, bool* done)
{
    int c;
    while ((c = fgetc(fp)) != EOF) {
        Zinc_string_add_char(line, (char)c);
        if (c == '\n') {
            break;
        }
    }

    if (c == EOF) {
        if (line->size == 0) {
            *done = true;
            return;
        }
    }

    *done = false;
}

void Apt_read_all_lines(FILE* fp, Zinc_string_list* list)
{
    bool done = false;
    Zinc_string line;
    Zinc_string_init(&line);
    while (true) {
        Apt_read_line(fp, &line, &done);
        if (done) {
            break;
        }
        Zinc_string_list_add_bf(list, &line);
    }
    Zinc_string_destroy(&line);
}