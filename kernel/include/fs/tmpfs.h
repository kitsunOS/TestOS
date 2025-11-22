#ifndef __FS_TEMPFS_H__
#define __FS_TEMPFS_H__

#include <types.h>
#include <fs/fsmod.h>

bool tmpfs_init_module(fs_module_t* tmpfs_module);

bool tmpfs_create_dir(uX* node_id, string_t name);

extern fs_module_t tmpfs_module;

#endif