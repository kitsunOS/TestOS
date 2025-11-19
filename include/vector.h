#ifndef __LIB_VECTOR_H__
#define __LIB_VECTOR_H__

#include <types.h>

typedef struct vector_t {
  vptr* items;
  sX capacity;
  sX pointer;
  u8 grow_factor;
} vector_t;

bool vector_init(vector_t* vector, uX init_capacity, u8 grow_factor);

// Realistically, reaching the top of sX already means we have problems, so the conversion from uX to sX does not matter as much.
sX vector_add(vector_t* vector, vptr item);

sX vector_length(vector_t* vector);

vptr vector_get(vector_t* vector, uX index);

void vector_free(vector_t* vector);

#endif