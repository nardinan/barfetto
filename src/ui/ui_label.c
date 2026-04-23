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
  if (self->reference_font) {
    if ((self->head.unoptimized_surface) || (self->head.unoptimized_surface = TTF_RenderText_Blended(self->reference_font,
      ((self->content) ? self->content : ""), (SDL_Color){self->color.red, self->color.green, self->color.blue, self->color.alpha}))) {
      if ((!self->head.texture) && (self->head.unoptimized_surface))
        self->head.texture = SDL_CreateTextureFromSurface(renderer->renderer, self->head.unoptimized_surface);
      if (self->head.texture) {
        int picked_width, picked_height;
        if ((self->visible_area.width == 0) &&
          (self->visible_area.height == 0)) {
          picked_width = (self->head.unoptimized_surface->w - self->visible_area.origin.x);
          picked_height = (self->head.unoptimized_surface->h - self->visible_area.origin.y);
          } else {
            /* we need to pick the smallest number between the texture size and the visible area: this is going to be our width
             * for both source and height */
            picked_width = ((self->head.unoptimized_surface->w - self->visible_area.origin.x) > self->visible_area.width) ? self->visible_area.width :
            (self->head.unoptimized_surface->w - self->visible_area.origin.x);
            picked_height = ((self->head.unoptimized_surface->h - self->visible_area.origin.y) > self->visible_area.height) ? self->visible_area.height :
            (self->head.unoptimized_surface->h - self->visible_area.origin.y);
          }
        SDL_Rect destination = {
          .x = (self->destination.x - renderer->selected_camera->visible_area.origin.x),
          .y = (self->destination.y - renderer->selected_camera->visible_area.origin.y),
          .w = picked_width,
          .h = picked_height
        }, source = {
          .x = self->visible_area.origin.x,
          .y = self->visible_area.origin.y,
          .w = picked_width,
          .h = picked_height
        };
        SDL_RenderCopy(renderer->renderer, self->head.texture, &source, &destination);
      }
      }
  }
}
static void p_ui_label_delete(s_ui_label *self) {
  if (self->content) {
    d_free(self->content);
    self->content = NULL;
  }
  if (self->head.unoptimized_surface)
    SDL_FreeSurface(self->head.unoptimized_surface);
  if (self->head.texture)
    SDL_DestroyTexture(self->head.texture);
  self->head.unoptimized_surface = NULL;
  self->head.texture = NULL;
}
s_barf_object *f_ui_label_malloc(s_ui_label *holder, const char *content, TTF_Font *reference_font, s_point destination) {
  s_ui_label *result = holder;
  if ((result) || (result = (s_ui_label *)d_malloc(sizeof(s_ui_label)))) {
    memset(result, 0, sizeof(s_ui_label));
    result->destination = destination;
    result->reference_font = reference_font;
    f_ui_label_update_text(result, content);
    result->head.head.f_barf_render = (l_barf_render)p_ui_label_render;
    result->head.head.f_barf_delete = (l_barf_delete)p_ui_label_delete;
  }
  return (s_barf_object *)result;
}
void f_ui_label_force_refresh(s_ui_label *self) {
  if (self->head.unoptimized_surface) {
    SDL_FreeSurface(self->head.unoptimized_surface);
    self->head.unoptimized_surface = NULL;
  }
  if (self->head.texture) {
    SDL_DestroyTexture(self->head.texture);
    self->head.texture = NULL;
  }
}
void f_ui_label_update_text(s_ui_label *self, const char *content) {
  if (content) {
    size_t length_content = strlen(content);
    if (self->allocated_space < (length_content + 1))
      if ((self->content = d_realloc(self->content, (length_content + 1))))
        self->allocated_space = (length_content + 1);
    if (self->content) {
      memset(self->content, 0, (length_content + 1));
      strncpy(self->content, content, length_content);
    }
  } else if (self->content)
    self->content[0] = 0;
  f_ui_label_force_refresh(self);
}
float f_ui_label_get_width_content(s_ui_label *self) {
  float width = 0;
  if (self->reference_font) {
    if ((self->head.unoptimized_surface) || (self->head.unoptimized_surface = TTF_RenderText_Blended(self->reference_font,
      d_ui_label_get_printable_content(self), (SDL_Color){self->color.red, self->color.green, self->color.blue, self->color.alpha}))) {
      width = self->head.unoptimized_surface->w;
    }
  }
  return width;
}
float f_ui_label_get_height_content(s_ui_label *self) {
  float height = 0;
  if (self->reference_font) {
    if ((self->head.unoptimized_surface) || (self->head.unoptimized_surface = TTF_RenderText_Blended(self->reference_font,
      d_ui_label_get_printable_content(self), (SDL_Color){self->color.red, self->color.green, self->color.blue, self->color.alpha}))) {
      height = self->head.unoptimized_surface->h;
    }
  }
  return height;
}