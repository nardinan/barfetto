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
#ifndef ENTITY_H
#define ENTITY_H
#include "animation.h"
typedef struct s_entity_status_node {
  s_dictionary_node head;
  s_color mask;
  unsigned int index_first_frame, index_last_frame, ticks_next_frame;
  e_animation_behaviours behaviour;
  float speed_x, speed_y;
} s_entity_status_node;
typedef struct s_entity {
  s_animation head;
  s_dictionary statuses;
  s_color mask;
  float speed_x, speed_y;
  unsigned int ticks_next_frame, ticks_last;
  bool hooverd;
} s_entity;
extern s_barf_object *f_entity_malloc(s_entity *holder, const char *source, s_point destination, size_t width, size_t height,
    unsigned int ticks_next_frame) __attribute__((unused));
extern s_entity_status_node *f_entity_get_status(s_entity *self, const char *key);
extern void f_entity_set_status(s_entity *self, const char *key);
#endif /* ENTITY_H */

