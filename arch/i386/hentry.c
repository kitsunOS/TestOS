#include "early_print.h"

#include <types.h>

void higher_half_entry() {
  early_print(S("TestOS"));
  early_print(S("Hello World from the higher half!"));
  while (true) {}
}