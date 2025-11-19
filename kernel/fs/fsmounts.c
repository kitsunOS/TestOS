#include <types.h>
#include <fs/fsmounts.h>
#include <fs/fsmod.h>
#include <fs/vfs.h>
#include <fs/devfs.h>

void fsmounts_init(void (*reqok) (bool condition, string_t ok_str, string_t fail_str)) {
    reqok(vfs_init(), S("Virtual Filesystem initialized"), S("Failed to initialize virtual filesystem"));

    uX devfs_root_node;
    reqok(devfs_init_module(&devfs_module), S(""), S("Failed to initialize device filesystem"));
    reqok(devfs_create_dir(&devfs_root_node), S(""), S("Failed to initialize device filesystem"));
    reqok(vfs_module.mount(vfs_root_node, &devfs_module, devfs_root_node), S("Device Filesystem initialized"), S("Failed to initialize device filesystem"));
}