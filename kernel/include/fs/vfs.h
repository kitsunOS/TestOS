#ifndef __FS_VFS_H__
#define __FS_VFS_H__

#include <types.h>
#include <fs/fsmod.h>

bool vfs_init_module(fs_module_t* vfs_module);

bool vfs_create_dir(uX* node_id);

extern fs_module_t vfs_module;
extern uX vfs_root_node;

bool vfs_init();

#endif