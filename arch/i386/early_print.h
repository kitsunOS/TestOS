#ifndef __I386_EARLY_PRINT_H__
#define __I386_EARLY_PRINT_H__

#include <types.h>

#define EP_BLACK 0
#define EP_BLUE 1
#define EP_GREEN 2
#define EP_CYAN 3
#define EP_RED 4
#define EP_MAGENTA 5
#define EP_BROWN 6
#define EP_LIGHT_GRAY 7
#define EP_DARK_GRAY 8
#define EP_LIGHT_BLUE 9
#define EP_LIGHT_GREEN 10
#define EP_LIGHT_CYAN 11
#define EP_LIGHT_RED 12
#define EP_LIGHT_MAGENTA 13
#define EP_LIGHT_BROWN 14
#define EP_WHITE 15

void early_print_init();
void early_print_set_color(u8 fg, u8 bg);
void early_print_reset_color();
void early_print_finish();
void early_print_char(u8 c);
void early_print(string_t str);
void early_println(string_t str);
void early_print_hex_32(u32 value);
void early_print_addr(vptr addr);

#endif