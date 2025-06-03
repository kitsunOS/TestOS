#ifndef __I386_PIC_H__
#define __I386_PIC_H__

void pic_init();

void pic_end_interrupt(int irq);

#endif