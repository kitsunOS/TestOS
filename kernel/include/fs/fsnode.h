#ifndef __FS_FSNODE_H__
#define __FS_FSNODE_H__

#include <types.h>

#define FS_M_READ 0x01
#define FS_M_WRITE 0x02
#define FS_M_EXECUTE 0x04
#define FS_M_CREATE 0x08
#define FS_M_REPLACE 0x10

#define FS_ERR_NOT_EXISTS 0x01
#define FS_ERR_NO_MEM 0x02
#define FS_ERR_NOT_MOUNTED 0x03
#define FS_ERR_ALREADY_MOUNTED 0x04
#define FS_ERR_NOT_EMPTY 0x05

typedef struct fs_node_t {
  s8 (*type) (u8* type);
  s8 (*close) (uX node_id);
  s8 (*size) (uX node_id, sX* size);
} fs_node_t;

bool vfs_init();

#endif