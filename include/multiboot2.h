#ifndef __MULTIBOOT2_H__
#define __MULTIBOOT2_H__

#include "types.h"

typedef struct __attribute__((packed)) {
    u32 total_size;
    u32 reserved;
} multiboot2_header_t;

typedef struct __attribute__((packed)) {
    u32 type;
    u32 size;
} multiboot2_tag_t;

typedef struct __attribute__((packed)) {
    u32 type;
    u32 size;
    u32 entry_size;
    u32 entry_version;
} multiboot2_memory_map_tag_t;

typedef struct __attribute__((packed)) {
    u32 base_addr_low;
    u32 base_addr_high;
    u32 length_low;
    u32 length_high;
    u32 type;
    u32 reserved;
} multiboot2_memory_map_entry_t;

#endif