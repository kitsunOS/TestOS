#include <strings.h>

inline uX strlen(string_t str) {
  return *(uX*)str;
}

inline uX strstart(string_t str) {
  return sizeof(uX);
}

inline uX strend(string_t str) {
  return strstart(str) + strlen(str);
}

inline bool streq(string_t str1, string_t str2) {
  uX strl = strlen(str1);
  if (strl != strlen(str2)) return false;
  u8* strdat1 = (u8*) strstart(str1);
  u8* strdat2 = (u8*) strstart(str2);
  uX strSize = strlen(str1);
  for (uX i = 0; i < strl; i++) {
    if (*(strdat1 + i) != *(strdat2 + i)) {
      return false;
    }
  }

  return true;
}