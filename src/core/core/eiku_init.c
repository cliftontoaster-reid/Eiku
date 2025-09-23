/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eiku_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiorell@student.42nice.fr <lfiorell>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 01:00:00 by lfiorell@st       #+#    #+#             */
/*   Updated: 2025/09/23 14:51:58 by lfiorell@st      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/core.h"
#include "core/platform.h"

#ifdef EIKU_PLATFORM_LINUX

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <stdlib.h>

#endif

// Initialize the Eiku X11 context
EIKU_API t_eiku_context *eiku_init(void) {
  t_eiku_context *ctx = NULL;

#ifdef EIKU_PLATFORM_LINUX
  // Allocate context
  ctx = calloc(1, sizeof(t_eiku_context));
  if (!ctx) return (NULL);
  // Open X11 display (NULL uses DISPLAY env; empty string can be problematic)
  ctx->display = XOpenDisplay(NULL);
  if (!ctx->display) {
    free(ctx);
    return (NULL);
  }
  // Set up basic X11 information
  ctx->screen = DefaultScreen(ctx->display);
  ctx->root = DefaultRootWindow(ctx->display);
  ctx->depth = DefaultDepth(ctx->display, ctx->screen);
  ctx->cmap = DefaultColormap(ctx->display, ctx->screen);
  ctx->private_cmap = 0;
  // Get visual (TrueColor preferred)
  if (eiku_int_get_visual(ctx) != EIKU_SUCCESS) {
    XCloseDisplay(ctx->display);
    free(ctx);
    return (NULL);
  }
  // Create a colormap compatible with the selected visual
  if (ctx->visual != DefaultVisual(ctx->display, ctx->screen)) {
    ctx->cmap =
        XCreateColormap(ctx->display, ctx->root, ctx->visual, AllocNone);
    ctx->private_cmap = 1;
  }
  // Set up shared memory extension
  if (eiku_int_deal_shm(ctx) != EIKU_SUCCESS) {
    XCloseDisplay(ctx->display);
    free(ctx);
    return (NULL);
  }
  // Set up RGB conversion parameters
  if (eiku_int_rgb_conversion(ctx) != EIKU_SUCCESS) {
    XCloseDisplay(ctx->display);
    free(ctx);
    return (NULL);
  }
  // Set up window manager atoms
  ctx->wm_delete_window = XInternAtom(ctx->display, "WM_DELETE_WINDOW", False);
  ctx->wm_protocols = XInternAtom(ctx->display, "WM_PROTOCOLS", False);
  // Initialize other fields
  ctx->win_list = NULL;
  ctx->loop_hook = NULL;
  ctx->loop_param = NULL;
  ctx->end_loop = false;
  ctx->do_flush = true;
  return (ctx);
#else
// Non-Linux platforms not supported yet
#error "Eiku currently supports only Linux with X11."
  return (NULL);
#endif
}