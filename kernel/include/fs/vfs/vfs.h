#ifndef __FS_VFS_H__
#define __FS_VFS_H__

#include <types.h>
#include <fs/fsdir.h>

bool vfs_init_dir(fs_dir_t* vfs_dir);

bool vfs_create_virt_dir(uX* node_id);

extern fs_dir_t vfs_dir;
extern fs_file_t vfs_file;
extern uX vfs_root_node;

#endif