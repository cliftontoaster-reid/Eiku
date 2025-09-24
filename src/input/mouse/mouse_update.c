/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_update.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 10:42:28 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 10:43:35 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <X11/Xlib.h>
#include <stdarg.h>

#include "input/mouse.h"

EIKU_API int mouse_update(t_mouse* mouse, ...) {
  if (!mouse || !mouse->ctx || !mouse->window) {
    return -1;  // Invalid mouse or context
  }

#ifdef EIKU_PLATFORM_LINUX
  // First varg is the XEvent pointer
  va_list args;
  va_start(args, mouse);
  XEvent* event = va_arg(args, XEvent*);
  va_end(args);

  if (event) {
    return eiku_mouse_handle_xevent(mouse, event);
  }
#endif

  return 0;  // No event processed
}