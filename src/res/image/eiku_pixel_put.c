/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_pixel_put.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:17:35 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/10/03 15:48:13 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <string.h>

#include "core/core.h"
#include "core/platform.h"

#ifdef EIKU_PLATFORM_LINUX
#include <X11/Xlib.h>
#include <stdlib.h>

EIKU_API int eiku_pixel_put(t_eiku_context *ctx, t_eiku_window *win, int x,
                            int y, int color) {
  unsigned long px;

  if (!ctx || !win) return (EIKU_ERROR_INVALID_PARAM);
#if defined(EIKU_PLATFORM_LINUX)
  if (!ctx->display || !win->window || !win->gc)
    return (EIKU_ERROR_WINDOW_FAILED);
  /* Convert color to pixel value using context conversion */
  px = (unsigned long)eiku_get_color_value(ctx, color);
  XSetForeground(ctx->display, win->gc, px);
  XDrawPoint(ctx->display, win->window, win->gc, x, y);
  if (ctx->do_flush) XFlush(ctx->display);
  return (EIKU_SUCCESS);
#else
  (void)ctx;
  (void)win;
  (void)x;
  (void)y;
  (void)color;
  return (EIKU_ERROR_INVALID_PARAM);
#endif
}

#endif
