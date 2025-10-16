#ifndef __DRIVER_INT_H__
#define __DRIVER_INT_H__

#include <types.h>

typedef void (*irq_handler_t) (u8 irq);

bool int_register(u8 irq, irq_handler_t irq_handler);
bool int_unregister(u8 irq, irq_handler_t irq_handler);

#endif