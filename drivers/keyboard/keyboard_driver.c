#include <types.h>
#include <drivers/int.h>
#include <input/keys.h>
#include <input/input.h>
#include <arch/early_print.h>
#include <arch/kio.h>
#include "keyboard_driver.h"

static void idt_onkey(u8 irq);

#define UP EVENT_KEYUP
#define DOWN EVENT_KEYDOWN

u8 scancode1_remap[256] = {
  0, 0, KEYS_1, KEYS_2, KEYS_3, KEYS_4, KEYS_5, KEYS_6, KEYS_7, KEYS_8, KEYS_9, KEYS_0, KEYS_MINUS, KEYS_EQUAL, KEYS_BACKSPACE, 0,
  KEYS_Q, KEYS_W, KEYS_E, KEYS_R, KEYS_T, KEYS_Y, KEYS_U, KEYS_I, KEYS_O, KEYS_P, KEYS_LBRACKET, KEYS_RBRACKET, KEYS_ENTER, KEYS_LCTRL, KEYS_A, KEYS_S,
  KEYS_D, KEYS_F, KEYS_G, KEYS_H, KEYS_J, KEYS_K, KEYS_L, KEYS_SEMICOLON, KEYS_APOSTROPHE, KEYS_GRAVE, KEYS_LSHIFT, KEYS_BACKSLASH, KEYS_Z, KEYS_X, KEYS_C, KEYS_V,
  KEYS_B, KEYS_N, KEYS_M, KEYS_COMMA, KEYS_PERIOD, KEYS_SLASH, KEYS_RSHIFT, 0, KEYS_LALT, KEYS_SPACE, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, KEYS_1, KEYS_2, KEYS_3, KEYS_4, KEYS_5, KEYS_6, KEYS_7, KEYS_8, KEYS_9, KEYS_0, KEYS_MINUS, KEYS_EQUAL, KEYS_BACKSPACE, 0,
  KEYS_Q, KEYS_W, KEYS_E, KEYS_R, KEYS_T, KEYS_Y, KEYS_U, KEYS_I, KEYS_O, KEYS_P, KEYS_LBRACKET, KEYS_RBRACKET, KEYS_ENTER, KEYS_LCTRL, KEYS_A, KEYS_S,
  KEYS_D, KEYS_F, KEYS_G, KEYS_H, KEYS_J, KEYS_K, KEYS_L, KEYS_SEMICOLON, KEYS_APOSTROPHE, KEYS_GRAVE, KEYS_LSHIFT, KEYS_BACKSLASH, KEYS_Z, KEYS_X, KEYS_C, KEYS_V,
  KEYS_B, KEYS_N, KEYS_M, KEYS_COMMA, KEYS_PERIOD, KEYS_SLASH, KEYS_RSHIFT, 0, KEYS_LALT, KEYS_SPACE, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

u8 event1_remap[256] = {
  0, 0, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, 0,
  DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN,
  DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN,
  DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, DOWN, 0, DOWN, DOWN, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, UP, UP, UP, UP, UP, UP, UP, UP, UP, UP, UP, UP, UP, 0,
  UP, UP, UP, UP, UP, UP, UP, UP, UP, UP, UP, UP, UP, UP, UP, UP,
  UP, UP, UP, UP, UP, UP, UP, UP, UP, UP, UP, UP, UP, UP, UP, UP,
  UP, UP, UP, UP, UP, UP, UP, 0, UP, UP, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

u8 lower_char1[256] = {
  0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', 0,
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's',
  'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
  'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' ', 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

u8 upper_char1[256] = {
  0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', 0,
  'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A', 'S',
  'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|', 'Z', 'X', 'C', 'V',
  'B', 'N', 'M', '<', '>', '?', 0, 0, 0, ' ', 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

void driver_init() {
  int_register(1, idt_onkey);
}

void driver_shutdown() {
  int_unregister(1, idt_onkey);
}

static bool shiftActive = false;
static void idt_onkey(u8 irq) {
  u8 scancode = inu8(0x60);
  u8 keycode = scancode1_remap[scancode];
  u8 event_type = event1_remap[scancode];

  if (keycode == KEYS_LSHIFT || keycode == KEYS_RSHIFT) {
    if (event_type == EVENT_KEYDOWN) {
      shiftActive = true;
    } else if (event_type == EVENT_KEYUP) {
      shiftActive = false;
    }
  }

  if (keycode != 0) {
    input_process_key_event(event_type, keycode, shiftActive ? upper_char1[scancode] : lower_char1[scancode]);
  }
}

driver_t keyboard_driver = {
  driver_init,
  driver_shutdown
};