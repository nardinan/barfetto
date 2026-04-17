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
#include <time.h>
#include <fcntl.h>
#include "../../include/barfetto/terminator_listener.h"
#include "../../include/barfetto/renderer.h"
#include "../../include/barfetto/animation_pack.h"
#include "../../include/barfetto/entity.h"
#include "../../include/barfetto/ui/ui_label.h"
#include "../../include/barfetto/font_manager.h"
const char *animations[] = {
  "down",
  "left",
  "right",
  "up"
};
e_barf_object_listener_processed_events f_vertical_move_up(s_entity *self, s_renderer *renderer, SDL_Event *event) {
  return ((event->type == SDL_KEYDOWN) && (event->key.keysym.sym == SDLK_UP)) ? e_barf_object_listener_processed_event_consumed
                                                                               : e_barf_object_listener_processed_event_ignored;
}
e_barf_object_listener_processed_events f_vertical_move_down(s_entity *self, s_renderer *renderer, SDL_Event *event) {
  return ((event->type == SDL_KEYDOWN) && (event->key.keysym.sym == SDLK_DOWN)) ? e_barf_object_listener_processed_event_consumed
                                                                                : e_barf_object_listener_processed_event_ignored;
}
e_barf_object_listener_processed_events f_vertical_move_left(s_entity *self, s_renderer *renderer, SDL_Event *event) {
  return ((event->type == SDL_KEYDOWN) && (event->key.keysym.sym == SDLK_LEFT)) ? e_barf_object_listener_processed_event_consumed
                                                                                : e_barf_object_listener_processed_event_ignored;
}
e_barf_object_listener_processed_events f_vertical_move_right(s_entity *self, s_renderer *renderer, SDL_Event *event) {
  return ((event->type == SDL_KEYDOWN) && (event->key.keysym.sym == SDLK_RIGHT)) ? e_barf_object_listener_processed_event_consumed
                                                                                : e_barf_object_listener_processed_event_ignored;
}
void f_vertical_trigger_up(s_entity *self) {
  printf("Up up up\n");
}
int main(int argc, char *argv[]) {
  s_renderer renderer;
  s_font_manager font_manager;
  memset(&renderer, 0, sizeof(s_renderer));
  srand(time(NULL));
  s_animation_pack *animation_pack;
  f_renderer_initialize(&renderer, "BARF editor", 800, 600, 30);
  f_layer_append(f_renderer_get_layer(&renderer, 0), f_terminator_listener_malloc());
  f_font_manager_initialize(&(font_manager), "./fonts");
  f_layer_append(f_renderer_get_layer(&renderer, 0), f_ui_label_malloc(NULL, "Prova scrittura",
    f_font_manager_get(&(font_manager), "opensans.ttf", 24), (s_point){10, 10}, (s_point){128, 32}, (s_color){0, 255, 100, 255}));
  if ((animation_pack = (s_animation_pack *)f_animation_pack_malloc(NULL, "gianmario.png", (s_point){(rand() % 700) + 50, (rand() % 500) + 50}, 128, 140, 50))) {
    f_color_set(&(animation_pack)->mask, (s_color){255, 255, 255, 255});
    ((s_image *)f_renderer_get_layer(&renderer, 0)->last)->angle = 180;
    f_animation_pack_get_status(animation_pack, "down")->index_first_frame = 0;
    f_animation_pack_get_status(animation_pack, "down")->index_last_frame = 11;
    f_animation_pack_get_status(animation_pack, "down")->speed_y = ((float)(rand() % 100) / 1000.0) + 0.1;
    f_animation_pack_get_status(animation_pack, "left")->index_first_frame = 12;
    f_animation_pack_get_status(animation_pack, "left")->index_last_frame = 23;
    f_animation_pack_get_status(animation_pack, "left")->speed_x = -(((float)(rand() % 100) / 1000.0) + 0.1);
    f_animation_pack_get_status(animation_pack, "right")->index_first_frame = 24;
    f_animation_pack_get_status(animation_pack, "right")->index_last_frame = 35;
    f_animation_pack_get_status(animation_pack, "right")->speed_x = ((float)(rand() % 100) / 1000.0) + 0.1;
    f_animation_pack_get_status(animation_pack, "up")->index_first_frame = 36;
    f_animation_pack_get_status(animation_pack, "up")->index_last_frame = 47;
    f_animation_pack_get_status(animation_pack, "up")->speed_y = -(((float)(rand() % 100) / 1000.0) + 0.1);
    s_entity *entity = (s_entity *)f_entity_malloc(NULL, (s_barf_object *)animation_pack, (s_list_node *[]){
      (s_list_node *)f_entity_new_trigger_change_status("up", (l_barf_listen)f_vertical_move_up, (l_entity_event)f_vertical_trigger_up, true),
      (s_list_node *)f_entity_new_trigger_change_status("down", (l_barf_listen)f_vertical_move_down, NULL, true),
      (s_list_node *)f_entity_new_trigger_change_status("left", (l_barf_listen)f_vertical_move_left, NULL, true),
      (s_list_node *)f_entity_new_trigger_change_status("right", (l_barf_listen)f_vertical_move_right, NULL, true),
      NULL
    });
    f_layer_append(f_renderer_get_layer(&renderer, 0), (s_barf_object *)entity);
  }
  f_color_set(&(f_renderer_get_camera(&renderer, 0)->background_color), (s_color){ 255, 255, 255, 255 });
  f_renderer_get_camera(&renderer, 0)->show_contour = true;
  f_renderer_get_camera(&renderer, 0)->screen_destination = (s_rectangle){(s_point){0, 0}, 800, 600};
  f_renderer_get_camera(&renderer, 0)->contour_color.blue = 255;
  f_renderer_get_camera(&renderer, 0)->contour_color.alpha = 255;
  f_renderer_launch(&renderer);
  f_dictionary_free(&dictionary_surface_cache);
  f_font_manager_delete(&font_manager);
  f_memory_print_plain();
  return 0;
}
