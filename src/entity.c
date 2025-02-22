/**
 * MIT License
 * Copyright (c) [2024] The Barfing Fox [Andrea Nardinocchi (andrea@nardinan.it)]
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
static void p_entity_render(s_entity *self, struct s_renderer *renderer) {
  if ((self->renderable) && (self->renderable->f_barf_render))
    self->renderable->f_barf_render(self->renderable, renderer);
}
static void p_entity_status(s_entity *self, const char *status) {
  if ((self->renderable) && (self->renderable->f_barf_status))
    self->renderable->f_barf_status(self->renderable, status);
}
static e_barf_object_listener_processed_events p_entity_listen(s_entity *self, struct s_renderer *renderer, SDL_Event *event) {
  e_barf_object_listener_processed_events result = e_barf_object_listener_processed_event_ignored;
  if ((self->renderable) && (self->renderable->f_barf_listen))
    result = self->renderable->f_barf_listen(self->renderable, renderer, event);
  if ((result != e_barf_object_listener_processed_event_consumed) && (result != e_barf_object_listener_processed_event_terminate))
    if (self->triggers.entries > 0) {
      s_entity_trigger *trigger;
      d_list_foreach(&(self->triggers), trigger, s_entity_trigger)
        if ((trigger->f_entity_listen) && (trigger->listening)) {
          e_barf_object_listener_processed_events local_event_result = trigger->f_entity_listen((s_barf_object *)self, renderer, event);
          if ((local_event_result == e_barf_object_listener_processed_event_processed_and_forwarded) ||
              (local_event_result == e_barf_object_listener_processed_event_consumed)) {
            if ((trigger->status_key) && (self->head.f_barf_status))
              self->head.f_barf_status((s_barf_object *)self, trigger->status_key);
            if (trigger->f_entity_event)
              trigger->f_entity_event(self, renderer);
            result = local_event_result;
          } else if (local_event_result == e_barf_object_listener_processed_event_terminate)
            result = local_event_result;
          if ((result == e_barf_object_listener_processed_event_consumed) || (result == e_barf_object_listener_processed_event_terminate))
            break;
        }
    }
  return result;
}
static void p_entity_delete(s_entity *self) {
  s_entity_trigger *node;
  while ((node = (s_entity_trigger *)self->triggers.head)) {
    f_list_remove(&(self->triggers), self->triggers.head);
    if (node->status_key)
      d_free(node->status_key);
    d_free(node);
  }
}
s_barf_object *f_entity_malloc(s_entity *holder, s_barf_object *renderable, s_list_node **triggers) {
  s_entity *result = holder;
  if ((result) || (result = (s_entity *)d_malloc(sizeof(s_entity)))) {
    memset(result, 0, sizeof(s_entity));
    result->renderable = renderable;
    result->head.f_barf_render = (l_barf_render)p_entity_render;
    result->head.f_barf_listen = (l_barf_listen)p_entity_listen;
    result->head.f_barf_status = (l_barf_status)p_entity_status;
    result->head.f_barf_delete = (l_barf_delete)p_entity_delete;
    if (triggers)
      for (size_t index = 0; (triggers[index]); ++index)
        f_list_append(&(result->triggers), triggers[index], e_list_insert_tail);
  }
  return (s_barf_object *)result;
}
s_entity_trigger *f_entity_new_trigger_change_status(const char *status_key, l_barf_listen f_event_listen, l_entity_event f_entity_event, bool listening) {
  s_entity_trigger *result;
  if ((result = (s_entity_trigger *)d_malloc(sizeof(s_entity_trigger)))) {
    memset(result, 0, sizeof(s_entity_trigger));
    if (status_key) {
      size_t length_status_key = strlen(status_key);
      if ((result->status_key = (char *)d_malloc(length_status_key + 1))) {
        strncpy(result->status_key, status_key, length_status_key);
        result->status_key[length_status_key] = 0;
      }
    }
    result->f_entity_listen = f_event_listen;
    result->f_entity_event = f_entity_event;
    result->listening = listening;
  }
  return result;
}