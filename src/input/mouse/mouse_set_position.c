/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_set_position.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 10:40:32 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 10:41:17 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input/mouse.h"

EIKU_API int mouse_set_position(t_mouse* mouse, int32_t x, int32_t y) {
  if (!mouse || !mouse->ctx || !mouse->window) {
    return -1;  // Invalid parameters
  }

#ifdef EIKU_PLATFORM_LINUX
  return eiku_mouse_move(mouse, x, y);
#endif

  return 0;
}