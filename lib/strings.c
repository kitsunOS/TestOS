#include <strings.h>

inline uX strlen(string_t str) {
  return *str;
}

inline uX strstart(string_t str) {
  return sizeof(uX);
}

inline uX strend(string_t str) {
  return *str + strstart(str);
}