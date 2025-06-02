#include "gdt.h"

segment_descriptor_t gdt[5];

gdt_descriptor_t gdt_descriptor = {
  .size = sizeof(gdt) - 1,
  .address = &gdt
};

static void gdt_init_descriptor(segment_descriptor_t *desc, u32 base, u32 limit, u8 access_byte, u8 flags);

void gdt_init() {
  gdt_init_descriptor(&gdt[0], 0, 0, 0, 0);
  gdt_init_descriptor(&gdt[1], 0, 0xFFFFFFFF, 0x9A, 0xC);
  gdt_init_descriptor(&gdt[2], 0, 0xFFFFFFFF, 0x92, 0xC);
  gdt_init_descriptor(&gdt[3], 0, 0xFFFFFFFF, 0xFA, 0xC);
  gdt_init_descriptor(&gdt[4], 0, 0xFFFFFFFF, 0xF2, 0xC);

  asm volatile(
    "lgdt %0\n"
    "mov $0x10, %%ax\n"
    "mov %%ax, %%ds\n"
    "mov %%ax, %%es\n"
    "mov %%ax, %%fs\n"
    "mov %%ax, %%gs\n"
    "mov %%ax, %%ss\n"
    "jmp $0x08, $.flush\n"
    ".flush:\n"
    : : "m"(gdt_descriptor) : "memory");
}

void gdt_init_descriptor(segment_descriptor_t *desc, u32 base, u32 limit, u8 access_byte, u8 flags) {
  desc->base_low = base & 0xFFFFFF;
  desc->base_high = (base >> 24) & 0xFF;
  desc->limit_low = limit & 0xFFFF;
  desc->limit_high = (limit >> 16) & 0x0F;
  desc->access_byte = access_byte;
  desc->flags = flags;
}