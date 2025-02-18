#ifndef UI_STYLE_H
#define UI_STYLE_H

#include "raylib.h"

#define DEFAULT_UI_PADDING 5


typedef enum ui_text_styling {
  TEXT_STYLING_NONE = 0,
  TEXT_STYLING_CENTER_VERTICAL = 1 << 1,
  TEXT_STYLING_CENTER_HORIZONTAL = 1 << 2,
  TEXT_STYLING_UNDER = 1 << 3,
  TEXT_STYLING_FIT = 1 << 4,          // Set UI element size to fit text
  TEXT_STYLING_INVERT_COLOR = 1 << 5, // TODO: Hacky
} ui_text_styling;

#define TEXT_STYLING_CENTER TEXT_STYLING_CENTER_HORIZONTAL | TEXT_STYLING_CENTER_VERTICAL

typedef struct ui_button_styling {
  Color bg_color;
  Color text_color;
  Color hover_color;
  Color clicked_color;
  ui_text_styling text_styling;
} ui_button_styling;

typedef enum UI_COLORS {
  UI_COLOR_NORMAL,
  UI_COLOR_HOVER,
  UI_COLOR_CLICKED,
  UI_COLOR_TEXT,
  UI_COLOR_TEXT_INVERT,
  UI_COLOR_COUNT,
} UI_COLORS;

#define C(r, g, b)                                                             \
  { r, g, b, 0xFF }

static const Color ui_default_colors[UI_COLOR_COUNT] = {
    [UI_COLOR_NORMAL] = C(0xDC, 0xDC, 0xDC),
    [UI_COLOR_HOVER] = C(0xC8, 0xC8, 0xC8),
    [UI_COLOR_CLICKED] = C(0x63, 0x63, 0x63),
    [UI_COLOR_TEXT] = C(0x00, 0x00, 0x00),
    [UI_COLOR_TEXT_INVERT] = C(0xFF, 0xFF, 0xFF)};

#endif
