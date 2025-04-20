#include <types.h>
#include "../pages.h"

page_directory_entry_pte page_directory[1024] __attribute__((aligned(4096))) = {0};
page_table_entry_fourkb page_table[1024 * 1024] __attribute__((aligned(4096))) = {0};

void init_identity_paging() {
  for (int i = 0; i < 1024; i++) {
    page_directory[i].in_memory = 1;
    page_directory[i].enable_write = 1;
    page_directory[i].pte_reference = ((u32) &page_table[i * 1024]) >> 12;

    for (int j = 0; j < 1024; j++) {
      int page_index = i * 1024 + j;
      page_table[page_index].in_memory = 1;
      page_table[page_index].enable_write = 1;
      page_table[page_index].physical_reference = page_index;
    }
  }
}