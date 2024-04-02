/**
 * MIT License
 * Copyright (c) [2021] The Barfing Fox - TBF [nardinan (andrea@nardinan.it)]
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
#include "../include/barfetto/entity.h"
#include "../include/barfetto/renderer.h"
e_barf_object_listener_processed_events p_entity_listener(s_entity *self, struct s_renderer *renderer, SDL_Event *event) {
  e_barf_object_listener_processed_events result = e_barf_object_listener_processed_event_ignored;
  s_animation *cast_animation = (s_animation *)&(self->head);
  s_image *cast_image = (s_image *)&(cast_animation->head);
  int mouse_x, mouse_y;
  f_camera_get_mouse_position(renderer->selected_camera, &mouse_x, &mouse_y);
  self->hooverd = false;
  if ((mouse_x >= cast_image->destination.x) && (mouse_x < (cast_image->destination.x + cast_image->source.width)) &&
      (mouse_y >= cast_image->destination.y) && (mouse_y < (cast_image->destination.y + cast_image->source.height)))
    self->hooverd = true;
  return result;
}
void p_entity_render(s_entity *self, struct s_renderer *renderer) {
  unsigned int now_ticks = SDL_GetTicks(), elapsed = (now_ticks - self->ticks_last);
  float displacement_x = (elapsed * self->speed_x), displacement_y = (elapsed * self->speed_y);
  self->head.head.destination.x += displacement_x;
  self->head.head.destination.y += displacement_y;
  self->ticks_last = now_ticks;
  p_animation_render(&(self->head), renderer);
}
s_barf_object *f_entity_malloc(s_entity *holder, const char *source, s_point destination, size_t width, size_t height, unsigned int ticks_next_frame) {
  s_entity *result = holder;
  if ((result) || (result = (s_entity *)malloc(sizeof(s_entity)))) {
    memset(result, 0, sizeof(s_entity));
    if ((result = (s_entity *)f_animation_malloc((s_animation *)result, source, destination, width, height, ticks_next_frame))) {
      result->ticks_next_frame = ticks_next_frame;
      result->head.behaviour = e_animation_behaviour_hidden;
      f_dictionary_initialize(&(result->statuses), sizeof(s_entity_status_node));
      result->head.head.head.f_barf_render = (l_barf_render)p_entity_render;
      result->head.head.head.f_barf_listen = (l_barf_listen)p_entity_listener;
    }
  }
  return (s_barf_object *)result;
}
s_entity_status_node *f_entity_get_status(s_entity *self, const char *key) {
  bool is_created = false;
  s_entity_status_node *result = (s_entity_status_node *)f_dictionary_get_informed(&(self->statuses), key, &is_created);
  if (is_created) {
    result->ticks_next_frame = self->ticks_next_frame;
    result->mask = self->mask;
  }
  return result;
}
void f_entity_set_status(s_entity *self, const char *key) {
  s_entity_status_node *current_status = f_entity_get_status(self, key);
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
