/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:21:26 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/10/03 11:37:38 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "core/core.h"
#include "core/platform.h"
#include "math/vec.h"
#include "res/resource.h"
#include "scene/component.h"

typedef struct s_eiku_entity {
  t_eiku_context *ctx;

  uint32_t id;
  char *name;
  t_vec2f position;
  t_vec2f scale;
  float rotation;
  bool active;

  eiku_res_id_t sprite_res_id;     /* Resource ID of the sprite image */
  eiku_res_id_t *animation_res_id; /* Resource ID of the animation data */
  size_t animation_count;
  size_t current_animation;
  float animation_time; /* Time accumulator for animation frame switching */

  eiku_res_id_t data_res_id; /* Resource ID for custom data */

  // Functions init/update/destroy with ctx
  void (*init)(struct s_eiku_entity *entity);
  void (*update)(struct s_eiku_entity *entity, float delta_time);
  void (*destroy)(struct s_eiku_entity *entity);
} t_eiku_entity;

#define EIKU_DATA_TYPE_PLAYER 170
typedef struct s_eiku_player_data {
  t_vec2f dir;
} t_eiku_player_data;

#define EIKU_DATA_TYPE_COLLECTIBLE 171
typedef struct s_eiku_collectible_data {
  bool collected;
} t_eiku_collectible_data;

t_eiku_entity *eiku_entity_create(t_eiku_context *ctx, const char *name);
void eiku_entity_destroy(t_eiku_entity *entity);

/* Basic getters/setters for transform and metadata */
void eiku_entity_set_position(t_eiku_entity *entity, t_vec2f pos);
void eiku_entity_set_position_f(t_eiku_entity *entity, float x, float y);
t_vec2f eiku_entity_get_position(const t_eiku_entity *entity);

void eiku_entity_set_scale(t_eiku_entity *entity, t_vec2f scale);
void eiku_entity_set_scale_f(t_eiku_entity *entity, float x, float y);
t_vec2f eiku_entity_get_scale(const t_eiku_entity *entity);

void eiku_entity_set_rotation(t_eiku_entity *entity, float rotation);
float eiku_entity_get_rotation(const t_eiku_entity *entity);

void eiku_entity_set_active(t_eiku_entity *entity, bool active);
bool eiku_entity_is_active(const t_eiku_entity *entity);

void eiku_entity_set_name(t_eiku_entity *entity, const char *name);
const char *eiku_entity_get_name(const t_eiku_entity *entity);

/* Resource helpers (sprite, animations, arbitrary data) */
void eiku_entity_set_sprite_res(t_eiku_entity *entity, eiku_res_id_t res_id);
eiku_res_id_t eiku_entity_get_sprite_res(const t_eiku_entity *entity);

/* animations: pass an array of resource ids (ownership: caller) */
void eiku_entity_set_animations(t_eiku_entity *entity, eiku_res_id_t *res_ids,
                                size_t count);
size_t eiku_entity_get_animation_count(const t_eiku_entity *entity);
eiku_res_id_t eiku_entity_get_animation_res(const t_eiku_entity *entity,
                                            size_t index);
void eiku_entity_play_animation(t_eiku_entity *entity, size_t index);
void eiku_entity_stop_animation(t_eiku_entity *entity);
void eiku_entity_update_animation(t_eiku_entity *entity, float delta_time);

void eiku_entity_set_data_res(t_eiku_entity *entity, eiku_res_id_t res_id);
eiku_res_id_t eiku_entity_get_data_res(const t_eiku_entity *entity);
