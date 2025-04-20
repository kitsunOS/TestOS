#include "early_print.h"
#include <types.h>

static const u8 MAX_X = 80, MAX_Y = 25;
static u16* FRAMEBUFFER = (u16*)0xB8000;

static u8 xPos = 0, yPos = 0;
static u8 color = 0x07;

static void scroll(int lines);
static void nextLine();
static void setCursor(u8 x, u8 y);
static void outb(u16 port, u8 value);

void early_print(string_t str) {
  for (u32 i = 0; i < str.size; i++) {
    u8 c = str.data[i];
    if (c == '\n') {
      nextLine();
    } else {
      if (xPos >= MAX_X) {
        nextLine();
      }
      u16* cell = FRAMEBUFFER + ((yPos * MAX_X) + xPos);
      *cell = (color << 8) | c;
      xPos++;
    }
  }

  if (xPos != 0) {
    nextLine();
  }

  setCursor(xPos, yPos);
}

static void nextLine() {
  yPos++;
  xPos = 0;
  if (yPos >= MAX_Y) {
    scroll(yPos - MAX_Y + 1);
    yPos = MAX_Y - 1;
  }
}

static void scroll(int lines) {
  if (lines > 0) {
    for (u8 y = 0; y < MAX_Y - lines; y++) {
      for (u8 x = 0; x < MAX_X; x++) {
        u16* srcCell = FRAMEBUFFER + ((y + lines) * MAX_X) + x;
        u16* destCell = FRAMEBUFFER + (y * MAX_X) + x;
        *destCell = *srcCell;
      }
    }
    for (u8 y = MAX_Y - lines; y < MAX_Y; y++) {
      for (u8 x = 0; x < MAX_X; x++) {
        u16* destCell = FRAMEBUFFER + (y * MAX_X) + x;
        *destCell = color << 8;
      }
    }
  }
  // Don't currently support negative scrolling
}

static void setCursor(u8 x, u8 y) {
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