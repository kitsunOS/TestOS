#ifndef __VFS_VFILE_H__
#define __VFS_VFILE_H__

#include <types.h>

#define VFS_SEEK_START 1
#define VFS_SEEK_END 2
#define VFS_SEEK_REL 3

typedef struct vfs_vfile_t {
  vfs_vfile_handle_t (*open);
} vfs_vfile_t;

typedef struct vfs_vfile_handle_t {
  bool (*close);
  sX (*read) (u8* buffer, u16 size);
  sX (*write) (u8* buffer, u16 size);
  s8 (*seek) (u8 mode, uX pos);
} vfs_vfile_handle_t;

#endif