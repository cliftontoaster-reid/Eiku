/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 11:50:53 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/10/03 11:53:51 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene/entity.h"

/// Create a player entity with default components and data.
t_eiku_entity *eiku_create_player(t_eiku_context *ctx, const char *name,
                                  t_vec2f position);
t_vec2f eiku_get_player_direction(const t_eiku_entity *player);
void eiku_set_player_direction(t_eiku_entity *player, t_vec2f dir);

/// Create a collectible entity with default components and data.
t_eiku_entity *eiku_create_collectible(t_eiku_context *ctx, const char *name,
                                       t_vec2f position);
bool eiku_is_collectible_collected(const t_eiku_entity *collectible);
void eiku_set_collectible_collected(t_eiku_entity *collectible, bool collected);