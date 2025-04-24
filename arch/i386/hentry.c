#include "early_print.h"

#include <types.h>

#ifdef DEBUG
#include "self_tests.h"
#endif

void higher_half_entry() {
  early_print_init();
  early_println(S("TestOS"));
  early_println(S("Hello World from the higher half!"));

  #ifdef DEBUG
  early_println(S("Debugging enabled, running self tests"));
  st_run_self_tests();
  #endif

  while (true) {}
}