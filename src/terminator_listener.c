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
#include "../include/barfetto/terminator_listener.h"
static e_barf_object_listener_processed_events p_terminator_listen(struct s_barf_object *self, struct s_renderer *renderer, SDL_Event *event) {
  e_barf_object_listener_processed_events result = e_barf_object_listener_processed_event_ignored;
  if (event->type == SDL_QUIT)
    result = e_barf_object_listener_processed_event_terminate;
  return result;
}
s_barf_object *f_terminator_listener_malloc(void) {
  s_barf_object *result;
  if ((result = (s_barf_object *)d_malloc(sizeof(s_barf_object)))) {
    memset(result, 0, sizeof(s_barf_object));
    result->f_barf_listen = p_terminator_listen;
  }
  return result;
}