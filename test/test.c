#include "assert.h"
#include "test_scan.h"
#include "test_parse.h"

int main(void) {
	test_scan();
	test_parse();
	print_results();
	return 0;
}
