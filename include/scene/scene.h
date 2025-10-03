/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:21:16 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/10/02 13:08:52 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "core/core.h"
#include "core/platform.h"
#include "math/vec.h"
#include "scene/entity.h"

typedef struct s_eiku_2dscene {
  t_eiku_context *ctx;
  t_eiku_window *win;
  t_eiku_image *img;
  int width;
  int height;
  uint32_t bg_color;

  uint8_t **map_data;
  t_vec2i map_size;

  /* Entity management */
  t_eiku_entity *entities;
  size_t entity_count;
  size_t entity_capacity;
} t_eiku_2dscene;