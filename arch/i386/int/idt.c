#include <types.h>
#include "idt.h"

idt_entry_t idt[256];
idt_descriptor_t idt_descriptor = {
    .size = sizeof(idt) - 1,
    .address = &idt
};

void idt_add_entry(u8 id, vptr offset, u8 type_attr) {
    idt_entry_t* entry = &idt[id];
    entry->offset_low = (u32) offset & 0xFFFF;
    entry->present = 1;
    entry->descriptor_privilege_level = 0;
    entry->zero = 0;
    entry->gate_type = type_attr;
    entry->reserved = 0;
    entry->type_attr = type_attr;
    entry->offset_high = ((u32) offset >> 16) & 0xFFFF;
}

void idt_init() {
    for (int i = 0; i < 256; i++) {
        idt[i].offset_low = 0;
        idt[i].present = 0;
        idt[i].descriptor_privilege_level = 0;
        idt[i].zero = 0;
        idt[i].gate_type = 0;
        idt[i].reserved = 0;
        idt[i].type_attr = 0;
        idt[i].offset_high = 0;
    }

    asm volatile(
        "lidt %0\n"
        : : "m"(idt_descriptor) : "memory"
    );
}