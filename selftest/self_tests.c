#include <types.h>
#include <strings.h>
#include <arch/early_print.h>
#include "self_tests.h"
#include "mm/pagestrap_tests.h"
#include "lib/vector_tests.h"

bool st_run_self_tests_early() {
  if (st_run_pagestrap_tests()) return true;

  return false;
}

bool st_run_self_tests_late() {
  if (st_run_vector_tests()) return true;

  return false;
}

static bool st_show_pass(string_t pass_message) {
  if (strlen(pass_message) == 0) {
    return false;
  }

  early_print_set_color(EP_GREEN, EP_BLACK);
  early_print(S("ST: "));
  early_println(pass_message);
  early_print_reset_color();

  return false;
}

bool st_assert_equals_vptr(vptr expected, vptr actual, string_t failMessage, string_t pass_message) {
  if (expected != actual) {
    early_print_set_color(EP_RED, EP_BLACK);
    early_print(S("ST: "));
    early_print(failMessage);
    early_print(S(" (Expected: "));
    early_print_addr(expected);
    early_print(S(", Actual: "));
    early_print_addr(actual);
    early_println(S(")"));
    early_print_reset_color();
    return true;
  }

  return st_show_pass(pass_message);
}

bool st_assert_equals_uX(uX expected, uX actual, string_t failMessage, string_t pass_message) {
  if (expected != actual) {
    early_print_set_color(EP_RED, EP_BLACK);
    early_print(S("ST: "));
    early_print(failMessage);
    early_print(S(" (Expected: "));
    early_print_uX(expected);
    early_print(S(", Actual: "));
    early_print_uX(actual);
    early_println(S(")"));
    early_print_reset_color();
    return true;
  }

  return st_show_pass(pass_message);
}