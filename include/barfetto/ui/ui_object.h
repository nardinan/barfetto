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
#ifndef UI_OBJECT_H
#define UI_OBJECT_H
#include "../barf_object.h"
#define d_ui_object_default_background (s_color){50, 58, 75, 255}
#define d_ui_object_default_shadow (s_color){22, 25, 35, 255}
#define d_ui_object_default_border (s_color){90, 100, 130, 255}
#define d_ui_object_default_font (s_color){200, 200, 200, 255}
#define d_ui_object_default_grayed_out_background (s_color){30, 33, 40, 255}
#define d_ui_object_default_grayed_out_shadow (s_color){12, 14, 18, 255}
#define d_ui_object_default_grayed_out_border (s_color){45, 50, 60, 255}
#define d_ui_object_default_grayed_out_font (s_color){70, 75, 90, 255}
#define d_ui_object_default_clicked_background (s_color){80, 95, 135, 255}
#define d_ui_object_default_clicked_shadow (s_color){15, 18, 28, 255}
#define d_ui_object_default_clicked_border (s_color){150, 165, 200, 255}
#define d_ui_object_default_clicked_font (s_color){255, 255, 255, 255}
#define d_ui_object_default_cursor (s_color){0, 190, 255, 255}
typedef enum e_ui_object_components {
  e_ui_object_component_dont_draw_border = 0x001,
  e_ui_object_component_dont_draw_background = 0x002,
  e_ui_object_component_dont_draw_shadow = 0x004,
  e_ui_object_component_dont_draw_cursor = 0x008
} e_ui_object_components;
typedef struct s_ui_object {
  s_barf_object head;
  SDL_Surface *unoptimized_surface;
  SDL_Texture *texture;
  bool active;
  int flag;
} s_ui_object;
#endif //UI_OBJECT_H
