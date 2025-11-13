#ifndef __FS_FSDIR_H__
#define __FS_FSDIR_H__

#include <types.h>
#include <vector.h>

#include "fsnode.h"
#include "fsfile.h"

#define FS_NODE_DIRECTORY 2

typedef struct fs_dir_t {
  fs_file_t file;

  s8 (*type) (uX node_id, u8* type);
  s8 (*close) (uX node_id);
  s8 (*size) (uX node_id, sX* size);
  
  s8 (*open) (uX node_id, uX* new_node_id, string_t name, u8 type, u8 mode);
  s8 (*list) (uX node_id, vector_t* entry_list);
  s8 (*exists) (uX node_id, string_t name);
  s8 (*remove) (uX node_id, string_t item);
  s8 (*mount) (uX node_id, struct fs_dir_t* mount_dirtype, uX mount_node_id);
  s8 (*unmount) (uX node_id);
  // s8 (*link) (uX node_id, uX link_node_id);
} fs_dir_t;

#endif