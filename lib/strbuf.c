#include <strbuf.h>

#include <types.h>
#include <mm/mem.h>
#include <strings.h>

// Based on vector code

// Grow factor has one decimal point
bool strbuf_init(string_buf_t* strbuf, uX init_capacity, u8 grow_factor) {
  strbuf->capacity = init_capacity;
  strbuf->grow_factor = grow_factor;
  strbuf->len = 0;
  strbuf->data = kmalloc(init_capacity);

  return strbuf->data != null;
}

bool strbuf_append(string_buf_t* strbuf, string_t str) {
  for (const u8* i = strstart(str); i < strend(str); i++) {
    if (!strbuf_append_char(strbuf, *i)) return false;
  }

  return true;
}

bool strbuf_append_char(string_buf_t* strbuf, u8 character) {
  if (strbuf->len == strbuf->capacity) {
    uX new_capacity = strbuf->capacity * strbuf->grow_factor / 10;
    u8* new_data = kmalloc(new_capacity);
    if (new_data == null) return false;
    
    for (uX i = 0; i < strbuf->capacity; i++) {
      new_data[i] = strbuf->data[i];
    }

    kfree(strbuf->data);
    strbuf->data = new_data;
    strbuf->capacity = new_capacity;
  }

  strbuf->data[strbuf->len] = character;
  strbuf->len++;

  return true;
}

void strbuf_splice(string_buf_t* strbuf, uX pos, uX skip) {
  if (pos >= strbuf->len || skip <= 0) return;
  if (pos + skip > strbuf->len) {
    skip = strbuf->len - strbuf->len;
  }

  for (uX i = 0; i < skip; i++) {
    strbuf->data[pos + i] = strbuf->data[pos + i + skip];
  }

  strbuf->len -= skip;
}

void strbuf_clear(string_buf_t* strbuf) {
  strbuf->len = 0;
}

void strbuf_free(string_buf_t* strbuf) {
  kfree(strbuf->data);
}