#ifndef __VFS_VDIR_H__
#define __VFS_VDIR_H__

#include <types.h>
#include <vector.h>

typedef struct vfs_vdir_handle_t {
  uX handle_id;
  sX (*size);
  s8 (*list) (vector_t* entry_list);
} vfs_vdir_handle_t;

typedef struct vfs_vdir_t {
  s8 (*remove) (string_t item);
  s8 (*mkdir) (string_t dirname);
  vfs_vdir_handle_t* (*open) (u8 mode, s8* err);
  void (*open) (vfs_vdir_handle_t* handle, s8* err);
} vfs_vdir_t;

#endif