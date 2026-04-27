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
#ifndef ENTITY_H
#define ENTITY_H
#include <SDL_events.h>
#include <coremio/list.h>
#include "barf_object.h"
#include "renderer.h"
struct s_entity;
typedef void (*l_entity_event)(struct s_entity *, s_renderer *);
typedef struct s_entity_trigger {
  s_list_node head;
  char *status_key;
  l_barf_listen f_entity_listen;
  l_entity_event f_entity_event;
  bool listening;
} s_entity_trigger;
typedef struct s_entity {
  s_barf_object head;
  s_barf_object *renderable;
  s_rectangle collision_box;
  s_list triggers;
} s_entity;
extern s_barf_object *f_entity_malloc(s_entity *holder, s_barf_object *renderable, s_list_node **triggers) __attribute__((unused));
extern s_entity_trigger *f_entity_new_trigger_change_status(const char *status_key, l_barf_listen f_event_listen, l_entity_event f_entity_event,
  bool listening) __attribute__((unused));
extern bool f_entity_verify_collision(s_entity *self, s_entity *other);
#endif /* ENTITY_H */