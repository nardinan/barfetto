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
#include "../include/barfetto/camera.h"
#include "../include/barfetto/renderer.h"
void p_camera_delete(s_camera *self) {
  if (self->container)
    SDL_DestroyTexture(self->container);
}
s_barf_object *f_camera_malloc(s_camera *holder, s_rectangle visible_area, s_rectangle screen_destination) {
  s_camera *result = holder;
  if ((result) || (result = (s_camera *)malloc(sizeof(s_camera)))) {
    memset(result, 0, sizeof(s_camera));
    result->visible_area = visible_area;
    result->screen_destination= screen_destination;
    result->head.f_barf_delete = (l_barf_delete)p_camera_delete;
  }
  return (s_barf_object *)result;
}
void f_camera_enable_rendering(s_camera *self, struct s_renderer *renderer) {
  if ((self->visible_area.width != self->_last_width) || (self->visible_area.height != self->_last_height)) {
    if (self->container)
      SDL_DestroyTexture(self->container);
    if ((self->container = SDL_CreateTexture(renderer->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, self->visible_area.width,
            self->visible_area.height))) {
      self->_last_width = self->visible_area.width;
      self->_last_height = self->visible_area.height;
    } else
      fprintf(stderr, "CREATION [failed] f_camera_enable_rendering cannot create the hosting texture (SDL says '%s')\n", SDL_GetError());
  }
  if (self->container) {
    SDL_Rect visible_area = {
      .x = 0,
      .y = 0,
      .w = self->visible_area.width,
      .h = self->visible_area.height
    };
    SDL_SetTextureBlendMode(self->container, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer->renderer, self->container);
    SDL_SetRenderDrawColor(renderer->renderer, self->background_color.red, self->background_color.green, self->background_color.blue,
        self->background_color.alpha);
    SDL_RenderFillRect(renderer->renderer, &visible_area);
  }
}
void f_camera_disable_rendering(s_camera *self, struct s_renderer *renderer) {
  if (self->container) {
    SDL_Rect screen_destination = {
      .x = self->screen_destination.origin.x,
      .y = self->screen_destination.origin.y,
      .w = self->screen_destination.width,
      .h = self->screen_destination.height
    };
    if (self->show_contour) {
      SDL_SetRenderDrawColor(renderer->renderer, self->contour_color.red, self->contour_color.green, self->contour_color.blue,
          self->contour_color.alpha);
      SDL_RenderDrawRect(renderer->renderer, NULL);
      SDL_SetRenderDrawColor(renderer->renderer, self->background_color.red, self->background_color.green, self->background_color.blue,
        self->background_color.alpha);
    }
    SDL_SetRenderTarget(renderer->renderer, NULL);
    SDL_RenderCopy(renderer->renderer, self->container, NULL, &screen_destination);
  }
}
void f_camera_get_mouse_position(s_camera *self, int *mouse_x, int *mouse_y) {
  float ratio_x = ((float)self->visible_area.width / (float)self->screen_destination.width),
         ratio_y = ((float)self->visible_area.height / (float)self->screen_destination.height);
  int raw_mouse_x, raw_mouse_y;
  SDL_GetMouseState(&raw_mouse_x, &raw_mouse_y);
  *mouse_x = ((raw_mouse_x - self->screen_destination.origin.x) * ratio_x) + self->visible_area.origin.x;
  *mouse_y = ((raw_mouse_y - self->screen_destination.origin.y) * ratio_y) + self->visible_area.origin.y;
}
