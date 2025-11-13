#include <fs/vfs/vfs.h>
#include <fs/fsdir.h>

#include <mm/mem.h>
#include <memory.h>
#include <vector.h>

#include "vdir_int.h"

static s8 vdir_type(uX node_id, u8* type);
static s8 vdir_close(uX node_id);
static s8 vdir_size(uX node_id, sX* size);

static s8 vdir_open(uX node_id, uX* new_node_id, string_t name, u8 type, u8 mode);
static s8 vdir_list(uX node_id, vector_t* entry_list);
static s8 vdir_exists(uX node_id, string_t name);
static s8 vdir_remove(uX node_id, string_t name);
static s8 vdir_mount(uX node_id, fs_dir_t* mount_dirtype, uX mount_node_id);
static s8 vdir_unmount(uX node_id);
// static s8 vdir_link(uX node_id, uX link_node_id);

static bool vfs_create_virt_dir_addr(vfs_vdir_int_t** node_addr);

fs_dir_t vfs_dir;
uX vfs_root_node;

bool vfs_init_dir(fs_dir_t* vfs_dir) {
  vfs_dir->type = vdir_type;
  vfs_dir->close = vdir_close;
  vfs_dir->size = vdir_size;

  vfs_dir->open = vdir_open;
  vfs_dir->list = vdir_list;
  vfs_dir->exists = vdir_exists;
  vfs_dir->remove = vdir_remove;
  vfs_dir->mount = vdir_mount;
  vfs_dir->unmount = vdir_unmount;

  return true;
}

bool vfs_create_virt_dir(uX* node_id) {
  return vfs_create_virt_dir_addr((vfs_vdir_int_t**) node_id);
}

static bool vfs_create_virt_dir_addr(vfs_vdir_int_t** node_addr) {
  vfs_vdir_int_t* node = kmalloc(sizeof(vfs_vdir_int_t));
  if (!node) return false;

  memfill(node, sizeof(vfs_vdir_int_t), 0);

  if (!vector_init(&(node->node_list), 16, .5)) return false;

  *node_addr = node;
  return true;
}

//

static s8 vdir_type(uX node_id, u8* type) {
  vfs_vdir_int_t* node = (vfs_vdir_int_t*) node_id;
  if (node->mount_dirtype != null && node->mount_node_id != 0) {
    return node->mount_dirtype->type(node->mount_node_id, type);
  }
  if (node->backing_dirtype != null && node->backing_node_id != 0) {
    return node->backing_dirtype->type(node->backing_node_id, type);
  }

  *type = FS_NODE_DIRECTORY;
  return true;
}

static s8 vdir_close(uX node_id) {
  // TODO: Handle unmounting the child when it is a mount
  vfs_vdir_int_t* node = (vfs_vdir_int_t*) node_id;
  if (node->mount_dirtype != null && node->mount_node_id != 0) {
    s8 status = vdir_unmount(node_id);
    if (status < 0) return status;
  }

  if (node->backing_dirtype != null && node->backing_node_id != 0) {
    s8 status = node->mount_dirtype->close(node->mount_node_id);
    if (status < 0) return status;
  }

  vector_free(&(node->node_list));
  kfree(node);

  return true;
}

static s8 vdir_size(uX node_id, sX* size) {
  vfs_vdir_int_t* node = (vfs_vdir_int_t*) node_id;
  if (node->mount_dirtype != null && node->mount_node_id != 0) {
    return node->mount_dirtype->size(node->mount_node_id, size);
  }
  if (node->backing_dirtype != null && node->backing_node_id != 0) {
    return node->backing_dirtype->size(node->backing_node_id, size);
  }
  
  *size = 0;
  return true;
}

//

static s8 vdir_open(uX node_id, uX* new_node_id, string_t name, u8 type, u8 mode) {
  vfs_vdir_int_t* node = (vfs_vdir_int_t*) node_id;
  if (
    (node->mount_dirtype == null || node->mount_node_id == 0)
    && (node->backing_dirtype == null || node->backing_node_id == 0)
  ) {
    return FS_ERR_NOT_MOUNTED;
  }

  fs_dir_t* dirtype = node->mount_dirtype != null && node->mount_node_id != 0 ?
    node->mount_dirtype : node->backing_dirtype;

  uX uw_node_id = node->mount_dirtype != null && node->mount_node_id != 0 ?
    node->mount_node_id : node->backing_node_id;

  uX inner_node_id;
  s8 status = dirtype->open(uw_node_id, &inner_node_id, name, type, mode);
  if (status < 1) return status;

  // TODO: Track the actual type for bookkeeping.
  vfs_vdir_int_t* new_node;
  if (!vfs_create_virt_dir_addr(&new_node)) return FS_ERR_NO_MEM;

  new_node->mount_dirtype = dirtype;
  new_node->mount_node_id = inner_node_id;

  *new_node_id = (uX) new_node;

  return true;
}

static s8 vdir_list(uX node_id, vector_t* entry_list) {
  vfs_vdir_int_t* node = (vfs_vdir_int_t*) node_id;
  if (node->mount_dirtype != null && node->mount_node_id != 0) {
    return node->mount_dirtype->list(node->mount_node_id, entry_list);
  }
  if (node->backing_dirtype != null && node->backing_node_id != 0) {
    return node->backing_dirtype->list(node->backing_node_id, entry_list);
  }

  *entry_list = node->node_list;
  return true;
}

static s8 vdir_exists(uX node_id, string_t name) {
  vfs_vdir_int_t* node = (vfs_vdir_int_t*) node_id;
  if (node->mount_dirtype != null && node->mount_node_id != 0) {
    return node->mount_dirtype->exists(node->mount_node_id, name);
  }
  if (node->backing_dirtype != null && node->backing_node_id != 0) {
    return node->backing_dirtype->exists(node->backing_node_id, name);
  }
  
  return false;
}

static s8 vdir_remove(uX node_id, string_t name) {
  vfs_vdir_int_t* node = (vfs_vdir_int_t*) node_id;

  // TODO: Should I make size return number of directory entries, to use that instead?
  vector_t file_list;
  u8 status = vdir_list(node_id, &file_list);
  if (status < 0) return status;

  if (vector_length(&file_list) != 0) {
    return FS_ERR_NOT_EMPTY;
  }

  if (node->mount_dirtype != null && node->mount_node_id != 0) {
    return node->mount_dirtype->remove(node->mount_node_id, name);
  }
  if (node->backing_dirtype != null && node->backing_node_id != 0) {
    return node->backing_dirtype->remove(node->backing_node_id, name);
  }
  
  return FS_ERR_NOT_EXISTS;
}

static s8 vdir_mount(uX node_id, fs_dir_t* mount_dirtype, uX mount_node_id) {
  vfs_vdir_int_t* node = (vfs_vdir_int_t*) node_id;
  if (node->mount_dirtype != null && node->mount_node_id != 0) {
    return FS_ERR_ALREADY_MOUNTED;
  }

  vector_t file_list;
  u8 status = vdir_list(node_id, &file_list);
  if (status < 0) return status;

  if (vector_length(&file_list) != 0) {
    return FS_ERR_NOT_EMPTY;
  }

  node->mount_dirtype = mount_dirtype;
  node->mount_node_id = mount_node_id;

  return true;
}

static s8 vdir_unmount(uX node_id) {
  vfs_vdir_int_t* node = (vfs_vdir_int_t*) node_id;
  if (node->mount_dirtype == null || node->mount_node_id == 0) {
    return FS_ERR_NOT_MOUNTED;
  }

  node->mount_dirtype = null;
  node->mount_node_id = 0;
  // TODO: Close it too?

  return true;
}