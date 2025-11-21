#include <strings.h>

inline bool streq(string_t str1, string_t str2) {
  uX strl = strlen(str1);
  if (strl != strlen(str2)) return false;
  
  const u8* strdat1 = strstart(str1);
  const u8* strdat2 = strstart(str2);
  uX strSize = strlen(str1);
  for (uX i = 0; i < strl; i++) {
    if (*(strdat1 + i) != *(strdat2 + i)) {
      return false;
    }
  }

  return true;
}