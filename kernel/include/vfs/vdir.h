#ifndef __VFS_VDIR_H__
#define __VFS_VDIR_H__

#include <types.h>
#include <vector.h>

#define VFS_NODE_DIRECTORY 2

typedef struct vfs_vdir_t {
  s8 (*close) (uX node_id);
  s8 (*open) (uX node_id, uX* new_node_id, string_t name, u8 type, u8 mode);
  s8 (*size) (uX node_id, sX* size);
  s8 (*list) (uX node_id, vector_t* entry_list);
  s8 (*exists) (uX node_id, string_t name);
  s8 (*remove) (uX node_id, string_t item);
  s8 (*link) (uX node_id, uX link_node_id);
  s8 (*type) (u8* type);
} vfs_vdir_t;

bool vfs_init_dir(vfs_vdir_t* vfs_dir);

bool vfs_create_virt_dir(uX* node_id);

extern vfs_vdir_t vfs_root_dir;
extern uX vfs_root_node;

#endif