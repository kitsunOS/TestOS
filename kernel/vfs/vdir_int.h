#ifndef __VFS_VDIR_INTERNAL_H__
#define __VFS_VDIR_INTERNAL_H__

#include <vector.h>

typedef struct vfs_vdir_int_t {
  vector_t* node_list;
} vfs_vdir_int_t;

#endif