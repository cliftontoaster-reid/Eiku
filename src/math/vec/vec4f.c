/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec4f.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:05:03 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/23 15:16:11 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "math/vec.h"

t_vec4f vec4f_add(t_vec4f a, t_vec4f b) {
  t_vec4f result;

  result.x = a.x + b.x;
  result.y = a.y + b.y;
  result.z = a.z + b.z;
  result.w = a.w + b.w;
  return (result);
}

t_vec4f vec4f_sub(t_vec4f a, t_vec4f b) {
  t_vec4f result;

  result.x = a.x - b.x;
  result.y = a.y - b.y;
  result.z = a.z - b.z;
  result.w = a.w - b.w;
  return (result);
}

t_vec4f vec4f_scale(t_vec4f v, float scalar) {
  t_vec4f result;

  result.x = v.x * scalar;
  result.y = v.y * scalar;
  result.z = v.z * scalar;
  result.w = v.w * scalar;
  return (result);
}

float vec4f_dot(t_vec4f a, t_vec4f b) {
  return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

float vec4f_length(t_vec4f v) {
  return (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w));
}

t_vec4f vec4f_normalize(t_vec4f v) {
  float length;
  t_vec4f result;

  length = vec4f_length(v);
  if (length == 0.0f) {
    result.x = 0.0f;
    result.y = 0.0f;
    result.z = 0.0f;
    result.w = 0.0f;
    return (result);
  }
  result.x = v.x / length;
  result.y = v.y / length;
  result.z = v.z / length;
  result.w = v.w / length;
  return (result);
}

float vec4f_distance(t_vec4f a, t_vec4f b) {
  t_vec4f diff;

  diff = vec4f_sub(a, b);
  return (vec4f_length(diff));
}

float vec4f_distance_squared(t_vec4f a, t_vec4f b) {
  t_vec4f diff;

  diff = vec4f_sub(a, b);
  return (diff.x * diff.x + diff.y * diff.y + diff.z * diff.z +
          diff.w * diff.w);
}

t_vec4f vec4f_lerp(t_vec4f a, t_vec4f b, float t) {
  t_vec4f result;

  result.x = a.x + t * (b.x - a.x);
  result.y = a.y + t * (b.y - a.y);
  result.z = a.z + t * (b.z - a.z);
  result.w = a.w + t * (b.w - a.w);
  return (result);
}

t_vec4f vec4f_clamp_length(t_vec4f v, float max_length) {
  float length;

  length = vec4f_length(v);
  if (length > max_length && length > 0.0f)
    return (vec4f_scale(vec4f_normalize(v), max_length));
  return (v);
}

bool vec4f_equal(t_vec4f a, t_vec4f b, float epsilon) {
  return (fabsf(a.x - b.x) <= epsilon && fabsf(a.y - b.y) <= epsilon &&
          fabsf(a.z - b.z) <= epsilon && fabsf(a.w - b.w) <= epsilon);
}

bool vec4f_is_zero(t_vec4f v, float epsilon) {
  return (fabsf(v.x) <= epsilon && fabsf(v.y) <= epsilon &&
          fabsf(v.z) <= epsilon && fabsf(v.w) <= epsilon);
}

t_vec4f vec4f_zero(void) {
  t_vec4f result;

  result.x = 0.0f;
  result.y = 0.0f;
  result.z = 0.0f;
  result.w = 0.0f;
  return (result);
}

t_vec4f vec4f_one(void) {
  t_vec4f result;

  result.x = 1.0f;
  result.y = 1.0f;
  result.z = 1.0f;
  result.w = 1.0f;
  return (result);
}

t_vec4f vec4f_min(t_vec4f a, t_vec4f b) {
  t_vec4f result;

  result.x = fminf(a.x, b.x);
  result.y = fminf(a.y, b.y);
  result.z = fminf(a.z, b.z);
  result.w = fminf(a.w, b.w);
  return (result);
}

t_vec4f vec4f_max(t_vec4f a, t_vec4f b) {
  t_vec4f result;

  result.x = fmaxf(a.x, b.x);
  result.y = fmaxf(a.y, b.y);
  result.z = fmaxf(a.z, b.z);
  result.w = fmaxf(a.w, b.w);
  return (result);
}

t_vec4f vec4f_abs(t_vec4f v) {
  t_vec4f result;

  result.x = fabsf(v.x);
  result.y = fabsf(v.y);
  result.z = fabsf(v.z);
  result.w = fabsf(v.w);
  return (result);
}