/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_reset_delta.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 10:33:32 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 10:39:05 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input/mouse.h"

EIKU_API void mouse_reset_delta(t_mouse* mouse) {
  if (!mouse) return;

  mouse->dx = 0;
  mouse->dy = 0;
  mouse->wheel_dx = 0;
  mouse->wheel_dy = 0;

  if (mouse->grabbed) {
    // If grabbed, keep the mouse at the center of the window
    if (mouse->window) {
      int32_t center_x = mouse->window->width / 2;
      int32_t center_y = mouse->window->height / 2;
      mouse->x = center_x;
      mouse->y = center_y;
      eiku_mouse_move(mouse, center_x, center_y);
    }
  }
}
