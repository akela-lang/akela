#ifndef _TEST_SCAN_SETUP_H
#define _TEST_SCAN_SETUP_H

void scan_setup(char* line, struct scan_state* sns, struct lookahead_char* lc, struct compile_error_list* el);

void scan_teardown(struct scan_state* sns);

#endif
