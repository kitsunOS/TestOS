#include <types.h>
#include "idt.h"

// Example stuff
#include "pic.h"
#include "early_print.h"
#include "../kio.h"

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
    entry->segment_selector = 0x08;
    entry->offset_high = ((u32) offset >> 16) & 0xFFFF;
}

void idt_preinit() {
    for (int i = 0; i < 256; i++) {
        idt[i].offset_low = 0;
        idt[i].present = 0;
        idt[i].descriptor_privilege_level = 0;
        idt[i].zero = 0;
        idt[i].gate_type = 0;
        idt[i].reserved = 0;
        idt[i].segment_selector = 0;
        idt[i].offset_high = 0;
    }
}

void idt_init() {
    asm volatile(
        "lidt %0\n"
        : : "m"(idt_descriptor) : "memory"
    );

    asm volatile("sti");
}


// Some example stuff for now
__attribute__((interrupt))
void idt_on_timer(void* frame) {
    pic_end_interrupt(0);
}

__attribute__((interrupt))
void idt_onkey(vptr frame) {
    u8 scancode = inu8(0x60);
    if (scancode == 0x9C) {
        early_println(S(""));
    } else {
        early_print_char(scancode - 0x1E + 'a');
    }
    pic_end_interrupt(1);
}

void idt_populate() {
    idt_add_entry(0x20, idt_on_timer, 0xE);
    idt_add_entry(0x21, idt_onkey, 0xE);
}