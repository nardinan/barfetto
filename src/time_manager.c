/**
* MIT License
 * Copyright (c) [$YEAR] The Barfing Fox - TBF [nardinan (andrea@nardinan.it)]
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
#include "../include/barfetto/time_manager.h"
static void p_time_manager_clock_group_delete(s_time_manager_clock_group *clock_group) {
  f_dictionary_free(&(clock_group->clocks));
}
void f_time_manager_initialize(s_time_manager *time_manager) {
  f_dictionary_initialize_custom(&(time_manager->groups), sizeof(s_time_manager_clock_group), NULL,
    (l_dictionary_node_delete)p_time_manager_clock_group_delete);
}
void f_time_manager_delete(s_time_manager *time_manager) {
  f_dictionary_free(&(time_manager->groups));
}
s_time_manager_clock *f_time_manager_get_clock(s_time_manager *time_manager, const char *group_name, const char *clock_name) {
  s_time_manager_clock_group *group;
  s_time_manager_clock *result = NULL;
  bool created = false;
  if ((group = (s_time_manager_clock_group *)f_dictionary_get_or_create_informed(&(time_manager->groups), group_name, &created))) {
    if (created) {
      f_dictionary_initialize(&(group->clocks), sizeof(s_time_manager_clock));
      group->time_speed = 1.0;
    }
    if ((result = (s_time_manager_clock *)f_dictionary_get_or_create_informed(&(group->clocks), clock_name, &created))) {
      if (created) {
        gettimeofday(&(result->last_update), NULL);
        result->reference_group = group;
        result->time_speed = 1.0;
      }
    }
  }
  return result;
}
void f_time_manager_set_group_speed(s_time_manager *time_manager, const char *group_name, double time_speed) {
  s_time_manager_clock_group *group;
  bool created = false;
  if ((group = (s_time_manager_clock_group *)f_dictionary_get_or_create_informed(&(time_manager->groups), group_name, &created))) {
    if (created)
      f_dictionary_initialize(&(group->clocks), sizeof(s_time_manager_clock));
    group->time_speed = time_speed;
  }
}
time_t f_time_manager_elapsed_time_milliseconds(s_time_manager_clock *clock, bool update) {
  time_t result = 0;
  struct timeval current_update, delta;
  gettimeofday(&current_update, NULL);
  timersub(&(current_update), &(clock->last_update), &(delta));
  result = (time_t)((((delta.tv_usec / 1000.0) + (delta.tv_sec * 1000.0)) * clock->time_speed) * clock->reference_group->time_speed);
  if (update)
    f_time_manager_reset(clock);
  return result;
}
void f_time_manager_reset(s_time_manager_clock *clock) {
  gettimeofday(&(clock->last_update), NULL);
}