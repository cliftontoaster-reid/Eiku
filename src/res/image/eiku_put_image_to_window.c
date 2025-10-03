/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_put_image_to_window.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: automation <automation>                     +#+  +:+
        +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:36:00 by automation        #+#    #+#             */
/*   Updated: 2025/09/22 19:36:00 by automation       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/core.h"
#include "core/platform.h"

#ifdef EIKU_PLATFORM_LINUX
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
#include <stdlib.h>

EIKU_API int eiku_put_image_to_window(t_eiku_context *ctx, t_eiku_window *win,
                                      t_eiku_image *img, int x, int y) {
  if (!ctx || !win || !img) return (EIKU_ERROR_INVALID_PARAM);
  if (!ctx->display) return (EIKU_ERROR_DISPLAY_FAILED);
  // Put image directly to window - avoid pixmap intermediate step
  if (img->type == EIKU_TYPE_SHM) {
    XShmPutImage(ctx->display, win->window, win->gc, img->image, 0, 0, x, y,
                 img->width, img->height, False);
  } else {
    XPutImage(ctx->display, win->window, win->gc, img->image, 0, 0, x, y,
              img->width, img->height);
  }
  if (ctx->do_flush) XFlush(ctx->display);
  return (EIKU_SUCCESS);
}

#else
EIKU_API int eiku_put_image_to_window(t_eiku_context *ctx, t_eiku_window *win,
                                      t_eiku_image *img, int x, int y) {
  (void)ctx;
  (void)win;
  (void)img;
  (void)x;
  (void)y;
  return (EIKU_ERROR_INVALID_PARAM);
}
#endif
