#include "pic.h"
#include "../kio.h"
#include <types.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define PIC_EOI 0x20

#define ICW1_ICW4	0x01
#define ICW1_INIT	0x10
#define ICW4_8086	0x01

void pic_init() {
    outu8(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outu8(PIC1_DATA, 0x20); io_wait();
    outu8(PIC1_DATA, 0x04); io_wait();
    outu8(PIC1_DATA, ICW4_8086); io_wait();

    outu8(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    outu8(PIC2_DATA, 0x28); io_wait();
    outu8(PIC2_DATA, 0x02); io_wait();
    outu8(PIC2_DATA, ICW4_8086); io_wait();

    outu8(PIC1_DATA, 0);
    outu8(PIC2_DATA, 0);

    while (inu8(0x64) & 0x01) {
        // If so, read from the data port to clear it
        inu8(0x60);
    }
}

void pic_end_interrupt(int irq_id) {
    if (irq_id >= 8) {
        outu8(PIC2_COMMAND, PIC_EOI); io_wait();
    }

    outu8(PIC1_COMMAND, PIC_EOI); io_wait();
}