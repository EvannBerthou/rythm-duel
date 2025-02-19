#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "context.h"
#include "style.h"

typedef enum ui_button_type {
  UI_BUTTON_COLOR = 1 << 0,
  UI_BUTTON_LABEL = 1 << 1,
  UI_BUTTON_IMAGE = 1 << 2,
  UI_BUTTON_DOUBLE_CLICK = 1 << 3
} ui_button_type;

typedef struct ui_button {
  ui_element element;
  // TODO: Don't use dynamic string ? Maybe use a fixed buffer ?
  const char *label;
  ui_text_styling text_styling;
  ui_button_type button_type;
  Texture2D image;
  Sound hover_sound;
  Sound clicked_sound;
  double *clicked_at;
  int font_size;
} ui_button;

// TODO: Should only be one with all
bool ui_button_label_styled(ui_context *ui, Rectangle position,
                            const char *label, ui_text_styling styling,
                            int font_size);
bool ui_button_label(ui_context *ui, Rectangle position, const char *label);
bool ui_button_label_fit(ui_context *ui, Vector2 position, const char *label);
// TODO: Sounds should not be here but it's for testing the main menu for now
bool ui_button_image_with_label(ui_context *ui, Rectangle position,
                                const char *label, Texture2D image,
                                int font_size, Sound hover_sound,
                                Sound clicked_sound);
bool ui_button_double_image_with_label(ui_context *ui, Rectangle position,
                                       const char *label, Texture2D image,
                                       double *clicked_at);

void ui_button_render(ui_button *button);
void free_ui_button(ui_button *button);

#endif
