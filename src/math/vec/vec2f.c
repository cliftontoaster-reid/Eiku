/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec2f.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:05:03 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/23 15:16:11 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "math/vec.h"

t_vec2f vec2f_add(t_vec2f a, t_vec2f b) {
  t_vec2f result;

  result.x = a.x + b.x;
  result.y = a.y + b.y;
  return (result);
}

t_vec2f vec2f_sub(t_vec2f a, t_vec2f b) {
  t_vec2f result;

  result.x = a.x - b.x;
  result.y = a.y - b.y;
  return (result);
}

t_vec2f vec2f_scale(t_vec2f v, float scalar) {
  t_vec2f result;

  result.x = v.x * scalar;
  result.y = v.y * scalar;
  return (result);
}

float vec2f_dot(t_vec2f a, t_vec2f b) { return (a.x * b.x + a.y * b.y); }

float vec2f_length(t_vec2f v) { return (sqrtf(v.x * v.x + v.y * v.y)); }

t_vec2f vec2f_normalize(t_vec2f v) {
  float length;
  t_vec2f result;

  length = vec2f_length(v);
  if (length == 0.0f) {
    result.x = 0.0f;
    result.y = 0.0f;
    return (result);
  }
  result.x = v.x / length;
  result.y = v.y / length;
  return (result);
}

float vec2f_distance(t_vec2f a, t_vec2f b) {
  t_vec2f diff;

  diff = vec2f_sub(a, b);
  return (vec2f_length(diff));
}

float vec2f_distance_squared(t_vec2f a, t_vec2f b) {
  t_vec2f diff;

  diff = vec2f_sub(a, b);
  return (diff.x * diff.x + diff.y * diff.y);
}

float vec2f_angle(t_vec2f v) { return (atan2f(v.y, v.x)); }

t_vec2f vec2f_from_angle(float angle, float length) {
  t_vec2f result;

  result.x = cosf(angle) * length;
  result.y = sinf(angle) * length;
  return (result);
}

float vec2f_angle_between(t_vec2f a, t_vec2f b) {
  float dot;
  float len_a;
  float len_b;
  float cos_angle;

  len_a = vec2f_length(a);
  len_b = vec2f_length(b);
  if (len_a == 0.0f || len_b == 0.0f) return (0.0f);
  dot = vec2f_dot(a, b);
  cos_angle = dot / (len_a * len_b);
  if (cos_angle > 1.0f)
    cos_angle = 1.0f;
  else if (cos_angle < -1.0f)
    cos_angle = -1.0f;
  return (acosf(cos_angle));
}

t_vec2f vec2f_lerp(t_vec2f a, t_vec2f b, float t) {
  t_vec2f result;

  result.x = a.x + t * (b.x - a.x);
  result.y = a.y + t * (b.y - a.y);
  return (result);
}

t_vec2f vec2f_rotate(t_vec2f v, float angle) {
  float cos_a;
  float sin_a;
  t_vec2f result;

  cos_a = cosf(angle);
  sin_a = sinf(angle);
  result.x = v.x * cos_a - v.y * sin_a;
  result.y = v.x * sin_a + v.y * cos_a;
  return (result);
}

t_vec2f vec2f_perpendicular(t_vec2f v) {
  t_vec2f result;

  result.x = -v.y;
  result.y = v.x;
  return (result);
}

t_vec2f vec2f_reflect(t_vec2f v, t_vec2f normal) {
  float dot;
  t_vec2f result;

  dot = vec2f_dot(v, normal);
  result.x = v.x - 2.0f * dot * normal.x;
  result.y = v.y - 2.0f * dot * normal.y;
  return (result);
}

t_vec2f vec2f_clamp_length(t_vec2f v, float max_length) {
  float length;

  length = vec2f_length(v);
  if (length > max_length && length > 0.0f)
    return (vec2f_scale(vec2f_normalize(v), max_length));
  return (v);
}

bool vec2f_equal(t_vec2f a, t_vec2f b, float epsilon) {
  return (fabsf(a.x - b.x) <= epsilon && fabsf(a.y - b.y) <= epsilon);
}

bool vec2f_is_zero(t_vec2f v, float epsilon) {
  return (fabsf(v.x) <= epsilon && fabsf(v.y) <= epsilon);
}

t_vec2f vec2f_zero(void) {
  t_vec2f result;

  result.x = 0.0f;
  result.y = 0.0f;
  return (result);
}

t_vec2f vec2f_one(void) {
  t_vec2f result;

  result.x = 1.0f;
  result.y = 1.0f;
  return (result);
}

t_vec2f vec2f_unit_x(void) {
  t_vec2f result;

  result.x = 1.0f;
  result.y = 0.0f;
  return (result);
}

t_vec2f vec2f_unit_y(void) {
  t_vec2f result;

  result.x = 0.0f;
  result.y = 1.0f;
  return (result);
}

t_vec2f vec2f_min(t_vec2f a, t_vec2f b) {
  t_vec2f result;

  result.x = fminf(a.x, b.x);
  result.y = fminf(a.y, b.y);
  return (result);
}

t_vec2f vec2f_max(t_vec2f a, t_vec2f b) {
  t_vec2f result;

  result.x = fmaxf(a.x, b.x);
  result.y = fmaxf(a.y, b.y);
  return (result);
}

t_vec2f vec2f_abs(t_vec2f v) {
  t_vec2f result;

  result.x = fabsf(v.x);
  result.y = fabsf(v.y);
  return (result);
}