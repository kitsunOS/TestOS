#include "early_print.h"

#include <types.h>
#include "mm/page_alloc.h"

#ifdef DEBUG
#include <selftest/self_tests.h>
#endif

u8 stack_end[4096] __attribute__((aligned(4096))) = {0};

static void reqok(bool condition, string_t ok_str, string_t fail_str);

void higher_half_entry() {
  early_print_init();
  early_println(S("TestOS"));
  early_println(S("Hello World from the higher half!"));

  #ifdef DEBUG
  early_println(S("Debugging enabled, running self tests"));
  reqok(st_run_self_tests(), S("All self tests passed"), S("Self tests failed"));
  #endif

  reqok(pam_init_os(), S("Page allocation manager initialized"), S("Failed to initialize page allocation manager"));

  asm volatile(
    "cli\n\t"
    "hlt\n\t"
  );
  while (true) {}
}

static void ok(string_t str) {
  early_print(S("["));
  early_print_set_color(EP_GREEN, EP_BLACK);
  early_print(S("OK"));
  early_print_reset_color();
  early_print(S("]: "));
  early_println(str);
}

static void fail(string_t str) {
  early_print(S("["));
  early_print_set_color(EP_RED, EP_BLACK);
  early_print(S("FAIL"));
  early_print_reset_color();
  early_print(S("]: "));
  early_println(str);
}

static void hang() {
  asm volatile(
    "cli\n\t"
    "hlt\n\t"
  );
  while (true);
}

static void reqok(bool condition, string_t ok_str, string_t fail_str) {
  if (condition) {
    fail(fail_str);
    hang();
  } else {
    ok(ok_str);
  }
}