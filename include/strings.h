#ifndef __LIB_STRINGS_H__
#define __LIB_STRINGS_H__

#include <types.h>

#define strlen(str) str.len

#define strstart(str) str.data

#define strend(str) (str.data + str.len)

#define strch(str, i) str.data[i]

bool streq(string_t str1, string_t str2);

#endif