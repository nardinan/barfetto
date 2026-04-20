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
#ifndef MANAGED_ANIMATION_H
#define MANAGED_ANIMATION_H
#include "animation.h"
typedef struct s_animation_pack_status_node {
  s_dictionary_node head;
  s_color mask;
  unsigned int index_first_frame, index_last_frame, ticks_next_frame;
  e_animation_behaviours behaviour;
  float speed_x, speed_y;
} s_animation_pack_status_node;
typedef struct s_animation_pack {
  s_animation head;
  s_dictionary statuses;
  s_color mask;
  float speed_x, speed_y;
  unsigned int ticks_next_frame, ticks_last;
  l_barf_render f_children_render;
  l_barf_delete f_children_delete;
} s_animation_pack;
extern s_animation_pack_status_node *f_animation_pack_get_status(s_animation_pack *self, const char *status);
extern s_barf_object *f_animation_pack_malloc(s_animation_pack *holder, const char *source, s_point destination, size_t width, size_t height,
    unsigned int ticks_next_frame) __attribute__((unused));
#endif /* MANAGED_ANIMATION_H */

