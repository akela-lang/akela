#ifndef _SETUP_H

extern struct token_list tl;
extern struct string s;
extern struct dag_node* root;

void setup(char* line);
void teardown();

#endif
