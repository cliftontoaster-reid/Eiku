/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3f.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:18:37 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/23 15:19:07 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "math/vec.h"

t_vec3f vec3f_add(t_vec3f a, t_vec3f b) {
  t_vec3f result;

  result.x = a.x + b.x;
  result.y = a.y + b.y;
  result.z = a.z + b.z;
  return (result);
}

t_vec3f vec3f_sub(t_vec3f a, t_vec3f b) {
  t_vec3f result;

  result.x = a.x - b.x;
  result.y = a.y - b.y;
  result.z = a.z - b.z;
  return (result);
}

t_vec3f vec3f_scale(t_vec3f v, float scalar) {
  t_vec3f result;

  result.x = v.x * scalar;
  result.y = v.y * scalar;
  result.z = v.z * scalar;
  return (result);
}

float vec3f_dot(t_vec3f a, t_vec3f b) {
  return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vec3f vec3f_cross(t_vec3f a, t_vec3f b) {
  t_vec3f result;

  result.x = a.y * b.z - a.z * b.y;
  result.y = a.z * b.x - a.x * b.z;
  result.z = a.x * b.y - a.y * b.x;
  return (result);
}

float vec3f_length(t_vec3f v) {
  return (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
}

t_vec3f vec3f_normalize(t_vec3f v) {
  float length;
  t_vec3f result;

  length = vec3f_length(v);
  if (length == 0.0f) {
    result.x = 0.0f;
    result.y = 0.0f;
    result.z = 0.0f;
    return (result);
  }
  result.x = v.x / length;
  result.y = v.y / length;
  result.z = v.z / length;
  return (result);
}

float vec3f_distance(t_vec3f a, t_vec3f b) {
  t_vec3f diff;

  diff = vec3f_sub(a, b);
  return (vec3f_length(diff));
}

float vec3f_distance_squared(t_vec3f a, t_vec3f b) {
  t_vec3f diff;

  diff = vec3f_sub(a, b);
  return (diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
}

float vec3f_angle_between(t_vec3f a, t_vec3f b) {
  float dot;
  float len_a;
  float len_b;
  float cos_angle;

  len_a = vec3f_length(a);
  len_b = vec3f_length(b);
  if (len_a == 0.0f || len_b == 0.0f) return (0.0f);
  dot = vec3f_dot(a, b);
  cos_angle = dot / (len_a * len_b);
  if (cos_angle > 1.0f)
    cos_angle = 1.0f;
  else if (cos_angle < -1.0f)
    cos_angle = -1.0f;
  return (acosf(cos_angle));
}

t_vec3f vec3f_lerp(t_vec3f a, t_vec3f b, float t) {
  t_vec3f result;

  result.x = a.x + t * (b.x - a.x);
  result.y = a.y + t * (b.y - a.y);
  result.z = a.z + t * (b.z - a.z);
  return (result);
}

t_vec3f vec3f_reflect(t_vec3f v, t_vec3f normal) {
  float dot;
  t_vec3f result;

  dot = vec3f_dot(v, normal);
  result.x = v.x - 2.0f * dot * normal.x;
  result.y = v.y - 2.0f * dot * normal.y;
  result.z = v.z - 2.0f * dot * normal.z;
  return (result);
}

t_vec3f vec3f_clamp_length(t_vec3f v, float max_length) {
  float length;

  length = vec3f_length(v);
  if (length > max_length && length > 0.0f)
    return (vec3f_scale(vec3f_normalize(v), max_length));
  return (v);
}

bool vec3f_equal(t_vec3f a, t_vec3f b, float epsilon) {
  return (fabsf(a.x - b.x) <= epsilon && fabsf(a.y - b.y) <= epsilon &&
          fabsf(a.z - b.z) <= epsilon);
}

bool vec3f_is_zero(t_vec3f v, float epsilon) {
  return (fabsf(v.x) <= epsilon && fabsf(v.y) <= epsilon &&
          fabsf(v.z) <= epsilon);
}

t_vec3f vec3f_zero(void) {
  t_vec3f result;

  result.x = 0.0f;
  result.y = 0.0f;
  result.z = 0.0f;
  return (result);
}

t_vec3f vec3f_one(void) {
  t_vec3f result;

  result.x = 1.0f;
  result.y = 1.0f;
  result.z = 1.0f;
  return (result);
}

t_vec3f vec3f_unit_x(void) {
  t_vec3f result;

  result.x = 1.0f;
  result.y = 0.0f;
  result.z = 0.0f;
  return (result);
}

t_vec3f vec3f_unit_y(void) {
  t_vec3f result;

  result.x = 0.0f;
  result.y = 1.0f;
  result.z = 0.0f;
  return (result);
}

t_vec3f vec3f_unit_z(void) {
  t_vec3f result;

  result.x = 0.0f;
  result.y = 0.0f;
  result.z = 1.0f;
  return (result);
}

t_vec3f vec3f_min(t_vec3f a, t_vec3f b) {
  t_vec3f result;

  result.x = fminf(a.x, b.x);
  result.y = fminf(a.y, b.y);
  result.z = fminf(a.z, b.z);
  return (result);
}

t_vec3f vec3f_max(t_vec3f a, t_vec3f b) {
  t_vec3f result;

  result.x = fmaxf(a.x, b.x);
  result.y = fmaxf(a.y, b.y);
  result.z = fmaxf(a.z, b.z);
  return (result);
}

t_vec3f vec3f_abs(t_vec3f v) {
  t_vec3f result;

  result.x = fabsf(v.x);
  result.y = fabsf(v.y);
  result.z = fabsf(v.z);
  return (result);
}