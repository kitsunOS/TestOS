#ifndef __I386_PAGES_H__
#define __I386_PAGES_H__

#include <types.h>

typedef struct __attribute__((packed)) {
  u32 in_memory:1;
  u32 enable_write:1;
  u32 supervisor_only:1;
  u32 write_through:1;
  u32 disable_cache:1;
  u32 accessed:1;
  u32 :1;
  u32 is_large_page:1;
  u32 :4;
  u32 pte_reference:20;
} page_directory_entry_pte;

typedef struct __attribute__((packed)) {
  u32 in_memory:1;
  u32 enable_write:1;
  u32 supervisor_only:1;
  u32 write_through:1;
  u32 disable_cache:1;
  u32 accessed:1;
  u32 dirty:1;
  u32 pat_mode:1;
  u32 keep_tlb:1;
  u32 :3;
  u32 physical_reference:20;
} page_table_entry_fourkb;

#endif