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
#ifndef BARF_OBJECT_H
#define BARF_OBJECT_H
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <coremio/list.h>
struct s_layer;
struct s_renderer;
struct s_barf_object;
typedef enum e_barf_object_listener_processed_events {
  e_barf_object_listener_processed_event_ignored,
  e_barf_object_listener_processed_event_processed_and_forwarded,
  e_barf_object_listener_processed_event_consumed,
  e_barf_object_listener_processed_event_terminate
} e_barf_object_listener_processed_events;
typedef e_barf_object_listener_processed_events (*l_barf_listen)(struct s_barf_object *, struct s_renderer *, SDL_Event *event);
typedef void (*l_barf_render)(struct s_barf_object *, struct s_renderer *);
typedef void (*l_barf_delete)(struct s_barf_object *);
typedef struct s_barf_object {
  s_list_node head;
  l_barf_listen f_barf_listen;
  l_barf_render f_barf_render;
  l_barf_delete f_barf_delete;
  struct s_layer *owner;
  unsigned int ticks_last_listen, ticks_last_render;
} s_barf_object;
extern void f_barf_object_show(s_barf_object *object, struct s_renderer *renderer, unsigned int layer);
extern void f_barf_object_hide(s_barf_object *object);
#endif /* BARF_OBJECT_H */

