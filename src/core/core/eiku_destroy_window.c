/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_destroy_window.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:35:04 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/24 14:42:31 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

#include "core/core.h"

#ifdef EIKU_PLATFORM_LINUX

#include <X11/Xlib.h>
#include <X11/Xutil.h>

EIKU_API int eiku_destroy_window(t_eiku_context *ctx, t_eiku_window *win) {
  if (!ctx || !win) {
    return (EIKU_ERROR_INVALID_PARAM);
  }
  // Remove from context's window list
  if (ctx->win_list == win) {
    ctx->win_list = win->next;
  } else {
    t_eiku_window *prev = ctx->win_list;
    while (prev && prev->next != win) {
      prev = prev->next;
    }
    if (prev) {
      prev->next = win->next;
    }
  }
  // Destroy X11 window
  XDestroyWindow(ctx->display, win->window);
  // Destroy graphics context
  if (win->gc) {
    XFreeGC(ctx->display, win->gc);
  }
  // Flush X11 display
  XFlush(ctx->display);
  // Free title string
  if (win->title) {
    free(win->title);
  }
  // Free window structure
  free(win);
  return (EIKU_SUCCESS);
}

#endif /* EIKU_PLATFORM_LINUX */