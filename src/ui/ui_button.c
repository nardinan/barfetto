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
#include "../../include/barfetto/ui/ui_button.h"
#include "../../include/barfetto/renderer.h"
static void p_ui_button_render(s_ui_button *self, struct s_renderer *renderer) {
  s_color current_font_color = self->head.head.font_color;
  if (!self->clickable) {
    f_color_set(&(self->head.background_color), self->grayed_out_background_color);
    f_color_set(&(self->head.shadow_color), self->grayed_out_shadow_color);
    f_color_set(&(self->head.border_color), self->grayed_out_border_color);
    f_color_set(&(self->head.head.font_color), self->grayed_out_font_color);
  } else if ((self->clicked) || (self->has_pending_single_click)) {
    f_color_set(&(self->head.background_color), self->clicked_background_color);
    f_color_set(&(self->head.shadow_color), self->clicked_shadow_color);
    f_color_set(&(self->head.border_color), self->clicked_border_color);
    f_color_set(&(self->head.head.font_color), self->clicked_font_color);
  } else {
    f_color_set(&(self->head.background_color), self->released_background_color);
    f_color_set(&(self->head.shadow_color), self->released_shadow_color);
    f_color_set(&(self->head.border_color), self->released_border_color);
    f_color_set(&(self->head.head.font_color), self->released_font_color);
  }
  if (!f_color_compare(&(self->head.head.font_color), &(current_font_color)))
    f_ui_label_force_refresh((s_ui_label *) self);
  if (self->f_children_render)
    self->f_children_render((s_barf_object *) &(self->head), renderer);
  if ((self->has_pending_single_click) && (self->reference_clock)) {
    const time_t elapsed_since_last_click = f_time_manager_elapsed_time_milliseconds(self->reference_clock, false) - self->elapsed_at_last_click;
    if (elapsed_since_last_click >= d_ui_button_double_click_threshold_milliseconds) {
      if (self->on_single_left_click)
        self->on_single_left_click(self, renderer);
      self->has_pending_single_click = false;
    }
  }
}
static e_barf_object_listener_processed_events p_ui_button_listen(s_ui_button *self, struct s_renderer *renderer, SDL_Event *event) {
  e_barf_object_listener_processed_events result = e_barf_object_listener_processed_event_ignored;
  if (self->f_children_listen)
    result = self->f_children_listen((s_barf_object *) &(self->head), renderer, event);
  if ((result != e_barf_object_listener_processed_event_consumed) && (result != e_barf_object_listener_processed_event_terminate)) {
    int mouse_x, mouse_y;
    if (((f_camera_get_mouse_position(renderer->selected_camera, &mouse_x, &mouse_y)) & SDL_BUTTON_LMASK) && (self->has_pending_single_click))
      self->has_pending_single_click = false;
    if ((event) && (event->type == SDL_MOUSEBUTTONDOWN) && (event->button.button == SDL_BUTTON_LEFT)) {
      if ((f_rectangle_contains_point(self->head.position, (s_point) {(float) mouse_x, (float) mouse_y})) && (self->reference_clock)) {
        const time_t elapsed_since_last_click = f_time_manager_elapsed_time_milliseconds(self->reference_clock, false) - self->elapsed_at_last_click;
        if (elapsed_since_last_click < d_ui_button_double_click_threshold_milliseconds) {
          self->has_pending_single_click = false;
          self->elapsed_at_last_click = f_time_manager_elapsed_time_milliseconds(self->reference_clock, false);
          if (!self->auto_release_after_click)
            self->clicked = !(self->clicked);
          if (self->on_double_left_click)
            self->on_double_left_click(self, renderer);
          result = e_barf_object_listener_processed_event_consumed;
        } else {
          self->elapsed_at_last_click = f_time_manager_elapsed_time_milliseconds(self->reference_clock, false);
          self->has_pending_single_click = true;
          if (!self->auto_release_after_click)
            self->clicked = !(self->clicked);
          result = e_barf_object_listener_processed_event_consumed;
        }
      }
    }
  }
  return result;
}
static void p_ui_button_delete(s_ui_button *self) {
  if (self->f_children_delete)
    self->f_children_delete((s_barf_object *)self);
}
s_barf_object *f_ui_button_malloc(s_ui_button *holder, s_time_manager_clock *reference_clock, const char *content, TTF_Font *reference_font,
    s_rectangle position, l_ui_button_click on_single_left_click, l_ui_button_click on_double_left_click) {
  s_ui_button *result = holder;
  if ((result) || (result = (s_ui_button *)d_malloc(sizeof(s_ui_button)))) {
    memset(result, 0, sizeof(s_ui_button));
    if ((result = (s_ui_button *) f_ui_single_line_field_malloc((s_ui_single_line_field *) result, NULL, content, reference_font, position))) {
      result->f_children_render = result->head.head.head.head.f_barf_render;
      result->f_children_delete = result->head.head.head.head.f_barf_delete;
      result->f_children_listen = result->head.head.head.head.f_barf_listen;
      result->head.editable = false;
      result->released_background_color = d_ui_object_default_background;
      result->released_shadow_color = d_ui_object_default_shadow;
      result->released_border_color = d_ui_object_default_border;
      result->released_font_color = d_ui_object_default_font;
      result->clicked_background_color = d_ui_object_default_clicked_background;
      result->clicked_shadow_color = d_ui_object_default_clicked_shadow;
      result->clicked_border_color = d_ui_object_default_clicked_border;
      result->clicked_font_color = d_ui_object_default_clicked_font;
      result->grayed_out_background_color = d_ui_object_default_grayed_out_background;
      result->grayed_out_shadow_color = d_ui_object_default_grayed_out_shadow;
      result->grayed_out_border_color = d_ui_object_default_grayed_out_border;
      result->grayed_out_font_color = d_ui_object_default_grayed_out_font;
      result->clickable = true;
      result->auto_release_after_click = true;
      result->on_single_left_click = on_single_left_click;
      result->on_double_left_click = on_double_left_click;
      result->reference_clock = reference_clock;
      result->head.head.head.head.f_barf_render = (l_barf_render)p_ui_button_render;
      result->head.head.head.head.f_barf_listen = (l_barf_listen)p_ui_button_listen;
      result->head.head.head.head.f_barf_delete = (l_barf_delete)p_ui_button_delete;
    }
  }
  return (s_barf_object *)result;
}
