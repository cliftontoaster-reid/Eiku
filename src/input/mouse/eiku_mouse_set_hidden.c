/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_mouse_set_hidden.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 10:37:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 10:45:19 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/platform.h"
#include "input/mouse.h"

#ifdef EIKU_PLATFORM_LINUX

#include <X11/Xlib.h>
#include <X11/cursorfont.h>

EIKU_INT int eiku_mouse_set_hidden(t_mouse* mouse, bool hidden) {
  if (!mouse || !mouse->ctx || !mouse->ctx->display || !mouse->window ||
      !mouse->window->window)
    return -1;

  if (hidden) {
    // Create an invisible cursor
    Pixmap blank;
    XColor dummy;
    char data[1] = {0};
    blank = XCreateBitmapFromData(mouse->ctx->display, mouse->window->window,
                                  data, 1, 1);
    if (blank == None) return -1;  // Failed to create bitmap

    Cursor invisible_cursor = XCreatePixmapCursor(mouse->ctx->display, blank,
                                                  blank, &dummy, &dummy, 0, 0);
    XFreePixmap(mouse->ctx->display, blank);
    if (invisible_cursor == None) return -1;  // Failed to create cursor

    XDefineCursor(mouse->ctx->display, mouse->window->window, invisible_cursor);
    XFlush(mouse->ctx->display);
    mouse->hidden = true;
  } else {
    // Restore the default cursor
    XUndefineCursor(mouse->ctx->display, mouse->window->window);
    XFlush(mouse->ctx->display);
    mouse->hidden = false;
  }
  return 0;
}

#endif