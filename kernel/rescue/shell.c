#include <rescue/shell.h>
#include <arch/early_print.h>
#include <fs/vfs.h>
#include <input/keys.h>
#include <types.h>
#include <strbuf.h>

#include "rescutils.h"

static s8 open_input(uX* input_handle);
static s8 get_line(uX input_handle, string_buf_t *line);

// Main code

static void print_shell_prompt() {
  early_print(S("Welcome to "));
  early_print_set_color(EP_CYAN, EP_BLACK);
  early_print(S("TestOS"));
  early_print_reset_color();
  early_println(S("!\nDropping into kernel-space rescue shell\n"));
}

void rescue_shell_run() {
  print_shell_prompt();

  uX input_handle;
  s8 result = rescutils_open_input(&input_handle);
  if (result < 0) return; // TODO: Better error handling

  string_buf_t line;
  strbuf_init(&line, 512, 15);
  while (true) {
    early_print(S("rescue> "));
    rescutils_get_line(input_handle, &line);
    if (result < 0) break;

    early_print(S("Got Text: "));
    early_println(SB2S(line));
  }
  strbuf_free(&line);

  vfs_module.close(input_handle);
}