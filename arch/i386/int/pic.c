#include "pic.h"
#include "../kio.h"
#include <types.h>

#define PIC1 0x20
#define PIC2 0xA0

#define PIC_EOI 0x20

#define ICW1_ICW4	0x01
#define ICW1_INIT	0x10
#define ICW4_8086	0x01

void pic_init() {
    outu8(PIC1, ICW1_INIT | ICW1_ICW4);
    outu8(PIC2, ICW1_INIT | ICW1_ICW4);

    outu8(PIC1 + 1, 0x20);
    outu8(PIC2 + 1, 0x28);

    outu8(PIC1 + 1, 0x04);
    outu8(PIC2 + 1, 0x02);

    outu8(PIC1 + 1, ICW4_8086);
    outu8(PIC2 + 1, ICW4_8086);

    outu8(PIC1, PIC_EOI);
    outu8(PIC2, PIC_EOI);

    outu8(PIC1 + 1, 0);
    outu8(PIC2 + 1, 0);
}