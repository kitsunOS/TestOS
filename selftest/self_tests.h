#ifndef __I386_SELF_TESTS_H
#define __I386_SELF_TESTS_H

#include <types.h>

bool st_run_self_tests_early();

bool st_run_self_tests_late();

bool st_assert_equals_vptr(vptr expected, vptr actual, string_t failMessage, string_t pass_message);

bool st_assert_equals_uX(uX expected, uX actual, string_t failMessage, string_t pass_message);

#endif