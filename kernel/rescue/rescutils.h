#include <types.h>
#include <strbuf.h>

#ifndef __RESCUE_RESCUTILS_H__
#define __RESCUE_RESCUTILS_H__

s8 rescutils_open_input(uX* input_handle);
s8 rescutils_get_line(uX input_handle, string_buf_t* line);

#endif