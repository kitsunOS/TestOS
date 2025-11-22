#include <types.h>
#include <fs/fsmounts.h>
#include <fs/fsmod.h>
#include <fs/vfs.h>
#include <fs/tmpfs.h>
#include <fs/devfs.h>

static bool tmpfs_init();

static uX tmpfs_root_node;
static uX vfs_devfs_node;

void fsmounts_init(void (*reqok) (s8 condition, string_t ok_str, string_t fail_str)) {
    reqok(vfs_init(), S("Virtual Filesystem initialized"), S("Failed to initialize virtual filesystem"));
    reqok(tmpfs_init(), S(""), S("Failed to initialize temporary filesystem"));
    reqok(vfs_module.mount(vfs_root_node, &tmpfs_module, tmpfs_root_node), S("Temporary Filesystem initialized"), S("Failed to initialize temporary filesystem"));

    reqok(vfs_module.open(vfs_root_node, &vfs_devfs_node, S("dev"), FS_NODE_DIRECTORY, FS_M_READWRITE | FS_M_CREATE), S(""), S("Failed to initialize device filesystem"));
    reqok(devfs_init(&devfs_module), S(""), S("Failed to initialize device filesystem"));
    reqok(vfs_module.mount(vfs_devfs_node, &devfs_module, devfs_root_node), S("Device Filesystem initialized"), S("Failed to initialize device filesystem"));
}

static bool tmpfs_init() {
  if (!tmpfs_init_module(&tmpfs_module)) {
    return false;
  }

  if (!tmpfs_create_dir(&tmpfs_root_node, S(""))) {
    return false;
  }  

  return true;
}