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
#ifndef IMAGE_H
#define IMAGE_H
#include <SDL_image.h>
#include <SDL_render.h>
#include <coremio/dictionary.h>
#include "barf_object.h"
#include "color.h"
#include "rectangle.h"
#define d_image_default_mask { 255, 255, 255, 255 }
typedef struct s_surface_cache_node {
  s_dictionary_node head;
  SDL_Surface *unoptimized_surface;
  SDL_Texture *texture;
  size_t references;
} s_surface_cache_node;
extern s_dictionary dictionary_surface_cache;
typedef enum e_surface_flips {
  e_surface_flip_none = SDL_FLIP_NONE,
  e_surface_flip_horizontal = SDL_FLIP_HORIZONTAL,
  e_surface_flip_vertical = SDL_FLIP_VERTICAL
} e_surface_flips;
typedef struct s_image {
  s_barf_object head;
  s_point destination;
  s_rectangle source;
  s_color mask;
  s_surface_cache_node *texture;
  double angle;
  e_surface_flips flip;
} s_image;
extern void p_image_render(s_image *self, struct s_renderer *renderer);
extern s_barf_object *f_image_malloc(s_image *holder, const char *source, s_point destination) __attribute__((unused));
#endif /* IMAGE_H */

