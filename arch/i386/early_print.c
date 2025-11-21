#include <arch/early_print.h>
#include <types.h>
#include <strings.h>

static const u8 MAX_X = 80, MAX_Y = 25;
static volatile u16* FRAMEBUFFER = (u16*)0xB8000;

static u8 xPos = 0, yPos = 0;
static u8 color = 0x07;

static void scroll(int lines);
static void next_line();
static void set_cursor(u8 x, u8 y);
static void outb(u16 port, u8 value);

void early_print_init() {
  xPos = 0, yPos = 0;
  color = 0x07;
  for (u16 i = 0; i < MAX_X * MAX_Y; i++) {
    FRAMEBUFFER[i] = (color << 8) | ' ';
  }
}

void early_print_set_color(u8 fg, u8 bg) {
  color = (bg << 4) | (fg & 0x0F);
}

void early_print_reset_color() {
  color = 0x07;
}

void early_print_finish() {
  if (xPos != 0) {
    next_line();
  }
}

void early_print_char(u8 c) {
  if (c == '\n') {
    next_line();
  } else {
    if (xPos >= MAX_X) {
      next_line();
    }
    volatile u16* cell = FRAMEBUFFER + ((yPos * MAX_X) + xPos);
    *cell = (color << 8) | c;
    xPos++;
  }

  set_cursor(xPos, yPos);
}

void early_print(string_t str) {
  for (const u8* i = strstart(str); i < strend(str); i++) {
    early_print_char(*i);
  }
}

void early_println(string_t str) {
  if (strlen(str) == 0) {
    next_line();
    return;
  }
  early_print(str);
  early_print_finish();
}

void early_print_hex_32(u32 value) {
  early_print(S("0x"));
  for (int i = 28; i >= 0; i -= 4) {
    u8 digit = (value >> i) & 0xF;
    if (digit < 10) {
      early_print_char('0' + digit);
    } else {
      early_print_char('A' + digit - 10);
    }
  }
}

void early_print_addr(vptr addr) {
  early_print_hex_32((u32)addr);
}

void early_print_uX(uX value) {
  if (value == 0) {
    early_print_char('0');
    return;
  }

  uX divisor = 1;

  while (value / divisor >= 10) {
    divisor = divisor * 10;
  }

  while (divisor > 0) {
    u8 digit = value / divisor;
    early_print_char('0' + digit);
    value = value % divisor;
    divisor = divisor / 10;
  }
}

static void next_line() {
  yPos++;
  xPos = 0;
  if (yPos >= MAX_Y) {
    scroll(yPos - MAX_Y + 1);
    yPos = MAX_Y - 1;
  }

  set_cursor(xPos, yPos);
}

static void scroll(int lines) {
  if (lines > 0) {
    for (u8 y = 0; y < MAX_Y - lines; y++) {
      for (u8 x = 0; x < MAX_X; x++) {
        volatile u16* srcCell = FRAMEBUFFER + ((y + lines) * MAX_X) + x;
        volatile u16* destCell = FRAMEBUFFER + (y * MAX_X) + x;
        *destCell = *srcCell;
      }
    }
    for (u8 y = MAX_Y - lines; y < MAX_Y; y++) {
      for (u8 x = 0; x < MAX_X; x++) {
        volatile u16* destCell = FRAMEBUFFER + (y * MAX_X) + x;
        *destCell = color << 8;
      }
    }
  }
  // Don't currently support negative scrolling
}

static void set_cursor(u8 x, u8 y) {
  u16 pos = y * MAX_X + x;
  outb(0x3D4, 14);
  outb(0x3D5, (u8)(pos >> 8));
  outb(0x3D4, 15);
  outb(0x3D5, (u8)(pos & 0xFF));
}

// TODO: Move to shared file
static void outb(u16 port, u8 value) {
  asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}