#ifndef __FS_FSFILE_H__
#define __FS_FSFILE_H__

#include <types.h>

#define FS_NODE_FILE 1

#define FS_SEEK_START 1
#define FS_SEEK_END 2
#define FS_SEEK_REL 3

typedef struct fs_file_t {
  s8 (*type) (u8* type);
  s8 (*close) (uX node_id);
  s8 (*size) (uX node_id, sX* size);

  sX (*read) (uX node_id, u8* buffer, u16 size);
  sX (*write) (uX node_id, u8* buffer, u16 size);
  s8 (*seek) (uX node_id, u8 mode, uX pos);
} fs_file_t;

#endif