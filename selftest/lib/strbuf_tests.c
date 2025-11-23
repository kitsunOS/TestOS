#include <types.h>
#include <selftest/self_tests.h>
#include <strbuf.h>

bool st_run_strbuf_tests() {
  
  string_buf_t strbuf;
  if (!st_assert_true(strbuf_init(&strbuf, 10, 30), S("String Buffer Test Setup Allocation Failed"), S(""))) return false;

  // Append
  if (!st_assert_true(strbuf_append(&strbuf, S("HELLO")), S("String Buffer Test \"Append\" (PT1) Failed"), S(""))) return false;
  if (!st_assert_true(strbuf_append(&strbuf, S("WORLD")), S("String Buffer Test \"Append\" (PT2) Failed"), S(""))) return false;

  if (!st_assert_equals_str(S("HELLOWORLD"), SB2S(strbuf), S("String Buffer Test \"Append\" (PT3) Failed"), S(""))) return false;
  if (!st_assert_equals_uX(10, strbuf.len, S("String Buffer Test \"Append\" (PT4) Failed"), S(""))) return false;
  if (!st_assert_equals_uX(10, strbuf.capacity, S("String Buffer Test \"Append\" (PT5) Failed"), S(""))) return false;
  if (!st_assert_equals_uX(30, strbuf.grow_factor, S("String Buffer Test \"Append\" (PT6) Failed"), S("String Buffer Test \"Append\" Passed"))) return false;

  // Overflow
  if (!st_assert_true(strbuf_append_char(&strbuf, '!'), S("String Buffer Test \"Overflow\" (PT1) Failed"), S(""))) return false;
  if (!st_assert_equals_str(S("HELLOWORLD!"), SB2S(strbuf), S("String Buffer Test \"Overflow\" (PT2) Failed"), S(""))) return false;
  if (!st_assert_equals_uX(11, strbuf.len, S("String Buffer Test \"Overflow\" (PT5) Failed"), S(""))) return false;
  if (!st_assert_equals_uX(30, strbuf.capacity, S("String Buffer Test \"Overflow\" (PT6) Failed"), S(""))) return false;
  if (!st_assert_equals_uX(30, strbuf.grow_factor, S("String Buffer Test \"Overflow\" (PT7) Failed"), S("String Buffer Test \"Overflow\" Passed"))) return false;

  // Splice
  strbuf_splice(&strbuf, 4, 1);
  if (!st_assert_equals_str(S("HELLWORLD!"), SB2S(strbuf), S("String Buffer Test \"Splice\" (PT1) Failed"), S(""))) return false;
  strbuf_splice(&strbuf, 6, 999);
  if (!st_assert_equals_str(S("HELLWO"), SB2S(strbuf), S("String Buffer Test \"Splice\" (PT2) Failed"), S("String Buffer Test \"Splice\" Passed"))) return false;

  strbuf_free(&strbuf);
  
  return true; 
}