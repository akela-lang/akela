#ifndef _TEST_SCAN_SETUP_H
#define _TEST_SCAN_SETUP_H

void lex_setup(char* line, struct lex_state* ls, struct lookahead_char* lc, struct error_list* el);

void lex_teardown(struct lex_state* ls);

#endif
