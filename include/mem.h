#ifndef __MEM_H__
#define __MEM_H__

#include <mm/pagestrap.h>

vptr kmalloc(uX size);
vptr kalloc_pages(uX num_pages);
bool pam_remove_bootstrap_regions(pagestrap_t* pagestrap, vptr start_addr, vptr end_addr);
void mem_init();

#endif