#ifndef __LIB_STRINGS_H__
#define __LIB_STRINGS_H__

#include <types.h>

uX strlen(string_t str);

uX strstart(string_t str);

uX strend(string_t str);

bool streq(string_t str1, string_t str2);

#endif