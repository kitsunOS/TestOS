#ifndef __LIB_STRBUF_H__
#define __LIB_STRBUF_H__

#include <types.h>

typedef struct string_buf_t {
  uX len;
  u8* data;
  uX capacity;
  u8 grow_factor;
} string_buf_t;

bool strbuf_init(string_buf_t* strbuf, uX init_capacity, u8 grow_factor);

bool strbuf_append(string_buf_t* strbuf, string_t str);

bool strbuf_append_char(string_buf_t* strbuf, u8 ch);

void strbuf_splice(string_buf_t* strbuf, uX pos, uX skip);

void strbuf_clear(string_buf_t* strbuf);

void strbuf_free(string_buf_t* strbuf);

#define SB2S(x) ((string_t) { (x).len, (x).data })

#endif