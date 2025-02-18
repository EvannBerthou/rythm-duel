#include "ui/textarea.h"
#include "ui/style.h"
#include "utils.h"
#include <raylib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool ui_textarea_behavior(ui_context *ui, ui_textarea *textarea) {
  (void)ui;
  ui_textarea_data *data = textarea->data;
  str_buf_array *lines = &data->lines;

  if (lines->length == 0) {
    str_buf_array_append(&data->lines, NULL);
    str_append_char(lines->bufs[lines->length - 1], '\0');
  }

  int key = 0;
  do {
    if ((key < 33 || key > 126) && key != KEY_SPACE)
      continue;

    str_append_char(lines->bufs[lines->length - 1], key);
  } while ((key = GetCharPressed()) > 0);

  if (KeyPressed(KEY_ENTER)) {
    str_buf_array_append(&data->lines, NULL);
  }

  /*str_append_char(lines->bufs[lines->length - 1], 'a' + lines->length);*/
  /*if (lines->bufs[lines->length - 1]->length == 32768) {*/
  /*  str_buf_array_append(&data->lines, NULL);*/
  /*  str_append_char(lines->bufs[lines->length - 1], 'a' + lines->length);*/
  /*}*/

  return true;
}

bool ui_textarea_do(ui_context *ui, Rectangle position,
                    ui_textarea_data *data) {
  ui_textarea *textarea = malloc(sizeof(ui_textarea));
  ui_element *element = &textarea->element;
  element->type = UI_TEXTAREA;
  element->rec = position;
  element->prev = NULL;
  element->state = UI_NORMAL;

  textarea->data = data;

  ui_add_element(ui, element);
  return ui_textarea_behavior(ui, textarea);
}

void ui_textarea_render(ui_textarea *textarea) {
  ui_textarea_data *data = textarea->data;

  Rectangle rec = textarea->element.rec;
  DrawRectangleRec(rec, BLACK);

  str_buf_array *lines = &data->lines;
  for (size_t i = 0; i < lines->length; i++) {
    const char *text = lines->bufs[i]->str;
    Vector2 line_position = {rec.x + DEFAULT_UI_PADDING,
                             rec.y + 20 * i + DEFAULT_UI_PADDING};
    DrawTextEx(GetDefaultFont(), text, line_position, 20, 3, WHITE);

    if (i == lines->length - 1) {
      int cursor_x = MeasureTextEx(GetDefaultFont(), text, 20, 3).x;
      Vector2 cursor = {.x = rec.x + cursor_x + DEFAULT_UI_PADDING,
                        .y = rec.y + (lines->length - 1) * 20 +
                             DEFAULT_UI_PADDING};
      DrawRectangleV(cursor, (Vector2){3, 18}, WHITE);
    }
  }
}
