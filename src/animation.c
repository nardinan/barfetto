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
#include "../include/barfetto/animation.h"
#include "../include/barfetto/renderer.h"
#include "barfetto/time_manager.h"
static void p_animation_render(s_animation *self, s_renderer *renderer) {
  if (self->behaviour != e_animation_behaviour_hidden) {
    if (self->reference_clock) {
      time_t now_milliseconds = f_time_manager_elapsed_time_milliseconds(self->reference_clock, false), elapsed;
      if (now_milliseconds < self->milliseconds_last)
        self->milliseconds_last = now_milliseconds;
      elapsed = (now_milliseconds - self->milliseconds_last);
      if ((self->milliseconds_next_frame > 0) && (self->grid_x > 0) && (elapsed > self->milliseconds_next_frame)) {
        unsigned int skip_frames = (elapsed / self->milliseconds_next_frame), residual = (elapsed - (skip_frames * self->milliseconds_next_frame));
        if (self->behaviour == e_animation_behaviour_stop)
          self->index = self->index_first_frame;
        else if ((self->behaviour == e_animation_behaviour_play_and_stop) || (self->behaviour == e_animation_behaviour_play_and_pause) ||
            (self->behaviour == e_animation_behaviour_play_and_hide) || (self->behaviour == e_animation_behaviour_play_endlessly)) {
          if ((self->index += skip_frames) > self->index_last_frame) {
            if (self->behaviour == e_animation_behaviour_play_and_hide)
              self->behaviour = e_animation_behaviour_hidden;
            else if (self->behaviour == e_animation_behaviour_play_and_stop) {
              self->behaviour = e_animation_behaviour_stop;
              self->index = self->index_first_frame;
            } else if (self->behaviour == e_animation_behaviour_play_and_pause) {
              self->behaviour = e_animation_behaviour_pause;
              self->index = self->index_last_frame;
            } else
              self->index = (((self->index - self->index_first_frame) % ((self->index_last_frame - self->index_first_frame) + 1)) + self->index_first_frame);
          }
        }
        self->milliseconds_last = now_milliseconds - residual;
      }
    }
    if (self->grid_x > 0) {
      self->selected_frame.origin.x = (self->selected_frame.width * (self->index % self->grid_x)) + self->offset.x;
      self->selected_frame.origin.y = (self->selected_frame.height * (size_t) (self->index / self->grid_x)) + self->offset.y;
    }
    self->head.source = self->selected_frame;
    if (self->f_children_render)
      self->f_children_render((s_barf_object *) &(self->head), renderer);
  }
}
s_barf_object *f_animation_malloc(s_animation *holder, s_time_manager_clock *reference_clock, const char *source, s_point destination, size_t width,
    size_t height, time_t milliseconds_next_frame) {
  s_animation *result = holder;
  if ((result) || (result = (s_animation *) d_malloc(sizeof(s_animation)))) {
    memset(result, 0, sizeof(s_animation));
    if ((result = (s_animation *) f_image_malloc((s_image *) result, source, destination))) {
      result->f_children_render = result->head.head.f_barf_render;
      result->selected_frame.width = width;
      result->selected_frame.height = height;
      if ((width > 0) && (height > 0)) {
        result->grid_x = (result->head.source.width / width);
        result->grid_y = (result->head.source.height / height);
      }
      result->milliseconds_next_frame = milliseconds_next_frame;
      result->reference_clock = reference_clock;
      if ((result->grid_x > 0) && (result->grid_y > 0))
        result->index_last_frame = ((result->grid_x * result->grid_y) - 1);
      result->head.head.f_barf_render = (l_barf_render) p_animation_render;
    }
  }
  return (s_barf_object *) result;
}
