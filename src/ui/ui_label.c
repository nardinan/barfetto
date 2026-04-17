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
#include "../../include/barfetto/ui/ui_label.h"
#include "../../include/barfetto/renderer.h"
static void p_ui_label_render(s_ui_label *self, struct s_renderer *renderer) {
  if (self->font) {
    if ((self->head.unoptimized_surface) || (self->head.unoptimized_surface = TTF_RenderText_Solid(self->font, self->text,
      (SDL_Color){self->color.red, self->color.green, self->color.blue, self->color.alpha}))) {
      if ((!self->head.texture) && (self->head.unoptimized_surface))
        self->head.texture = SDL_CreateTextureFromSurface(renderer->renderer, self->head.unoptimized_surface);
      if (self->head.texture) {
        SDL_Rect destination = {
          .x = (self->destination.x - renderer->selected_camera->visible_area.origin.x),
          .y = (self->destination.y - renderer->selected_camera->visible_area.origin.y),
          .w = self->head.unoptimized_surface->w,
          .h = self->head.unoptimized_surface->h
        }, source = {
          .x = self->source.origin.x,
          .y = self->source.origin.y,
          .w = self->source.width,
          .h = self->source.height
        }, *picked_source = &source;
        if ((source.x == 0) && (source.y == 0) && (source.w == 0) && (source.h == 0))
          picked_source = NULL;
        SDL_RenderCopy(renderer->renderer, self->head.texture, picked_source, &destination);
      }
      }
  }
}
static void p_ui_label_delete(s_ui_label *self) {
  if (self->head.unoptimized_surface) {
    SDL_FreeSurface(self->head.unoptimized_surface);
    self->head.unoptimized_surface = NULL;
  }
  if (self->head.texture) {
    SDL_DestroyTexture(self->head.texture);
    self->head.texture = NULL;
  }
}
s_barf_object *f_ui_label_malloc(s_ui_label *holder, const char *text, TTF_Font *font, s_point destination, s_rectangle source, s_color color) {
  s_ui_label *result = holder;
  if ((result) || (result = (s_ui_label *)d_malloc(sizeof(s_ui_label)))) {
    memset(result, 0, sizeof(s_ui_label));
    result->destination = destination;
    result->source = source;
    result->color = color;
    result->font = font;
    strncpy(result->text, text, (d_ui_label_size - 1));
    result->head.head.f_barf_render = (l_barf_render)p_ui_label_render;
    result->head.head.f_barf_delete = (l_barf_delete)p_ui_label_delete;
  }
  return (s_barf_object *)result;
}
void f_ui_label_update_text(s_ui_label *self, const char *text) {
  memset(self->text, 0, d_ui_label_size);
  strncpy(self->text, text, (d_ui_label_size - 1));
  /* we need to clean everything, so it will be re-generated during the next update */
  if (self->head.unoptimized_surface) {
    SDL_FreeSurface(self->head.unoptimized_surface);
    self->head.unoptimized_surface = NULL;
  }
  if (self->head.texture) {
    SDL_DestroyTexture(self->head.texture);
    self->head.texture = NULL;
  }
}
