#ifndef __I386_IDT_H__
#define __I386_IDT_H__

#include <types.h>

typedef struct __attribute__((packed)) {
    u16 size;
    void* address;
} idt_descriptor_t;

typedef struct __attribute__((packed)) {
    u16 offset_low:16;
    u16 segment_selector:16;
    u8 reserved:8;
    u8 gate_type:4;
    u8 zero:1;
    u8 descriptor_privilege_level:2;
    u8 present:1;
    u16 offset_high:16;
} idt_entry_t;

void idt_add_entry(u8 id, vptr pointer, u8 type_attr);

void idt_populate();

void idt_preinit();

void idt_init();

#endif