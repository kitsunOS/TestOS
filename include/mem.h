#ifndef __MEM_H__
#define __MEM_H__

#include "pagestrap.h"

vptr kalloc_pages(uX num_pages);
bool pam_remove_bootstrap_regions(pagestrap_t* pagestrap, vptr start_addr, vptr end_addr);

#endif