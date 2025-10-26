#include <rescue/shell.h>
#include <arch/early_print.h>

static void print_shell_prompt() {
  early_print(S("Welcome to "));
  early_print_set_color(EP_CYAN, EP_BLACK);
  early_print(S("TestOS"));
  early_print_reset_color();
  early_println(S("!\nDropping into kernel-space rescue shell\n"));
}

void rescue_shell_run() {
  print_shell_prompt();

  while (true) {
    asm volatile("hlt");
  }
}