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
#include "../include/barfetto/font_manager.h"
#include "coremio/local.string.h"
static void p_font_manager_family_delete(s_font_manager_family *font_family) {
  for (size_t index_font = 0; index_font < d_font_family_pool_size; ++index_font)
    if (font_family->fonts[index_font].ttf_font) {
      TTF_CloseFont(font_family->fonts[index_font].ttf_font);
      font_family->fonts[index_font].ttf_font = NULL;
    }
}
void f_font_manager_initialize(s_font_manager *font_manager, const char *reference_base_path) {
  memset(font_manager, 0, sizeof(s_font_manager));
  f_dictionary_initialize_custom(&(font_manager->fonts), sizeof(s_font_manager_family), NULL,
    (l_dictionary_node_delete)p_font_manager_family_delete);
  font_manager->reference_base_path = reference_base_path;
}
void f_font_manager_delete(s_font_manager *font_manager) {
  f_dictionary_free(&(font_manager->fonts));
}
TTF_Font *f_font_manager_get(s_font_manager *font_manager, const char *name, size_t size) {
  s_font_manager_family *font_family = (s_font_manager_family *)f_dictionary_get_or_create(&(font_manager->fonts), name);
  TTF_Font *result = NULL;
  if (font_family) {
    size_t index_font = 0;
    for (; (index_font < font_family->entries) && (!result); ++index_font)
      if (font_family->fonts[index_font].size == size)
        result = font_family->fonts[index_font].ttf_font;
    if (!result) {
      char buffer_path[d_string_buffer_size];
      snprintf(buffer_path, (d_string_buffer_size - 1), "%s/%s", font_manager->reference_base_path, name);
      if ((result = font_family->fonts[index_font].ttf_font = TTF_OpenFont(buffer_path, size))) {
        font_family->fonts[index_font].size = size;
        ++(font_family->entries);
      }
    }
  }
  return result;
}