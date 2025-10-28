#ifndef __LIB_TYPES_H__
#define __LIB_TYPES_H__

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned int uX;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed int sX;

typedef u8 bool;

typedef void* vptr;

typedef u8* string_t;

#define S(x) ((string_t)&(const struct { \
  uX len; \
  u8 data[sizeof(x) - 1]; \
}){ sizeof(x) - 1, x })

#define true 1
#define false 0
#define null (vptr) 0

#endif