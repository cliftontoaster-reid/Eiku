/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:05:03 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/23 15:16:11 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct s_vec2i {
  int x;
  int y;
} t_vec2i;

typedef struct s_vec2f {
  float x;
  float y;
} t_vec2f;

typedef struct s_vec3i {
  int x;
  int y;
  int z;
} t_vec3i;

typedef struct s_vec3f {
  float x;
  float y;
  float z;
} t_vec3f;

typedef struct s_vec4i {
  int x;
  int y;
  int z;
  int w;
} t_vec4i;

typedef struct s_vec4f {
  float x;
  float y;
  float z;
  float w;
} t_vec4f;

// Vector2i operations
t_vec2i vec2i_add(t_vec2i a, t_vec2i b);
t_vec2i vec2i_sub(t_vec2i a, t_vec2i b);
t_vec2i vec2i_scale(t_vec2i v, int scalar);
int vec2i_dot(t_vec2i a, t_vec2i b);
float vec2i_length(t_vec2i v);
t_vec2i vec2i_normalize(t_vec2i v);
float vec2i_distance(t_vec2i a, t_vec2i b);
float vec2i_distance_squared(t_vec2i a, t_vec2i b);
bool vec2i_equal(t_vec2i a, t_vec2i b);
bool vec2i_is_zero(t_vec2i v);
t_vec2i vec2i_zero(void);
t_vec2i vec2i_one(void);
t_vec2i vec2i_unit_x(void);
t_vec2i vec2i_unit_y(void);
t_vec2i vec2i_min(t_vec2i a, t_vec2i b);
t_vec2i vec2i_max(t_vec2i a, t_vec2i b);
t_vec2i vec2i_abs(t_vec2i v);

// Vector2f operations
t_vec2f vec2f_add(t_vec2f a, t_vec2f b);
t_vec2f vec2f_sub(t_vec2f a, t_vec2f b);
t_vec2f vec2f_scale(t_vec2f v, float scalar);
float vec2f_dot(t_vec2f a, t_vec2f b);
float vec2f_length(t_vec2f v);
t_vec2f vec2f_normalize(t_vec2f v);
float vec2f_distance(t_vec2f a, t_vec2f b);
float vec2f_distance_squared(t_vec2f a, t_vec2f b);
float vec2f_angle(t_vec2f v);
t_vec2f vec2f_from_angle(float angle, float length);
float vec2f_angle_between(t_vec2f a, t_vec2f b);
t_vec2f vec2f_lerp(t_vec2f a, t_vec2f b, float t);
t_vec2f vec2f_rotate(t_vec2f v, float angle);
t_vec2f vec2f_perpendicular(t_vec2f v);
t_vec2f vec2f_reflect(t_vec2f v, t_vec2f normal);
t_vec2f vec2f_clamp_length(t_vec2f v, float max_length);
bool vec2f_equal(t_vec2f a, t_vec2f b, float epsilon);
bool vec2f_is_zero(t_vec2f v, float epsilon);
t_vec2f vec2f_zero(void);
t_vec2f vec2f_one(void);
t_vec2f vec2f_unit_x(void);
t_vec2f vec2f_unit_y(void);
t_vec2f vec2f_min(t_vec2f a, t_vec2f b);
t_vec2f vec2f_max(t_vec2f a, t_vec2f b);
t_vec2f vec2f_abs(t_vec2f v);

// Vector3i operations
t_vec3i vec3i_add(t_vec3i a, t_vec3i b);
t_vec3i vec3i_sub(t_vec3i a, t_vec3i b);
t_vec3i vec3i_scale(t_vec3i v, int scalar);
int vec3i_dot(t_vec3i a, t_vec3i b);
t_vec3i vec3i_cross(t_vec3i a, t_vec3i b);
float vec3i_length(t_vec3i v);
t_vec3i vec3i_normalize(t_vec3i v);
float vec3i_distance(t_vec3i a, t_vec3i b);
float vec3i_distance_squared(t_vec3i a, t_vec3i b);
bool vec3i_equal(t_vec3i a, t_vec3i b);
bool vec3i_is_zero(t_vec3i v);
t_vec3i vec3i_zero(void);
t_vec3i vec3i_one(void);
t_vec3i vec3i_unit_x(void);
t_vec3i vec3i_unit_y(void);
t_vec3i vec3i_unit_z(void);
t_vec3i vec3i_min(t_vec3i a, t_vec3i b);
t_vec3i vec3i_max(t_vec3i a, t_vec3i b);
t_vec3i vec3i_abs(t_vec3i v);

// Vector3f operations
t_vec3f vec3f_add(t_vec3f a, t_vec3f b);
t_vec3f vec3f_sub(t_vec3f a, t_vec3f b);
t_vec3f vec3f_scale(t_vec3f v, float scalar);
float vec3f_dot(t_vec3f a, t_vec3f b);
t_vec3f vec3f_cross(t_vec3f a, t_vec3f b);
float vec3f_length(t_vec3f v);
t_vec3f vec3f_normalize(t_vec3f v);
float vec3f_distance(t_vec3f a, t_vec3f b);
float vec3f_distance_squared(t_vec3f a, t_vec3f b);
float vec3f_angle_between(t_vec3f a, t_vec3f b);
t_vec3f vec3f_lerp(t_vec3f a, t_vec3f b, float t);
t_vec3f vec3f_reflect(t_vec3f v, t_vec3f normal);
t_vec3f vec3f_clamp_length(t_vec3f v, float max_length);
bool vec3f_equal(t_vec3f a, t_vec3f b, float epsilon);
bool vec3f_is_zero(t_vec3f v, float epsilon);
t_vec3f vec3f_zero(void);
t_vec3f vec3f_one(void);
t_vec3f vec3f_unit_x(void);
t_vec3f vec3f_unit_y(void);
t_vec3f vec3f_unit_z(void);
t_vec3f vec3f_min(t_vec3f a, t_vec3f b);
t_vec3f vec3f_max(t_vec3f a, t_vec3f b);
t_vec3f vec3f_abs(t_vec3f v);

// Vector4i operations
t_vec4i vec4i_add(t_vec4i a, t_vec4i b);
t_vec4i vec4i_sub(t_vec4i a, t_vec4i b);
t_vec4i vec4i_scale(t_vec4i v, int scalar);
int vec4i_dot(t_vec4i a, t_vec4i b);
float vec4i_length(t_vec4i v);
t_vec4i vec4i_normalize(t_vec4i v);
float vec4i_distance(t_vec4i a, t_vec4i b);
float vec4i_distance_squared(t_vec4i a, t_vec4i b);
bool vec4i_equal(t_vec4i a, t_vec4i b);
bool vec4i_is_zero(t_vec4i v);
t_vec4i vec4i_zero(void);
t_vec4i vec4i_one(void);
t_vec4i vec4i_min(t_vec4i a, t_vec4i b);
t_vec4i vec4i_max(t_vec4i a, t_vec4i b);
t_vec4i vec4i_abs(t_vec4i v);

// Vector4f operations
t_vec4f vec4f_add(t_vec4f a, t_vec4f b);
t_vec4f vec4f_sub(t_vec4f a, t_vec4f b);
t_vec4f vec4f_scale(t_vec4f v, float scalar);
float vec4f_dot(t_vec4f a, t_vec4f b);
float vec4f_length(t_vec4f v);
t_vec4f vec4f_normalize(t_vec4f v);
float vec4f_distance(t_vec4f a, t_vec4f b);
float vec4f_distance_squared(t_vec4f a, t_vec4f b);
t_vec4f vec4f_lerp(t_vec4f a, t_vec4f b, float t);
t_vec4f vec4f_clamp_length(t_vec4f v, float max_length);
bool vec4f_equal(t_vec4f a, t_vec4f b, float epsilon);
bool vec4f_is_zero(t_vec4f v, float epsilon);
t_vec4f vec4f_zero(void);
t_vec4f vec4f_one(void);
t_vec4f vec4f_min(t_vec4f a, t_vec4f b);
t_vec4f vec4f_max(t_vec4f a, t_vec4f b);
t_vec4f vec4f_abs(t_vec4f v);