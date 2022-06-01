#ifndef _PARSE_H
#define _PARSE_H

enum result old_parse(struct token_list* tl, struct dag_node** root);
enum result parse(struct token_list* tl, struct dag_node** root);

#endif
