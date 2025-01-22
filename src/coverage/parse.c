#include "cobble/compile.h"
#include "cobble/compile_data.h"
#include "cobble/compile_tools.h"
#include <string.h>
#include "zinc/input_unicode_string.h"

Cob_re Cover_gcov_line_re()
{
    char* s = "\\s*(\\-|#####|\\d+)\\*?:\\s*(\\d+):(Source:)?(.*)";
    return Cob_compile_str(s);
}

Cob_re Cover_gcov_ext_re()
{
    char* s = "\\.gcov$";
    return Cob_compile_str(s);
}

Cob_re Cover_test_dir_re()
{
    char* s = "\\-(unit|test)$";
    return Cob_compile_str(s);
}

void Cover_re_cleanup(Cob_re* re)
{
    Zinc_error_list_destroy(re->errors);
    free(re->errors);
    Cob_ast_destroy(re->root);
    free(re->root);
}