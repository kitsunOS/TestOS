#include <types.h>
#include <input/input_events.h>

#include <arch/early_print.h> // Testing

void process_key_event(u8 event_type, u8 keycode, char charCode) {
  if (charCode != 0 && event_type == EVENT_KEYDOWN) {
    early_print_char(charCode);
  }
}