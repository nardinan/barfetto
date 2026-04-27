/**
* MIT License
 * Copyright (c) [2026] The Barfing Fox - TBF [nardinan (andrea@nardinan.it)]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "../../include/barfetto/ui/ui_single_line_field.h"
#include "../../include/barfetto/renderer.h"
static size_t p_ui_single_line_get_cursor_position_in_text(s_ui_single_line_field *self) {
  const size_t length_label = strlen(d_ui_label_get_printable_content(&(self->head)));
  if (self->cursor_position > length_label)
    self->cursor_position = length_label;
  return self->cursor_position;
}
static int p_ui_single_line_get_cursor_position_in_pixels(s_ui_single_line_field *self) {
  const size_t cursor_position = p_ui_single_line_get_cursor_position_in_text(self), length_label = strlen(d_ui_label_get_printable_content(&(self->head)));
  int position_x = 0;
  if ((length_label > 0) && (cursor_position > 0)) {
    if (cursor_position < length_label) {
      char backup_character = self->head.content[cursor_position];
      int position_height;
      self->head.content[cursor_position] = 0; /* truncate in-place, without substring */
      TTF_SizeText(self->head.reference_font, self->head.content, &(position_x), &(position_height));
      self->head.content[cursor_position] = backup_character;
    } else
      position_x = (int)f_ui_label_get_width_content(&(self->head));
  }
  return position_x;
}
static void p_ui_single_line_field_render(s_ui_single_line_field *self, struct s_renderer *renderer) {
  SDL_Rect destination = {
    .x = (self->position.origin.x - renderer->selected_camera->visible_area.origin.x),
    .y = (self->position.origin.y - renderer->selected_camera->visible_area.origin.y),
    .w = self->position.width,
    .h = self->position.height
  };
  int cursor_image_position_x = p_ui_single_line_get_cursor_position_in_pixels(self), delta_position_x = 0;
  self->head.destination.x = self->position.origin.x + d_ui_single_line_field_container_extra_space_pixels;
  self->head.destination.y = self->position.origin.y + d_ui_single_line_field_container_extra_space_pixels;
  self->head.visible_area.width = self->position.width - (2 * d_ui_single_line_field_container_extra_space_pixels);
  self->head.visible_area.height = self->position.height - (2 * d_ui_single_line_field_container_extra_space_pixels);
  if (self->background_draw) {
    SDL_SetRenderDrawColor(renderer->renderer, self->background_color.red, self->background_color.green,
      self->background_color.blue, self->background_color.alpha);
    SDL_RenderFillRect(renderer->renderer, &destination);
  }
  if (self->border_draw) {
    SDL_SetRenderDrawColor(renderer->renderer, self->border_color.red, self->border_color.green, self->border_color.blue,
      self->border_color.alpha);
    SDL_RenderDrawRect(renderer->renderer, &destination);
  }
  self->head.visible_area.origin.x = 0;
  if ((delta_position_x = cursor_image_position_x - (self->position.width - (2 * d_ui_single_line_field_container_extra_space_pixels))) > 0)
    self->head.visible_area.origin.x = (float)delta_position_x;
  if (self->f_children_render)
    self->f_children_render((s_barf_object *)&(self->head), renderer);
  if (self->cursor_draw) {
    SDL_Rect cursor_position = {
      .x = cursor_image_position_x,
      .y = 0,
      .w = d_ui_single_line_field_container_extra_space_pixels,
      .h = TTF_FontHeight(self->head.reference_font)
    };
    cursor_position.x += (int)(self->head.destination.x - self->head.visible_area.origin.x);
    cursor_position.y += (int)self->head.destination.y;
    SDL_SetRenderDrawColor(renderer->renderer, self->cursor_color.red, self->cursor_color.green, self->cursor_color.blue, self->cursor_color.alpha);
    SDL_RenderDrawRect(renderer->renderer, &cursor_position);
  }
}
static void p_ui_single_line_field_listen(s_ui_single_line_field *self, struct s_renderer *renderer, SDL_Event *event) {
  int mouse_x, mouse_y;
  if (f_camera_get_mouse_position(renderer->selected_camera, &mouse_x, &mouse_y) & SDL_BUTTON_LMASK) {
    if (f_rectangle_contains_point(self->position, (s_point){(float)mouse_x, (float)mouse_y}))
      self->head.head.active = true;
    else
      self->head.head.active = false;
  }
  if (self->head.head.active)
  if (event) {
    if (event->type == SDL_TEXTINPUT) {
      size_t length_additional_content = strlen(event->text.text), length_current_content = strlen(d_ui_label_get_printable_content(&(self->head))),
      length_final_content = length_additional_content + length_current_content + 1;
      if (length_additional_content > 0) {
        char stack_buffer[length_final_content];
        if (self->cursor_position > length_current_content)
          self->cursor_position = length_current_content;
        if (self->cursor_position > 0) /* we have a block of text before the cursor */
          strncpy(&(stack_buffer[0]), self->head.content, self->cursor_position);
        strcpy(&(stack_buffer[self->cursor_position]), event->text.text);
        if (self->cursor_position < length_current_content) /* we have a block of text at the right of the cursor that should scoot */
          strcpy(&(stack_buffer[self->cursor_position + length_additional_content]), &(self->head.content[self->cursor_position]));
        stack_buffer[length_final_content - 1] = 0;
        self->cursor_position += length_additional_content;
        f_ui_label_update_text(&(self->head), stack_buffer);
      }
    } else if (event->type == SDL_KEYDOWN) {
      switch (event->key.keysym.sym) {
        case SDLK_RIGHT: {
          const size_t length_text = strlen(d_ui_label_get_printable_content(&(self->head)));
          if (self->cursor_position < length_text)
            ++(self->cursor_position);
          break;
        }
        case SDLK_LEFT: {
          if (self->cursor_position > 0)
            --(self->cursor_position);
          break;
        }
        case SDLK_BACKSPACE: {
          const size_t length_text = strlen(d_ui_label_get_printable_content(&(self->head)));
          if (length_text > 0)
            if (self->cursor_position > 0) {
              memmove((self->head.content + self->cursor_position - 1),
                (self->head.content + self->cursor_position), strlen(self->head.content + self->cursor_position));
              self->head.content[length_text - 1] = 0;
              f_ui_label_force_refresh(&(self->head));
              --(self->cursor_position);
            }
          break;
        }
        default: {
          break;
        }
      }
    }
  }
}
static void p_ui_single_line_field_delete(s_ui_single_line_field *self) {
  if (self->f_children_delete)
    self->f_children_delete((s_barf_object *)self);
}
s_barf_object *f_ui_single_line_field_malloc(s_ui_single_line_field *holder, s_time_manager_clock *reference_clock, const char *content,
  TTF_Font *reference_font, s_rectangle position) {
  s_ui_single_line_field *result = holder;
  if ((result) || (result = (s_ui_single_line_field *)d_malloc(sizeof(s_ui_single_line_field)))) {
    memset(result, 0, sizeof(s_ui_single_line_field));
    if ((result = (s_ui_single_line_field *)f_ui_label_malloc((s_ui_label *)result, content, reference_font, position.origin))) {
      result->f_children_render = result->head.head.head.f_barf_render;
      result->f_children_delete = result->head.head.head.f_barf_delete;
      result->position = position;
      result->reference_clock = reference_clock;
      result->background_color = d_ui_object_default_background;
      result->border_color = d_ui_object_default_border;
      result->cursor_color = d_ui_object_default_cursor;
      if (content)
        result->cursor_position = strlen(content);
      result->background_draw = true;
      result->border_draw = true;
      result->cursor_draw = true;
      result->head.head.head.f_barf_render = (l_barf_render)p_ui_single_line_field_render;
      result->head.head.head.f_barf_listen = (l_barf_listen)p_ui_single_line_field_listen;
      result->head.head.head.f_barf_delete = (l_barf_delete)p_ui_single_line_field_delete;
    }
  }
  return (s_barf_object *)result;
}