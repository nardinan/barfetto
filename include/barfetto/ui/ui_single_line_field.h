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
#ifndef UI_SINGLE_LINE_FIELD_H
#define UI_SINGLE_LINE_FIELD_H
#include "ui_label.h"
#include "../time_manager.h"
#define d_ui_single_line_field_container_extra_space_pixels 2
typedef struct s_ui_single_line_field {
  s_ui_label head;
  s_rectangle position;
  s_color background_color, border_color, cursor_color;
  bool cursor_draw, border_draw, background_draw, editable;
  size_t cursor_position;
  s_time_manager_clock *reference_clock;
  l_barf_render f_children_render;
  l_barf_delete f_children_delete;
} s_ui_single_line_field;
extern s_barf_object *f_ui_single_line_field_malloc(s_ui_single_line_field *holder, s_time_manager_clock *reference_clock, const char *content,
  TTF_Font *reference_font, s_rectangle position) __attribute__((unused));
#endif //UI_SINGLE_LINE_FIELD_H
