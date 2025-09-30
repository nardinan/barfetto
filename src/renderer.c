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
#include "../include/barfetto/renderer.h"
d_result_define(SHIT_NO_SDL, 1, "Failure, cannot initialize SDL library");
d_result_define(SHIT_NO_SDL_WINDOW, 2, "Failure, cannot initialize a SDL window");
d_result_define(SHIT_NO_SDL_RENDERER, 3, "Failure, cannot create and initialize a SDL renderer");
d_result_define(SHIT_NO_SDL_IMG, 4, "Failure, cannot initialize SDL IMG library");
coremio_result f_renderer_initialize(s_renderer *renderer, const char *title, size_t screen_width, size_t screen_height, size_t frames_per_second) {
  coremio_result result = NOICE;
  memset(renderer, 0, sizeof(s_renderer));
  if (((renderer->layers = (s_layer **)f_array_malloc(d_renderer_layer_bucket, sizeof(s_layer *)))) &&
      ((renderer->cameras = (s_camera **)f_array_malloc(d_renderer_camera_bucket, sizeof(s_camera *))))) {
    /* TTF initialization */
    if (SDL_Init(SDL_INIT_VIDEO) >= 0) {
      SDL_StartTextInput();
      renderer->screen_width = screen_width;
      renderer->screen_height = screen_height;
      renderer->frames_per_second = frames_per_second;
      if ((renderer->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
              SDL_WINDOWPOS_CENTERED, renderer->screen_width, renderer->screen_height, (SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL)))) {
        if ((renderer->renderer = SDL_CreateRenderer(renderer->window, -1, (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))) {
          int real_screen_width, real_screen_height;
          SDL_SetRenderDrawBlendMode(renderer->renderer, SDL_BLENDMODE_BLEND);
          SDL_GetWindowSize(renderer->window, &real_screen_width, &real_screen_height);
          renderer->real_screen_width = real_screen_width;
          renderer->real_screen_height = real_screen_height;
          if (IMG_Init((IMG_INIT_PNG | IMG_INIT_JPG)) != (IMG_INIT_PNG | IMG_INIT_JPG))
            result = SHIT_NO_SDL_IMG;
        } else
          result = SHIT_NO_SDL_RENDERER;
      } else
        result = SHIT_NO_SDL_WINDOW;
    } else
      result = SHIT_NO_SDL;
    if (result != NOICE)
      fprintf(stderr, "INITIALIZATION [failed] f_renderer_initialize returned %s and SDL says '%s'\n", result->name, SDL_GetError());
  } else
    result = SHIT;
  return result;
}
static void p_renderer_teardown(s_renderer *renderer) {
  if (renderer->layers) {
    for (size_t index = 0; index < d_array_size(renderer->layers); ++index)
      if (renderer->layers[index]) {
        s_barf_object *current_object = NULL;
        while ((current_object = (s_barf_object *)(renderer->layers[index]->list.head))) {
          f_list_remove(&(renderer->layers[index]->list), (s_list_node *)current_object);
          if (current_object->f_barf_delete)
            current_object->f_barf_delete(current_object);
          d_free(current_object);
        }
        d_free(renderer->layers[index]);
        renderer->layers[index] = NULL;
      }
    f_array_free(renderer->layers);
  }
  if (renderer->cameras) {
    for (size_t index = 0; index < d_array_size(renderer->cameras); ++index)
      if (renderer->cameras[index]) {
        if (((s_barf_object *)renderer->cameras[index])->f_barf_delete)
          ((s_barf_object *)renderer->cameras[index])->f_barf_delete((s_barf_object *)renderer->cameras[index]);
        d_free(renderer->cameras[index]);
        renderer->cameras[index] = NULL;
      }
    f_array_free(renderer->cameras);
  }
  if (renderer->renderer)
    SDL_DestroyRenderer(renderer->renderer);
  if (renderer->window)
    SDL_DestroyWindow(renderer->window);
  SDL_StopTextInput();
  SDL_Quit();
}
e_renderer_statuses f_renderer_launch(s_renderer *renderer) {
  e_renderer_statuses result = e_renderer_status_running;
  unsigned int ticks_per_frame = (1000 / renderer->frames_per_second);
  while (result == e_renderer_status_running) {
    unsigned int initial_ticks = SDL_GetTicks(), final_ticks, elapsed_ticks;
    size_t index_acquired_event = 0;
    memset(renderer->events, d_renderer_events_pool_bucket, sizeof(SDL_Event));
    while ((index_acquired_event < d_renderer_events_pool_bucket) &&
        (SDL_PollEvent(&(renderer->events[index_acquired_event])) != 0))
      ++index_acquired_event;
    SDL_RenderClear(renderer->renderer);
    for (size_t index_camera = 0;
        (index_camera < d_array_size(renderer->cameras)) &&
        (result == e_renderer_status_running);
        ++index_camera)
      if ((renderer->cameras[index_camera]) && (renderer->cameras[index_camera]->visible)) {
        renderer->selected_camera = renderer->cameras[index_camera];
        f_camera_enable_rendering(renderer->selected_camera, renderer);
        for (size_t index_layer = 0;
            (index_layer < d_array_size(renderer->layers)) &&
            (result == e_renderer_status_running);
            ++index_layer)
          if ((renderer->layers[index_layer]) && (renderer->layers[index_layer]->visible)) {
            s_barf_object *current_object;
            d_list_foreach(&(renderer->layers[index_layer]->list), current_object, s_barf_object) {
              if (current_object->f_barf_listen) {
                e_barf_object_listener_processed_events listener_processed_event = e_barf_object_listener_processed_event_ignored;
                for (size_t index_event = 0;
                    (index_event < index_acquired_event) &&
                    (listener_processed_event != e_barf_object_listener_processed_event_terminate); ) {
                  if ((listener_processed_event = current_object->f_barf_listen(current_object, renderer, &(renderer->events[index_event]))) ==
                      e_barf_object_listener_processed_event_consumed) {
                    if ((index_acquired_event - index_event) > 1)
                      memmove(&(renderer->events[index_event]), &(renderer->events[index_event + 1]),
                          (sizeof(SDL_Event) * (index_acquired_event - index_event - 1)));
                    --index_acquired_event;
                  } else
                    ++index_event;
                }
                if (listener_processed_event == e_barf_object_listener_processed_event_terminate)
                  result = e_renderer_status_quit;
              }
              if (current_object->f_barf_render) {
                current_object->f_barf_render(current_object, renderer);
                current_object->ticks_last_render = initial_ticks;
              }
            }
          }
        f_camera_disable_rendering(renderer->selected_camera, renderer);
      }
    renderer->selected_camera = NULL;
    if (renderer->f_barf_update_scene)
      result = renderer->f_barf_update_scene(renderer);
    SDL_RenderPresent(renderer->renderer);
    final_ticks = SDL_GetTicks();
    if ((elapsed_ticks = (final_ticks - initial_ticks)) < ticks_per_frame)
      SDL_Delay(ticks_per_frame - elapsed_ticks);
  }
  if ((result == e_renderer_status_quit) && (renderer->f_barf_teardown))
    renderer->f_barf_teardown(renderer);
  p_renderer_teardown(renderer);
  return result;
}
s_layer *f_renderer_get_layer(s_renderer *renderer, unsigned int layer) {
  s_layer *result = NULL;
  if ((renderer->layers = (s_layer **)f_array_validate_access(renderer->layers, layer)))
    if (!(result = renderer->layers[layer]))
      if ((renderer->layers[layer] = (struct s_layer *)d_malloc(sizeof(s_layer)))) {
        memset(renderer->layers[layer], 0, sizeof(s_layer));
        renderer->layers[layer]->visible = true;
        result = renderer->layers[layer];
      }
  return result;
}
s_camera *f_renderer_get_camera(s_renderer *renderer, unsigned int camera) {
  s_camera *result = NULL;
  if ((renderer->cameras = (s_camera **)f_array_validate_access(renderer->cameras, camera)))
    if (!(result = renderer->cameras[camera])) {
      const s_rectangle visible_area = {(s_point){0, 0}, renderer->screen_width, renderer->screen_height};
      if ((renderer->cameras[camera] = (s_camera *)f_camera_malloc(NULL, visible_area, visible_area))) {
        renderer->cameras[camera]->visible = true;
        result = renderer->cameras[camera];
      }
    }
  return result;
}