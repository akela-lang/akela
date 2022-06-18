#ifndef _TEST_PARSE_H
#define _TEST_PARSE_H

void parse_setup(struct allocator* al, char* line, struct token_state* ts, struct dag_node** root);
void parse_teardown(struct allocator* al, struct token_state* ts);

#endif
