#ifndef UI_CONTEXT_H
#define UI_CONTEXT_H

#include "raylib.h"

typedef enum UI_TYPE {
  UI_NONE,
  UI_BUTTON,
  UI_TEXTAREA,
} UI_TYPE;

typedef enum ui_element_state {
  UI_NORMAL,
  UI_HOVER,
  UI_CLICKED,
} ui_element_state;

typedef struct ui_element {
  UI_TYPE type;
  Rectangle rec;
  struct ui_element *prev;
  ui_element_state state;
} ui_element;

typedef struct ui_context {
  ui_element *head;
  ui_element *last;

  Vector2 prev_mouse;
  Vector2 down_at;
} ui_context;

void init_ui_context(ui_context *ui);
void update_ui_context(ui_context *ui);
void render_ui_context(ui_context *ui);
void free_ui_context(ui_context *ui);

void ui_add_element(ui_context *ui, ui_element *ui_element);

#endif
