/**
 * MIT License
 * Copyright (c) [2024] The Barfing Fox - TBF [nardinan (andrea@nardinan.it)]
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
#include "../include/barfetto/animation_pack.h"
#include "../include/barfetto/renderer.h"
#include "barfetto/time_manager.h"
s_animation_pack_status_node *f_animation_pack_get_status(s_animation_pack *self, const char *status) {
  bool is_created = false;
  s_animation_pack_status_node *current_status = (s_animation_pack_status_node *) f_dictionary_get_or_create_informed(&(self->statuses), status, &is_created);
  if (is_created) {
    current_status->milliseconds_next_frame = self->milliseconds_next_frame;
    current_status->mask = self->mask;
  }
  return current_status;
}
static void p_animation_pack_render(s_animation_pack *self, struct s_renderer *renderer) {
  if (self->reference_clock) {
    time_t now_milliseconds = f_time_manager_elapsed_time_milliseconds(self->reference_clock, false), elapsed;
    if (now_milliseconds < self->milliseconds_last)
      self->milliseconds_last = now_milliseconds;
    elapsed = (now_milliseconds - self->milliseconds_last);
    self->head.head.destination.x += (elapsed * self->speed_x);
    self->head.head.destination.y += (elapsed * self->speed_y);
    self->milliseconds_last = now_milliseconds;
  }
  if (self->f_children_render)
    self->f_children_render((s_barf_object *) &(self->head), renderer);
}
static void p_animation_pack_status(s_animation_pack *self, const char *status) {
  s_animation_pack_status_node *current_status = (s_animation_pack_status_node *) f_animation_pack_get_status(self, status);
  if (current_status) {
    bool bootstrap = ((!self->current_status) || (self->current_status != current_status) || (self->head.behaviour != current_status->behaviour));
    self->head.milliseconds_next_frame = current_status->milliseconds_next_frame;
    self->head.index_first_frame = current_status->index_first_frame;
    self->head.index_last_frame = current_status->index_last_frame;
    self->head.behaviour = current_status->behaviour;
    self->head.head.mask = current_status->mask;
    self->speed_x = current_status->speed_x;
    self->speed_y = current_status->speed_y;
    if (bootstrap) {
      self->head.index = self->head.index_first_frame;
      if (self->reference_clock)
        self->head.milliseconds_last = f_time_manager_elapsed_time_milliseconds(self->reference_clock, false);
    } else if (self->head.index < self->head.index_first_frame)
      self->head.index = self->head.index_first_frame;
    if (self->head.index > self->head.index_last_frame)
      self->head.index = self->head.index_last_frame;
    self->current_status = current_status;
  }
}
static void p_animation_pack_delete(s_animation_pack *self) {
  f_dictionary_free(&(self->statuses));
  if (self->f_children_delete)
    self->f_children_delete((s_barf_object *) self);
}
s_barf_object *f_animation_pack_malloc(s_animation_pack *holder, s_time_manager_clock *reference_clock, const char *source, s_point destination, size_t width,
    size_t height, time_t milliseconds_next_frame) {
  s_animation_pack *result = holder;
  if ((result) || (result = (s_animation_pack *) d_malloc(sizeof(s_animation_pack)))) {
    memset(result, 0, sizeof(s_animation_pack));
    if ((result = (s_animation_pack *) f_animation_malloc((s_animation *) result, reference_clock, source, destination, width, height,
             milliseconds_next_frame))) {
      result->f_children_render = result->head.head.head.f_barf_render;
      result->f_children_delete = result->head.head.head.f_barf_delete;
      result->milliseconds_next_frame = milliseconds_next_frame;
      result->reference_clock = reference_clock;
      result->head.behaviour = e_animation_behaviour_hidden;
      f_dictionary_initialize(&(result->statuses), sizeof(s_animation_pack_status_node));
      f_color_set(&(result->mask), (s_color) d_image_default_mask);
      result->head.head.head.f_barf_render = (l_barf_render) p_animation_pack_render;
      result->head.head.head.f_barf_status = (l_barf_status) p_animation_pack_status;
      result->head.head.head.f_barf_delete = (l_barf_delete) p_animation_pack_delete;
    }
  }
  return (s_barf_object *) result;
}
