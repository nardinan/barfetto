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
#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H
#include <sys/time.h>
#include "coremio/dictionary.h"
typedef struct s_time_manager_clock_group {
  s_dictionary_node head;
  s_dictionary clocks;
  double time_speed;
} s_time_manager_clock_group;
typedef struct s_time_manager_clock {
  s_dictionary_node head;
  const s_time_manager_clock_group *reference_group;
  struct timeval last_update;
  double time_speed;
} s_time_manager_clock;
typedef struct s_time_manager {
  s_dictionary groups;
} s_time_manager;
extern void f_time_manager_initialize(s_time_manager *time_manager);
extern void f_time_manager_delete(s_time_manager *time_manager);
extern s_time_manager_clock *f_time_manager_get_clock(s_time_manager *time_manager, const char *group_name, const char *clock_name);
extern void f_time_manager_set_group_speed(s_time_manager *time_manager, const char *group_name, double time_speed);
extern time_t f_time_manager_elapsed_time_milliseconds(s_time_manager_clock *clock, bool update);
extern void f_time_manager_reset(s_time_manager_clock *clock);
#endif //TIME_MANAGER_H
