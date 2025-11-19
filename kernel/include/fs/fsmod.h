#ifndef __FS_FS_H__
#define __FS_FS_H__

#include <types.h>
#include <vector.h>

#define FS_NODE_FILE 1
#define FS_NODE_DIRECTORY 2
// TODO: This exists because code stores files and handles differently, but is a bit awkward...
#define FS_NODE_HANDLE_CHAR 3

#define FS_M_READ 0x01
#define FS_M_WRITE 0x02
#define FS_M_EXECUTE 0x04
#define FS_M_CREATE 0x08
#define FS_M_REPLACE 0x10

#define FS_ERR_NOT_EXISTS -0x01
#define FS_ERR_NO_MEM -0x02
#define FS_ERR_NOT_MOUNTED -0x03
#define FS_ERR_ALREADY_MOUNTED -0x04
#define FS_ERR_NOT_EMPTY -0x05
#define FS_ERR_NOT_SUPPORTED -0x06
#define FS_ERR_UNIMPLEMENTED -0x07
#define FS_ERR_WRONG_TYPE -0x08
#define FS_ERR_ALREADY_EXISTS -0x09

#define FS_SEEK_START 1
#define FS_SEEK_END 2
#define FS_SEEK_REL 3

typedef struct fs_module_t {
  s8 (*type) (uX node_id, u8* type);
  s8 (*close) (uX node_id);
  s8 (*size) (uX node_id, sX* size);
  
  s8 (*open) (uX node_id, uX* new_node_id, string_t name, u8 type, u8 mode);
  s8 (*list) (uX node_id, vector_t* entry_list);
  s8 (*exists) (uX node_id, string_t name);
  s8 (*remove) (uX node_id, string_t item);
  s8 (*mount) (uX node_id, struct fs_module_t* mount_dirtype, uX mount_node_id);
  s8 (*unmount) (uX node_id);
  // s8 (*link) (uX node_id, uX link_node_id);

  sX (*read) (uX node_id, u8* buffer, u16 size);
  sX (*write) (uX node_id, u8* buffer, u16 size);
  s8 (*seek) (uX node_id, u8 mode, uX pos);
} fs_module_t;

#endif