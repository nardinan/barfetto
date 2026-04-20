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
#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "point.h"
typedef struct s_rectangle {
  s_point origin;
  size_t width, height;
} s_rectangle;
extern s_rectangle f_rectangle_unserialize(s_json *json, s_json_node *node);
extern s_rectangle f_rectangle_get(s_json *json, s_json_node *parent, const char *key, const s_rectangle default_value);
extern bool f_rectangle_overlaps(const s_rectangle left, const s_rectangle right);
extern bool f_rectangle_touches(const s_rectangle left, const s_rectangle right);
extern bool f_rectangle_contains_point(const s_rectangle rectangle, const s_point point);
extern bool f_rectangle_intersection(const s_rectangle left, const s_rectangle right, s_rectangle *out);
#endif /* RECTANGLE_H */

