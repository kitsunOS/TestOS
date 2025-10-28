#ifndef __VFS_VNODE_H__
#define __VFS_VNODE_H__

#include <types.h>

#include "vdir.h"
#include "vfile.h"

#define VFS_NODE_FILE 1
#define VFS_NODE_DIRECTORY 2

typedef struct vfs_vnode_t {
  u8 (*node_type);

  string_t (*name);
  uX (*length);
  
  bool (*can_write);

  vfs_vfile_t (*get_as_file);
  vfs_vdir_t (*get_as_dir);
} vfs_vnode_t;

#endif