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
#include "../include/barfetto/point.h"
s_point f_point_unserialize(s_json *json, s_json_node *node) {
  s_point result = {0.0, 0.0};
  if ((json) && (node) && (node->type == e_json_type_array)) {
    result.x = f_json_get_value(json, node, "d", (long)0);
    result.y = f_json_get_value(json, node, "d", (long)1);
  }
  return result;
}
s_point f_point_get(s_json *json, s_json_node *parent, const char *key, const s_point default_value) {
  s_point result = default_value;
  s_json_node *node;
  if ((json) && (parent) && (key) && ((node = f_json_get_node(json, parent, "s", (char *)key))))
    result = f_point_unserialize(json, node);
  return result;
}