#include <vfs/vfs.h>
#include <vfs/vdir.h>

bool vfs_init() {
  if (!vfs_init_dir(&vfs_root_dir)) {
    return false;
  }

  if (!vfs_create_virt_dir(&vfs_root_node)) {
    return false;
  }

  return true;
}