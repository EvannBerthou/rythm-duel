#ifndef STR_BUF_H
#define STR_BUF_H

#include <stddef.h>

typedef struct str_buf {
  char *str;
  size_t capacity;
  size_t length;
} str_buf;

str_buf *new_str_buf(void);
str_buf *str_append(str_buf *buf, const char *str);
str_buf *str_append_char(str_buf *buf, const char c);

typedef struct str_buf_array {
    str_buf **bufs;
    size_t capacity;
    size_t length;
} str_buf_array;

void str_buf_array_init(str_buf_array *array);
void str_buf_array_append(str_buf_array *array, str_buf *buf);

#endif
