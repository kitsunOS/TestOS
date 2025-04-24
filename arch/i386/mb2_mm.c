// TODO: Move to arch mm

#include <multiboot2.h>
#include <types.h>
#include <pagestrap.h>

extern multiboot2_header_t* _multiboot2_info;
extern vptr __phys_kernel_start;
extern vptr __phys_kernel_end;

static void handle_mem_map(pagestrap_t* pagestrap, multiboot2_memory_map_tag_t* mem_map);
static void handle_mem_map_entry(pagestrap_t* pagestrap, multiboot2_memory_map_entry_t* entry);

void mb2_mm_setup(pagestrap_t* pagestrap) {
  multiboot2_tag_t* tag = (multiboot2_tag_t*) ((u8*) _multiboot2_info + sizeof(multiboot2_header_t));
  while (tag->type != 0) {
    if (tag->type == 6) {
      multiboot2_memory_map_tag_t* mem_map = (multiboot2_memory_map_tag_t*) tag;
      handle_mem_map(pagestrap, mem_map);
    }
    tag = (multiboot2_tag_t*) ((u8*) tag + tag->size);
  }
}

// TODO: A method to also release mb2 data when it's not needed?

static bool ranges_overlap(vptr start1, vptr end1, vptr start2, vptr end2) {
  return
    (start1 >= start2 && start1 < end2) ||
    (end1 > start2 && end1 <= end2) ||
    (start2 >= start1 && start2 < end1) ||
    (end2 > start1 && end2 <= end1);
}

static void handle_mem_map(pagestrap_t* pagestrap, multiboot2_memory_map_tag_t* mem_map) {
  u8* entry_ptr = (u8*) mem_map + sizeof(multiboot2_memory_map_tag_t);
  u32 entry_size = mem_map->entry_size;
  u32 num_entries = (mem_map->size - sizeof(multiboot2_memory_map_tag_t)) / entry_size;

  for (u32 i = 0; i < num_entries; i++) {
    multiboot2_memory_map_entry_t* entry = (multiboot2_memory_map_entry_t*) entry_ptr;
    handle_mem_map_entry(pagestrap, entry);
    entry_ptr += entry_size;
  }
}

static void handle_mem_map_entry(pagestrap_t* pagestrap, multiboot2_memory_map_entry_t* entry) {
  if (
    entry->base_addr_high != 0
    || entry->length_high != 0
    || entry->base_addr_low + entry->length_low < entry->base_addr_low
  ) {
    return; // Ignore until 64-bit support is added
  }

  u32 base_addr = entry->base_addr_low;
  u32 length = entry->length_low;

  vptr multiboot2_end = (vptr) _multiboot2_info + _multiboot2_info->total_size;
  
  if (entry->type == 1) {
    pagestrap_add_pages_se(pagestrap, (vptr) base_addr, (vptr) base_addr + length);
    if (ranges_overlap((vptr) base_addr, (vptr) base_addr + length, __phys_kernel_start, __phys_kernel_end)) {
      pagestrap_remove_pages_se(pagestrap, (vptr) __phys_kernel_start, (vptr) __phys_kernel_end);
    }
    if (ranges_overlap((vptr) base_addr, (vptr) base_addr + length, (vptr) _multiboot2_info, multiboot2_end)) {
      pagestrap_remove_pages_se(pagestrap, (vptr) _multiboot2_info, multiboot2_end);
    }
  }
}