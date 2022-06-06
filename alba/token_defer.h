#ifndef _TOKEN_DEFER_H
#define _TOKEN_DEFER_H

enum result token_list_make_defer(struct token_list** tl);
enum result token_list_add_defer(struct token_list* tl, struct token* t);
enum result token_list_slice_defer(struct token_list* tl, int start, int end, struct token_list** slice);
void token_list_reset_defer(struct token_list* tl);
void token_list_destroy_defer(struct token_list* tl);
enum result token_list_print_defer(struct token_list* tl, char** token_name);
struct token* token_list_pop_defer(struct token_list* tl);
void token_destroy_defer(struct token* t);

#endif
