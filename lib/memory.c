#include "memory.h"

void memfill(vptr start, uX size, u8 value) {
  for (u32 i = (u32) start; i < (u32) start + size; i++) {
    *((u8*) i) = value;
  }
}