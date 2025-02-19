#include "button.h"
#include "../utils.h"
#include "context.h"
#include "raylib.h"
#include "style.h"
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DOUBLE_CLICK_TIMING 0.250f

static bool ui_button_behavior(ui_context *ui, ui_button *button) {
  ui_element *element = &button->element;
  Rectangle *rec = &element->rec;
  Vector2 text_size =
      MeasureTextEx(GetDefaultFont(), button->label, button->font_size, 1);

  if (button->text_styling & TEXT_STYLING_FIT) {
    if (text_size.x + DEFAULT_UI_PADDING * 2 > rec->width) {
      rec->width = text_size.x + DEFAULT_UI_PADDING * 2;
    }

    if (text_size.y + DEFAULT_UI_PADDING * 2 > rec->height) {
      rec->height = text_size.y + DEFAULT_UI_PADDING * 2;
    }
  }

  // If we are hovering but we were not at the previous frame
  bool hovering_before = CheckCollisionPointRec(ui->prev_mouse, element->rec);
  if (CheckCollisionPointRec(GetMousePosition(), element->rec)) {
    element->state = UI_HOVER;

    if (!hovering_before) {
      PlaySound(button->hover_sound);
    }

    if (IsMouseButtonDown(0)) {
      element->state = UI_CLICKED;
    }

    bool was_in_on_pressed = CheckCollisionPointRec(ui->down_at, element->rec);
    if (was_in_on_pressed && IsMouseButtonReleased(0)) {
      if (button->button_type & UI_BUTTON_DOUBLE_CLICK) {
        double current_time = GetTime();
        double delta = current_time - *button->clicked_at;
        if (delta > DOUBLE_CLICK_TIMING) {
          *button->clicked_at = current_time;
          return false;
        }
        PlaySound(button->clicked_sound);
        return true;
      }

      PlaySound(button->clicked_sound);
      return true;
    }
  }

  return false;
}

bool ui_button_all(ui_context *ui, Rectangle position, const char *label,
                   ui_text_styling styling, int font_size, ui_button_type type,
                   Texture2D *image, double *clicked_at, Sound hover_sound,
                   Sound clicked_sound) {
  ui_button *button = malloc(sizeof(ui_button));
  ui_element *element = &button->element;
  element->type = UI_BUTTON;
  element->rec = position;
  element->prev = NULL;
  element->state = UI_NORMAL;

  button->label = label ? strdup(label) : NULL;
  button->text_styling = styling;
  button->button_type = type;
  button->image = image != NULL ? (*image) : (Texture2D){0};
  button->clicked_at = clicked_at;
  button->font_size = font_size;
  button->hover_sound = hover_sound;
  button->clicked_sound = clicked_sound;

  ui_add_element(ui, element);

  return ui_button_behavior(ui, button);
}

// TODO: Hacky
Sound empty_sound;

bool ui_button_label_styled(ui_context *ui, Rectangle position,
                            const char *label, ui_text_styling styling,
                            int font_size) {
  return ui_button_all(ui, position, label, styling, font_size,
                       UI_BUTTON_LABEL | UI_BUTTON_COLOR, NULL, 0, empty_sound,
                       empty_sound);
}

bool ui_button_image_with_label(ui_context *ui, Rectangle position,
                                const char *label, Texture2D image,
                                int font_size, Sound hover_sound,
                                Sound clicked_sound) {
  ui_text_styling styling = TEXT_STYLING_INVERT_COLOR | TEXT_STYLING_CENTER;
  ui_button_type button_type = UI_BUTTON_LABEL | UI_BUTTON_IMAGE;
  return ui_button_all(ui, position, label, styling, font_size, button_type,
                       &image, 0, hover_sound, clicked_sound);
}

bool ui_button_double_image_with_label(ui_context *ui, Rectangle position,
                                       const char *label, Texture2D image,
                                       double *clicked_at) {
  ui_text_styling styling = TEXT_STYLING_INVERT_COLOR | TEXT_STYLING_CENTER;
  ui_button_type button_type =
      UI_BUTTON_LABEL | UI_BUTTON_IMAGE | UI_BUTTON_DOUBLE_CLICK;
  return ui_button_all(ui, position, label, styling, 16, button_type, &image,
                       clicked_at, empty_sound, empty_sound);
}

bool ui_button_label(ui_context *ui, Rectangle position, const char *label) {
  return ui_button_label_styled(ui, position, label, 16, TEXT_STYLING_CENTER);
}

bool ui_button_label_fit(ui_context *ui, Vector2 position, const char *label) {
  // Width and height will be set by ui_button_label in order to fit text
  Rectangle rec = {.x = position.x, .y = position.y, .width = 0, .height = 0};
  return ui_button_label_styled(ui, rec, label,
                                TEXT_STYLING_CENTER | TEXT_STYLING_FIT, 16);
}

void ui_button_render(ui_button *button) {
  Rectangle rec = button->element.rec;

  if (button->button_type & UI_BUTTON_COLOR) {
    Color bg_color;
    switch (button->element.state) {
    case UI_NORMAL:
      bg_color = ui_default_colors[UI_COLOR_NORMAL];
      break;
    case UI_HOVER:
      bg_color = ui_default_colors[UI_COLOR_HOVER];
      break;
    case UI_CLICKED:
      bg_color = ui_default_colors[UI_COLOR_CLICKED];
      break;
    }
    DrawRectangleRec(rec, bg_color);
    DrawRectangleLinesEx(rec, 2, ui_default_colors[UI_COLOR_TEXT]);
  }

  if (button->button_type & UI_BUTTON_IMAGE) {
    Color tint;
    switch (button->element.state) {
    case UI_NORMAL:
      tint = WHITE;
      break;
    case UI_HOVER:
      tint = LIGHTGRAY;
      break;
    case UI_CLICKED:
      tint = DARKGRAY;
      break;
    }
    Rectangle source = {0, 0, button->image.width, button->image.height};
    Rectangle dest = rec;
    DrawTexturePro(button->image, source, dest, Vector2Zero(), 0, tint);
  }

  if (button->button_type & UI_BUTTON_LABEL) {
    Vector2 text_position = (Vector2){rec.x, rec.y};
    Vector2 text_size =
        MeasureTextEx(GetDefaultFont(), button->label, button->font_size, 4);
    if (button->text_styling & TEXT_STYLING_CENTER_HORIZONTAL) {
      text_position.x = rec.x + (rec.width - text_size.x) / 2;
    }

    if (button->text_styling & TEXT_STYLING_CENTER_VERTICAL) {
      text_position.y = rec.y + (rec.height - text_size.y) / 2;
    }

    if (button->text_styling & TEXT_STYLING_UNDER) {
      text_position.y += rec.height + DEFAULT_UI_PADDING;
    }

    Color text_color =
        ui_default_colors[button->text_styling & TEXT_STYLING_INVERT_COLOR
                              ? UI_COLOR_TEXT_INVERT
                              : UI_COLOR_TEXT];

    DrawTextEx(GetDefaultFont(), button->label, text_position,
               button->font_size, 4, text_color);
  }
}

void free_ui_button(ui_button *button) { free((char *)button->label); }
