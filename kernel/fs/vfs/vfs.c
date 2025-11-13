#include <fs/vfs/vfs.h>
#include <fs/fsdir.h>

bool vfs_init() {
  if (!vfs_init_dir(&vfs_dir)) {
    return false;
  }

  if (!vfs_create_virt_dir(&vfs_root_node)) {
    return false;
  }

  return true;
}