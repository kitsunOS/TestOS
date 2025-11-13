#include <boot/multiboot2.h>
#include <mm/mem.h>
#include <mm/pagestrap.h>
#include <types.h>

extern multiboot2_header_t* _multiboot2_info;

static bool handle_mem_map(pagestrap_t* pagestrap, multiboot2_memory_map_tag_t* mem_map);
static bool handle_mem_map_entry(pagestrap_t* pagestrap, multiboot2_memory_map_entry_t* entry);

bool mb2_mm_setup(pagestrap_t* pagestrap) {
  multiboot2_tag_t* tag = (multiboot2_tag_t*) ((u8*) _multiboot2_info + sizeof(multiboot2_header_t));
  while (tag->type != 0) {
    if (tag->type == 6) {
      multiboot2_memory_map_tag_t* mem_map = (multiboot2_memory_map_tag_t*) tag;
      if (!handle_mem_map(pagestrap, mem_map)) return false;
    }
    tag = (multiboot2_tag_t*) (((u32) tag + tag->size + 7) & ~7);
  }

  return true;
}

// TODO: A method to also release mb2 data when it's not needed?

static bool handle_mem_map(pagestrap_t* pagestrap, multiboot2_memory_map_tag_t* mem_map) {
  u8* entry_ptr = (u8*) mem_map + sizeof(multiboot2_memory_map_tag_t);
  u32 entry_size = mem_map->entry_size;
  u32 num_entries = (mem_map->size - sizeof(multiboot2_memory_map_tag_t)) / entry_size;

  for (u32 i = 0; i < num_entries; i++) {
    multiboot2_memory_map_entry_t* entry = (multiboot2_memory_map_entry_t*) entry_ptr;
    if (!handle_mem_map_entry(pagestrap, entry)) return false;
    entry_ptr += entry_size;
  }

  return true;
}

static bool handle_mem_map_entry(pagestrap_t* pagestrap, multiboot2_memory_map_entry_t* entry) {
  if (
    entry->base_addr_high != 0
    || entry->length_high != 0
    || entry->base_addr_low + entry->length_low < entry->base_addr_low
  ) {
    return true; // Ignore until 64-bit support is added
  }

  u32 base_addr = entry->base_addr_low;
  u32 length = entry->length_low;
  
  if (entry->type == 1) {
    if (!pagestrap_add_pages_se(pagestrap, (vptr) base_addr, (vptr) base_addr + length)) return false;
    if (!pam_remove_bootstrap_regions(pagestrap, (vptr) base_addr, (vptr) base_addr + length)) return false;
  }

  return true;
}