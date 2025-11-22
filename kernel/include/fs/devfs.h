#ifndef __FS_DEVFS_H__
#define __FS_DEVFS_H__

#include <types.h>
#include <fs/fsmod.h>

typedef struct devfs_charfile_t {
  s8 (*open) (uX* new_node_id, u8 mode, vptr meta);
  sX (*read) (uX node_id, u8* buffer, u16 size);
  sX (*write) (uX node_id, u8* buffer, u16 size);
  s8 (*close) (uX node_id);
} devfs_charfile_t;

bool devfs_init();

bool devfs_init_module(fs_module_t* devfs_module);

bool devfs_create_dir(uX* node_id, string_t name);

s8 devfs_create_character_file(uX node_id, string_t name, devfs_charfile_t* charfile, vptr meta);

s8 devfs_release_character_file(uX node_id, string_t name);

extern fs_module_t devfs_module;
extern uX devfs_root_node;

#endif