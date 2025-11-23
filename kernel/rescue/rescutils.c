#include <types.h>
#include <strings.h>
#include <strbuf.h>
#include <arch/early_print.h>
#include <fs/vfs.h>
#include <input/keys.h>

static s8 read_char(uX input_handle, u8* character);
static void handle_backspace(string_buf_t *line);

s8 rescutils_open_input(uX* input_handle) {
  uX devfs_node;
  s8 result = vfs_module.open(vfs_root_node, &devfs_node, S("dev"), FS_NODE_DIRECTORY, FS_M_READ);
  if (result < 0) return result;

  return vfs_module.open(devfs_node, input_handle, S("input"), FS_NODE_FILE, FS_M_READ);
}

s8 rescutils_get_line(uX input_handle, string_buf_t* line) {
  u8 character = 0;

  strbuf_clear(line);
  while (true) {
    s8 result = read_char(input_handle, &character);
    if (result < 0) return result;
    if (result == false) {
      asm volatile("hlt");
    } else if (character == '\n') {
      early_print_char(character);
      break;
    } else if (character == '\b') {
      handle_backspace(line);
    } else if (character != 0) {
      early_print_char(character);
      strbuf_append_char(line, character);
    }
  }
}

static s8 read_char(uX input_handle, u8* character) {
  u8 input_buffer[3];
  sX size = vfs_module.read(input_handle, input_buffer, 3);
  if (size < 0) return size;
  if (size == 0) return false;
  
  *character = input_buffer[0] == EVENT_KEYDOWN ? input_buffer[2] : 0;

  return true;
}

static void handle_backspace(string_buf_t *line) {
  uX str_size = strlen(SB2S(*line));
  if (str_size == 0) return;

  strbuf_splice(line, str_size - 1, 1);

  u8 xPos, yPos, xRes, yRes;
  early_print_getpos(&xPos, &yPos);
  early_print_getres(&xRes, &yRes);
  
  if (yPos == 0 && xPos == 0) return;
  if (xPos == 0) {
    yPos--;
    xPos = xRes - 1;
  } else {
    xPos--;
  }

  early_print_setpos(xPos, yPos);
  early_print(S(" "));
  early_print_setpos(xPos, yPos);
}