#ifndef _TEST_SCAN_SETUP_H
#define _TEST_SCAN_SETUP_H

void AkeUnit_lex_setup(char* line, struct Ake_lex_state* ls, struct Zinc_error_list* el);

void AkeUnit_lex_teardown(struct Ake_lex_state* ls);

#endif
