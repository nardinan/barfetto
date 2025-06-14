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
s_animation_pack_status_node *f_animation_pack_get_status(s_animation_pack *self, const char *status) {
  bool is_created = false;
  s_animation_pack_status_node *current_status = (s_animation_pack_status_node *)f_dictionary_get_or_create_informed(&(self->statuses), status, &is_created);
  if (is_created) {
    current_status->ticks_next_frame = self->ticks_next_frame;
    current_status->mask = self->mask;
  }
  return current_status;
}
static void p_animation_pack_render(s_animation_pack *self, struct s_renderer *renderer) {
  unsigned int now_ticks = SDL_GetTicks(), elapsed = (now_ticks - self->ticks_last);
  float displacement_x = (elapsed * self->speed_x), displacement_y = (elapsed * self->speed_y);
  self->head.head.destination.x += displacement_x;
  self->head.head.destination.y += displacement_y;
  self->ticks_last = now_ticks;
  p_animation_render(&(self->head), renderer);
}
static void p_animation_pack_status(s_animation_pack *self, const char *status) {
  s_animation_pack_status_node *current_status = (s_animation_pack_status_node *)f_animation_pack_get_status(self, status);
  if (current_status) {
    self->head.ticks_next_frame = current_status->ticks_next_frame;
    self->head.index_first_frame = current_status->index_first_frame;
    self->head.index_last_frame = current_status->index_last_frame;
    if (self->head.index < self->head.index_first_frame)
      self->head.index = self->head.index_first_frame;
    if (self->head.index > self->head.index_last_frame)
      self->head.index = self->head.index_last_frame;
    self->head.behaviour = current_status->behaviour;
    self->head.head.mask = current_status->mask;
    self->speed_x = current_status->speed_x;
    self->speed_y = current_status->speed_y;
  }
}
static void p_animation_pack_delete(s_animation_pack *self) {
  f_dictionary_free(&(self->statuses));
}
s_barf_object *f_animation_pack_malloc(s_animation_pack *holder, const char *source, s_point destination, size_t width, size_t height,
  unsigned int ticks_next_frame) {
  s_animation_pack *result = holder;
  if ((result) || (result = (s_animation_pack *)d_malloc(sizeof(s_animation_pack)))) {
    memset(result, 0, sizeof(s_animation_pack));
    if ((result = (s_animation_pack *)f_animation_malloc((s_animation *)result, source, destination, width, height, ticks_next_frame))) {
      result->ticks_next_frame = ticks_next_frame;
      result->head.behaviour = e_animation_behaviour_hidden;
      f_dictionary_initialize(&(result->statuses), sizeof(s_animation_pack_status_node));
      result->head.head.head.f_barf_render = (l_barf_render)p_animation_pack_render;
      result->head.head.head.f_barf_status = (l_barf_status)p_animation_pack_status;
      result->head.head.head.f_barf_delete = (l_barf_delete)p_animation_pack_delete;
    }
  }
  return (s_barf_object *)result;
}