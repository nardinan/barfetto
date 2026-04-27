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
#include "../include/barfetto/color.h"
void f_color_set(s_color *destination, s_color source) {
  destination->red = source.red;
  destination->green = source.green;
  destination->blue = source.blue;
  destination->alpha = source.alpha;
}
static s_color p_color_json_unserialize(s_json *json, s_json_node *node) {
  s_color result = {0, 0, 0, 255};
  if ((json) && (node) && (node->type == e_json_type_array)) {
    result.red = f_json_get_value(json, node, "d", (long)0);
    result.green = f_json_get_value(json, node, "d", (long)1);
    result.blue = f_json_get_value(json, node, "d", (long)2);
    result.alpha = f_json_get_value(json, node, "d", (long)3);
  }
  return result;
}
s_color f_color_json_get(s_json *json, s_json_node *parent, const char *key, const s_color default_value) {
  s_color result = default_value;
  s_json_node *node;
  if ((json) && (parent) && (key) && ((node = f_json_get_node(json, parent, "s", (char *)key))))
    result = p_color_json_unserialize(json, node);
  return result;
}