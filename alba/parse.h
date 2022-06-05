#ifndef _PARSE_H
#define _PARSE_H

enum result parse_rr(struct token_list* tl, struct dag_node** root);
enum result parse_defer(struct token_list* tl, struct dag_node** root);
enum result parse_al(struct allocator* al, struct token_list* tl, struct dag_node** root);

#endif
