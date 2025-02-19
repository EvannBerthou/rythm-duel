#ifndef UI_TEXTAREA_H
#define UI_TEXTAREA_H

#include "../str_buf.h"
#include "context.h"
#include <stddef.h>

typedef struct ui_textarea_data {
  str_buf_array lines;
} ui_textarea_data;

typedef struct ui_textarea {
  ui_element element;
  ui_textarea_data *data;
} ui_textarea;

bool ui_textarea_do(ui_context *ui, Rectangle position, ui_textarea_data *data);

void ui_textarea_render(ui_textarea *textarea);

#endif
