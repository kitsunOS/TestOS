#ifndef __VFS_VDIR_INTERNAL_H__
#define __VFS_VDIR_INTERNAL_H__

#include <vector.h>

typedef struct vfs_vdir_int_t {
  vector_t node_list;
  fs_dir_t* mount_dirtype;
  uX mount_node_id;
  fs_dir_t* backing_dirtype;
  uX backing_node_id;
} vfs_vdir_int_t;

#endif