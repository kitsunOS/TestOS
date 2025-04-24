#include <types.h>
#include "../pages.h"

extern vptr KERNEL_ADDRESS;
extern vptr __phys_kernel_start;
extern vptr __phys_kernel_end;

page_directory_entry_pte_t page_directory[1024] __attribute__((aligned(4096))) = {0};
page_table_entry_fourkb_t page_table[1024 * 1024] __attribute__((aligned(4096))) = {0};

static void init_identity_paging();
static void map_higher_half();

void setup_init_page_map() {
  init_identity_paging();
  map_higher_half();
}

static void init_identity_paging() {
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

static void map_higher_half() {
  u32 kernel_start = (u32) &__phys_kernel_start;
  u32 kernel_end = (u32) &__phys_kernel_end;
  u32 kernel_size = kernel_end - kernel_start;

  for (u32 i = 0; i < kernel_size; i += 4096) {
    u32 page_index = ((u32) &KERNEL_ADDRESS + i) >> 12;
    page_table[page_index].in_memory = 1;
    page_table[page_index].enable_write = 1;
    page_table[page_index].physical_reference = (kernel_start + i) >> 12;
  }
}