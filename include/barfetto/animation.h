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
#ifndef ANIMATION_H
#define ANIMATION_H
#include "image.h"
#include "rectangle.h"
typedef enum e_animation_behaviours {
  e_animation_behaviour_play_endlessly = 0,
  e_animation_behaviour_play_and_stop,
  e_animation_behaviour_play_and_pause,
  e_animation_behaviour_play_and_hide,
  e_animation_behaviour_pause,
  e_animation_behaviour_stop,
  e_animation_behaviour_hidden
} e_animation_behaviours;
typedef struct s_animation {
  s_image head;
  s_rectangle selected_frame;
  s_point offset;
  e_animation_behaviours behaviour;
  size_t grid_x, grid_y, index;
  unsigned int ticks_next_frame, ticks_last, index_first_frame, index_last_frame;
} s_animation;
extern void p_animation_render(s_animation *self, struct s_renderer *renderer);
extern s_barf_object *f_animation_malloc(s_animation *holder, const char *source, s_point destination, size_t width, size_t height,
    unsigned int ticks_next_frame) __attribute__((unused));
#endif /* ANIMATION_H */

