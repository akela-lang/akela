#ifndef APT_COMPARE_ERRORS_H
#define APT_COMPARE_ERRORS_H

bool Apt_check_errors(
    Zinc_test* top_test,
    Zinc_test* case_test,
    Zinc_error_list* errors,
    Cent_value* expected);

#endif