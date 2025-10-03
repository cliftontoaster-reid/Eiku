/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_destroy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: automation <automation>                     +#+  +:+
        +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 02:00:00 by automation        #+#    #+#             */
/*   Updated: 2025/09/22 02:00:00 by automation       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/core.h"
#include "core/platform.h"
#include "res/resource.h"

#ifdef EIKU_PLATFORM_LINUX

#include <X11/Xlib.h>
#include <stdlib.h>

#endif

EIKU_API int eiku_destroy(t_eiku_context *ctx) {
  t_eiku_window *w;
  t_eiku_window *next;

  if (!ctx) return (EIKU_ERROR_INVALID_PARAM);
#ifdef EIKU_PLATFORM_LINUX
  // Clean up windows list (best-effort). Each window holds a GC and a Window
  if (ctx->win_list) {
    w = ctx->win_list;
    while (w) {
      next = w->next;
      if (w->gc && ctx->display) XFreeGC(ctx->display, w->gc);
      if (w->window && ctx->display) XDestroyWindow(ctx->display, w->window);
      if (w->title) free(w->title);
      free(w);
      w = next;
    }
    ctx->win_list = NULL;
  }
  // If we created a private colormap, free it
  if (ctx->private_cmap && ctx->cmap && ctx->display)
    XFreeColormap(ctx->display, ctx->cmap);
  // Flush pending requests and close the display
  if (ctx->display) {
    XSync(ctx->display, False);
    XCloseDisplay(ctx->display);
    ctx->display = NULL;
  }
  // Free resource manager
  if (ctx->res_manager) {
    eiku_res_manager_free(ctx->res_manager);
    ctx->res_manager = NULL;
  }
  free(ctx);
#endif
  return (EIKU_SUCCESS);
}
