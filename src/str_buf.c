#include "str_buf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void str_init(str_buf *buf) {
  buf->str = malloc(sizeof(char));
  *buf->str = '\0';
}

str_buf *new_str_buf(void) {
  str_buf *buf = malloc(sizeof(str_buf));
  buf->str = NULL;
  buf->capacity = 0;
  buf->length = 0;
  return buf;
}

str_buf *str_append(str_buf *buf, const char *str) {
  size_t str_len = strlen(str);
  if (buf->length + str_len + 1 > buf->capacity) {
    buf->capacity = buf->capacity == 0 ? 16 : buf->capacity * 2;
    buf->str = realloc(buf->str, sizeof(char *) * buf->capacity);
  }
  memcpy(buf->str + buf->length, str, str_len);
  buf->length += str_len;
  buf->str[buf->length] = '\0';
  return buf;
}

str_buf *str_append_char(str_buf *bufp, const char c) {
  char str[2] = {c, '\0'};
  return str_append(bufp, str);
}

void str_buf_array_init(str_buf_array *array) {
  array->bufs = NULL;
  array->capacity = 0;
  array->length = 0;
}

void str_buf_array_append(str_buf_array *array, str_buf *buf) {
  if (buf == NULL) {
    buf = new_str_buf();
    str_init(buf);
  }

  if (array->length + 1 > array->capacity) {
    array->capacity = array->capacity == 0 ? 16 : array->capacity * 2;
    array->bufs = realloc(array->bufs, sizeof(str_buf *) * array->capacity);
  }
  array->bufs[array->length] = buf;
  array->length++;
}
