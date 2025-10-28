#include <vector.h>

#include <mm/mem.h>

bool vector_init(vector_t* vector, uX init_capacity, float grow_factor) {
  vector -> capacity = init_capacity;
  vector -> grow_factor = grow_factor;
  vector -> pointer = 0;
  vector -> items = kmalloc(sizeof(vptr) * init_capacity);
}

sX vector_add(vector_t* vector, vptr item) {
  if (vector -> pointer == vector -> capacity) {
    uX new_capacity = vector -> capacity * vector -> grow_factor;
    vptr* new_items = kmalloc(sizeof(vptr) * new_capacity);
    if (new_items == null) return -1; // TODO: Consistent error codes
    
    for (int i = 0; i < vector -> capacity; i++) {
      new_items[i] = vector -> items[i];
    }

    kfree(vector -> items);
    vector -> items = new_items;
    vector -> capacity = new_capacity;
  }

  vector -> items[vector -> pointer] = item;

  return vector -> pointer++;
}

sX vector_length(vector_t* vector) {
  return vector -> pointer;
}

vptr vector_get(vector_t* vector, uX index) {
  if (index >= (uX) vector -> pointer) {
    return null;
  }

  return vector -> items[index];
}

bool vector_free(vector_t* vector) {
  kfree(vector -> items);
}