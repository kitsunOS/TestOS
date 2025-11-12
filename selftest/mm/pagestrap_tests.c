#include <types.h>
#include <selftest/self_tests.h>
#include <mm/pagestrap.h>

bool st_run_pagestrap_tests() {
  pagestrap_t pagestrap;
  pagestrap_alloc_t first_alloc;
  // For now, we ignore the case where more internal allocations are needed
  pagestrap_init(&pagestrap, &first_alloc, null);

  // Basic allocation
  if (!st_assert_true(pagestrap_add_pages(&pagestrap, 0x2000, 1), S("Pagestrap Test \"Basic Allocation\" (SETUP ADD) Failed"), S(""))) return false;
  if (!st_assert_true(pagestrap_add_pages(&pagestrap, 0x4000, 1), S("Pagestrap Test \"Basic Allocation\" (SETUP ADD) Failed"), S(""))) return false;
  void* page1 = pagestrap_allocate_page(&pagestrap, 1);
  if (!st_assert_equals_vptr((vptr)0x4000, page1, S("Pagestrap Test \"Basic Allocation\" (PT1) Failed"), S(""))) return false;
  void* page2 = pagestrap_allocate_page(&pagestrap, 1);
  if (!st_assert_equals_vptr((vptr)0x2000, page2, S("Pagestrap Test \"Basic Allocation\" (PT2) Failed"), S(""))) return false;
  void* page3 = pagestrap_allocate_page(&pagestrap, 1);
  if (!st_assert_equals_vptr(null, page3, S("Pagestrap Test \"Basic Allocation\" (PT3) Failed"), S("Pagestrap Test \"Basic Allocation\" Passed"))) return false;

  // Add multiple pages in same call
  if (!st_assert_true(pagestrap_add_pages(&pagestrap, 0x7000, 2), S("Pagestrap Test \"Multipage Added\" (SETUP ADD) Failed"), S(""))) return false;
  void* page4 = pagestrap_allocate_page(&pagestrap, 1);
  if (!st_assert_equals_vptr((vptr)0x7000, page4, S("Pagestrap Test \"Multipage Added\" (PT1) Failed"), S(""))) return false;
  void* page5 = pagestrap_allocate_page(&pagestrap, 1);
  if (!st_assert_equals_vptr((vptr)0x8000, page5, S("Pagestrap Test \"Multipage Added\" (PT2) Failed"), S(""))) return false;
  void* page6 = pagestrap_allocate_page(&pagestrap, 1);
  if (!st_assert_equals_vptr(null, page6, S("Pagestrap Test \"Multipage Added\" (PT3) Failed"), S("Pagestrap Test \"Multipage Added\" Passed"))) return false;

  // Remove tail end
  if (!st_assert_true(pagestrap_add_pages(&pagestrap, 0x9000, 2), S("Pagestrap Test \"Remove Tail End\" (SETUP ADD) Failed"), S(""))) return false;
  if (!st_assert_true(pagestrap_remove_pages(&pagestrap, 0xA000, 1), S("Pagestrap Test \"Remove Tail End\" (SETUP REMOVE) Failed"), S(""))) return false;
  void* page7 = pagestrap_allocate_page(&pagestrap, 1);
  if (!st_assert_equals_vptr((vptr)0x09000, page7, S("Pagestrap Test \"Remove Tail End\" (PT1) Failed"), S(""))) return false;
  void* page8 = pagestrap_allocate_page(&pagestrap, 1);
  if (!st_assert_equals_vptr(null, page8, S("Pagestrap Test \"Remove Tail End\" (PT2) Failed"), S("Pagestrap Test \"Remove Tail End\" Passed"))) return false;

  // Remove head
  if (!st_assert_true(pagestrap_add_pages(&pagestrap, 0xC000, 2), S("Pagestrap Test \"Remove Head\" (SETUP ADD) Failed"), S(""))) return false;
  if (!st_assert_true(pagestrap_remove_pages(&pagestrap, 0xC000, 1), S("Pagestrap Test \"Remove Head\" (SETUP REMOVE) Failed"), S(""))) return false;
  void* page9 = pagestrap_allocate_page(&pagestrap, 1);
  if (!st_assert_equals_vptr((vptr)0xD000, page9, S("Pagestrap Test \"Remove Head\" (PT1) Failed"), S(""))) return false;
  void* page10 = pagestrap_allocate_page(&pagestrap, 1);
  if (!st_assert_equals_vptr(null, page10, S("Pagestrap Test \"Remove Head\" (PT2) Failed"), S("Pagestrap Test \"Remove Head\" Passed"))) return false;

  // Remove middle
  if (!st_assert_true(pagestrap_add_pages(&pagestrap, 0x10000, 3), S("Pagestrap Test \"Remove Middle\" (SETUP ADD) Failed"), S(""))) return false;
  if (!st_assert_true(pagestrap_remove_pages(&pagestrap, 0x11000, 1), S("Pagestrap Test \"Remove Middle\" (SETUP REMOVE) Failed"), S(""))) return false;
  void* page11 = pagestrap_allocate_page(&pagestrap, 1);
  if (!st_assert_equals_vptr((vptr)0x10000, page11, S("Pagestrap Test \"Remove Middle\" (PT1) Failed"), S(""))) return false;
  void* page12 = pagestrap_allocate_page(&pagestrap, 1);
  if (!st_assert_equals_vptr((vptr)0x12000, page12, S("Pagestrap Test \"Remove Middle\" (PT2) Failed"), S(""))) return false;
  void* page13 = pagestrap_allocate_page(&pagestrap, 1);
  if (!st_assert_equals_vptr(null, page13, S("Pagestrap Test \"Remove Middle\" (PT3) Failed"), S("Pagestrap Test \"Remove Middle\" Passed"))) return false;

  // Remove all
  if (!st_assert_true(pagestrap_add_pages(&pagestrap, 0x14000, 4), S("Pagestrap Test \"Remove All\" (SETUP ADD) Failed"), S(""))) return false;
  if (!st_assert_true(pagestrap_remove_pages(&pagestrap, 0x14000, 4), S("Pagestrap Test \"Remove All\" (SETUP REMOVE) Failed"), S(""))) return false;
  void* page14 = pagestrap_allocate_page(&pagestrap, 1);
  if (!st_assert_equals_vptr(null, page14, S("Pagestrap Test \"Remove All\" Failed"), S("Pagestrap Test \"Remove All\" Passed"))) return false;

  return true;
}