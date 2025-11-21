#include <types.h>
#include <input/input.h>
#include <fs/devfs.h>

#define NUM_EVENTS 256
#define EVENT_SIZE 3

u8 event_buffer[NUM_EVENTS * EVENT_SIZE];
u8 write_index = 0;
u8 read_index = 0;

static s8 input_open(uX* new_node_id, u8 mode, vptr meta);
static sX input_read(uX node_id, u8* buffer, u16 size);
static sX input_write(uX node_id, u8* buffer, u16 size);
static s8 input_close(uX node_id);

// TODO: There should probably be one for each input device...
static devfs_charfile_t input_charfile = {
  .open = input_open,
  .read = input_read,
  .write = input_write,
  .close = input_close
};

bool input_init() {
  s8 result = devfs_create_character_file(devfs_root_node, S("input"), &input_charfile, null);
  if (result < 0) return false;

  return true;
}

bool input_shutdown() {
  devfs_release_character_file(devfs_root_node, S("input"));
}

// TODO: Expand to a u16 keycode?
void input_process_key_event(u8 event_type, u8 keycode, char charCode) {
  uX idx = (uX) write_index * EVENT_SIZE;
  event_buffer[idx++] = event_type;
  event_buffer[idx++] = keycode;
  event_buffer[idx++] = charCode;
  write_index = (write_index + 1) % NUM_EVENTS;
  if (write_index == read_index) {
    read_index = read_index + 1 % NUM_EVENTS;
  }
}

// TODO: Enforce only one copy open
static s8 input_open(uX* new_node_id, u8 mode, vptr meta) {
  return true;
}

static sX input_read(uX node_id, u8* buffer, u16 size) {
  u32 read = 0;
  while (read + EVENT_SIZE < size && read_index != write_index) {
    for (int i = 0; i < EVENT_SIZE; i++) {
      buffer[read++] = event_buffer[read_index * EVENT_SIZE + i];
    }
    read_index = (read_index + 1) % NUM_EVENTS;
  }

  return read;
}

static sX input_write(uX node_id, u8* buffer, u16 size) {
  return FS_ERR_NOT_SUPPORTED;
}

static s8 input_close(uX node_id) {
  return true;
}