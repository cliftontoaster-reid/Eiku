/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_mouse_move.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 10:35:08 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 10:36:19 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input/mouse.h"

#ifdef EIKU_PLATFORM_LINUX

#include <X11/Xlib.h>
#include <X11/Xutil.h>

EIKU_INT int eiku_mouse_move(t_mouse* mouse, int32_t x, int32_t y) {
  if (!mouse || !mouse->ctx || !mouse->ctx->display || !mouse->window)
    return -1;

  // Move the mouse pointer to (x, y) within the window
  XWarpPointer(mouse->ctx->display, None, mouse->window->window, 0, 0, 0, 0, x,
               y);
  XFlush(mouse->ctx->display);  // Ensure the command is sent immediately

  // Update internal mouse state
  mouse->dx += (int32_t)(x - mouse->x);  // Calculate delta
  mouse->dy += (int32_t)(y - mouse->y);
  mouse->x = x;
  mouse->y = y;

  return 0;  // Success
}

#endif