#ifndef __I386_PAGE_ALLOC_H__
#define __I386_PAGE_ALLOC_H__
#include <types.h>
#include <mm/pagestrap.h>
#include "pages.h"

typedef struct page_alloc_manager_t {
  pagestrap_t* phys_pagestrap;
  pagestrap_t* virt_pagestrap;
  volatile page_directory_entry_pte_t* page_directory;
  volatile page_table_entry_fourkb_t* page_table;
  void* directory_phys_addr;
  void* table_phys_addr;
} page_alloc_manager_t;

bool pam_init_os();
vptr pam_alloc_page(page_alloc_manager_t* manager, uX num_subsequent);
void pam_enable_page_alloc_manager(page_alloc_manager_t* alloc_manager);

#endif