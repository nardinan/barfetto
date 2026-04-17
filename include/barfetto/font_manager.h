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
#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H
#include <coremio/dictionary.h>
#include <SDL_ttf.h>
#define d_font_family_pool_size 16
typedef struct s_font_manager_family_font {
 TTF_Font *ttf_font;
 size_t size;
} s_font_manager_family_font;
typedef struct s_font_manager_family {
 s_dictionary_node head;
 s_font_manager_family_font fonts[d_font_family_pool_size];
 size_t entries;
} s_font_manager_family;
typedef struct s_font_manager {
  s_dictionary fonts;
  const char *reference_base_path;
} s_font_manager;
extern void f_font_manager_initialize(s_font_manager *font_manager, const char *reference_base_path);
extern void f_font_manager_delete(s_font_manager *font_manager);
extern TTF_Font *f_font_manager_get(s_font_manager *font_manager, const char *name, size_t size);
#endif //FONT_MANAGER_H
