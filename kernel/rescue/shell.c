#include <rescue/shell.h>
#include <arch/early_print.h>
#include <fs/vfs.h>
#include <input/keys.h>
#include <types.h>

static void print_shell_prompt() {
  early_print(S("Welcome to "));
  early_print_set_color(EP_CYAN, EP_BLACK);
  early_print(S("TestOS"));
  early_print_reset_color();
  early_println(S("!\nDropping into kernel-space rescue shell\n"));
}

static void process_key_event(u8 event_type, u8 keycode, char charCode) {
  if (charCode != 0 && event_type == EVENT_KEYDOWN) {
    early_print_char(charCode);
  }
}

static string_t read() {
  uX devfs_node;
  s8 result = vfs_module.open(vfs_root_node, &devfs_node, S("dev"), FS_NODE_DIRECTORY, FS_M_READ);
  if (result < 0) return NULL_STR;

  uX input_handle;
  result = vfs_module.open(devfs_node, &input_handle, S("input"), FS_NODE_FILE, FS_M_READ);
  if (result < 0) return NULL_STR;
  
  bool done = false;
  u8 input_buffer[16 * 3];

  while (!done) {
    sX size = vfs_module.read(input_handle, input_buffer, 16 * 3);
    if (size < 0) return NULL_STR; // TODO: Close resource?
    if (!done) asm volatile("hlt");

    for (int i = 0; i + 2 < size; i += 3) {
      process_key_event(input_buffer[i], input_buffer[i + 1], input_buffer[i + 2]);
    }
  }

  vfs_module.close(input_handle);
}

void rescue_shell_run() {
  print_shell_prompt();

  // while (true) {
    read();
  // }
}