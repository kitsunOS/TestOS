#include <types.h>
#include <arch/kio.h>
#include <drivers/int.h>
#include "idt.h"

// Example stuff
#include "pic.h"

extern void* isr_stub_table[];

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

// int

irq_handler_t irq_handlers[15];

bool int_register(u8 irq, irq_handler_t irq_handler) {
    if (irq > 15 || irq_handlers[irq] != null) {
        return false;
    }

    irq_handlers[irq] = irq_handler;
    return true;
}

bool int_unregister(u8 irq, irq_handler_t irq_handler) {
    if (irq > 15 || irq_handlers[irq] != irq_handler) {
        return false;
    }

    irq_handlers[irq] = null;
    return true;
}

extern void* idt_handle_irq(u8 irq) {
    if (irq_handlers[irq] != null) {
        irq_handlers[irq](irq);
    }

    pic_end_interrupt(irq);
}

// Some example stuff for now
void idt_on_timer(u8 irq) {
    
}

void idt_populate() {
    int_register(0, idt_on_timer);

    for (int i = 0; i < 255; i++) {
        idt_add_entry(i, isr_stub_table[i], 0xE);
    }
}