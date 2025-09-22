#ifndef __I386_KIO_H__
#define __I386_KIO_H__

#include <types.h>

void outu8(u16 port, u8 value);
u8 inu8(u16 port);
void outu16(u16 port, u16 value);
u16 inu16(u16 port);
void outu32(u16 port, u32 value);
u32 inu32(u16 port);

void io_wait();

#endif