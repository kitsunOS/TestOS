#ifndef __I386_GDT_H__
#define __I386_GDT_H__

#include <types.h>

typedef struct __attribute__((packed)) {
  u16 size;
  void* address;
} gdt_descriptor_t;

typedef struct __attribute__((packed)) {
  u16 limit_low:16;
  u32 base_low:24;
  u8 access_byte:8;
  u8 limit_high:4;
  u8 flags:4;
  u8 base_high:8;
} segment_descriptor_t;

void gdt_init();

#endif