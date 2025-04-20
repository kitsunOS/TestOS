#ifndef __TYPES_H__
#define __TYPES_H__

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned int uX;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed int sX;

typedef u8 bool;

typedef struct {
  u32 size;
  const u8* data;
} string_t;

#define S(x) ((string_t){sizeof(x), x})

#define true 1
#define false 0

#endif