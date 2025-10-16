#ifndef __INPUT_EVENTS_H__
#define __INPUT_EVENTS_H__

#include <types.h>
#include <input/keys.h>

void process_key_event(u8 event_type, u8 keycode, char charCode);

#endif