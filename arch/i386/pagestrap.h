#ifndef __MM_PAGESTRAP_H__
#define __MM_PAGESTRAP_H__

#include <types.h>

#define _PS_PAGE_SIZE 4096
#define _PS_NUM_PAGES_PER_ALLOC ((_PS_PAGE_SIZE - sizeof(u16) - sizeof(struct pagestrap_alloc_t*)) / sizeof(pageref_t))

typedef struct pageref_t {
  uX num_subsequent;
  vptr phys_ref;
  struct pageref_t* next_page_ref;
} pageref_t;

typedef struct pagestrap_alloc_t {
  u16 next_pointer;
  pageref_t page_refs[_PS_NUM_PAGES_PER_ALLOC];
  struct pagestrap_alloc_t* next_alloc;
} pagestrap_alloc_t;

typedef struct pagestrap_t {
  pagestrap_alloc_t* next_alloc;
  pageref_t available_pages_root;
  pageref_t freelist_root;
  pageref_t used_page_refs_root;
  bool is_mid_alloc;
  vptr (*os_allocate_page) (void);
} pagestrap_t;

void pagestrap_init(pagestrap_t* pagestrap, pagestrap_alloc_t* first_alloc, vptr (*os_allocate_page) (void));
vptr pagestrap_allocate_page(pagestrap_t* pagestrap, uX num_subsequent);
bool pagestrap_add_pages(pagestrap_t* pagestrap, vptr start_addr, uX num_subsequent);
bool pagestrap_add_pages_se(pagestrap_t* pagestrap, vptr start_addr, vptr end_addr);
bool pagestrap_remove_pages(pagestrap_t* pagestrap, vptr start_addr, uX num_subsequent);
bool pagestrap_remove_pages_se(pagestrap_t* pagestrap, vptr start_addr, vptr end_addr);

#endif