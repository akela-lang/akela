#include "text_interface.h"

void get_text_data_init(struct get_text_data* gtd, void* input_data)
{
    gtd->text.s = NULL;
    gtd->text.start = 0;
    gtd->text.end = 0;
    gtd->input_done = false;
    gtd->input_data = input_data;
}

void get_char_data_init(struct get_char_data* gcd, void* input_data)
{
    gcd->info.utf8 = NULL;
    gcd->info.utf32 = NULL;
    gcd->input_done = false;
    gcd->input_data = input_data;
}