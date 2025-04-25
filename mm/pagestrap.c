
#include <types.h>
#include "pagestrap.h"

static pageref_t* obtain_pageref(pagestrap_t* pagestrap);
static uX pageref_end(pagestrap_t* pagestrap, pageref_t* page_ref);

void pagestrap_init(pagestrap_t* pagestrap, pagestrap_alloc_t* first_alloc, vptr (*os_allocate_page) (void)) {
  pagestrap->next_alloc = first_alloc;
  first_alloc->next_alloc = null;
  first_alloc->next_pointer = 0;
  pagestrap->is_mid_alloc = false;
  pagestrap->os_allocate_page = os_allocate_page;
  pagestrap->unit_size = _PS_PAGE_SIZE;
  pagestrap->available_pages_root.num_subsequent = 0;
  pagestrap->available_pages_root.page_addr = 0;
  pagestrap->available_pages_root.next_page_ref = null;
  pagestrap->freelist_root.num_subsequent = 0;
  pagestrap->freelist_root.page_addr = 0;
  pagestrap->freelist_root.next_page_ref = null;
  pagestrap->used_page_refs_root.num_subsequent = 0;
  pagestrap->used_page_refs_root.page_addr = 0;
  pagestrap->used_page_refs_root.next_page_ref = null;

  first_alloc->next_pointer = 0;
}

bool pagestrap_add_pages(pagestrap_t* pagestrap, uX start_addr, uX num_subsequent) {
  pageref_t* page_ref = obtain_pageref(pagestrap);
  if (page_ref == null) return true;

  pageref_t* prev_ref = &(pagestrap->available_pages_root);
  while (
    prev_ref->next_page_ref != null
    && start_addr < prev_ref->next_page_ref->page_addr
  ) {
    prev_ref = prev_ref->next_page_ref;
  }

  page_ref->num_subsequent = num_subsequent;
  page_ref->page_addr = start_addr;
  page_ref->next_page_ref = prev_ref->next_page_ref;
  prev_ref->next_page_ref = page_ref;

  return false;
}

bool pagestrap_add_pages_se(pagestrap_t* pagestrap, vptr start_addr, vptr end_addr) {
  // TODO: What if they're not bounded perfectly
  uX num_subsequent = ((uX) end_addr - (uX) start_addr) / _PS_PAGE_SIZE;
  return pagestrap_add_pages(pagestrap, (uX) start_addr, num_subsequent);
}

bool pagestrap_remove_pages(pagestrap_t* pagestrap, uX start_addr, uX num_subsequent) {
  uX end_addr = start_addr + num_subsequent * _PS_PAGE_SIZE;
  pageref_t* prev_ref = &(pagestrap->available_pages_root);
  pageref_t* page_ref = prev_ref->next_page_ref;
  while (page_ref != null && pageref_end(pagestrap, page_ref) <= start_addr) {
    prev_ref = page_ref;
    page_ref = page_ref->next_page_ref;
  }

  if (page_ref == null) return false;
  
  while (page_ref != null && page_ref->page_addr < end_addr) {
    pageref_t* next_ref = page_ref->next_page_ref;
    uX page_end = pageref_end(pagestrap, page_ref);
    if (page_ref->page_addr == start_addr && page_end <= end_addr) {
      // All of pageref is consumed
      prev_ref->next_page_ref = page_ref->next_page_ref;
      page_ref->next_page_ref = pagestrap->freelist_root.next_page_ref;
      pagestrap->freelist_root.next_page_ref = page_ref;
    } else if (page_ref->page_addr == start_addr) {
      // Head of pageref is consumed
      uX old_num_subsequent = page_ref->num_subsequent;
      page_ref->num_subsequent = (page_end - end_addr) / _PS_PAGE_SIZE;
      page_ref->page_addr += (old_num_subsequent - page_ref->num_subsequent) * _PS_PAGE_SIZE;
      prev_ref = page_ref;
    } else if (page_end <= end_addr) {
      // Tail of pageref is consumed
      uX num_subsequent = (start_addr - page_ref->page_addr) / _PS_PAGE_SIZE;
      page_ref->num_subsequent = num_subsequent;
      prev_ref = page_ref;
    } else {
      // Subsection of pageref is consumed
      pageref_t* new_next_ref = obtain_pageref(pagestrap);
      if (new_next_ref == null) return true;
      new_next_ref->page_addr = end_addr;
      new_next_ref->num_subsequent = (page_end - end_addr) / _PS_PAGE_SIZE;
      new_next_ref->next_page_ref = next_ref;
      page_ref->num_subsequent = (start_addr - page_ref->page_addr) / _PS_PAGE_SIZE;
      page_ref->next_page_ref = new_next_ref;
      prev_ref = new_next_ref;
    }
    if (page_end < end_addr) {
      start_addr = page_end;
    }
    page_ref = next_ref;
  }

  return false;
}

bool pagestrap_remove_pages_se(pagestrap_t* pagestrap, vptr start_addr, vptr end_addr) {
  // TODO: What if they're not bounded perfectly
  return pagestrap_remove_pages(pagestrap, (uX) start_addr, ((uX) end_addr - (uX) start_addr) / _PS_PAGE_SIZE);
}

vptr pagestrap_allocate_page(pagestrap_t* pagestrap, uX num_subsequent) {
  pageref_t* prev_ref = &(pagestrap->available_pages_root);
  pageref_t* page_ref = prev_ref->next_page_ref;
  while (page_ref != null && page_ref->num_subsequent < num_subsequent) {
    prev_ref = page_ref;
    page_ref = page_ref->next_page_ref;
  }
  if (page_ref == null) return null;

  pageref_t* new_next_ref;
  if (page_ref->num_subsequent > num_subsequent) {
    new_next_ref = obtain_pageref(pagestrap);
    if (new_next_ref == null) return null;
    new_next_ref->num_subsequent = page_ref->num_subsequent - num_subsequent;
    new_next_ref->page_addr = page_ref->page_addr + num_subsequent * pagestrap->unit_size;
    new_next_ref->next_page_ref = page_ref->next_page_ref;
    page_ref->num_subsequent = num_subsequent;
  } else {
    new_next_ref = page_ref->next_page_ref;
  }
  prev_ref->next_page_ref = new_next_ref;

  page_ref->next_page_ref = pagestrap->used_page_refs_root.next_page_ref;
  pagestrap->used_page_refs_root.next_page_ref = page_ref;

  return (vptr) page_ref->page_addr;
}

static bool refill_if_needed(pagestrap_t* pagestrap) {
  pagestrap_alloc_t* prev_alloc = pagestrap->next_alloc;
  if (prev_alloc->next_pointer == _PS_NUM_PAGES_PER_ALLOC && prev_alloc->next_alloc == null) {
    return true;
  } else if (prev_alloc->next_pointer >= _PS_NUM_PAGES_PER_ALLOC - 5 && !pagestrap->is_mid_alloc) {
    pagestrap->is_mid_alloc = true;
    pagestrap_alloc_t* new_alloc = (pagestrap_alloc_t*) pagestrap->os_allocate_page();
    pagestrap->is_mid_alloc = false;
    if (new_alloc == null) return true;
    prev_alloc->next_alloc = new_alloc;
    pagestrap->next_alloc = new_alloc;
    new_alloc->next_pointer = 0;
    new_alloc->next_alloc = null;
  }
  return false;
}

static pageref_t* obtain_pageref(pagestrap_t* pagestrap) {
  pageref_t* page_ref = pagestrap->freelist_root.next_page_ref;
  if (page_ref) {
    pagestrap->freelist_root.next_page_ref = page_ref->next_page_ref;
  } else {
    if (refill_if_needed(pagestrap)) return null;
    page_ref = &(pagestrap->next_alloc)->page_refs[pagestrap->next_alloc->next_pointer++];
  }
  page_ref->next_page_ref = null;

  return page_ref;
}

static uX pageref_end(pagestrap_t* pagestrap, pageref_t* page_ref) {
  return page_ref->page_addr + page_ref->num_subsequent * _PS_PAGE_SIZE;
}