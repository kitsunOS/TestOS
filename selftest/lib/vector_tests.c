#include <types.h>
#include <selftest/self_tests.h>
#include <arch/early_print.h>
#include <vector.h>

bool st_run_vector_tests() {
  
  vector_t vector;
  vector_init(&vector, 2, 3.0f);

  // Basic add
  if (st_assert_equals_uX(0, vector_add(&vector, (vptr) 0x1122), S("Vector Test \"Basic Add\" (PT1) Failed"), S(""))) return true;
  if (st_assert_equals_uX(1, vector_add(&vector, (vptr) 0x3344), S("Vector Test \"Basic Add\" (PT2) Failed"), S(""))) return true;

  if (st_assert_equals_vptr((vptr) 0x1122, vector_get(&vector, 0), S("Vector Test \"Basic Add\" (PT3) Failed"), S(""))) return true;
  if (st_assert_equals_vptr((vptr) 0x3344, vector_get(&vector, 1), S("Vector Test \"Basic Add\" (PT4) Failed"), S(""))) return true;
  if (st_assert_equals_uX(2, vector.pointer, S("Vector Test \"Basic Add\" (PT5) Failed"), S(""))) return true;
  if (st_assert_equals_uX(2, vector.capacity, S("Vector Test \"Basic Add\" (PT6) Failed"), S(""))) return true;
  if (st_assert_equals_uX(3, vector.grow_factor, S("Vector Test \"Basic Add\" (PT7) Failed"), S("Vector Test \"Basic Add\" Passed"))) return true;

  // Overflow
  if (st_assert_equals_uX(2, vector_add(&vector, (vptr) 0x5555), S("Vector Test \"Overflow\" (PT1) Failed"), S(""))) return true;
  if (st_assert_equals_vptr((vptr) 0x1122, vector_get(&vector, 0), S("Vector Test \"Overflow\" (PT2) Failed"), S(""))) return true;
  if (st_assert_equals_vptr((vptr) 0x3344, vector_get(&vector, 1), S("Vector Test \"Overflow\" (PT3) Failed"), S(""))) return true;
  if (st_assert_equals_vptr((vptr) 0x5555, vector_get(&vector, 2), S("Vector Test \"Overflow\" (PT4) Failed"), S(""))) return true;
  if (st_assert_equals_uX(3, vector.pointer, S("Vector Test \"Overflow\" (PT5) Failed"), S(""))) return true;
  if (st_assert_equals_uX(6, vector.capacity, S("Vector Test \"Overflow\" (PT6) Failed"), S(""))) return true;
  if (st_assert_equals_uX(3, vector.grow_factor, S("Vector Test \"Overflow\" (PT7) Failed"), S("Vector Test \"Overflow\" Passed"))) return true;

  vector_free(&vector);
  
  return false; 
}