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
#include "../include/barfetto/image.h"
#include "../include/barfetto/renderer.h"
s_dictionary dictionary_surface_cache;
void p_image_render(s_image *self, struct s_renderer *renderer) {
  if (self->texture) {
    if ((!self->texture->texture) && (self->texture->unoptimized_surface))
      self->texture->texture = SDL_CreateTextureFromSurface(renderer->renderer, self->texture->unoptimized_surface);
    if (self->texture->texture) {
      SDL_Rect destination = {
        .x = (self->destination.x - renderer->selected_camera->visible_area.origin.x),
        .y = (self->destination.y - renderer->selected_camera->visible_area.origin.y),
        .w = self->source.width,
        .h = self->source.height
      }, source = {
        .x = self->source.origin.x,
        .y = self->source.origin.y,
        .w = self->source.width,
        .h = self->source.height
      };
      SDL_SetTextureColorMod(self->texture->texture, self->mask.red, self->mask.green, self->mask.blue);
      SDL_SetTextureAlphaMod(self->texture->texture, self->mask.alpha);
      SDL_RenderCopyEx(renderer->renderer, self->texture->texture, &source, &destination, self->angle,
          NULL, (SDL_RendererFlip)self->flip);
    }
  }
}
void p_image_delete(s_image *self) {
  if (self->texture)
    if ((self->texture->references > 0) && ((--self->texture->references) == 0)) {
      if (self->texture->unoptimized_surface)
        SDL_FreeSurface(self->texture->unoptimized_surface);
      if (self->texture->texture)
        SDL_DestroyTexture(self->texture->texture);
      self->texture->unoptimized_surface = NULL;
      self->texture->texture = NULL;
    }
}
s_barf_object *f_image_malloc(s_image *holder, const char *source, s_point destination) {
  s_image *result = holder;
  if ((result) || (result = (s_image *)malloc(sizeof(s_image)))) {
    memset(result, 0, sizeof(s_image));
    if (dictionary_surface_cache.node_size != sizeof(s_surface_cache_node))
      f_dictionary_initialize(&dictionary_surface_cache, sizeof(s_surface_cache_node));
    if ((result->texture = (s_surface_cache_node *)f_dictionary_get(&dictionary_surface_cache, source)) &&
        ((result->texture->unoptimized_surface) || (result->texture->unoptimized_surface = IMG_Load(source)))) {
      ++result->texture->references;
      result->source.width = result->texture->unoptimized_surface->w;
      result->source.height = result->texture->unoptimized_surface->h;
      result->destination = destination;
      f_color_set(&(result->mask), (s_color)d_image_default_mask);
      result->head.f_barf_render = (l_barf_render)p_image_render;
      result->head.f_barf_delete = (l_barf_delete)p_image_delete;
    } else
      fprintf(stderr, "LOADING [failed] f_image_malloc says that '%s' cannot be loaded\n", source);
  }
  return (s_barf_object *)result;
}
