/**
* MIT License
 * Copyright (c) [2026] The Barfing Fox - TBF [nardinan (andrea@nardinan.it)]
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
#ifndef UI_LABEL_H
#define UI_LABEL_H
#include <SDL_ttf.h>
#include "ui_object.h"
#include "../point.h"
#include "../color.h"
#define d_ui_label_size 64
typedef struct s_ui_label {
  s_ui_object head;
  s_point destination;
  s_rectangle visible_area;
  s_color color;
  TTF_Font *reference_font;
  char text[d_ui_label_size];
} s_ui_label;
extern s_barf_object *f_ui_label_malloc(s_ui_label *holder, const char *text, TTF_Font *reference_font, s_point destination) __attribute__((unused));
extern void f_ui_label_force_refresh(s_ui_label *self);
extern void f_ui_label_update_text(s_ui_label *self, const char *text);
extern float f_ui_label_get_width_content(s_ui_label *self);
extern float f_ui_label_get_height_content(s_ui_label *self);
#endif //UI_LABEL_H
