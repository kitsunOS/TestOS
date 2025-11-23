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

typedef struct string_t {
  uX len;
  const u8* data;
} string_t;

#define S(x) ((string_t) { sizeof(x) - 1, (const u8*) x })
#define NULL_STR (string_t) { 0, 0 }

#define true 1
#define false 0
#define null (vptr) 0

#endif