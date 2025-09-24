/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_mouse_handle_xevent.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 10:21:13 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 10:29:32 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input/mouse.h"

#ifdef EIKU_PLATFORM_LINUX

#include <X11/Xlib.h>

EIKU_INT int eiku_mouse_handle_xevent(t_mouse* mouse, XEvent* event) {
  if (event->type == MotionNotify) {
    XMotionEvent* mev = (XMotionEvent*)event;
    mouse->dx += (int32_t)(mev->x - mouse->x);  // Calculate delta
    mouse->dy += (int32_t)(mev->y - mouse->y);
    mouse->x = (int32_t)mev->x;
    mouse->y = (int32_t)mev->y;
    return 1;
  } else if (event->type == ButtonPress) {
    XButtonEvent* bev = (XButtonEvent*)event;
    // Handle regular buttons (1-3 and 8), exclude wheel (4-7)
    if ((bev->button >= Button1 && bev->button <= Button3) ||
        bev->button == 8) {
      mouse->buttons[bev->button - Button1] = true;
    }
    // Handle wheel as button presses (standard way)
    if (bev->button == Button4) {  // Scroll up
      mouse->wheel_dy += 1;
      mouse->wheel_y += 1;
    } else if (bev->button == Button5) {  // Scroll down
      mouse->wheel_dy -= 1;
      mouse->wheel_y -= 1;
    } else if (bev->button == 6) {  // Scroll left (convention)
      mouse->wheel_dx -= 1;
      mouse->wheel_x -= 1;
    } else if (bev->button == 7) {  // Scroll right (convention)
      mouse->wheel_dx += 1;
      mouse->wheel_x += 1;
    }
    return 1;
  } else if (event->type == ButtonRelease) {
    XButtonEvent* bev = (XButtonEvent*)event;
    // Handle regular buttons (1-3 and 8), exclude wheel (4-7)
    if ((bev->button >= Button1 && bev->button <= Button3) ||
        bev->button == 8) {
      mouse->buttons[bev->button - Button1] = false;
    }
    return 1;
  }
  return 0;  // Not a mouse event
}

#endif