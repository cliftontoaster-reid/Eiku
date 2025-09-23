/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3i.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:27:27 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/23 16:27:41 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "math/vec.h"

t_vec3i vec3i_add(t_vec3i a, t_vec3i b) {
  t_vec3i result;

  result.x = a.x + b.x;
  result.y = a.y + b.y;
  result.z = a.z + b.z;
  return (result);
}

t_vec3i vec3i_sub(t_vec3i a, t_vec3i b) {
  t_vec3i result;

  result.x = a.x - b.x;
  result.y = a.y - b.y;
  result.z = a.z - b.z;
  return (result);
}

t_vec3i vec3i_scale(t_vec3i v, int scalar) {
  t_vec3i result;

  result.x = v.x * scalar;
  result.y = v.y * scalar;
  result.z = v.z * scalar;
  return (result);
}

int vec3i_dot(t_vec3i a, t_vec3i b) {
  return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vec3i vec3i_cross(t_vec3i a, t_vec3i b) {
  t_vec3i result;

  result.x = a.y * b.z - a.z * b.y;
  result.y = a.z * b.x - a.x * b.z;
  result.z = a.x * b.y - a.y * b.x;
  return (result);
}

float vec3i_length(t_vec3i v) {
  return (sqrtf((float)(v.x * v.x + v.y * v.y + v.z * v.z)));
}

t_vec3i vec3i_normalize(t_vec3i v) {
  float length;
  t_vec3i result;

  length = vec3i_length(v);
  if (length == 0.0f) {
    result.x = 0;
    result.y = 0;
    result.z = 0;
    return (result);
  }
  result.x = (int)(v.x / length);
  result.y = (int)(v.y / length);
  result.z = (int)(v.z / length);
  return (result);
}

float vec3i_distance(t_vec3i a, t_vec3i b) {
  t_vec3i diff;

  diff = vec3i_sub(a, b);
  return (vec3i_length(diff));
}

float vec3i_distance_squared(t_vec3i a, t_vec3i b) {
  t_vec3i diff;

  diff = vec3i_sub(a, b);
  return ((float)(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z));
}

bool vec3i_equal(t_vec3i a, t_vec3i b) {
  return (a.x == b.x && a.y == b.y && a.z == b.z);
}

bool vec3i_is_zero(t_vec3i v) { return (v.x == 0 && v.y == 0 && v.z == 0); }

t_vec3i vec3i_zero(void) {
  t_vec3i result;

  result.x = 0;
  result.y = 0;
  result.z = 0;
  return (result);
}

t_vec3i vec3i_one(void) {
  t_vec3i result;

  result.x = 1;
  result.y = 1;
  result.z = 1;
  return (result);
}

t_vec3i vec3i_unit_x(void) {
  t_vec3i result;

  result.x = 1;
  result.y = 0;
  result.z = 0;
  return (result);
}

t_vec3i vec3i_unit_y(void) {
  t_vec3i result;

  result.x = 0;
  result.y = 1;
  result.z = 0;
  return (result);
}

t_vec3i vec3i_unit_z(void) {
  t_vec3i result;

  result.x = 0;
  result.y = 0;
  result.z = 1;
  return (result);
}

t_vec3i vec3i_min(t_vec3i a, t_vec3i b) {
  t_vec3i result;

  result.x = (a.x < b.x) ? a.x : b.x;
  result.y = (a.y < b.y) ? a.y : b.y;
  result.z = (a.z < b.z) ? a.z : b.z;
  return (result);
}

t_vec3i vec3i_max(t_vec3i a, t_vec3i b) {
  t_vec3i result;

  result.x = (a.x > b.x) ? a.x : b.x;
  result.y = (a.y > b.y) ? a.y : b.y;
  result.z = (a.z > b.z) ? a.z : b.z;
  return (result);
}

t_vec3i vec3i_abs(t_vec3i v) {
  t_vec3i result;

  result.x = (v.x < 0) ? -v.x : v.x;
  result.y = (v.y < 0) ? -v.y : v.y;
  result.z = (v.z < 0) ? -v.z : v.z;
  return (result);
}