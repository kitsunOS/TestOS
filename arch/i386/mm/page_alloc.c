#include <types.h>
#include <boot/multiboot2.h>
#include <mm/mem.h>
#include <mm/pagestrap.h>
#include <mm/mb2_mm.h>
#include "pages.h"
#include "page_alloc.h"

#define PAGE_TABLE_SIZE (sizeof(page_table_entry_fourkb_t) * 1024)
#define PAGE_DIR_SIZE (sizeof(page_directory_entry_pte_t) * 1024)

// TODO: C does not like extern void
extern multiboot2_header_t _multiboot2_info;
extern u8 __phys_kernel_start[];
extern u8 __phys_kernel_end[];
extern u8 __virt_kernel_start[];
extern u8 __virt_kernel_end[];

static page_alloc_manager_t os_page_alloc_manager = {0};
static volatile page_directory_entry_pte_t os_page_directory[1024] __attribute__((aligned(4096))) = {0};
static volatile page_table_entry_fourkb_t os_page_table[1024 * 1024] __attribute__((aligned(4096))) = {0};
static pagestrap_t os_phys_pagestrap = {0};
static pagestrap_t os_virt_pagestrap = {0};
static pagestrap_alloc_t os_pagestrap_alloc = {0};

static vptr os_allocate_page();
static bool remove_bootstrap_region_if_overlap(pagestrap_t* pagestrap, vptr start_addr, vptr end_addr, vptr region_start, vptr region_end);
static bool remove_shared_regions(pagestrap_t* pagestrap, vptr start_addr, vptr end_addr);
static bool remove_virtual_regions(pagestrap_t* pagestrap, vptr start_addr, vptr end_addr);
static void init_pages(page_alloc_manager_t* manager);
static void map_os_regions();
static bool ranges_overlap(vptr start1, vptr end1, vptr start2, vptr end2);

bool pam_init_os() {
  os_page_alloc_manager.phys_pagestrap = &os_phys_pagestrap;
  os_page_alloc_manager.virt_pagestrap = &os_virt_pagestrap;
  os_page_alloc_manager.page_directory = os_page_directory;
  os_page_alloc_manager.page_table = os_page_table;
  os_page_alloc_manager.directory_phys_addr = ((u8*) os_page_directory) - __virt_kernel_start + __phys_kernel_start;
  os_page_alloc_manager.table_phys_addr = ((u8*) os_page_table) - __virt_kernel_start + __phys_kernel_start;

  pagestrap_init(&os_phys_pagestrap, &os_pagestrap_alloc, os_allocate_page);
  pagestrap_init(&os_virt_pagestrap, &os_pagestrap_alloc, os_allocate_page);

  init_pages(&os_page_alloc_manager);
  map_os_regions();
  pam_enable_page_alloc_manager(&os_page_alloc_manager);

  if (pagestrap_add_pages_se(&os_virt_pagestrap, (vptr) 0, (vptr) 0xFFFFFFFF)) return true;
  if (remove_virtual_regions(&os_virt_pagestrap, (vptr) 0, (vptr) 0xFFFFFFFF)) return true;

  if (mb2_mm_setup(&os_phys_pagestrap)) return true;

  return false;
}

// TODO: It would be nice if the page table entries weren't allocated until needed (and instead pointed to limited pte)
vptr pam_alloc_page(page_alloc_manager_t* manager, uX num_subsequent) {
  vptr virt_addr = pagestrap_allocate_page(manager->virt_pagestrap, num_subsequent);
  if (virt_addr == null) return null;

  // TODO: Cleanup phys_addr if this fails
  vptr phys_addr = pagestrap_allocate_page(manager->phys_pagestrap, num_subsequent);
  if (phys_addr == null) return null;

  u32 pde_index = (u32) virt_addr >> 22;
  u32 pte_index_base = pde_index * 1024 + (((u32) virt_addr >> 12) & 0x3FF);
  for (uX i = 0; i < num_subsequent; i++) {
    u32 pte_index = pte_index_base + i;
    manager->page_directory[pde_index].in_memory = 1;
    manager->page_table[pte_index].in_memory = 1;
    manager->page_table[pte_index].enable_write = 1;
    manager->page_table[pte_index].physical_reference = (u32) phys_addr >> 12;
  }

  return virt_addr;
}

vptr kalloc_pages(uX num_pages) {
  return pam_alloc_page(&os_page_alloc_manager, num_pages);
}

void pam_enable_page_alloc_manager(page_alloc_manager_t* alloc_manager) {
  asm volatile (
    "movl %0, %%eax\n\t"
    "andl $0xFFFFF000, %%eax\n\t"
    "movl %%eax, %%cr3"
    :
    : "r" (alloc_manager->directory_phys_addr)
    : "%eax", "memory", "cc"
  );
}

// TODO: Is this the proper file for this?
bool pam_remove_bootstrap_regions(pagestrap_t* pagestrap, vptr start_addr, vptr end_addr) {
  if (remove_shared_regions(pagestrap, start_addr, end_addr)) return true;
  if (remove_bootstrap_region_if_overlap(pagestrap, start_addr, end_addr, (vptr) __phys_kernel_start, (vptr) __phys_kernel_end)) return true;

  return false;
}

static vptr os_allocate_page() {
  return kalloc_pages(1);
}

static bool remove_bootstrap_region_if_overlap(pagestrap_t* pagestrap, vptr start_addr, vptr end_addr, vptr region_start, vptr region_end) {
  if (ranges_overlap(start_addr, end_addr, region_start, region_end)) {
    if (pagestrap_remove_pages_se(pagestrap, region_start, region_end)) return true;
  }
  return false;
}

static bool remove_shared_regions(pagestrap_t* pagestrap, vptr start_addr, vptr end_addr) {
  vptr multiboot2_end = (vptr) &_multiboot2_info + _multiboot2_info.total_size;
  if (remove_bootstrap_region_if_overlap(pagestrap, start_addr, end_addr, (vptr) &_multiboot2_info, multiboot2_end)) return true;
  if (remove_bootstrap_region_if_overlap(pagestrap, start_addr, end_addr, (vptr) 0xB8000, (vptr) 0xB9000)) return true;
  if (remove_bootstrap_region_if_overlap(pagestrap, start_addr, end_addr, (vptr) 0x0000, (vptr) 0x1000)) return true;

  return false;
}

static bool remove_virtual_regions(pagestrap_t* pagestrap, vptr start_addr, vptr end_addr) {
  if (remove_shared_regions(pagestrap, start_addr, end_addr)) return true;
  if (remove_bootstrap_region_if_overlap(pagestrap, start_addr, end_addr, (vptr) __virt_kernel_start, (vptr) __virt_kernel_end)) return true;
  return false;
}

static void init_pages(page_alloc_manager_t* manager) {
  volatile page_directory_entry_pte_t* page_directory = manager->page_directory;
  volatile page_table_entry_fourkb_t* page_table = manager->page_table;
  page_table_entry_fourkb_t* table_phys_addr = manager->table_phys_addr;
  for (int i = 0; i < 1024; i++) {
    page_directory[i].in_memory = 0;
    page_directory[i].pte_reference = ((u32) table_phys_addr + PAGE_TABLE_SIZE * i) >> 12;
    for (int j = 0; j < 1024; j++) {
      int page_index = i * 1024 + j;
      page_table[page_index].in_memory = 0;
      page_table[page_index].physical_reference = page_index;
    }
  }
}

static void ident_map_region(page_alloc_manager_t* manager, vptr start_addr, vptr end_addr) {
  volatile page_directory_entry_pte_t* page_directory = manager->page_directory;
  volatile page_table_entry_fourkb_t* page_table = manager->page_table;
  u32 start = ((u32) start_addr) & ~0xFFF;
  u32 end = (((u32) end_addr) + 0xFFF) & ~0xFFF;
  for (u32 i = (u32) start; i < (u32) end; i += _PS_PAGE_SIZE) {
    u32 directory_index = i >> 22;
    u32 page_index = directory_index * 1024 + (((u32) i >> 12) & 0x3FF);
    page_directory[directory_index].in_memory = 1;
    page_directory[directory_index].enable_write = 1;
    page_table[page_index].in_memory = 1;
    page_table[page_index].enable_write = 1;
    page_table[page_index].physical_reference = i >> 12;
  }
}

static void map_higher_half(page_alloc_manager_t* manager) {
  volatile page_directory_entry_pte_t* page_directory = manager->page_directory;
  volatile page_table_entry_fourkb_t* page_table = manager->page_table;
  u32 kernel_start = (u32) __phys_kernel_start;
  u32 kernel_end = (u32) __phys_kernel_end;
  u32 kernel_size = kernel_end - kernel_start;
  for (u32 i = 0; i < kernel_size; i += _PS_PAGE_SIZE) {
    u32 directory_index = ((u32) __virt_kernel_start + i) >> 22;
    page_directory[directory_index].in_memory = 1;
    page_directory[directory_index].enable_write = 1;
    u32 page_index = ((u32) __virt_kernel_start + i) >> 12;
    page_table[page_index].in_memory = 1;
    page_table[page_index].enable_write = 1;
    page_table[page_index].physical_reference = (kernel_start + i) >> 12;
  }
}

static void map_os_regions() {
  ident_map_region(&os_page_alloc_manager, (vptr) &_multiboot2_info, (vptr) &_multiboot2_info + _multiboot2_info.total_size);
  ident_map_region(&os_page_alloc_manager, (vptr) 0xB8000, (vptr) 0xB9000);
  map_higher_half(&os_page_alloc_manager);
}

static bool ranges_overlap(vptr start1, vptr end1, vptr start2, vptr end2) {
  return
    (start1 >= start2 && start1 < end2) ||
    (end1 > start2 && end1 <= end2) ||
    (start2 >= start1 && start2 < end1) ||
    (end2 > start1 && end2 <= end1);
}