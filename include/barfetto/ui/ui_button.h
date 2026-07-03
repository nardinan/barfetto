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
#ifndef UI_BUTTON_H
#define UI_BUTTON_H
#include <coremio/local.string.h>
#include "ui_single_line_field.h"
#define d_ui_button_double_click_threshold_milliseconds 200
struct s_ui_button;
typedef void (*l_ui_button_click)(struct s_ui_button *, struct s_renderer *);
typedef struct s_ui_button {
  s_ui_single_line_field head;
  s_color released_background_color, released_shadow_color, released_border_color, released_font_color,
       clicked_background_color, clicked_shadow_color, clicked_border_color, clicked_font_color,
       grayed_out_background_color, grayed_out_shadow_color, grayed_out_border_color, grayed_out_font_color;
  bool clickable, clicked, auto_release_after_click /* when 'auto_release_after_click' is false, the button becomes a toggle button */;
  l_barf_render f_children_render;
  l_barf_delete f_children_delete;
  l_barf_listen f_children_listen;
  l_ui_button_click on_single_left_click, on_double_left_click;
  s_time_manager_clock *reference_clock;
  time_t elapsed_at_last_click;
  bool has_pending_single_click;
} s_ui_button;
extern s_barf_object *f_ui_button_malloc(s_ui_button *holder, s_time_manager_clock *reference_clock, const char *content, TTF_Font *reference_font,
    s_rectangle position, l_ui_button_click on_single_left_click, l_ui_button_click on_double_left_click) __attribute__((unused));
#endif // UI_BUTTON_H
