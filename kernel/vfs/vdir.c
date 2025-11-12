#include <vfs/vdir.h>
#include <mm/mem.h>

#include "vdir_int.h"

static s8 vdir_close(uX node_id);
static s8 vdir_open(uX node_id, uX* new_node_id, string_t name, u8 type, u8 mode);
static s8 vdir_size(uX node_id, sX* size);
static s8 vdir_list(uX node_id, vector_t* entry_list);
static s8 vdir_exists(uX node_id, string_t name);
static s8 vdir_remove(uX node_id, string_t item);
static s8 vdir_link(uX node_id, uX link_node_id);
static s8 vdir_type(u8* type);

vfs_vdir_t vfs_root_dir;
uX vfs_root_node;

bool vfs_init_dir(vfs_vdir_t* vfs_dir) {
  vfs_dir->close = vdir_close;

  return true;
}

bool vfs_create_virt_dir(uX* node_id) {
  vfs_vdir_int_t* node = kmalloc(sizeof(vfs_vdir_int_t));
  if (!node) return false;

  vector_init(node->node_list, 16, .5);

  *node_id = (uX) node;
  return true;
}

static s8 vdir_close(uX node_id) {
  vfs_vdir_int_t* node = (vfs_vdir_int_t*) node_id;
  vector_free(node->node_list);
  kfree(node);
}