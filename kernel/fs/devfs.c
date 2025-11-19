#include <fs/devfs.h>
#include <fs/fsmod.h>

#include <mm/mem.h>
#include <memory.h>
#include <vector.h>
#include <strings.h>

typedef struct devfs_node_int_t {
  u8 type;
  string_t name;
} devfs_node_int_t;

typedef struct devfs_dvfsd_int_t {
  u8 type;
  vector_t node_list;
} devfs_dvfsd_int_t;

typedef struct devfs_dvfsf_int_t {
  u8 type;
  devfs_charfile_t* charfile;
  vptr meta;
} devfs_dvfsf_int_t;

typedef struct devfs_dvfsf_handle_t {
  u8 type;
  devfs_charfile_t* charfile;
  uX inner_node_handle;
} devfs_dvfsf_handle_t;

static s8 dvfsd_type(uX node_id, u8* type);
static s8 dvfsd_close(uX node_id);
static s8 dvfsd_size(uX node_id, sX* size);

static s8 dvfsd_open(uX node_id, uX* new_node_id, string_t name, u8 type, u8 mode);
static s8 dvfsd_list(uX node_id, vector_t* entry_list);
static s8 dvfsd_exists(uX node_id, string_t name);
static s8 dvfsd_remove(uX node_id, string_t name);
static s8 dvfsd_mount(uX node_id, fs_module_t* mount_dirtype, uX mount_node_id);
static s8 dvfsd_unmount(uX node_id);
// static s8 dvfsd_link(uX node_id, uX link_node_id);

static bool devfs_create_dir_addr(devfs_dvfsd_int_t** node_addr);

//

static s8 dvfsd_find_file(uX node_id, uX* result_id, string_t name);

//

fs_module_t devfs_module;

bool devfs_init_module(fs_module_t* devfs_module) {
  devfs_module->type = dvfsd_type;
  devfs_module->close = dvfsd_close;
  devfs_module->size = dvfsd_size;

  devfs_module->open = dvfsd_open;
  devfs_module->list = dvfsd_list;
  devfs_module->exists = dvfsd_exists;
  devfs_module->remove = dvfsd_remove;
  devfs_module->mount = dvfsd_mount;
  devfs_module->unmount = dvfsd_unmount;

  return true;
}

bool devfs_create_dir(uX* node_id) {
  return devfs_create_dir_addr((devfs_dvfsd_int_t**) node_id);
}

s8 devfs_create_character_file(uX node_id, string_t name, devfs_charfile_t* charfile, vptr meta) {
  devfs_dvfsd_int_t* node = (devfs_dvfsd_int_t*) node_id;
  if (node->type != FS_NODE_DIRECTORY) return FS_ERR_WRONG_TYPE;

  s8 exists_result = dvfsd_exists(node_id, name);
  if (exists_result < 0) return exists_result;
  if (exists_result) return FS_ERR_ALREADY_EXISTS;

  devfs_dvfsf_int_t* fnode = kmalloc(sizeof(devfs_dvfsf_int_t));
  if (!fnode) return FS_ERR_NO_MEM;
  fnode->type = FS_NODE_FILE;
  fnode->charfile = charfile;
  fnode->meta = meta;

  sX result = vector_add((vptr) node, fnode);
  if (result < 0) return result;

  return true;
}

s8 devfs_release_character_file(uX node_id, string_t name) {
  devfs_dvfsd_int_t* node = (devfs_dvfsd_int_t*) node_id;
  if (node->type != FS_NODE_DIRECTORY) return FS_ERR_WRONG_TYPE;

  devfs_node_int_t* match_node;
  sX i;
  for (i = 0; i < vector_length(&node->node_list); i++) {
    match_node = vector_get(&node->node_list, i);
    if (streq(match_node->name, name)) break;
    match_node = null;
  }

  if (match_node == null) return FS_ERR_NOT_EXISTS;
  if (match_node->type != FS_NODE_FILE) return FS_ERR_WRONG_TYPE;

  // TODO: Remove from list

  kfree(match_node);
}

static bool devfs_create_dir_addr(devfs_dvfsd_int_t** node_addr) {
  devfs_dvfsd_int_t* node = kmalloc(sizeof(devfs_dvfsd_int_t));
  if (!node) return false;

  // TODO: List will also need to store file housekeeping
  memfill(node, sizeof(devfs_dvfsd_int_t), 0);

  if (!vector_init(&(node->node_list), 16, 15)) return false;

  *node_addr = node;
  return true;
}

//

static s8 dvfsd_type(uX node_id, u8* type) {
  *type = FS_NODE_DIRECTORY;
  return true;
}

static s8 dvfsd_close(uX node_id) {
  devfs_dvfsd_int_t* node = (devfs_dvfsd_int_t*) node_id;
  
  vector_free(&(node->node_list));
  kfree(node);

  return true;
}

static s8 dvfsd_size(uX node_id, sX* size) {
  *size = 0;

  return true;
}

//

static s8 dvfsd_find_file(uX node_id, uX* result_id, string_t name) {
  devfs_dvfsd_int_t* node = (devfs_dvfsd_int_t*) node_id;
  if (node->type != FS_NODE_DIRECTORY) return FS_ERR_WRONG_TYPE;

  for (sX i = 0; i < vector_length(&node->node_list); i++) {
    devfs_node_int_t* current_node = vector_get(&node->node_list, i);
    if (streq(current_node->name, name)) {
      *result_id = (uX) current_node;
      return true;
    }
  }

  return false;
}

static s8 dvfsd_open_dir(uX node_id, uX* new_node_id, devfs_node_int_t* match_node, u8 mode) {
  // TODO: xor mode with an allowed mask, return error if not equal
  return FS_ERR_UNIMPLEMENTED;
}

static s8 dvfsd_open_file(uX node_id, uX* new_node_id, devfs_node_int_t* match_node, u8 mode) {
  // TODO: xor mode with an allowed mask, return error if not equal
  devfs_dvfsf_int_t* file_node = (devfs_dvfsf_int_t*) match_node;
  devfs_dvfsf_handle_t* handle = (devfs_dvfsf_handle_t*) kmalloc(sizeof(devfs_dvfsf_handle_t));
  handle->type = FS_NODE_HANDLE_CHAR;
  handle->charfile = file_node->charfile;
  file_node->charfile->open(&handle->inner_node_handle, mode, file_node->meta);
  *new_node_id = (uX) handle;

  return true;
}

//

static s8 dvfsd_open(uX node_id, uX* new_node_id, string_t name, u8 type, u8 mode) {
  devfs_dvfsd_int_t* node = (devfs_dvfsd_int_t*) node_id;

  devfs_node_int_t* match_node;
  for (sX i = 0; i < vector_length(&node->node_list); i++) {
    match_node = vector_get(&node->node_list, i);
    if (streq(match_node->name, name)) break;
    match_node = null;
  }

  if (match_node == null) return FS_ERR_NOT_EXISTS;
  if (match_node->type != type) return FS_ERR_WRONG_TYPE;

  switch (type) {
  case FS_NODE_DIRECTORY:
    return dvfsd_open_dir(node_id, new_node_id, match_node, mode);
  case FS_NODE_FILE:
    return dvfsd_open_file(node_id, new_node_id, match_node, mode);
  default:
    return FS_ERR_UNIMPLEMENTED;
  }
}

static s8 dvfsd_list(uX node_id, vector_t* entry_list) {
  devfs_dvfsd_int_t* node = (devfs_dvfsd_int_t*) node_id;
  
  *entry_list = node->node_list;
  return true;
}

static s8 dvfsd_exists(uX node_id, string_t name) {
  uX result_id;
  return dvfsd_find_file(node_id, &result_id, name);
}

static s8 dvfsd_remove(uX node_id, string_t name) {
  return FS_ERR_UNIMPLEMENTED;
}

static s8 dvfsd_mount(uX node_id, fs_module_t* mount_dirtype, uX mount_node_id) {
  return FS_ERR_NOT_SUPPORTED;
}

static s8 dvfsd_unmount(uX node_id) {
  return FS_ERR_NOT_SUPPORTED;
}

//

// TODO: Determine how this interacts with string_t...
sX read(uX node_id, u8* buffer, u16 size) {
  devfs_dvfsf_handle_t* handle = (devfs_dvfsf_handle_t*) node_id;
  if (handle->type != FS_NODE_HANDLE_CHAR) return FS_ERR_WRONG_TYPE;
  return handle->charfile->read(handle->inner_node_handle, buffer, size);
}

sX write(uX node_id, u8* buffer, u16 size) {
  devfs_dvfsf_handle_t* handle = (devfs_dvfsf_handle_t*) node_id;
  if (handle->type != FS_NODE_HANDLE_CHAR) return FS_ERR_WRONG_TYPE;
  return handle->charfile->write(handle->inner_node_handle, buffer, size);
}