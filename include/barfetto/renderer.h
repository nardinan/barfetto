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
#ifndef RENDERER_H
#define RENDERER_H
#include <SDL.h>
#include <SDL_image.h>
#include <string.h>
#include <coremio/array.h>
#include <coremio/result.h>
#include "color.h"
#include "layer.h"
#include "camera.h"
#define d_renderer_layer_bucket 8
#define d_renderer_camera_bucket 8
#define d_renderer_events_pool_bucket 32
d_result_declare(SHIT_NO_SDL);
d_result_declare(SHIT_NO_SDL_WINDOW);
d_result_declare(SHIT_NO_SDL_RENDERER);
d_result_declare(SHIT_NO_SDL_IMG);
struct s_renderer;
typedef enum e_renderer_statuses {
  e_renderer_status_running = 0,
  e_renderer_status_quit,
  e_renderer_status_abort
} e_renderer_statuses;
typedef void (*l_barf_initialize)(struct s_renderer *);
typedef e_renderer_statuses (*l_barf_update_scene)(struct s_renderer *);
typedef e_renderer_statuses (*l_barf_update_logic)(struct s_renderer *);
typedef void (*l_barf_teardown)(struct s_renderer *);
typedef struct s_renderer {
  l_barf_initialize f_barf_initialize;
  l_barf_update_scene f_barf_update_scene;
  l_barf_update_logic f_barf_update_logic;
  l_barf_teardown f_barf_teardown;
  size_t screen_width, screen_height, real_screen_width, real_screen_height, frames_per_second;
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event events[d_renderer_events_pool_bucket];
  s_camera **cameras, *selected_camera;
  s_layer **layers;
} s_renderer;
extern coremio_result f_renderer_initialize(s_renderer *renderer, const char *title, size_t screen_width, size_t screen_height, size_t frames_per_second);
extern e_renderer_statuses f_renderer_launch(s_renderer *renderer);
extern s_layer *f_renderer_get_layer(s_renderer *renderer, unsigned int layer);
extern s_camera *f_renderer_get_camera(s_renderer *renderer, unsigned int camera);
#endif /* RENDERER_H */

