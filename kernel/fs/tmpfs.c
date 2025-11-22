#include <fs/tmpfs.h>
#include <fs/fsmod.h>

#include <mm/mem.h>
#include <memory.h>
#include <vector.h>
#include <strings.h>

typedef struct tmpfs_node_int_t {
  u8 type;
  string_t name;
} tmpfs_node_int_t;

typedef struct tmpfs_tmpfsd_int_t {
  u8 type;
  string_t name;
  vector_t node_list;
} tmpfs_tmpfsd_int_t;

// TODO: Actually support files...
typedef struct tmpfs_tmpfsf_int_t {
  u8 type;
  string_t name;
} tmpfs_tmpfsf_int_t;

static s8 tmpfsd_type(uX node_id, u8* type);
static s8 tmpfsd_close(uX node_id);
static s8 tmpfsd_size(uX node_id, sX* size);

static s8 tmpfsd_open(uX node_id, uX* new_node_id, string_t name, u8 type, u8 mode);
static s8 tmpfsd_list(uX node_id, vector_t* entry_list);
static s8 tmpfsd_exists(uX node_id, string_t name);
static s8 tmpfsd_remove(uX node_id, string_t name);
static s8 tmpfsd_mount(uX node_id, fs_module_t* mount_dirtype, uX mount_node_id);
static s8 tmpfsd_unmount(uX node_id);
// static s8 tmpfsd_link(uX node_id, uX link_node_id);

static sX tmpfsf_read(uX node_id, u8* buffer, u16 size);
static s8 tmpfsf_write(uX node_id, u8* buffer, u16 size);

static bool tmpfs_create_dir_addr(tmpfs_tmpfsd_int_t** node_addr, string_t name);

//

static s8 tmpfsd_find_file(uX node_id, uX* result_id, string_t name);

//

fs_module_t tmpfs_module;
uX tmpfs_root_node;

bool tmpfs_init_module(fs_module_t* tmpfs_module) {
  tmpfs_module->type = tmpfsd_type;
  tmpfs_module->close = tmpfsd_close;
  tmpfs_module->size = tmpfsd_size;

  tmpfs_module->open = tmpfsd_open;
  tmpfs_module->list = tmpfsd_list;
  tmpfs_module->exists = tmpfsd_exists;
  tmpfs_module->remove = tmpfsd_remove;
  tmpfs_module->mount = tmpfsd_mount;
  tmpfs_module->unmount = tmpfsd_unmount;

  tmpfs_module->read = tmpfsf_read;
  tmpfs_module->write = tmpfsf_write;

  return true;
}

bool tmpfs_create_dir(uX* node_id, string_t name) {
  return tmpfs_create_dir_addr((tmpfs_tmpfsd_int_t**) node_id, name);
}

static bool tmpfs_create_dir_addr(tmpfs_tmpfsd_int_t** node_addr, string_t name) {
  tmpfs_tmpfsd_int_t* node = kmalloc(sizeof(tmpfs_tmpfsd_int_t));
  if (!node) return false;

  memfill(node, sizeof(tmpfs_tmpfsd_int_t), 0);

  node->type = FS_NODE_DIRECTORY;
  node->name = name;
  if (!vector_init(&(node->node_list), 16, 15)) return false;

  *node_addr = node;
  return true;
}

//

static s8 tmpfsd_type(uX node_id, u8* type) {
  *type = FS_NODE_DIRECTORY;
  return true;
}

static s8 tmpfsd_close(uX node_id) {
  tmpfs_tmpfsd_int_t* node = (tmpfs_tmpfsd_int_t*) node_id;
  
  if (node->type == FS_NODE_HANDLE_CHAR) {
    return FS_ERR_UNIMPLEMENTED;
  }
  
  vector_free(&(node->node_list));
  kfree(node);

  return true;
}

static s8 tmpfsd_size(uX node_id, sX* size) {
  *size = 0;

  return true;
}

//

static s8 tmpfsd_find_file(uX node_id, uX* result_id, string_t name) {
  tmpfs_tmpfsd_int_t* node = (tmpfs_tmpfsd_int_t*) node_id;
  if (node->type != FS_NODE_DIRECTORY) return FS_ERR_WRONG_TYPE;

  for (sX i = 0; i < vector_length(&node->node_list); i++) {
    tmpfs_node_int_t* current_node = vector_get(&node->node_list, i);
    if (streq(current_node->name, name)) {
      *result_id = (uX) current_node;
      return true;
    }
  }

  return false;
}

static s8 tmpfsd_create_dir(tmpfs_tmpfsd_int_t* parent_node, string_t name, tmpfs_node_int_t** match_node, u8 mode) {
  tmpfs_tmpfsd_int_t* new_node;
  if (!tmpfs_create_dir_addr(&new_node, name)) return FS_ERR_NO_MEM;
  if (vector_add(&parent_node->node_list, new_node) < 0) return FS_ERR_NO_MEM;

  *match_node = (tmpfs_node_int_t*) new_node;

  return true;
}

static s8 tmpfsd_open_dir(uX node_id, uX* new_node_id, tmpfs_node_int_t* match_node, u8 mode) {
  *new_node_id = (uX) match_node;
  return true;
}

static s8 tmpfsd_open_file(uX node_id, uX* new_node_id, tmpfs_node_int_t* match_node, u8 mode) {
  return FS_ERR_UNIMPLEMENTED;
}

//

// TODO: validate type-mode combo, return error if not valid
static s8 tmpfsd_open(uX node_id, uX* new_node_id, string_t name, u8 type, u8 mode) {
  tmpfs_tmpfsd_int_t* node = (tmpfs_tmpfsd_int_t*) node_id;
  if (node->type != FS_NODE_DIRECTORY) return FS_ERR_WRONG_TYPE;

  tmpfs_node_int_t* match_node = null;
  for (sX i = 0; i < vector_length(&node->node_list); i++) {
    match_node = vector_get(&node->node_list, i);
    if (streq(match_node->name, name)) break;
    match_node = null;
  }

  if (match_node != null && match_node->type != type) return FS_ERR_WRONG_TYPE;

  switch (mode & 0x0C) {
  case FS_M_OPEN:
    if (match_node != null) break;
    return FS_ERR_NOT_EXISTS;
  case FS_M_CREATE:
    if (match_node == null) break;
    return FS_ERR_ALREADY_EXISTS;
  case FS_M_APPEND:
  case FS_M_REPLACE:
    break;
  default:
    return FS_ERR_NOT_SUPPORTED;
  }

  switch (type) {
  case FS_NODE_DIRECTORY:
    if (match_node == null) {
      s8 result = tmpfsd_create_dir(node, name, &match_node, mode);
      if (result < 0) return result;
    }
    return tmpfsd_open_dir(node_id, new_node_id, match_node, mode);
  case FS_NODE_FILE:
    return tmpfsd_open_file(node_id, new_node_id, match_node, mode);
  default:
    return FS_ERR_UNIMPLEMENTED;
  }
}

static s8 tmpfsd_list(uX node_id, vector_t* entry_list) {
  tmpfs_tmpfsd_int_t* node = (tmpfs_tmpfsd_int_t*) node_id;
  
  *entry_list = node->node_list;
  return true;
}

static s8 tmpfsd_exists(uX node_id, string_t name) {
  uX result_id;
  return tmpfsd_find_file(node_id, &result_id, name);
}

static s8 tmpfsd_remove(uX node_id, string_t name) {
  return FS_ERR_UNIMPLEMENTED;
}

static s8 tmpfsd_mount(uX node_id, fs_module_t* mount_dirtype, uX mount_node_id) {
  return FS_ERR_NOT_SUPPORTED;
}

static s8 tmpfsd_unmount(uX node_id) {
  return FS_ERR_NOT_SUPPORTED;
}

//

sX tmpfsf_read(uX node_id, u8* buffer, u16 size) {
  return FS_ERR_UNIMPLEMENTED;
}

s8 tmpfsf_write(uX node_id, u8* buffer, u16 size) {
  return FS_ERR_UNIMPLEMENTED;
}