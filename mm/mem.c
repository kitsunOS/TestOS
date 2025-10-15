#include <types.h>
#include <mm/mem.h>
#include <mm/pagestrap.h>

#define NUM_PAGES_SPARE 20

static pagestrap_t pagestrap;
static pagestrap_alloc_t first_alloc;

static vptr alloc_one_page();

void mem_init() {
  pagestrap_init(&pagestrap, &first_alloc, alloc_one_page);
  pagestrap.unit_size = 1;
}

vptr kmalloc(uX size) {
  if (size == 0) return null;
  vptr mem = pagestrap_allocate_page(&pagestrap, size);
  if (mem == null) {
    uX num_pages = size / _PS_PAGE_SIZE + NUM_PAGES_SPARE;
    vptr page = kalloc_pages(num_pages);
    if (page == null) return null;
    if (pagestrap_add_pages(&pagestrap, (uX) page, num_pages * _PS_PAGE_SIZE)) {
      return null;
    }
    mem = pagestrap_allocate_page(&pagestrap, size);
  }

  return mem;
}

static vptr alloc_one_page() {
  return kalloc_pages(1);
}