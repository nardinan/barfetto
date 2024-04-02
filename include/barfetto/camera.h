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
#ifndef CAMERA_H
#define CAMERA_H
#include "barf_object.h"
#include "rectangle.h"
#include "color.h"
typedef struct s_camera {
  s_barf_object head;
  s_rectangle visible_area, screen_destination;
  s_color background_color, contour_color;
  size_t _last_width, _last_height;
  SDL_Texture *container;
  bool visible, show_contour;
} s_camera;
extern s_barf_object *f_camera_malloc(s_camera *holder, s_rectangle visible_area, s_rectangle screen_destination) __attribute__((unused));
extern void f_camera_enable_rendering(s_camera *self, struct s_renderer *renderer);
extern void f_camera_disable_rendering(s_camera *self, struct s_renderer *renderer);
extern void f_camera_get_mouse_position(s_camera *self, int *mouse_x, int *mouse_y);
#endif /* CAMERA_H */

