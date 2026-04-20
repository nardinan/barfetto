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
#include "../include/barfetto/rectangle.h"
static double p_rectangle_value(s_json *json, s_json_node *node, const char *key, const double default_value) {
  double result = default_value;
  s_json_node *value_node;
  if ((value_node = f_json_get_node(json, node, "s", (char *)key)))
    if (value_node->type == e_json_type_value)
      result = f_json_get_value(json, node, "s", (char *)key);
  return result;
}
static float p_rectangle_min(const float left, const float right) {
  return ((left < right) ? left : right);
}
static float p_rectangle_max(const float left, const float right) {
  return ((left > right) ? left : right);
}
s_rectangle f_rectangle_unserialize(s_json *json, s_json_node *node) {
  s_rectangle result = {{0.0, 0.0}, 0, 0};
  if ((json) && (node)) {
    result.origin.x = p_rectangle_value(json, node, "x", 0.0);
    result.origin.y = p_rectangle_value(json, node, "y", 0.0);
    result.width = p_rectangle_value(json, node, "width", p_rectangle_value(json, node, "w", 0.0));
    result.height = p_rectangle_value(json, node, "height", p_rectangle_value(json, node, "h", 0.0));
  }
  return result;
}
s_rectangle f_rectangle_get(s_json *json, s_json_node *parent, const char *key, const s_rectangle default_value) {
  s_rectangle result = default_value;
  s_json_node *node;
  if ((json) && (parent) && (key) && ((node = f_json_get_node(json, parent, "s", (char *)key))))
    result = f_rectangle_unserialize(json, node);
  return result;
}
bool f_rectangle_overlaps(const s_rectangle left, const s_rectangle right) {
  bool result = false;
  if ((left.origin.x < (right.origin.x + (float)right.width)) &&
      ((left.origin.x + (float)left.width) > right.origin.x) &&
      (left.origin.y < (right.origin.y + (float)right.height)) &&
      ((left.origin.y + (float)left.height) > right.origin.y))
    result = true;
  return result;
}
bool f_rectangle_touches(const s_rectangle left, const s_rectangle right) {
  bool result = false;
  if ((!f_rectangle_overlaps(left, right)) &&
      (left.origin.x <= (right.origin.x + (float)right.width)) &&
      ((left.origin.x + (float)left.width) >= right.origin.x) &&
      (left.origin.y <= (right.origin.y + (float)right.height)) &&
      ((left.origin.y + (float)left.height) >= right.origin.y))
    result = true;
  return result;
}
bool f_rectangle_contains_point(const s_rectangle rectangle, const s_point point) {
  bool result = false;
  if ((point.x >= rectangle.origin.x) && (point.x <= (rectangle.origin.x + (float)rectangle.width)) &&
      (point.y >= rectangle.origin.y) && (point.y <= (rectangle.origin.y + (float)rectangle.height)))
    result = true;
  return result;
}
bool f_rectangle_intersection(const s_rectangle left, const s_rectangle right, s_rectangle *out) {
  bool result = false;
  if (out) {
    const float origin_x = p_rectangle_max(left.origin.x, right.origin.x), origin_y = p_rectangle_max(left.origin.y, right.origin.y),
        far_x = p_rectangle_min(left.origin.x + (float)left.width, right.origin.x + (float)right.width),
        far_y = p_rectangle_min(left.origin.y + (float)left.height, right.origin.y + (float)right.height);
    out->origin.x = origin_x;
    out->origin.y = origin_y;
    if ((far_x > origin_x) && (far_y > origin_y)) {
      out->width = (size_t)(far_x - origin_x);
      out->height = (size_t)(far_y - origin_y);
      result = true;
    } else {
      out->width = 0;
      out->height = 0;
    }
  }
  return result;
}
